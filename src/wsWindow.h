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

//------------------------------- FPS Control -------------------------------//
protected:
	wsClock *fpsClock;
	int fpsControl, FPS, needRedraw, needRedisplay;
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
				if (!needRedraw)
					return false;
				else {
					needRedraw = false;
					return true;
				}
			}
		} else return true;
	}
public:

	void setFPS(int fps) {
		fpsControl = true;
		FPS = fps;
		if (fpsClock == nullptr)
			fpsClock = new wsClock;
	}

	bool postRedisplay() {
		if(!fpsControl || FPS != 0) return false;
		needRedraw = true;
		return true;
	}

//------------------------------- Framebuffer -------------------------------//
protected:
	unsigned framebuffer, depthbuffer;
	bool genFramebuffer() {
		int generateFramebuffer(GLuint &frameBuffer,
								GLuint &depthBuffer,
								GLuint &texture,
								const int width,
								const int height);

		int generateFramebuffer(GLuint &frameBuffer,
								GLuint &texture,
								const int width,
								const int height);

		bufferSize = size;
		if(styleMask & WS_STYLE_NO_DEPTHBUFFER) {
			generateFramebuffer(framebuffer, texture, size.x, size.y);
			depthbuffer = 0;
		} else {
			generateFramebuffer(framebuffer, depthbuffer, texture,
								size.x, size.y);
		}
		return true;
	}
	friend int wsCreateWindow(const char *windowName,
							  int x, int y, int width, int height,
							  void *windowData, int windowStyle,
							  int fatherWindowID);

	friend int wsSetWindowDisplayCallback(int windowID,
										  wsDisplayCallback callback);

public:
	unsigned getFramebuffer() {
		return (styleMask & WS_STYLE_NO_FRAMEBUFFER) ? 0 : framebuffer;
	}

	unsigned getDepthbuffer() {
		return (styleMask & WS_STYLE_NO_DEPTHBUFFER) ? 0 : depthbuffer;
	}

//----------------------------- Father and Root -----------------------------//
protected:
	wsWindow *fatherWindow;
	wsBaseWindow *topWindow;
	bool topWindowChangeTag;	//mutex need
	wsWindowListNode *thisWindowNode;

	void pushDownTopWindowChangeTag() {
		if(topWindowChangeTag) {
			for (auto *node = subWindow.front(); node; node = node->next) {
				if(node->data->topWindow != topWindow) {
					node->data->topWindow = topWindow;
					node->data->topWindowChangeTag = true;
				}
			}
			topWindowChangeTag = false;
		}
	}

	friend int wsAttachWindow(int subwindowID, int fatherWindowID);
	friend int wsFocusWindow(int windowID);
	friend GLFWwindow* wsGetGLFWWindow(int windowID);
public:
	const wsWindow* getFatherWindow() const { return fatherWindow; }
	const wsBaseWindow* getTopWindow() const { return topWindow; }
	const wsWindowListNode* getThisNode() const { return thisWindowNode; }

//------------------------------- Sub Windows -------------------------------//
	wsList<wsWindow *> subWindow;
	wsWindowListNode *windowUnderCursor;
	// Must ensure that the window is his subwindow
	void focusWindow(wsWindowListNode *window) {
		// if(window->data->fatherWindow != this) ERR;
		if (window == subWindow.front()) return;
		if (subWindow.front()->data->focused)
			subWindow.front()->data->windowFocusReceiver(false);
		if(window) {
			subWindow.putFront(window);
			window->data->windowFocusReceiver(true);
		}
	}
	void focusWindowUnderCursor() {
		focusWindow(windowUnderCursor);
	}

	int pointIn(wsCoord2 point) {
		return point.x >= position.x && point.x <= position.x + size.x
			&& point.y >= position.y && point.y <= position.y + size.y;
	}
	int updateCursorWindow() {
		for (auto node = subWindow.front(); node; node = node->next) {
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
	wsList<wsWindow *>* getSubWindowList() { return &subWindow; };
	wsWindowListNode* getWindowUnderCursor() { return windowUnderCursor; }
public:
	const wsList<wsWindow *>* getSubWindowList() const { return &subWindow; };
	const wsWindowListNode* getWindowUnderCursor() const {
		return windowUnderCursor;
	}

//---------------------------- Construct function ---------------------------//
public:
	wsWindow() = delete;
	wsWindow(const char *title,
			 const wsCoord2 position,
			 const wsCoord2 size,
			 const int style);
	void deleteWindow();
protected:
	~wsWindow();

//-------------------------------- Callbacks --------------------------------//
public:
	void display(int lastX, int lastY,
				 int cutX, int cutY,
				 int cutWidth, int cutHeight);

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

	inline void makeContextCurrent();
	void freeContext() {
		extern thread_local wsWindow *currentWindow;
		currentWindow = nullptr;
		glPopMatrix();
	}
} wsWindow;

typedef class wsBaseWindow : public wsWindow {
public:
	GLFWwindow *glfwWindow;

	wsBaseWindow(const char *title,
				 const wsCoord2 position,
				 const wsCoord2 size,
				 int style)
		:wsWindow(title, position, size, style) {};
} wsBaseWindow;

inline void wsWindow::makeContextCurrent() {
	extern thread_local wsWindow *currentWindow;
	extern thread_local wsBaseWindow *currentBaseWindow;
	if(topWindow != currentBaseWindow) {
		int wsInitGLEW();
		glfwMakeContextCurrent(topWindow->glfwWindow);
		currentBaseWindow = topWindow;
		wsInitGLEW();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glPushMatrix();
	glViewport(0, 0, bufferSize.x, bufferSize.y);
	currentWindow = this;
}

#endif
