// memcpy
#include <string.h>

#include "wsWindow.h"
#include "wsWindowManager.h"
#include "checkInitAndFindWindow.h"

extern thread_local wsWindow *currentWindow;

int wsGetForegroundWindow() {
	extern thread_local wsBaseWindow* currentBaseWindow;
	checkInit(WS_INVALID_WINDOW_ID);
	wsWindow *window = currentBaseWindow;
	while (window && !window->getSubWindowList()->isEmpty())
		window = window->getSubWindowList()->front()->data;
	if (!window) {
		wsSetError(WS_ERR_UNKNOWN_ERROR);
		return WS_INVALID_WINDOW_ID;
	}
	return window->windowID;
}
int wsGetWindowUnderCursor() {
	extern thread_local wsBaseWindow* currentBaseWindow;
	checkInit(WS_INVALID_WINDOW_ID);
	wsWindow *window = currentBaseWindow;
	while (window && window->getWindowUnderCursor())
		window = window->getWindowUnderCursor()->data;
	if (!window) {
		wsSetError(WS_ERR_UNKNOWN_ERROR);
		return WS_INVALID_WINDOW_ID;
	}
	return window->windowID;
}
int wsGetWindowOnPos(int x, int y) {
	extern thread_local wsBaseWindow* currentBaseWindow;
	checkInit(WS_INVALID_WINDOW_ID);
	wsWindow *window = currentBaseWindow;
	do {
		wsListNode<wsWindow*> *node;
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

int wsGetWindowByName(const char * name) {
	wsWindow *window;
	checkInit(WS_INVALID_WINDOW_ID);
	window = windowManager.findWindowByName(name);
	if (window) return window->windowID;
	wsSetError(WS_ERR_INVALID_VALUE);
	return WS_INVALID_WINDOW_ID;
}

int wsGetWindowInfo(int windowID, wsWindowInfo * info) {
	wsWindow *window;
	if(windowID == WS_ROOT_WINDOW_ID) {
		wsSetError(WS_ERR_ILLEGAL_OPERATION);
		return false;
	}
	checkInitAndFindWindow(window, windowID, false);
	if (info) *info = *window;
	return true;
}

int wsGetFatherWindow(int windowID) {
	wsWindow *window;
	if(windowID == WS_ROOT_WINDOW_ID) {
		wsSetError(WS_ERR_ILLEGAL_OPERATION);
		return false;
	}
	checkInitAndFindWindow(window, windowID, WS_INVALID_WINDOW_ID);
	if (window->getFatherWindow()) return window->getFatherWindow()->windowID;
	wsSetError(WS_ERR_INVALID_VALUE);
	return WS_INVALID_WINDOW_ID;
}

int wsGetTopSubWindow(int windowID) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, WS_INVALID_WINDOW_ID);
	if (window->getSubWindowList()->isEmpty()) {
		wsSetError(WS_ERR_INVALID_VALUE);
		return WS_INVALID_WINDOW_ID;
	}
	return window->getSubWindowList()->front()->data->windowID;
}

int wsGetPrevWindow(int windowID) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	const wsWindowListNode* node = window->getThisNode();
	if (node->last) return node->last->data->windowID;
	wsSetError(WS_ERR_WINDOW_NOT_FOUND);
	return WS_INVALID_WINDOW_ID;
}

int wsGetNextWindow(int windowID) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	const wsWindowListNode* node = window->getThisNode();
	if (node->next) return node->next->data->windowID;
	wsSetError(WS_ERR_WINDOW_NOT_FOUND);
	return WS_INVALID_WINDOW_ID;
}

int wsGetWindowSize(int windowID, int *w, int *h) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	if (w) *w = window->size.x;
	if (h) *h = window->size.y;
	return true;
}
int wsGetWindowPos(int windowID, int *x, int *y) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	if (x) *x = window->position.x;
	if (y) *y = window->position.y;
	return true;
}
int wsGetWindowStyle(int windowID, int * style) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	if (style) *style = window->styleMask;
	return true;
}
int wsGetWindowData(int windowID, void **data) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	*data = window->userData;
	return true;
}
int wsGetWindowCursorPos(int windowID, int *x, int *y) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	if (x) *x = window->cursorPos.x;
	if (y) *y = window->cursorPos.y;
	return true;
}

GLFWwindow* wsGetGLFWWindow(int windowID) {
	extern thread_local wsBaseWindow* currentBaseWindow;
	wsWindow *window;
	checkInit(nullptr);
	if(windowID != WS_ROOT_WINDOW_ID)
		findWindow(window, windowID, nullptr);
	else window = currentBaseWindow;
	return window->topWindow->glfwWindow;
}