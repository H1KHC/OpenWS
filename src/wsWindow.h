#ifndef __WS_WINDOW__
#define __WS_WINDOW__
#include <map>
#include <GL/glew.h>
#include <openWS.h>
#include <GLFW/glfw3.h>
#include "wsList.h"
#include "wsClock.h"

void wsSetError(const int err);

class wsWindow;
typedef wsListNode<wsWindow *> wsWindowListNode;

struct wsBaseWindow;

typedef class wsWindow : public wsWindowInfo {
	wsClock *fpsClock;
	int fpsControl, FPS, needRedisplay, needRedraw;
	bool needFlush() {
		if (fpsControl) {
			if (FPS) {
				if (fpsClock->getPeriodSinceLastCall(false) * FPS <= 1000000000.0)
					return false;
				else {
					fpsClock->getPeriodSinceLastCall(true);
					return true;
				}
			} else {
				if (!needRedisplay)
					return false;
				else {
					needRedisplay = false;
					return true;
				}
			}
		} else return true;
	}
protected:
	unsigned framebuffer, depthbuffer;
	wsWindow *fatherWindow;
	wsBaseWindow *topWindow;
	wsList<wsWindow *> subWindow;
	wsWindowListNode *windowUnderCursor, *thisWindowNode;

	friend class wsWindowManager;
	
	friend int wsSetWindowDisplayCallback(int windowID, wsDisplayCallback callback);
	friend int wsSetWindowIconifyCallback(int windowID, wsWindowIconifyCallback callback);
	friend int wsCreateWindow(const char *windowName, int x, int y, int width, int height, void *windowData, int windowStyle, int fatherWindowID);
	friend int wsAttachWindow(int subwindowID, int fatherWindowID);
	friend int wsFocusWindow(int windowID);
	friend void wsFlush(wsWindow* baseWindow);
	friend GLFWwindow* wsGetGLFWWindow(int windowID);
	friend void wsMakeContextCurrent(wsWindow* window);

	// Must ensure that the window is his subwindow
	void focusWindow(wsWindowListNode *window) {
		if (window == subWindow.front()) return;
		if (subWindow.front()->data->focused)
			subWindow.front()->data->windowFocusReceiver(false);
		if(window) {
			subWindow.putFront(window);
			window->data->windowFocusReceiver(true);
		}
	}
public:
	unsigned getFramebuffer() { return (styleMask & WS_STYLE_NO_FRAMEBUFFER) ? framebuffer : 0; }
	unsigned getDepthbuffer() { return (styleMask & WS_STYLE_NO_DEPTHBUFFER) ? depthbuffer : 0; }
	wsList<wsWindow *>* getSubWindowList() { return &subWindow; };
	wsWindow* getFather() { return fatherWindow; }
	wsWindowListNode* getWindowUnderCursor() { return windowUnderCursor; }
	wsWindowListNode* getThisNode() { return thisWindowNode; }

	int pointIn(wsCoord2 point) {
		return point.x >= position.x && point.y >= position.y && point.x <= position.x + size.x && point.y <= position.y + size.y;
	}

	wsWindow() = delete;
	wsWindow(const char *title, const wsCoord2 position, const wsCoord2 size, const int style);
	~wsWindow();

	void display(int lastX, int lastY, int cutX, int cutY, int cutWidth, int cutHeight);

	void displayReceiver();
	int keyboardReceiver(int key, int scancode, int action, int mods);
	int charReceiver(unsigned key, int mods);
	int mouseReceiver(int button, int action, int mods);
	int cursorMoveReceiver(double xpos, double ypos);
	int scrollReceiver(double xoffset, double yoffset);
	int fileDropReceiver(int count, const char **filename);

	void cursorEnterReceiver(int entered);
	void windowResizeReceiver(wsCoord2 newSize);
	void windowMoveReceiver(wsCoord2 newPos);
	void windowCloseReceiver();
	void windowFocusReceiver(int focused);

	void focusWindowUnderCursor() {
		focusWindow(windowUnderCursor);
	}
	int updateCursorWindow() {
		for (wsWindowListNode *node = subWindow.front(); node; node = node->next) {
			if (node->data->styleMask & WS_STYLE_NOFOCUS) continue;
			if (node->data->pointIn(cursorPos)) {
				if (windowUnderCursor != node) {
					if (windowUnderCursor)
						windowUnderCursor->data->cursorEnterReceiver(false);
					windowUnderCursor = node;
					windowUnderCursor->data->cursorEnterReceiver(true);
					return true;
				}
				else return false;
			}
		}
		if (windowUnderCursor) {
			windowUnderCursor->data->cursorEnterReceiver(false);
			windowUnderCursor = nullptr;
		}
		return false;
	}

	bool genFramebuffer() {
		int generateFramebuffer(GLuint &frameBuffer, GLuint &depthBuffer, GLuint &texture, const int width, const int height);
		int generateFramebuffer(GLuint &frameBuffer, GLuint &texture, const int width, const int height);
		bufferSize = size;
		if(styleMask & WS_STYLE_NO_DEPTHBUFFER) {
			generateFramebuffer(framebuffer, texture, size.x, size.y);
			depthbuffer = 0;
		} else {
			generateFramebuffer(framebuffer, depthbuffer, texture, size.x, size.y);
		}
		return true;
	}

	void deleteWindow();

	inline void makeContextCurrent();
	void freeContext() {
		extern thread_local wsWindow *currentWindow;
		currentWindow = nullptr;
		glPopMatrix();
	}
} wsWindow;

typedef struct wsBaseWindow : public wsWindow {
	GLFWwindow *glfwwindow;
	wsBaseWindow(const char *title, const wsCoord2 position, const wsCoord2 size, int style)
		:wsWindow(title, position, size, style) {};
} wsBaseWindow;

inline void wsWindow::makeContextCurrent() {
	extern thread_local wsWindow *currentWindow;
	extern thread_local wsBaseWindow *currentBaseWindow;
	if(topWindow != currentBaseWindow) {
		int wsInitGLEW();
		glfwMakeContextCurrent(topWindow->glfwwindow);
		currentBaseWindow = topWindow;
		wsInitGLEW();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glPushMatrix();
	glViewport(0, 0, bufferSize.x, bufferSize.y);
	currentWindow = this;
}

#endif
