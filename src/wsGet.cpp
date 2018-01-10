// memcpy
#include <string.h>

#include "wsWindow.h"
#include "wsWindowManager.h"

extern int inited;
extern wsWindow *currentWindow;

#define checkInit(failReturnValue) do {\
		if (!inited) {\
			wsSetError(WS_ERR_NOT_INITIALIZED);\
			return (failReturnValue);\
		}\
	}while(0)
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

WS_API int wsGetForegroundWindow() {
	wsWindow *window = baseWindow;
	checkInit(WS_INVALID_WINDOW_ID);
	while (window && !window->getSubWindowList()->isEmpty())
		window = window->getSubWindowList()->front()->data;
	if (!window) {
		wsSetError(WS_ERR_UNKNOWN_ERROR);
		return WS_INVALID_WINDOW_ID;
	}
	return window->windowID;
}
WS_API int wsGetWindowUnderCursor() {
	wsWindow *window = baseWindow;
	checkInit(WS_INVALID_WINDOW_ID);
	while (window && window->getWindowUnderCursor())
		window = window->getWindowUnderCursor()->data;
	if (!window) {
		wsSetError(WS_ERR_UNKNOWN_ERROR);
		return WS_INVALID_WINDOW_ID;
	}
	return window->windowID;
}
WS_API int wsGetWindowOnPos(int x, int y) {
	wsListNode<wsWindow*> *node;
	wsWindow *window = baseWindow;
	checkInit(WS_INVALID_WINDOW_ID);
	do {
		for (node = window->getSubWindowList()->front(); node; node = node->next) {
			if (node->data->pointIn({x, y})) {
				x -= node->data->position.x;
				y -= node->data->position.y;
				window = node->data;
				break;
			}
		}
		if (!node) return window->windowID;
	} while (window);
	wsSetError(WS_ERR_UNKNOWN_ERROR);
	return WS_INVALID_WINDOW_ID;
}

WS_API int wsGetWindowByName(const char * name) {
	wsWindow *window;
	checkInit(WS_INVALID_WINDOW_ID);
	window = windowManager.findWindowByName(name);
	if (window) return window->windowID;
	wsSetError(WS_ERR_INVALID_VALUE);
	return WS_INVALID_WINDOW_ID;
}

WS_API int wsGetWindowInfo(int windowID, wsWindowInfo * info) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	if (info) *info = *window;
	return true;
}

WS_API int wsGetFatherWindow(int windowID) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, WS_INVALID_WINDOW_ID);
	if (window->getFather()) return window->getFather()->windowID;
	wsSetError(WS_ERR_INVALID_VALUE);
	return WS_INVALID_WINDOW_ID;
}

WS_API int wsGetTopSubWindow(int windowID) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, WS_INVALID_WINDOW_ID);
	if (window->getSubWindowList()->isEmpty()) {
		wsSetError(WS_ERR_INVALID_VALUE);
		return WS_INVALID_WINDOW_ID;
	}
	return window->getSubWindowList()->front()->data->windowID;
}

WS_API int wsGetPrevWindow(int windowID) {
	wsWindow *window;
	wsListNode<wsWindow *> *node;
	checkInitAndFindWindow(window, windowID, false);
	node = window->getThisNode();
	if (node->last) return node->last->data->windowID;
	wsSetError(WS_ERR_WINDOW_NOT_FOUND);
	return WS_INVALID_WINDOW_ID;
}

WS_API int wsGetNextWindow(int windowID) {
	wsWindow *window;
	wsListNode<wsWindow *> *node;
	checkInitAndFindWindow(window, windowID, false);
	node = window->getThisNode();
	if (node->next) return node->next->data->windowID;
	wsSetError(WS_ERR_WINDOW_NOT_FOUND);
	return WS_INVALID_WINDOW_ID;
}

WS_API int wsGetWindowCallbacks(int windowID, wsWindowCallbacks * windowCallbacks) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	memcpy(windowCallbacks, &window->callbacks, sizeof(wsWindowCallbacks));
	return int();
}
WS_API int wsGetWindowSize(int windowID, int *w, int *h) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	if (w) *w = window->size.x;
	if (h) *h = window->size.y;
	return true;
}
WS_API int wsGetWindowPos(int windowID, int *x, int *y) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	if (x) *x = window->position.x;
	if (y) *y = window->position.y;
	return true;
}
WS_API int wsGetWindowStyle(int windowID, int * style) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	if (style) *style = window->styleMask;
	return true;
}
WS_API int wsGetWindowData(int windowID, void **data) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	*data = window->userData;
	return true;
}
WS_API int wsGetWindowCursorPos(int windowID, int *x, int *y) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	if (x) *x = window->cursorPos.x;
	if (y) *y = window->cursorPos.y;
	return true;
}

WS_API GLFWwindow* wsGetGLFWWindow() {
	checkInit(nullptr);
	return baseWindow->glfwwindow;
}
#undef checkInitAndFindWindow
#undef checkInit
