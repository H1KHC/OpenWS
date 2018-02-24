#include "wsWindow.h"
#include "wsWindowManager.h"

extern int inited;
extern thread_local wsWindow *currentWindow;

#define checkInit(failReturnValue) do {\
		if (!inited) {\
			wsSetError(WS_ERR_NOT_INITIALIZED);\
			return (failReturnValue);\
		}\
	}while(0)
#define findWindow(window, id, failReturnValue) do{\
		if(currentWindow && currentWindow->windowID == (id))\
			(window) = currentWindow;\
		else (window) = windowManager.findWindow(id);\
		if (!(window)) {\
			wsSetError(WS_ERR_WINDOW_NOT_FOUND);\
			return (failReturnValue);\
		}\
	}while(0)
#define checkInitAndFindWindow(window, id, failReturnValue) do{\
		checkInit(failReturnValue);\
		findWindow(window, id, failReturnValue);\
	}while(0)

int wsCreateWindow(const char *windowName, int x, int y, int width, int height, void *windowData, int windowStyle, int fatherWindowID) {
	extern thread_local wsBaseWindow* currentBaseWindow;

	checkInit(WS_INVALID_WINDOW_ID);

	if(fatherWindowID == WS_ROOT_WINDOW_ID) {
		int createGLFWwindow(wsBaseWindow*);
		wsBaseWindow *newWindow = new wsBaseWindow(windowName, wsCoord2{ x, y }, wsCoord2{ width, height }, windowStyle);
		if(!createGLFWwindow(newWindow)) {
			if(newWindow->glfwwindow)
				glfwSetWindowShouldClose(newWindow->glfwwindow, 1);
			delete newWindow;
			wsSetError(WS_ERR_FRAMEBUFFER_GENERATE_FAILED);
			return WS_INVALID_WINDOW_ID;
		}
		int wsInitGLEW();
		wsInitGLEW();
		if(!newWindow->genFramebuffer()) {
			extern std::map<GLFWwindow*, wsBaseWindow*> baseWindows;
			glfwSetWindowShouldClose(newWindow->glfwwindow, 1);
			baseWindows[newWindow->glfwwindow] = nullptr;
			delete newWindow;
			newWindow = nullptr;
			wsSetError(WS_ERR_FRAMEBUFFER_GENERATE_FAILED);
			return WS_INVALID_WINDOW_ID;
		}
		currentBaseWindow = newWindow;
		currentWindow = newWindow;
		newWindow->topWindow = newWindow;
		return newWindow->windowID;
	} else {
		wsWindow *toBeAttached, *newWindow;
		findWindow(toBeAttached, fatherWindowID, WS_INVALID_WINDOW_ID);
		newWindow = new wsWindow(windowName, wsCoord2{ x, y }, wsCoord2{ width, height }, windowStyle);
		newWindow->fatherWindow = toBeAttached;
		newWindow->userData = windowData;
		if(currentBaseWindow != toBeAttached->topWindow) {
			int wsInitGLEW();
			glfwMakeContextCurrent(toBeAttached->topWindow->glfwwindow);
			currentBaseWindow = toBeAttached->topWindow;
			wsInitGLEW();
		}
		if(!newWindow->genFramebuffer()) {
			delete newWindow;
			wsSetError(WS_ERR_FRAMEBUFFER_GENERATE_FAILED);
			return WS_INVALID_WINDOW_ID;
		}
		newWindow->focused = true;
		toBeAttached->subWindow.addFront(newWindow);
		newWindow->thisWindowNode = toBeAttached->getSubWindowList()->front();
		currentBaseWindow = newWindow->topWindow;
		currentWindow = newWindow;
		newWindow->topWindow = toBeAttached->topWindow;
		return newWindow->windowID;
	}
}
int wsCloseWindow(int windowID) {
	wsWindow *window;
	if(windowID == WS_ROOT_WINDOW_ID) {
		wsSetError(WS_ERR_ILLEGAL_OPERATION);
		return false;
	}
	checkInitAndFindWindow(window, windowID, false);
	window->windowCloseReceiver();
	return true;
}
int wsAttachWindow(int subwindowID, int fatherWindowID) {
	wsWindow *subWindow, *fatherWindow;
	checkInit(false);
	if (subwindowID == WS_ROOT_WINDOW_ID || subwindowID == fatherWindowID) {
		wsSetError(WS_ERR_ILLEGAL_OPERATION);
		return false;
	}
	if(fatherWindowID == WS_ROOT_WINDOW_ID) {
		wsSetError(WS_ERR_UNIMPLEMENTED);
		return false;
	}
	findWindow(subWindow, subwindowID, false);
	findWindow(fatherWindow, fatherWindowID, false);
	if(subWindow->fatherWindow == nullptr ||
	  fatherWindow->fatherWindow == nullptr) {
		wsSetError(WS_ERR_UNIMPLEMENTED);
		return false;
	}
	subWindow->fatherWindow->subWindow.deleteNode(subWindow);
	fatherWindow->subWindow.addBack(subWindow);
	subWindow->fatherWindow = fatherWindow;
	subWindow->topWindow = fatherWindow->topWindow;
	return true;
}
int wsFocusWindow(int windowID) {
	wsWindow *window;
	if(windowID == WS_ROOT_WINDOW_ID) {
		wsSetError(WS_ERR_ILLEGAL_OPERATION);
		return false;
	}
	checkInitAndFindWindow(window, windowID, false);
	while(window->fatherWindow) {
		window->fatherWindow->focusWindow(window->thisWindowNode);
		window = window->fatherWindow;
	}
	return true;
}

#undef checkInitAndFindWindow
#undef findWindow
#undef checkInit