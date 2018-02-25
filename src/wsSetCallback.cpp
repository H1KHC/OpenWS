#include "wsWindowManager.h"
#include <GLFW/glfw3.h>
#include "checkInitAndFindWindow.h"

int wsSetJoystickConnectionCallback(void(*func)(int joystickID, int joystickState)) {
	checkInit(false);
	glfwSetJoystickCallback(func);
	return true;
}

void windowIconifyReceiver(GLFWwindow *window, int iconified) {
	extern std::map<GLFWwindow*, wsBaseWindow*> baseWindows;
	wsWindow* wswindow = baseWindows[window];
	wswindow->windowIconifyCallback(wswindow->windowID, iconified == GLFW_TRUE);
}

int wsSetWindowIconifyCallback(int windowID, wsWindowIconifyCallback callback) {
	wsWindow* baseWindow;
	checkInitAndFindWindow(baseWindow, windowID, false);
	if(baseWindow->getFatherWindow() != nullptr) {
		wsSetError(WS_ERR_UNIMPLEMENTED);
		return false;
	}
	baseWindow->windowIconifyCallback = callback;
	glfwSetWindowIconifyCallback(((wsBaseWindow*)baseWindow)->glfwWindow, nullptr);
	return true;
}

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