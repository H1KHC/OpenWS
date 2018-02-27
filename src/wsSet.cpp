#include "wsWindow.h"
#include "wsWindowManager.h"
#include "wsClock.h"
#include "checkInitAndFindWindow.h"

int wsSetDebugMode(int mode) {
	extern int debugOutput;
	if (mode ^ WS_SDM_FULL) {
		wsSetError(WS_ERR_INVALID_VALUE);
		return false;
	}
	debugOutput = mode;
	return true;
}

int wsPostRedisplay(int windowID) {
	wsWindow* window;
	checkInitAndFindWindow(window, windowID, false);
	if(window->postRedisplay())
		return true;
	else {
		wsSetError(WS_ERR_ILLEGAL_OPERATION);
		return false;
	}
}

int wsSetSwapInterval(int windowID, int interval) {
	extern thread_local wsBaseWindow* currentBaseWindow;
	wsWindow* window;
	checkInitAndFindWindow(window, windowID, false);
	if(window->getFatherWindow() != nullptr) {
		wsSetError(WS_ERR_UNIMPLEMENTED);
		return false;
	}

	if(currentBaseWindow != window)
		glfwMakeContextCurrent(((wsBaseWindow*)window)->glfwWindow);

	glfwSwapInterval(interval);

	if(currentBaseWindow != window) {
		int wsInitGLEW();
		glfwMakeContextCurrent(currentBaseWindow->glfwWindow);
		wsInitGLEW();
	}
	return true;
}

int wsSetFPS(int windowID, int fps) {
	extern thread_local wsBaseWindow* currentBaseWindow;
	wsWindow* window;
	checkInitAndFindWindow(window, windowID, false);
	if(window->getFatherWindow() != nullptr) {
		wsSetError(WS_ERR_UNIMPLEMENTED);
		return false;
	}
	if(currentBaseWindow != window)
		glfwMakeContextCurrent(((wsBaseWindow*)window)->glfwWindow);

	glfwSwapInterval(0);

	if(currentBaseWindow != window) {
		int wsInitGLEW();
		glfwMakeContextCurrent(currentBaseWindow->glfwWindow);
		wsInitGLEW();
	}
	window->setFPS(fps);
	return true;
}

int wsSetWindowSize(int windowID, double w, double h, int mode) {
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
int wsSetWindowBufferSize(int windowID, double w, double h, int mode) {
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
int wsSetWindowPos(int windowID, int x, int y, int mode) {
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

	if (window->getFatherWindow() != nullptr && (target.x > window->getFatherWindow()->size.x || target.y > window->getFatherWindow()->size.y)) {
		wsSetError(WS_ERR_INVALID_VALUE);
		return false;
	}
	if(window->getFatherWindow() == nullptr) {
		glfwSetWindowPos(((wsBaseWindow*)window)->glfwWindow, target.x, target.y);
	} else window->windowMoveReceiver(target);
	return true;
}
int wsSetWindowStyle(int windowID, int style, int mode) {
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
int wsSetWindowUserPointer(int windowID, void * data) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	window->userData = data;
	return true;
}
