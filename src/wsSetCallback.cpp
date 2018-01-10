#include "wsWindowManager.h"
#include <GLFW/glfw3.h>

extern wsWindow *currentWindow;
extern int inited;
#define checkInit(failReturnValue) do {\
		if (!inited) {\
			wsSetError(WS_ERR_NOT_INITIALIZED);\
			return (failReturnValue);\
		}\
	}while(0)

int wsSetJoystickConnectionCallback(void(*func)(int joystickID, int joystickState)) {
	checkInit(false);
	glfwSetJoystickCallback(func);
	return true;
}

static void(*iconifyCallback)(int iconified);
void windowIconifyReceiver(GLFWwindow *, int iconified) {
	iconifyCallback(iconified == GLFW_TRUE);
}

int wsSetWindowIconifyCallback(void(*func)(int iconified)) {
	checkInit(false);
	if (func)glfwSetWindowIconifyCallback(baseWindow->glfwwindow, windowIconifyReceiver);
	else glfwSetWindowIconifyCallback(baseWindow->glfwwindow, nullptr);
	return true;
}

#define checkInitAndFindWindow(window, id, failReturnValue) do{\
		checkInit(failReturnValue);\
		if(currentWindow && currentWindow->windowID == (id))\
				(window) = currentWindow;\
		else (window) = windowManager.findWindow(id);\
		if (!(window)) {\
			wsSetError(WS_ERR_WINDOW_NOT_FOUND);\
			return (failReturnValue);\
		}\
	}while(0)

int wsSetWindowDisplayCallback(int windowID, wsDisplayCallback callback) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	if(window->styleMask & WS_STYLE_NO_FRAMEBUFFER && window->framebuffer) {
		window->makeContextCurrent();
		callback(windowID, window->framebuffer);
		window->freeContext();
		if(!(window->styleMask & WS_STYLE_NO_DEPTHBUFFER) && window->depthbuffer)
			glDeleteRenderbuffers(1, &window->depthbuffer);
		glDeleteFramebuffers(1, &window->framebuffer);
	} else {
		window->displayCallback = callback;
	}
	return true;
}
int wsSetWindowMouseButtonCallback(int windowID, wsMouseButtonCallback callback) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	window->mouseButtonCallback = callback;
	return true;
}
int wsSetWindowCursorMoveCallback(int windowID, wsCursorMoveCallback callback) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	window->cursorMoveCallback = callback;
	return true;
}
int wsSetWindowCursorEnterCallback(int windowID, wsCursorEnterCallback callback) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	window->cursorEnterCallback = callback;
	return true;
}
int wsSetWindowScrollCallback(int windowID, wsScrollCallback callback) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	window->scrollCallback = callback;
	return true;
}
int wsSetWindowKeyboardCallback(int windowID, wsKeyboardCallback callback) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	window->keyboardCallback = callback;
	return true;
}
int wsSetWindowCharCallback(int windowID, wsCharCallback callback) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	window->charCallback = callback;
	return true;
}
int wsSetWindowFileDropCallback(int windowID, wsFileDropCallback callback) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	window->fileDropCallback = callback;
	return true;
}
int wsSetWindowMoveCallback(int windowID, wsWindowMoveCallback callback) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	window->windowMoveCallback = callback;
	return true;
}
int wsSetWindowResizeCallback(int windowID, wsWindowResizeCallback callback) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	window->windowResizeCallback = callback;
	return true;
}
int wsSetWindowCloseCallback(int windowID, wsWindowCloseCallback callback) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	window->windowCloseCallback = callback;
	return true;
}
int wsSetWindowFocusCallback(int windowID, wsWindowFocusCallback callback) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	window->windowFocusCallback = callback;
	return true;
}