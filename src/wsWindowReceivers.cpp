#include "wsWindow.h"

// This pointer nunnull check is needed in order to avoid the problem that when
// user closes a window's fatherwindow, and returns that the event should be
// pushed up, his father will access its member variables, by when the program
// will segfault, as its father has been deleted
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnonnull-compare"

void wsWindow::displayReceiver() {
	if (!(styleMask & WS_STYLE_NODISPLAY) && displayCallback && needFlush()) {
		makeContextCurrent();
		displayCallback(windowID, framebuffer);
		freeContext();

		// In order to avoid the issue that all the colors except blue will
		//  be erased in the subwindow on some machine
		glColor3f(1.0f, 1.0f, 1.0f);

		needRedisplay = true;
	}
	for (wsWindowListNode *node = subWindow.front(); node; node = node->next) {
		node->data->displayReceiver();
		if(node->data->needRedisplay) needRedisplay = true;
	}
}
int wsWindow::keyboardReceiver(int key, int scancode, int action, int mods) {
	if (subWindow.isEmpty() || !subWindow.front()->data->focused || subWindow.front()->data->keyboardReceiver(key, scancode, action, mods)) {

		if(!this) return false;

		if (keyboardCallback) {
			makeContextCurrent();
			int ret = keyboardCallback(windowID, key, scancode, action, mods);
			freeContext();
			return ret;
		}
	}
	return true;
}
int wsWindow::charReceiver(unsigned key, int mods) {
	if (subWindow.isEmpty() || !subWindow.front()->data->focused || subWindow.front()->data->charReceiver(key, mods)) {
		if(!this) return false;
		if (charCallback) {
			makeContextCurrent();
			int ret = charCallback(windowID, key, mods);
			freeContext();
			return ret;
		}
		return true;
	}
	return false;
}
int wsWindow::mouseReceiver(int button, int action, int mods) {
	focusWindowUnderCursor();
	if (windowUnderCursor == nullptr || windowUnderCursor->data->mouseReceiver(button, action, mods)) {
		if(!this) return false;
		if (mouseButtonCallback) {
			makeContextCurrent();
			int ret = mouseButtonCallback(windowID, button, action, mods);
			freeContext();
			return ret;
		}
		return true;
	}
	return false;
}
int wsWindow::cursorMoveReceiver(double xpos, double ypos) {
	cursorPos = wsCoord2{ (int)xpos, (int)ypos };
	updateCursorWindow();
	if (windowUnderCursor == nullptr || windowUnderCursor->data->cursorMoveReceiver(xpos - windowUnderCursor->data->position.x, ypos - windowUnderCursor->data->position.y)) {
		if(!this) return false;
		if (cursorMoveCallback) {
			makeContextCurrent();
			int ret = cursorMoveCallback(windowID, xpos, ypos);
			freeContext();
			return ret;
		}
		return true;
	}
	return false;
}
int wsWindow::scrollReceiver(double xoffset, double yoffset) {
	// send to windowUnderCursor first, then to frontWindow
	if ((windowUnderCursor == nullptr || windowUnderCursor->data->scrollReceiver(xoffset, yoffset))
		|| (subWindow.isEmpty() || subWindow.front()->data->scrollReceiver(xoffset, yoffset))) {
		if(!this) return false;
		if (scrollCallback) {
			makeContextCurrent();
			int ret = scrollCallback(windowID, xoffset, yoffset);
			freeContext();
			return ret;
		}
		return true;
	}
	return false;
}
int wsWindow::fileDropReceiver(int count, const char ** filename) {
	if (windowUnderCursor == nullptr || windowUnderCursor->data->fileDropReceiver(count, filename)) {
		if(!this) return false;
		if (fileDropCallback) {
			makeContextCurrent();
			int ret = fileDropCallback(windowID, count, filename);
			freeContext();
			return ret;
		}
		return true;
	}
	return false;
}
void wsWindow::cursorEnterReceiver(int entered) {
	if (windowUnderCursor) {
		windowUnderCursor->data->cursorEnterReceiver(entered);
		if(!this) return;
		if (!entered)windowUnderCursor = nullptr;
	}
	else underCursor = entered;
	if (cursorEnterCallback) {
		makeContextCurrent();
		cursorEnterCallback(windowID, entered);
		freeContext();
	}
}
void wsWindow::windowResizeReceiver(wsCoord2 newSize) {
	wsCoord2 deltaSize{ newSize.x - size.x, newSize.y - size.y };
	size = newSize;

	if (!(styleMask & WS_STYLE_NO_BUFFER_RESIZE) || !(styleMask & WS_STYLE_NO_FRAMEBUFFER)) {
		if(styleMask & WS_STYLE_NO_DEPTHBUFFER) {
			int adjustFramebufferSize(GLuint frameBuffer, GLuint texture, const int width, const int height);
			if (!adjustFramebufferSize(framebuffer, texture, size.x, size.y)) {
				wsSetError(WS_ERR_BUFFER_RESIZE_FAILED);
				return;
			}
		} else {
			int adjustFramebufferSize(GLuint frameBuffer, GLuint depthBuffer, GLuint texture, const int width, const int height);
			if (!adjustFramebufferSize(framebuffer, depthbuffer, texture, size.x, size.y)) {
				wsSetError(WS_ERR_BUFFER_RESIZE_FAILED);
				return;
			}
		}
		bufferSize = newSize;
	}

	for (wsWindowListNode *node = subWindow.front(); node; node = node->next) {
		switch (node->data->styleMask & WS_STYLE_ALIGN_MASK) {
		case WS_STYLE_ALIGN_LU: break;
		case WS_STYLE_ALIGN_LD: node->data->position.y += deltaSize.y; break;
		case WS_STYLE_ALIGN_RU: node->data->position.x += deltaSize.x; break;
		case WS_STYLE_ALIGN_RD: node->data->position.x += deltaSize.x;
								node->data->position.y += deltaSize.y; break;
		}
	}
	if(fatherWindow == nullptr) {
		glfwSetWindowPos(((wsBaseWindow*)this)->glfwWindow, position.x, position.y);
		glfwSetWindowSize(((wsBaseWindow*)this)->glfwWindow, size.x, size.y);
	}
	if (windowResizeCallback) {
		makeContextCurrent();
		windowResizeCallback(windowID, newSize.x, newSize.y);
		freeContext();
	}
}
void wsWindow::windowMoveReceiver(wsCoord2 newPos) {
	position = newPos;
	if (windowMoveCallback) {
		makeContextCurrent();
		windowMoveCallback(windowID, newPos.x, newPos.y);
		freeContext();
	}
}
void wsWindow::windowCloseReceiver() {
	while (subWindow.front())
		subWindow.front()->data->windowCloseReceiver();
	if (windowCloseCallback) {
		makeContextCurrent();
		windowCloseCallback(windowID);
		freeContext();
	}
	if(this) deleteWindow();
}
void wsWindow::windowFocusReceiver(int focus) {
	focused = focus;
	if (windowFocusCallback) {
		makeContextCurrent();
		windowFocusCallback(windowID, focus);
		freeContext();
	}
}

#pragma GCC diagnostic pop
