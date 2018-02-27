#include "wsWindowManager.h"
#include <GLFW/glfw3.h>
#include "checkInitAndFindWindow.h"

wsJoystickCallback wsSetJoystickCallback(wsJoystickCallback func) {
	checkInit(nullptr);
	return glfwSetJoystickCallback(func);
}

wsDisplayCallback wsSetDisplayCallback(int windowID, wsDisplayCallback callback) {
	wsWindow *window;
	wsDisplayCallback prev;
	checkInitAndFindWindow(window, windowID, nullptr);
	if(window->styleMask & WS_STYLE_NO_FRAMEBUFFER && window->framebuffer) {
		window->makeContextCurrent();
		callback(windowID, window->framebuffer);
		window->freeContext();
		if(!(window->styleMask & WS_STYLE_NO_DEPTHBUFFER) && window->depthbuffer)
			glDeleteRenderbuffers(1, &window->depthbuffer);
		glDeleteFramebuffers(1, &window->framebuffer);
		return nullptr;
	} else {
		prev = window->displayCallback;
		window->displayCallback = callback;
		return prev;
	}
}

wsMouseButtonCallback wsSetMouseButtonCallback(int windowID, wsMouseButtonCallback callback) {
	wsWindow *window;
	wsMouseButtonCallback prev;
	checkInitAndFindWindow(window, windowID, nullptr);
	prev = window->mouseButtonCallback;
	window->mouseButtonCallback = callback;
	return prev;
}

wsCursorPosCallback wsSetCursorPosCallback(int windowID, wsCursorPosCallback callback) {
	wsWindow *window;
	wsCursorPosCallback prev;
	checkInitAndFindWindow(window, windowID, nullptr);
	prev = window->cursorMoveCallback;
	window->cursorMoveCallback = callback;
	return prev;
}
wsCursorEnterCallback wsSetCursorEnterCallback(int windowID, wsCursorEnterCallback callback) {
	wsWindow *window;
	wsCursorEnterCallback prev;
	checkInitAndFindWindow(window, windowID, nullptr);
	prev = window->cursorEnterCallback;
	window->cursorEnterCallback = callback;
	return prev;
}
wsScrollCallback wsSetScrollCallback(int windowID, wsScrollCallback callback) {
	wsWindow *window;
	wsScrollCallback prev;
	checkInitAndFindWindow(window, windowID, nullptr);
	prev = window->scrollCallback;
	window->scrollCallback = callback;
	return prev;
}
wsKeyCallback wsSetKeyCallback(int windowID, wsKeyCallback callback) {
	wsWindow *window;
	wsKeyCallback prev;
	checkInitAndFindWindow(window, windowID, nullptr);
	prev = window->keyboardCallback;
	window->keyboardCallback = callback;
	return prev;
}
wsCharModsCallback wsSetCharModsCallback(int windowID, wsCharModsCallback callback) {
	wsWindow *window;
	wsCharModsCallback prev;
	checkInitAndFindWindow(window, windowID, nullptr);
	prev = window->charCallback;
	window->charCallback = callback;
	return prev;
}
wsFileDropCallback wsSetDropCallback(int windowID, wsFileDropCallback callback) {
	wsWindow *window;
	wsFileDropCallback prev;
	checkInitAndFindWindow(window, windowID, nullptr);
	prev = window->fileDropCallback;
	window->fileDropCallback = callback;
	return prev;
}
wsWindowPosCallback wsSetWindowPosCallback(int windowID, wsWindowPosCallback callback) {
	wsWindow *window;
	wsWindowPosCallback prev;
	checkInitAndFindWindow(window, windowID, nullptr);
	prev = window->windowMoveCallback;
	window->windowMoveCallback = callback;
	return prev;
}
wsWindowsizeCallback wsSetWindowSizeCallback(int windowID, wsWindowsizeCallback callback) {
	wsWindow *window;
	wsWindowsizeCallback prev;
	checkInitAndFindWindow(window, windowID, nullptr);
	prev = window->windowResizeCallback;
	window->windowResizeCallback = callback;
	return prev;
}
wsWindowCloseCallback wsSetWindowCloseCallback(int windowID, wsWindowCloseCallback callback) {
	wsWindow *window;
	wsWindowCloseCallback prev;
	checkInitAndFindWindow(window, windowID, nullptr);
	prev = window->windowCloseCallback;
	window->windowCloseCallback = callback;
	return prev;
}
wsWindowFocusCallback wsSetWindowFocusCallback(int windowID, wsWindowFocusCallback callback) {
	wsWindow *window;
	wsWindowFocusCallback prev;
	checkInitAndFindWindow(window, windowID, nullptr);
	prev = window->windowFocusCallback;
	window->windowFocusCallback = callback;
	return prev;
}

wsWindowIconifyCallback wsSetWindowIconifyCallback(int windowID, wsWindowIconifyCallback callback) {
	void windowIconifyReceiver(GLFWwindow *window, int iconified);
	wsWindow* baseWindow;
	wsWindowIconifyCallback prev;
	checkInitAndFindWindow(baseWindow, windowID, nullptr);
	if(baseWindow->getFatherWindow() != nullptr) {
		wsSetError(WS_ERR_UNIMPLEMENTED);
		return nullptr;
	}
	prev = baseWindow->windowIconifyCallback;
	baseWindow->windowIconifyCallback = callback;
	glfwSetWindowIconifyCallback(((wsBaseWindow*)baseWindow)->glfwWindow,
		callback ? windowIconifyReceiver : nullptr);
	return prev;
}