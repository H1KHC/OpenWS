#include "wsWindow.h"

extern std::map<GLFWwindow*, wsBaseWindow*> baseWindows;

void wsKeyboardReceiver(GLFWwindow* window, int key, int scancode, int action, int mods) {
	auto windowPair = baseWindows.find(window);
	if(windowPair->second)
		windowPair->second->keyboardReceiver(key, scancode, action, mods);
}

void wsCharReceiver(GLFWwindow* window, unsigned key, int mods) {
	auto windowPair = baseWindows.find(window);
	if(windowPair->second)
		windowPair->second->charReceiver(key, mods);
}

void wsMouseReceiver(GLFWwindow* window, int button, int action, int mods) {
	auto windowPair = baseWindows.find(window);
	if(windowPair->second)
		windowPair->second->mouseReceiver(button, action, mods);
}

void wsCursorMoveReceiver(GLFWwindow* window, double xpos, double ypos) {
	auto windowPair = baseWindows.find(window);
	if(windowPair->second)
		windowPair->second->cursorMoveReceiver(xpos, ypos);
}

void wsCursorEnterReceiver(GLFWwindow* window, int entered) {
	auto windowPair = baseWindows.find(window);
	if(windowPair->second)
		windowPair->second->cursorEnterReceiver(int(entered == GLFW_TRUE));
}

void wsScrollReceiver(GLFWwindow* window, double xoffset, double yoffset) {
	auto windowPair = baseWindows.find(window);
	if(windowPair->second)
		windowPair->second->scrollReceiver(xoffset, yoffset);
}

void wsWindowResizeReceiver(GLFWwindow* window, int w, int h) {
	auto windowPair = baseWindows.find(window);
	if(windowPair->second)
		windowPair->second->windowResizeReceiver(wsCoord2{ w, h });
}

void wsWindowMoveReceiver(GLFWwindow* window, int x, int y) {
	auto windowPair = baseWindows.find(window);
	if(windowPair->second)
		windowPair->second->windowMoveReceiver(wsCoord2{ x, y });
}

void wsWindowCloseReceiver(GLFWwindow* window) {
	auto windowPair = baseWindows.find(window);
	windowPair->second->windowCloseReceiver();
	windowPair->second = nullptr;
}

void wsWindowFocusReceiver(GLFWwindow* window, int focused) {
	extern thread_local wsBaseWindow* currentBaseWindow;
	wsBaseWindow *newWindow = baseWindows[window];
	if(!newWindow) return;
	if(focused && currentBaseWindow != newWindow) {
		//mutex
		currentBaseWindow = newWindow;
		currentBaseWindow->windowFocusReceiver(int(focused == GLFW_TRUE));
	} else {
		newWindow->windowFocusReceiver(int(focused == GLFW_TRUE));
	}
}

void wsFileDropReceiver(GLFWwindow* window, int count, const char **filename) {
	auto windowPair = baseWindows.find(window);
	if(windowPair->second)
		windowPair->second->fileDropReceiver(count, filename);
}

void windowIconifyReceiver(GLFWwindow *window, int iconified) {
	extern std::map<GLFWwindow*, wsBaseWindow*> baseWindows;
	wsWindow* wswindow = baseWindows[window];
	wswindow->windowIconifyCallback(wswindow->windowID, iconified == GLFW_TRUE);
}

struct windowHintData { int hint, value; };
static wsList<windowHintData> windowHintLists;
void wsWindowHint(int hint, int value) {
	windowHintLists.addBack(windowHintData{hint, value});
}

int createGLFWwindow(wsBaseWindow* window) {
	for(auto* data = windowHintLists.front(); data; data = data->next)
		glfwWindowHint(data->data.hint, data->data.value);

	window->glfwWindow = glfwCreateWindow(window->size.x, window->size.y, window->name, nullptr, nullptr);

	if(window->glfwWindow == nullptr) return false;
	//if(contextInUse)	//unimplemented
	glfwMakeContextCurrent(window->glfwWindow);
	glfwSetKeyCallback(window->glfwWindow, wsKeyboardReceiver);
	glfwSetCharModsCallback(window->glfwWindow, wsCharReceiver);
	glfwSetMouseButtonCallback(window->glfwWindow, wsMouseReceiver);
	glfwSetCursorPosCallback(window->glfwWindow, wsCursorMoveReceiver);
	glfwSetCursorEnterCallback(window->glfwWindow, wsCursorEnterReceiver);
	glfwSetScrollCallback(window->glfwWindow, wsScrollReceiver);
	glfwSetWindowSizeCallback(window->glfwWindow, wsWindowResizeReceiver);
	glfwSetWindowPosCallback(window->glfwWindow, wsWindowMoveReceiver);
	glfwSetWindowCloseCallback(window->glfwWindow, wsWindowCloseReceiver);
	glfwSetWindowFocusCallback(window->glfwWindow, wsWindowFocusReceiver);
	glfwSetDropCallback(window->glfwWindow, wsFileDropReceiver);
	glfwSwapInterval(1);

	baseWindows.insert(std::make_pair(window->glfwWindow, window));
	// glfwSetWindowPos(window->glfwWindow, window->position.x > 0 ? window->position.x : 1,
	// 						 window->position.y > 0 ? window->position.y : 1);

	return true;
}