#include <stdio.h>
#include <GL/gl.h>
#include <openWS.h>
#include <GLFW/glfw3.h>

int theta = 0;

void display(int, int) {
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glRotatef(theta++, 0, 0, 1);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(0.0f, 1.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(-0.87f, -0.5f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(0.87f, -0.5f);
	glEnd();
}

int main() {
	wsWindowCallbacks wndCbk;
	wsSetWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
	wsSetWindowHint(GLFW_DECORATED, GLFW_FALSE);
	wsSetWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	wsSetWindowHint(GLFW_FLOATING, GLFW_TRUE);
	if(!wsInit("Example", 200, 200, 256, 256)) {
		unsigned err = wsLastError();
		printf("Init failed, error code: %d %s", err, wsErrorString(err));
		return -1;
	}
	wndCbk.displayCallback = display;
	wsCreateWindow(WS_STYLE_DEFAULT, "", &wndCbk, 0, 0, 256, 256, WS_ROOT_WINDOW_ID);
	wsMainLoop();
	return 0;
}