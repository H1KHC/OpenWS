#include "wsWindow.h"
#include <GLFW/glfw3.h>

extern int inited;
#define checkInit(failReturnValue) do {\
		if (!inited) {\
			wsSetError(WS_ERR_NOT_INITIALIZED);\
			return (failReturnValue);\
		}\
	}while(0)

WS_API int wsSetJoystickConnectionCallback(void(*func)(int joystickID, int joystickState)) {
	checkInit(false);
	glfwSetJoystickCallback(func);
	return true;
}

void(*iconifyCallback)(int iconified);
void windowIconifyReceiver(GLFWwindow *, int iconified) {
	iconifyCallback(iconified == GLFW_TRUE);
}

WS_API int wsSetWindowIconifyCallback(void(*func)(int iconified)) {
	checkInit(false);
	if (func)glfwSetWindowIconifyCallback(baseWindow->glfwwindow, windowIconifyReceiver);
	else glfwSetWindowIconifyCallback(baseWindow->glfwwindow, nullptr);
	return true;
}
