#include "wsWindow.h"
#include "wsWindowManager.h"

extern int inited;
extern wsWindow *currentWindow;

wsWindow::wsWindow(const char * title, const wsCoord2 position, const wsCoord2 size, const int style, wsWindowCallbacks *wndCallbacks) :
	wsWindowInfo{ nullptr, 0, position, size, 0, size, false, false, wsCoord2{ 0, 0 }, style, *wndCallbacks, nullptr },
	fatherWindow(nullptr),
	windowUnderCursor(nullptr)
{
	int len = 0;
	while (title[len]) ++len;
	name = new char[len + 1];
	while (~len) name[len] = title[len], --len;
	windowManager.registerWindow(this);
}

wsWindow::~wsWindow() {
	if (framebuffer) glDeleteFramebuffers(1, &framebuffer);
	if (depthbuffer) glDeleteRenderbuffers(1, &depthbuffer);
	if (texture) glDeleteTextures(1, &texture);
}

void wsWindow::deleteWindow() {
	if (fatherWindow) {
		if (fatherWindow->windowUnderCursor && fatherWindow->windowUnderCursor->data == this)
			fatherWindow->windowUnderCursor = nullptr;
		fatherWindow->subWindow.deleteNode(this);
	}
	windowManager.cancelWindow(this);
	delete[] name;
	delete this;
}

void wsWindow::display(int lastX, int lastY, int cutX, int cutY, int cutWidth, int cutHeight) {
	extern int debugOutput;
	//Update this window's global position
	if(windowID != WS_ROOT_WINDOW_ID)
		lastX += position.x, lastY += position.y;

	//Update scissor range
	if (cutX < lastX) cutWidth -= lastX - cutX, cutX = lastX;
	if (cutY < lastY) cutHeight -= lastY - cutY, cutY = lastY;
	if (cutX + cutWidth > lastX + size.x) cutWidth = lastX + size.x - cutX;
	if (cutY + cutHeight > lastY + size.y) cutHeight = lastY + size.y - cutY;

	//Out of scissor range check
	if (lastX > cutX + cutWidth || lastY > cutY + cutHeight
		|| lastX + size.x < cutX || lastY + size.y < cutY) return;

	//Draw window
	if (!(styleMask & WS_STYLE_NODISPLAY)) {
		glBindTexture(GL_TEXTURE_2D, texture);
		glBegin(GL_QUADS);
		glTexCoord2d(0, 0); glVertex2i(lastX, baseWindow->size.y - lastY - size.y);
		glTexCoord2d(1, 0); glVertex2i(lastX + size.x, baseWindow->size.y - lastY - size.y);
		glTexCoord2d(1, 1); glVertex2i(lastX + size.x, baseWindow->size.y - lastY);
		glTexCoord2d(0, 1); glVertex2i(lastX, baseWindow->size.y - lastY);
		glEnd();
#ifdef _DEBUG
		//Debug output
		if (debugOutput) {
			glDisable(GL_TEXTURE_2D);
			glBegin(GL_LINES);
			if (!subWindow.isEmpty()) {
				if ((debugOutput & WS_SDM_CURSORWINDOW) && windowUnderCursor) {
					glColor3d(1, 0, 0);
					glVertex2i(lastX + size.x, baseWindow->size.y - lastY);
					glVertex2i((lastX + windowUnderCursor->data->position.x + windowUnderCursor->data->size.x),
						baseWindow->size.y - (lastY + windowUnderCursor->data->position.y));
				}
				if (debugOutput && WS_SDM_FOCUSWINDOW) {
					glColor3d(1, 1, 1);
					glVertex2i(lastX, baseWindow->size.y - lastY);
					glVertex2i((lastX + subWindow.front()->data->position.x),
						baseWindow->size.y - (lastY + subWindow.front()->data->position.y));
				}
			}
			if ((debugOutput & WS_SDM_CURSORPOS) && underCursor && windowUnderCursor == nullptr) {
				glColor3d(1, 1, 1);
				glVertex2i(lastX + cursorPos.x, baseWindow->size.y - lastY);
				glVertex2i(lastX + cursorPos.x, baseWindow->size.y - (lastY + size.y + 1));
				glVertex2i(lastX, baseWindow->size.y - (lastY + cursorPos.y));
				glVertex2i(lastX + size.x + 1, baseWindow->size.y - (lastY + cursorPos.y));
			}
			glEnd();
			glEnable(GL_TEXTURE_2D);
		}
#endif
	}

	glPushAttrib(GL_SCISSOR_BIT);
	glScissor(cutX, baseWindow->size.y - cutY - cutHeight, cutWidth, cutHeight);
	//Enum subwindows
	for (wsWindowListNode *node = subWindow.back(); node; node = node->last) {
		node->data->display(lastX, lastY, cutX, cutY, cutWidth, cutHeight);
	}
	glPopAttrib();
}

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

WS_API int wsCreateWindow(int windowStyle, const char *windowName, wsWindowCallbacks *windowCallbacks, int x, int y, int width, int height, void *windowData, int fatherWindowID) {
	wsWindow *newWindow, *toBeAttached;
	if((windowStyle & WS_STYLE_NO_FRAMEBUFFER) && windowCallbacks->displayCallback == nullptr) {
		wsSetError(WS_ERR_ILLEGAL_OPERATION);
		return WS_INVALID_WINDOW_ID;
	}
	checkInitAndFindWindow(toBeAttached, fatherWindowID, WS_INVALID_WINDOW_ID);
	newWindow = new wsWindow(windowName, wsCoord2{ x, y }, wsCoord2{ width, height }, windowStyle, windowCallbacks);
	newWindow->fatherWindow = toBeAttached;
	newWindow->userData = windowData;
	if(!newWindow->genFramebuffer()) {
		delete newWindow;
		return WS_INVALID_WINDOW_ID;
	}
	newWindow->focused = true;
	toBeAttached->subWindow.addFront(newWindow);
	newWindow->thisWindowNode = toBeAttached->getSubWindowList()->front();
	return newWindow->windowID;
}
WS_API int wsCloseWindow(int windowID) {
	wsWindow *window;
	checkInitAndFindWindow(window, windowID, false);
	window->windowCloseReceiver();
	return true;
}
WS_API int wsAttachWindow(int subwindowID, int fatherWindowID) {
	wsWindow *subWindow, *fatherWindow;
	checkInit(false);
	if (!subwindowID || subwindowID == fatherWindowID) {
		wsSetError(WS_ERR_INVALID_VALUE);
		return false;
	}
	findWindow(subWindow, subwindowID, false);
	findWindow(fatherWindow, fatherWindowID, false);
	subWindow->fatherWindow->subWindow.deleteNode(subWindow);
	fatherWindow->subWindow.addBack(subWindow);
	subWindow->fatherWindow = fatherWindow;
	return true;
}
WS_API int wsFocusWindow(int windowID) {
	wsWindow *window;
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