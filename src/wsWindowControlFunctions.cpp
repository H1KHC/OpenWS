#include "wsWindow.h"
#include "wsWindowManager.h"
#include "checkInitAndFindWindow.h"

int wsCreateWindow(const char *title, int x, int y, int width, int height,
				   void *windowData, int windowStyle, int fatherWindowID) {
	extern thread_local wsBaseWindow* currentBaseWindow;
	extern thread_local wsWindow* currentWindow;

	checkInit(WS_INVALID_WINDOW_ID);

	if(fatherWindowID == WS_ROOT_WINDOW_ID) {
		int createGLFWwindow(wsBaseWindow*);

		wsBaseWindow *newWindow =
			new wsBaseWindow(title,
							 wsCoord2{ x, y },
							 wsCoord2{ width, height },
							 windowStyle);

		if(!createGLFWwindow(newWindow)) {
			if(newWindow->glfwWindow)
				glfwSetWindowShouldClose(newWindow->glfwWindow, 1);
			delete newWindow;
			wsSetError(WS_ERR_FRAMEBUFFER_GENERATE_FAILED);
			return WS_INVALID_WINDOW_ID;
		}
		int wsInitGLEW();
		wsInitGLEW();
		if(!newWindow->genFramebuffer()) {
			extern std::map<GLFWwindow*, wsBaseWindow*> baseWindows;
			glfwSetWindowShouldClose(newWindow->glfwWindow, 1);
			baseWindows[newWindow->glfwWindow] = nullptr;
			delete newWindow;
			newWindow = nullptr;
			wsSetError(WS_ERR_FRAMEBUFFER_GENERATE_FAILED);
			return WS_INVALID_WINDOW_ID;
		}
		currentBaseWindow = newWindow;
		currentWindow = newWindow;
		newWindow->topWindow = newWindow;
		newWindow->userData = windowData;
		return newWindow->windowID;
	} else {
		wsWindow *toBeAttached, *newWindow;
		findWindow(toBeAttached, fatherWindowID, WS_INVALID_WINDOW_ID);

		newWindow = new wsWindow(title,
								 wsCoord2{ x, y },
								 wsCoord2{ width, height },
								 windowStyle);

		newWindow->fatherWindow = toBeAttached;
		newWindow->userData = windowData;
		if(currentBaseWindow != toBeAttached->topWindow) {
			int wsInitGLEW();
			glfwMakeContextCurrent(toBeAttached->topWindow->glfwWindow);
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
	findWindow(subWindow, subwindowID, false);
	
	if(fatherWindowID == WS_ROOT_WINDOW_ID) {
		if(subWindow->fatherWindow == nullptr) return true;
		wsBaseWindow newBaseWindow = subWindow;
		if(!createGLFWwindow(newBaseWindow)) return false;
		else {
			extern std::map<GLFWwindow*, wsBaseWindow*> baseWindows;
			baseWindows.insert(std::make_pair(newBaseWindow->glfwWindow, newBaseWindow));
		} return true;
	}
	
	findWindow(fatherWindow, fatherWindowID, false);
	if(subWindow->fatherWindow == nullptr) {
		wsSetError(WS_ERR_UNIMPLEMENTED);
		return false;
	}
	subWindow->fatherWindow->subWindow.deleteNode(subWindow);
	fatherWindow->subWindow.addBack(subWindow);
	subWindow->fatherWindow = fatherWindow;
	subWindow->topWindow = fatherWindow->topWindow;
	subWindow->topWindowChangeTag = true;
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