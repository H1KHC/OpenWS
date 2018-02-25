#include "wsWindow.h"
#include "wsList.h"
#include <openWS.h>

inline int wsInitGLFW() {
	return glfwInit();
}
int wsInitGLEW() {
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		return false;
	}

#define RETARGET(func) do{\
		if(!(func)){\
			if(!(func##EXT)) return false;\
			(func)=(func##EXT);\
		}\
	}while(0)

	RETARGET(glGenFramebuffers);
	RETARGET(glDeleteFramebuffers);
	RETARGET(glGenRenderbuffers);
	RETARGET(glDeleteRenderbuffers);
	RETARGET(glBindFramebuffer);
	RETARGET(glBindRenderbuffer);
	RETARGET(glRenderbufferStorage);
	RETARGET(glFramebufferRenderbuffer);
	RETARGET(glFramebufferTexture2D);
	RETARGET(glCheckFramebufferStatus);
#undef RETARGET
	return true;
}
int wsInit() {
	extern int inited;
	if (!wsInitGLFW())
		return false;
//	if (!wsInitGLEW())
//		return false;
	inited = true;
	return true;
}

int wsDeinit() {
	extern std::map<GLFWwindow*, wsBaseWindow*> baseWindows;
	extern int inited;
	if (!inited) {
		wsSetError(WS_ERR_NOT_INITIALIZED);
		return false;
	}

	if (baseWindows.size()) {
		for(auto baseWindow = baseWindows.begin(); baseWindows.size();) {
			if(baseWindow->second) baseWindow->second->deleteWindow();
			baseWindow = baseWindows.erase(baseWindow);
		}
	}

	glfwTerminate();

	inited = false;
	return true;
}