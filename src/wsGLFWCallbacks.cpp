#include "wsWindow.h"

void wsKeyboardReceiver(GLFWwindow*, int key, int scancode, int action, int mods) {
	baseWindow->keyboardReceiver(key, scancode, action, mods);
}

void wsCharReceiver(GLFWwindow*, unsigned key, int mods) {
	baseWindow->charReceiver(key, mods);
}

void wsMouseReceiver(GLFWwindow*, int button, int action, int mods) {
	baseWindow->mouseReceiver(button, action, mods);
}

void wsCursorMoveReceiver(GLFWwindow*, double xpos, double ypos) {
	baseWindow->cursorMoveReceiver(xpos, ypos);
}

void wsCursorEnterReceiver(GLFWwindow*, int entered) {
	baseWindow->cursorEnterReceiver(int(entered == GLFW_TRUE));
}

void wsScrollReceiver(GLFWwindow*, double xoffset, double yoffset) {
	baseWindow->scrollReceiver(xoffset, yoffset);
}

void wsWindowResizeReceiver(GLFWwindow*, int w, int h) {
	baseWindow->windowResizeReceiver(wsCoord2{ w, h });
}

void wsWindowMoveReceiver(GLFWwindow*, int x, int y) {
	x = x, y = y;
}

void wsWindowCloseReceiver(GLFWwindow*) {
	baseWindow->windowCloseReceiver();
	//	baseWindow->deleteWindow();
	baseWindow = nullptr;
	wsDeinit();
}

void wsWindowFocusReceiver(GLFWwindow*, int focused) {
	baseWindow->windowFocusReceiver(int(focused == GLFW_TRUE));
}

void wsFileDropReceiver(GLFWwindow*, int count, const char **filename) {
	baseWindow->fileDropReceiver(count, filename);
}