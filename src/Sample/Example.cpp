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
	if(!wsInit("Example", 200, 200, 512, 512)) {
		unsigned err = wsLastError();
		printf("Init failed, error code: %d %s", err, wsErrorString(err));
		return -1;
	}
	int id = wsCreateWindow("", 128, 128, 256, 256, nullptr, WS_ROOT_WINDOW_ID);
	wsSetWindowDisplayCallback(id, display);
	wsMainLoop();
	return 0;
}