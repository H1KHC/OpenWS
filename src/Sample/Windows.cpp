#include <openWS.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

float color[4][3] = {
	{0.93359375f, 0.30859375f, 0.12890625f},
	{0.4921875f,  0.72265625f, 0.01171875f},
	{0.01171875f, 0.640625f,   0.9296875f},
	{0.9765625f,  0.70703125f, 0.01171875f}
};

void display(int windowID, int) {
	float* color;
	wsGetWindowData(windowID, (void**)&color);
	glClearColor(color[0], color[1], color[2], 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

int main() {
	int id;
	wsInit();
	wsSetWindowHint(GLFW_DECORATED, GLFW_FALSE);
	id = wsCreateWindow("LU", 128, 128, 128, 128, color[0]);
	wsSetWindowDisplayCallback(id, display);
	wsSetWindowHint(GLFW_DECORATED, GLFW_FALSE);
	id = wsCreateWindow("RU", 256, 128, 128, 128, color[1]);
	wsSetWindowDisplayCallback(id, display);
	wsSetWindowHint(GLFW_DECORATED, GLFW_FALSE);
	id = wsCreateWindow("LD", 128, 256, 128, 128, color[2]);
	wsSetWindowDisplayCallback(id, display);
	wsSetWindowHint(GLFW_DECORATED, GLFW_FALSE);
	id = wsCreateWindow("RD", 256, 256, 128, 128, color[3]);
	wsSetWindowDisplayCallback(id, display);
	wsMainLoop();
}