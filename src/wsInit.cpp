#include "wsWindow.h"
#include "wsList.h"
#include <openWS.h>
extern int inited;

struct windowHintData {
	int hint, value;
};
static wsList<windowHintData> windowHintLists;

int wsSetWindowHint(int hint, int value) {
	if(inited) {
		wsSetError(WS_ERR_ILLEGAL_OPERATION);
		return false;
	}
	windowHintLists.addBack(windowHintData{hint, value});
	return true;
}

inline void setGLFWWindowCallbacks() {
	void wsKeyboardReceiver(GLFWwindow*, int key, int, int action, int mods);
	void wsCharReceiver(GLFWwindow*, unsigned key, int mods);
	void wsMouseReceiver(GLFWwindow*, int button, int action, int mods);
	void wsCursorMoveReceiver(GLFWwindow*, double xpos, double ypos);
	void wsCursorEnterReceiver(GLFWwindow*, int entered);
	void wsScrollReceiver(GLFWwindow*, double xoffset, double yoffset);
	void wsWindowResizeReceiver(GLFWwindow*, int w, int h);
	void wsWindowMoveReceiver(GLFWwindow*, int x, int y);
	void wsWindowCloseReceiver(GLFWwindow*);
	void wsWindowFocusReceiver(GLFWwindow*, int focused);
	void wsFileDropReceiver(GLFWwindow*, int count, const char **filename);

	glfwSetKeyCallback(baseWindow->glfwwindow, wsKeyboardReceiver);
	glfwSetCharModsCallback(baseWindow->glfwwindow, wsCharReceiver);
	glfwSetMouseButtonCallback(baseWindow->glfwwindow, wsMouseReceiver);
	glfwSetCursorPosCallback(baseWindow->glfwwindow, wsCursorMoveReceiver);
	glfwSetCursorEnterCallback(baseWindow->glfwwindow, wsCursorEnterReceiver);
	glfwSetScrollCallback(baseWindow->glfwwindow, wsScrollReceiver);
	glfwSetWindowSizeCallback(baseWindow->glfwwindow, wsWindowResizeReceiver);
	glfwSetWindowPosCallback(baseWindow->glfwwindow, wsWindowMoveReceiver);
	glfwSetWindowCloseCallback(baseWindow->glfwwindow, wsWindowCloseReceiver);
	glfwSetWindowFocusCallback(baseWindow->glfwwindow, wsWindowFocusReceiver);
	glfwSetDropCallback(baseWindow->glfwwindow, wsFileDropReceiver);
}

inline int openGLFWWindow(const char *title) {
	if (!glfwInit()) return false;

	for(auto* data = windowHintLists.front(); data; data = data->next)
		glfwWindowHint(data->data.hint, data->data.value);
	
	baseWindow->glfwwindow = glfwCreateWindow(baseWindow->size.x, baseWindow->size.y, title, 0, 0);
	if (!baseWindow->glfwwindow) {
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(baseWindow->glfwwindow);
	setGLFWWindowCallbacks();
	glfwSwapInterval(1);
	return true;
}
inline int initGLEW() {
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		return false;
	}

#define RETARGET(func) do{\
		if(!(func)){\
			if(!(func##EXT)) return false;\
			(func)=(func##EXT);\
		}\
	}while(0)

	RETARGET(glGenFramebuffers);
	RETARGET(glDeleteFramebuffers);
	RETARGET(glGenRenderbuffers);
	RETARGET(glDeleteRenderbuffers);
	RETARGET(glBindFramebuffer);
	RETARGET(glBindRenderbuffer);
	RETARGET(glRenderbufferStorage);
	RETARGET(glFramebufferRenderbuffer);
	RETARGET(glFramebufferTexture2D);
	RETARGET(glCheckFramebufferStatus);
#undef RETARGET
	return true;
}
int wsInit(const char * title, const int x, const int y, const int width, const int height) {
	baseWindow = new wsBaseWindow(title, { x, y }, {width, height});
	if (!openGLFWWindow(title)) {
		wsSetError(WS_ERR_GLFW_INITIALIZATION_FAILED);
		glfwSetWindowShouldClose(baseWindow->glfwwindow, 1);
		baseWindow->deleteWindow();
		delete baseWindow;
		baseWindow = nullptr;
		return false;
	}
	glfwSetWindowPos(baseWindow->glfwwindow, x > 0 ? x : 1, y > 0 ? y : 1);
	if (!initGLEW()) {
		wsSetError(WS_ERR_GLEW_INITIALIZATION_FAILED);
		glfwSetWindowShouldClose(baseWindow->glfwwindow, 1);
		baseWindow->deleteWindow();
		delete baseWindow;
		baseWindow = nullptr;
		return false;
	}
	baseWindow->genFramebuffer();

	inited = true;
	return true;
}

int wsDeinit() {
	if (!inited) {
		wsSetError(WS_ERR_NOT_INITIALIZED);
		return false;
	}

	if (baseWindow) {
		baseWindow->deleteWindow();
		delete baseWindow;
		baseWindow = nullptr;
	}

	glfwTerminate();

	inited = false;
	return true;
}