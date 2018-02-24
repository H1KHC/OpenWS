#include "wsWindow.h"
#include "wsClock.h"
extern int inited;

inline void wsFlush(wsBaseWindow* baseWindow) {
	if (baseWindow->displayCallback == nullptr) {
		glBindFramebuffer(GL_FRAMEBUFFER, baseWindow->getFramebuffer());
		glViewport(0, 0, baseWindow->size.x, baseWindow->size.y);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	baseWindow->displayReceiver();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, baseWindow->size.x, baseWindow->size.y);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
	glfwSwapBuffers(baseWindow->glfwwindow);
}

static bool inMainLoop = false, terminated = false;

int wsTerminate() {
	if(!inMainLoop) {
		wsSetError(WS_ERR_ILLEGAL_OPERATION);
		return false;
	}
	terminated = true;
	return true;
}

void wsMain() {
	extern thread_local wsWindow* currentBaseWindow;
	extern std::map<GLFWwindow*, wsBaseWindow*> baseWindows;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	while (baseWindows.size()) {
		for(auto windowPair = baseWindows.begin(), _end = baseWindows.end();
			windowPair != _end; ) {
			wsBaseWindow* baseWindow = windowPair->second;
			if (!baseWindow || glfwWindowShouldClose(windowPair->first) || terminated) {
				windowPair = baseWindows.erase(windowPair);
				continue;
			}
			if(currentBaseWindow != baseWindow) {
				int wsInitGLEW();
				glfwMakeContextCurrent(windowPair->first);
				currentBaseWindow = baseWindow;
				wsInitGLEW();
			}

			wsFlush(baseWindow);
			++windowPair;
		}
		glfwPollEvents();
	}
	return;
}

void wsMainLoop() {
	if (!inited) {
		wsSetError(WS_ERR_NOT_INITIALIZED);
		return;
	}
	inMainLoop = true;
	wsMain();
	inMainLoop = false;
}
