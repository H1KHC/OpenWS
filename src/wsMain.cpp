#include "wsWindow.h"
#include "wsClock.h"

inline void wsFlush(wsBaseWindow* baseWindow) {
	extern thread_local wsBaseWindow* currentBaseWindow;
	if(currentBaseWindow != baseWindow) {
		int wsInitGLEW();
		glfwMakeContextCurrent(baseWindow->glfwWindow);
		currentBaseWindow = baseWindow;
		wsInitGLEW();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, baseWindow->size.x, baseWindow->size.y);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	baseWindow->displayReceiver();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, baseWindow->size.x, baseWindow->size.y);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, baseWindow->size.x, 0, baseWindow->size.y, -1, 1);
	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glScissor(0, 0, baseWindow->size.x, baseWindow->size.y);
	baseWindow->display(0, 0, 0, 0, baseWindow->size.x, baseWindow->size.y);
	glPopAttrib();
	glPopMatrix();
	glfwSwapBuffers(baseWindow->glfwWindow);
}

static bool inMainLoop = false, terminated = false;

void wsTerminate(void) {
	if(!inMainLoop) {
		wsSetError(WS_ERR_ILLEGAL_OPERATION);
	} else terminated = true;
}

void wsMain() {
	extern std::map<GLFWwindow*, wsBaseWindow*> baseWindows;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	while (baseWindows.size() && !terminated) {
		for(auto windowPair = baseWindows.begin(), _end = baseWindows.end();
			windowPair != _end; ) {
			wsBaseWindow* baseWindow = windowPair->second;
			
			if (!baseWindow || glfwWindowShouldClose(windowPair->first)) {
				glfwDestroyWindow(windowPair->first);
				windowPair = baseWindows.erase(windowPair);
			} else {
				wsFlush(baseWindow);
				++windowPair;
			}
			glfwPollEvents();
		}
	}
	if(terminated) {
		for(auto windowPair = baseWindows.begin(), _end = baseWindows.end();
		  windowPair != _end; windowPair = baseWindows.erase(windowPair)) {
			glfwMakeContextCurrent(windowPair->first);
			if(windowPair->second) windowPair->second->deleteWindow();
			glfwSetWindowShouldClose(windowPair->first, GLFW_TRUE);
			glfwPollEvents();
		}
		glfwTerminate();
	}
	return;
}

void wsMainLoop() {
	extern int inited;
	if (!inited) {
		wsSetError(WS_ERR_NOT_INITIALIZED);
		return;
	}
	inMainLoop = true;
	wsMain();
	inMainLoop = false;
}
