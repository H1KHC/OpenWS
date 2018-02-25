#include "wsWindowManager.h"
#include <GLFW/glfw3.h>
#include "checkInitAndFindWindow.h"

int wsSetJoystickCallback(wsJoystickCallback func) {
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
int wsSetMouseButtonCallback(int windowID, wsMouseButtonCallback callback) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	window->mouseButtonCallback = callback;
	return true;
}
int wsSetCursorPosCallback(int windowID, wsCursorPosCallback callback) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	window->cursorMoveCallback = callback;
	return true;
}
int wsSetCursorEnterCallback(int windowID, wsCursorEnterCallback callback) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	window->cursorEnterCallback = callback;
	return true;
}
int wsSetScrollCallback(int windowID, wsScrollCallback callback) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	window->scrollCallback = callback;
	return true;
}
int wsSetKeyCallback(int windowID, wsKeyCallback callback) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	window->keyboardCallback = callback;
	return true;
}
int wsSetCharModsCallback(int windowID, wsCharModsCallback callback) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	window->charCallback = callback;
	return true;
}
int wsSetDropCallback(int windowID, wsFileDropCallback callback) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	window->fileDropCallback = callback;
	return true;
}
int wsSetWindowPosCallback(int windowID, wsWindowPosCallback callback) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	window->windowMoveCallback = callback;
	return true;
}
int wsSetWindowSizeCallback(int windowID, wsWindowsizeCallback callback) {
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