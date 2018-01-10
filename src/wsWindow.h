#ifndef __WS_WINDOW__
#define __WS_WINDOW__
#include <GL/glew.h>
#include <openWS.h>
#include <GLFW/glfw3.h>
#include "wsList.h"

void wsSetError(const int err);

typedef class wsWindow : public wsWindowInfo {
public:
	typedef wsListNode<wsWindow *> wsWindowListNode;
protected:
	unsigned framebuffer, depthbuffer;
	wsWindow *fatherWindow;
	wsList<wsWindow *> subWindow;
	wsWindowListNode *windowUnderCursor, *thisWindowNode;

	friend class wsWindowManager;
	
	friend int wsSetWindowDisplayCallback(int windowID, wsDisplayCallback callback);
	friend int wsCreateWindow(int windowStyle, const char *windowName, int x, int y, int width, int height, void *windowData, int fatherWindowID);
	friend int wsAttachWindow(int subwindowID, int fatherWindowID);
	friend int wsFocusWindow(int windowID);

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

	void makeContextCurrent() {
		extern wsWindow *currentWindow;
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glPushMatrix();
		glViewport(0, 0, bufferSize.x, bufferSize.y);
		currentWindow = this;
	}
	void freeContext() {
		extern wsWindow *currentWindow;
		currentWindow = nullptr;
		glPopMatrix();
	}
} wsWindow;

typedef struct wsBaseWindow : public wsWindow {
	GLFWwindow *glfwwindow;
	wsBaseWindow(const char *title, const wsCoord2 position, const wsCoord2 size)
		:wsWindow(title, position, size, WS_STYLE_DEFAULT) {};
} wsBaseWindow;

extern wsBaseWindow *baseWindow;

#endif
