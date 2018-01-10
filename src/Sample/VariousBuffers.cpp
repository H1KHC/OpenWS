#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <openWS.h>
#include <GLFW/glfw3.h>

struct windowData {
	int theta, theta1, theta2;
	windowData() : theta(0), theta1(0), theta2(0) {}
};

void drawCube() {
	glBegin(GL_QUADS);
	glColor3f(1.0,1.0,0.0), glVertex3f( 1.0, 1.0,-1.0);
	glColor3f(0.0,1.0,0.0), glVertex3f(-1.0, 1.0,-1.0);
	glColor3f(0.0,1.0,1.0), glVertex3f(-1.0, 1.0, 1.0);
	glColor3f(1.0,1.0,1.0), glVertex3f( 1.0, 1.0, 1.0);

	glColor3f(1.0,0.0,1.0), glVertex3f( 1.0,-1.0, 1.0);
	glColor3f(0.0,0.0,1.0), glVertex3f(-1.0,-1.0, 1.0);
	glColor3f(0.0,0.0,0.0), glVertex3f(-1.0,-1.0,-1.0);
	glColor3f(1.0,0.0,0.0), glVertex3f( 1.0,-1.0,-1.0);

	glColor3f(1.0,1.0,1.0), glVertex3f( 1.0, 1.0, 1.0);
	glColor3f(0.0,1.0,1.0), glVertex3f(-1.0, 1.0, 1.0);
	glColor3f(0.0,0.0,1.0), glVertex3f(-1.0,-1.0, 1.0);
	glColor3f(1.0,0.0,1.0), glVertex3f( 1.0,-1.0, 1.0);

	glColor3f(1.0,0.0,0.0), glVertex3f( 1.0,-1.0,-1.0);
	glColor3f(0.0,0.0,0.0), glVertex3f(-1.0,-1.0,-1.0);
	glColor3f(0.0,1.0,0.0), glVertex3f(-1.0, 1.0,-1.0);
	glColor3f(1.0,1.0,0.0), glVertex3f( 1.0, 1.0,-1.0);

	glColor3f(0.0,1.0,1.0), glVertex3f(-1.0, 1.0, 1.0);
	glColor3f(0.0,1.0,0.0), glVertex3f(-1.0, 1.0,-1.0);
	glColor3f(0.0,0.0,0.0), glVertex3f(-1.0,-1.0,-1.0);
	glColor3f(0.0,0.0,1.0), glVertex3f(-1.0,-1.0, 1.0);

	glColor3f(1.0,1.0,0.0), glVertex3f( 1.0, 1.0,-1.0);
	glColor3f(1.0,1.0,1.0), glVertex3f( 1.0, 1.0, 1.0);
	glColor3f(1.0,0.0,1.0), glVertex3f( 1.0,-1.0, 1.0);
	glColor3f(1.0,0.0,0.0), glVertex3f( 1.0,-1.0,-1.0);
	glEnd();
}

void display(int windowID, int) {
	int w, h;
	windowData *data;
	glEnable(GL_DEPTH_TEST);
	wsGetWindowData(windowID, (void **)&data);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	wsGetWindowSize(windowID, &w, &h);
	gluPerspective(60, w * 1.0 / h, 0.01, 100.0);
	glTranslatef(0, 0, -5);
	glRotatef(data->theta, 0.0f, 0.0f, 1.0f);
	glRotatef(data->theta1, 1.0f, 0.0f, 0.0f);
	glRotatef(data->theta2, 0.0f, 1.0f, 0.0f);
	drawCube();
	data->theta += 1.0f;
	data->theta1 += 3.0f;
	data->theta2 += 2.6f;
}

int main() {
	if(!wsInit("Example", 200, 200, 512, 512)) {
		unsigned err = wsLastError();
		printf("Init failed, error code: %d %s", err, wsErrorString(err));
		return -1;
	}
	wsSetDebugMode(WS_SDM_CURSORPOS);
	int id = wsCreateWindow(WS_STYLE_DEFAULT | WS_STYLE_STATIC_WINDOW, "", 0, 0, 256, 256, new windowData, WS_ROOT_WINDOW_ID);
	wsSetWindowDisplayCallback(id, display);
	id = wsCreateWindow(WS_STYLE_DEFAULT, "", 0, 256, 256, 256, new windowData, WS_ROOT_WINDOW_ID);
	wsSetWindowDisplayCallback(id, display);
	id = wsCreateWindow(WS_STYLE_STATIC_WINDOW, "", 256, 0, 256, 256, new windowData, WS_ROOT_WINDOW_ID);
	wsSetWindowDisplayCallback(id, display);
	id = wsCreateWindow(0, "", 256, 256, 256, 256, new windowData, WS_ROOT_WINDOW_ID);
	wsSetWindowDisplayCallback(id, display);
	wsMainLoop();
	return 0;
}