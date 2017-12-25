#include "wsWindow.h"
#include "wsWindowManager.h"
#include "wsClock.h"

extern int inited;
extern wsWindow *currentWindow;

#ifdef _DEBUG
int wsSetDebugMode(int mode) {
	extern int debugOutput;
	if (mode ^ WS_SDM_FULL) {
		wsSetError(WS_ERR_INVALID_VALUE);
		return false;
	}
	debugOutput = mode;
	return true;
}
#endif

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


WS_API int wsPostRedisplay() {
	extern int wsNeedRedisplay, fpsControl;
	extern int wsFPS;
	checkInit(false);
	if(fpsControl == true && wsFPS == 0) return wsNeedRedisplay = true;
	else wsSetError(WS_ERR_ILLEGAL_OPERATION);
	return false;
}

WS_API int wsSetSwapInterval(int interval) {
	extern int fpsControl;
	extern wsClock *fpsClock;
	checkInit(false);
	fpsControl = false;
	if (fpsClock) delete fpsClock, fpsClock = nullptr;
	glfwSwapInterval(interval);
	return true;
}

WS_API int wsSetFPS(int fps) {
	extern int wsFPS;
	extern int fpsControl;
	extern wsClock *fpsClock;
	checkInit(false);
	fpsControl = true;
	glfwSwapInterval(0);
	wsFPS = fps;
	if (fpsClock == nullptr) {
		fpsClock = new wsClock;
	}
	return true;
}

WS_API int wsSetWindowCallbacks(int windowID, const wsWindowCallbacks * windowCallbacks, int mode) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	switch (mode) {
	case WS_SWC_COVER:
		memcpy(&window->callbacks, windowCallbacks, sizeof(*windowCallbacks));
		break;
	case WS_SWC_ADD:
#define CALLBACK_ADD(callbackName) \
		do{\
			if(!window->callbacks.callbackName)\
				window->callbacks.callbackName = windowCallbacks->callbackName;\
		}while(0)
		CALLBACK_ADD(displayCallback);
		CALLBACK_ADD(mouseCallback);
		CALLBACK_ADD(cursorMoveCallback);
		CALLBACK_ADD(cursorEnterCallback);
		CALLBACK_ADD(scrollCallback);
		CALLBACK_ADD(keyboardCallback);
		CALLBACK_ADD(charCallback);
		CALLBACK_ADD(fileDropCallback);
		CALLBACK_ADD(windowMoveCallback);
		CALLBACK_ADD(windowResizeCallback);
		CALLBACK_ADD(windowCloseCallback);
		CALLBACK_ADD(windowFocusCallback);
#undef CALLBACK_ADD
		break;
	case WS_SWC_UPDATE:
#define CALLBACK_UPDATE(callbackName) \
		do{\
			if(windowCallbacks->callbackName)\
				window->callbacks.callbackName = windowCallbacks->callbackName;\
		}while(0)
		CALLBACK_UPDATE(displayCallback);
		CALLBACK_UPDATE(mouseCallback);
		CALLBACK_UPDATE(cursorMoveCallback);
		CALLBACK_UPDATE(cursorEnterCallback);
		CALLBACK_UPDATE(scrollCallback);
		CALLBACK_UPDATE(keyboardCallback);
		CALLBACK_UPDATE(charCallback);
		CALLBACK_UPDATE(fileDropCallback);
		CALLBACK_UPDATE(windowMoveCallback);
		CALLBACK_UPDATE(windowResizeCallback);
		CALLBACK_UPDATE(windowCloseCallback);
		CALLBACK_UPDATE(windowFocusCallback);
#undef CALLBACK_UPDATE
		break;
	default:
		wsSetError(WS_ERR_INVALID_VALUE);
		return false;
	}
	return true;
}
WS_API int wsSetWindowSize(int windowID, double w, double h, int mode) {
	wsWindow *window;
	wsCoord2 target;
	checkInitAndFindWindow(window, windowID, false);

	if (window->styleMask & WS_STYLE_NORESIZE) {
		wsSetError(WS_ERR_ILLEGAL_OPERATION);
		return false;
	}
	switch (mode & WS_SWSI_MOVEMODE_MASK) {
	case WS_SWSI_ABSOLUTE:
		target = { (int)w, (int)h };
		break;
	case WS_SWSI_RELATIVE:
		target = { window->size.x + (int)w, window->size.y + (int)h };
		break;
	case WS_SWSI_PROPORTIONAL:
		target = { (int)(window->size.x * w), (int)(window->size.y * h) };
		break;
	default:
		wsSetError(WS_ERR_INVALID_VALUE);
		return false;
	}
	switch (window->styleMask & WS_STYLE_ALIGN_MASK) {
	case WS_STYLE_ALIGN_LU:
		break;
	case WS_STYLE_ALIGN_LD:
		window->position = { window->position.x, window->position.y - target.y + window->size.y };
		break;
	case WS_STYLE_ALIGN_RU:
		window->position = { window->position.x - target.x + window->size.x, window->position.y };
		break;
	case WS_STYLE_ALIGN_RD:
		window->position = { window->position.x - target.x + window->size.x, window->position.y - target.y + window->size.y };
		break;
	}
	window->windowResizeReceiver(target);
	return true;
}
WS_API int wsSetWindowBufferSize(int windowID, double w, double h, int mode) {
	wsWindow *window;
	wsCoord2 target;
	checkInitAndFindWindow(window, windowID, false);

	if (!(window->styleMask & WS_STYLE_NO_BUFFER_RESIZE)) {
		wsSetError(WS_ERR_ILLEGAL_OPERATION);
		return false;
	}
	switch (mode & WS_SWSI_MOVEMODE_MASK) {
	case WS_SWSI_ABSOLUTE:
		target = { (int)w, (int)h };
		break;
	case WS_SWSI_RELATIVE:
		target = { window->size.x + (int)w, window->size.y + (int)h };
		break;
	case WS_SWSI_PROPORTIONAL:
		target = { (int)(window->size.x * w), (int)(window->size.y * h) };
		break;
	default:
		wsSetError(WS_ERR_INVALID_VALUE);
		return false;
	}
	int adjustFramebufferSize(GLuint frameBuffer, GLuint depthBuffer, GLuint texture, const int width, const int height);
	if (!adjustFramebufferSize(window->getFramebuffer(), window->getDepthbuffer(), window->texture, target.x, target.y)) {
		wsSetError(WS_ERR_BUFFER_RESIZE_FAILED);
		return false;
	}
	window->bufferSize = target;

	return true;
}
WS_API int wsSetWindowPos(int windowID, int x, int y, int mode) {
	wsCoord2 target;
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	if (window->styleMask & WS_STYLE_NOMOVE) {
		wsSetError(WS_ERR_ILLEGAL_OPERATION);
		return false;
	}
	switch (mode) {
	case WS_SWP_ABSOLUTE:
		target = wsCoord2{ x, y };
		break;
	case WS_SWP_RELATIVE:
		target = { window->position.x + x, window->position.y + y };
		break;
	default:
		wsSetError(WS_ERR_INVALID_VALUE);
		return false;
	}

	if (windowID != 0 && (target.x > window->getFather()->size.x || target.y > window->getFather()->size.y)) {
		wsSetError(WS_ERR_INVALID_VALUE);
		return false;
	}
	window->windowMoveReceiver(target);
	return true;
}
WS_API int wsSetWindowStyle(int windowID, int style, int mode) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	switch (mode) {
	case WS_SWST_COVER:
		window->styleMask = style;
		break;
	case WS_SWST_ADD:
		window->styleMask |= style;
		break;
	case WS_SWST_UPDATE:
		window->styleMask &= style;
		break;
	default:
		wsSetError(WS_ERR_INVALID_VALUE);
		return false;
	}
	return true;
}
WS_API int wsSetWindowData(int windowID, void * data) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	window->userData = data;
	return true;
}
#undef checkInitAndFindWindow
#undef checkInit
