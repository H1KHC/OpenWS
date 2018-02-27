#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#if defined(__linux)
#define GLFW_EXPOSE_NATIVE_X11
#include <X11/extensions/shape.h>
#elif defined(WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#include <windows.h>
#endif
#include <GLFW/glfw3native.h>
#include <openWS.h>

int windowID;

struct windowData {
	bool onMove, fast, slow, speedUp, speedDown, randomForce;
	int horizontal, vertical, onPress, tick, randTime;
	double theta, speed;
	windowData() : onMove(false), fast(false), slow(false),
		speedUp(false), speedDown(false), randomForce(false),
		horizontal(0), vertical(0), onPress(0),
		tick(0), randTime(10),
		theta(0), speed(0.0) {}
};

inline void draw() {
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(0.0f, 1.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(-0.87f, -0.5f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(0.87f, -0.5f);
	glEnd();
}

void display(int windowID, int) {
	windowData* data;
	wsGetWindowUserPointer(windowID, (void**)&data);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f * (0 < data->onPress));
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glRotatef(data->theta+=data->speed, 0, 0, 1);
	draw();
	if(data->theta >= 360) data->theta -= 360;
	if(data->onMove) {
		int delta = (data->fast ? 10 : (data->slow ? 1 : 5)),
			x = data->horizontal * delta,
			y = data->vertical * delta;
		wsSetWindowPos(windowID, x, y, WS_SWP_RELATIVE);
	}
	int ds = data->speedUp - data->speedDown;
	if(ds) {
		data->speed += ds * 0.1;
	} else {
		if(data->speed > 0.02) data->speed -= 0.03;
		else if(data->speed < -0.02) data->speed += 0.03;
		else data->speed = 0;
	}
	if(data->speed > -1e-5 && data->speed < 1e-5) data->speed = 0;
	if(data->randomForce) {
		data->tick++;
		if(data->tick == data->randTime) {
			data->speed += rand() / (double)RAND_MAX * 12.0 * ((rand() & 1) ? -1 : 1);
			data->randTime += rand() % 120 + 60;
		}
	}
}

int key(int windowID, int key, int, int action, int) {
	windowData* data;
	wsGetWindowUserPointer(windowID, (void**)&data);
	if(key == WS_KEY_ESCAPE) {
		wsTerminate();
		return 0;
	}
	data->onPress += (action == WS_RELEASE ? -1 : (action == WS_PRESS ? 1 : 0));
	if(action == WS_REPEAT) return 0;
	int delta = (action == WS_PRESS ? 1 : -1);
	switch(key) {
		case WS_KEY_LEFT: data->horizontal -= delta; break;
		case WS_KEY_RIGHT: data->horizontal += delta; break;
		case WS_KEY_UP: data->vertical -= delta; break;
		case WS_KEY_DOWN: data->vertical += delta; break;
		case WS_KEY_LEFT_CONTROL: data->fast = (delta == 1); break;
		case WS_KEY_LEFT_SHIFT: data->slow = (delta == 1); break;
		case WS_KEY_MINUS: data->speedUp = (delta == 1); break;
		case WS_KEY_EQUAL: data->speedDown = (delta == 1); break;
		case WS_KEY_R: data->randomForce ^= (delta == 1); break;
	}
	data->onMove = ((data->horizontal != 0) || (data->vertical != 0));
	return 0;
}

int main() {
	srand(time(0));
	wsInit();
	wsWindowHint(GLFW_DEPTH_BITS, 16);
	wsWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
	wsWindowHint(GLFW_DECORATED, GLFW_FALSE);
	wsWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	wsWindowHint(GLFW_FLOATING, GLFW_TRUE);
	windowID = wsCreateWindow("Example", 200, 200, 256, 256, new windowData);
	wsSetDisplayCallback(windowID, display);
	wsSetKeyCallback(windowID, key);
#if defined(__linux)
//	wsSetFPS(60);
	XShapeCombineRectangles(glfwGetX11Display(), glfwGetX11Window(wsGetGLFWWindow()), ShapeInput, 0, 0, NULL, 0, ShapeSet, YXBanded);
#elif defined(WIN32)
	HWND hWnd = glfwGetWin32Window(wsGetGLFWWindow());
	LONG w = GetWindowLong(hWnd, GWL_EXSTYLE);
	w |= WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST;
	SetWindowLong(hWnd, GWL_EXSTYLE, w);
	SetLayeredWindowAttributes(hWnd, 0, 192, LWA_COLORKEY | LWA_ALPHA);
#endif
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POINT_SMOOTH, GL_NICEST);
	glHint(GL_LINE_SMOOTH, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH, GL_NICEST);
	wsMainLoop();
	return 0;
}
