#if defined(_WIN32)
#include <Windows.h>
#define DELAY(x) Sleep(x)
#else
#include <unistd.h>
#define DELAY(x) usleep((x)*1000)
#endif
#include "wsWindow.h"
#include "wsClock.h"
extern int inited;

inline void wsFlush() {
	if (baseWindow->callbacks.displayCallback == nullptr) {
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

WS_API int wsTerminate() {
	if(!inMainLoop) {
		wsSetError(WS_ERR_ILLEGAL_OPERATION);
		return false;
	}
	terminated = true;
	return true;
}

void wsMain() {
	extern int fpsControl, wsNeedRedisplay;
	extern wsClock *fpsClock;
	extern int wsFPS;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	while (true) {
		if (!baseWindow || glfwWindowShouldClose(baseWindow->glfwwindow) || terminated)
			break;

		do {
			if (fpsControl) {
				if (wsFPS) {
					if (fpsClock->getPeriodSinceLastCall(false) * wsFPS <= 1000000000.0) {
						DELAY(0);
						break;
					}
					else fpsClock->getPeriodSinceLastCall(true);
				}
				else {
					if (!wsNeedRedisplay) {
						DELAY(0);
						break;
					}
					else wsNeedRedisplay = false;
				}
			}
			wsFlush();
		} while (0);

		glfwPollEvents();
	}
	return;
}

WS_API void wsMainLoop() {
	if (!inited) {
		wsSetError(WS_ERR_NOT_INITIALIZED);
		return;
	}
	inMainLoop = true;
	wsMain();
	inMainLoop = false;
}
