#include "wsWindow.h"

extern std::map<GLFWwindow*, wsBaseWindow*> baseWindows;

void wsKeyboardReceiver(GLFWwindow* window, int key, int scancode, int action, int mods) {
	baseWindows[window]->keyboardReceiver(key, scancode, action, mods);
}

void wsCharReceiver(GLFWwindow* window, unsigned key, int mods) {
	baseWindows[window]->charReceiver(key, mods);
}

void wsMouseReceiver(GLFWwindow* window, int button, int action, int mods) {
	baseWindows[window]->mouseReceiver(button, action, mods);
}

void wsCursorMoveReceiver(GLFWwindow* window, double xpos, double ypos) {
	baseWindows[window]->cursorMoveReceiver(xpos, ypos);
}

void wsCursorEnterReceiver(GLFWwindow* window, int entered) {
	baseWindows[window]->cursorEnterReceiver(int(entered == GLFW_TRUE));
}

void wsScrollReceiver(GLFWwindow* window, double xoffset, double yoffset) {
	baseWindows[window]->scrollReceiver(xoffset, yoffset);
}

void wsWindowResizeReceiver(GLFWwindow* window, int w, int h) {
	baseWindows[window]->windowResizeReceiver(wsCoord2{ w, h });
}

void wsWindowMoveReceiver(GLFWwindow* window, int x, int y) {
	baseWindows[window]->windowMoveReceiver(wsCoord2{ x, y });
}

void wsWindowCloseReceiver(GLFWwindow* window) {
	baseWindows[window]->windowCloseReceiver();
	//	baseWindows[window]->deleteWindow();
	wsDeinit();
}

void wsWindowFocusReceiver(GLFWwindow* window, int focused) {
	extern thread_local wsWindow* currentBaseWindow;
	wsWindow *newWindow = baseWindows[window];
	if(focused && currentBaseWindow != newWindow) {
		//mutex
		currentBaseWindow = baseWindows[window];
		currentBaseWindow->windowFocusReceiver(int(focused == GLFW_TRUE));
	} else {
		baseWindows[window]->windowFocusReceiver(int(focused == GLFW_TRUE));
	}
}

void wsFileDropReceiver(GLFWwindow* window, int count, const char **filename) {
	baseWindows[window]->fileDropReceiver(count, filename);
}

struct windowHintData { int hint, value; };
static wsList<windowHintData> windowHintLists;
int wsSetWindowHint(int hint, int value) {
	extern int inited;
	if(inited) {
		wsSetError(WS_ERR_ILLEGAL_OPERATION);
		return false;
	}
	windowHintLists.addBack(windowHintData{hint, value});
	return true;
}

int createGLFWwindow(wsBaseWindow* window) {
	for(auto* data = windowHintLists.front(); data; data = data->next)
		glfwWindowHint(data->data.hint, data->data.value);

	window->glfwwindow = glfwCreateWindow(window->size.x, window->size.y, window->name, nullptr, nullptr);

	if(window->glfwwindow == nullptr) return false;
	//if(contextInUse)	//unimplemented
	glfwMakeContextCurrent(window->glfwwindow);
	glfwSetKeyCallback(window->glfwwindow, wsKeyboardReceiver);
	glfwSetCharModsCallback(window->glfwwindow, wsCharReceiver);
	glfwSetMouseButtonCallback(window->glfwwindow, wsMouseReceiver);
	glfwSetCursorPosCallback(window->glfwwindow, wsCursorMoveReceiver);
	glfwSetCursorEnterCallback(window->glfwwindow, wsCursorEnterReceiver);
	glfwSetScrollCallback(window->glfwwindow, wsScrollReceiver);
	glfwSetWindowSizeCallback(window->glfwwindow, wsWindowResizeReceiver);
	glfwSetWindowPosCallback(window->glfwwindow, wsWindowMoveReceiver);
	glfwSetWindowCloseCallback(window->glfwwindow, wsWindowCloseReceiver);
	glfwSetWindowFocusCallback(window->glfwwindow, wsWindowFocusReceiver);
	glfwSetDropCallback(window->glfwwindow, wsFileDropReceiver);
	glfwSwapInterval(1);

	baseWindows.insert(std::make_pair(window->glfwwindow, window));
	glfwSetWindowPos(window->glfwwindow, window->position.x > 0 ? window->position.x : 1,
							 window->position.y > 0 ? window->position.y : 1);

	return true;
}