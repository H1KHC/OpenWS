#include <stdio.h>
#include <GL/gl.h>
#include <openWS.h>

struct windowData {
	float theta, theta1, theta2;
	int count, lastx, lasty;
	int pressed, rpressed;
	windowData() : theta(0), theta1(0), theta2(0),
		count(0), lastx(-1), lasty(-1),
		pressed(false), rpressed(false) {};
};

void display(int windowID, int) {
	windowData *data;
	wsGetWindowData(windowID, (void **)&data);
	glClearColor(0.0f, 0.1f, 0.3f, 0.5f);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, -1, 1);
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-0.9999998808f, -0.9999998808f);
	glVertex2f(0.9999998808f, -0.9999998808f);
	glVertex2f(0.9999998808f, 0.9999998808f);
	glVertex2f(-0.9999998808f, 0.9999998808f);
	glEnd();

	glRotatef(data->theta, 0.0f, 0.0f, 1.0f);
	glRotatef(data->theta1, 1.0f, 0.0f, 0.0f);
	glRotatef(data->theta2, 0.0f, 1.0f, 0.0f);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);   glVertex2f(0.0f, 1.0f);
	glColor3f(0.0f, 1.0f, 0.0f);   glVertex2f(0.87f, -0.5f);
	glColor3f(0.0f, 0.0f, 1.0f);   glVertex2f(-0.87f, -0.5f);
	glEnd();
	glPopMatrix();

	data->theta += 1.0f;
	data->theta1 += 0.3f;
	data->theta2 += 0.4f;
}
int remPos(int windowID, int xpos, int ypos) {
	windowData *data;
	wsGetWindowData(windowID, (void **)&data);
	data->lastx = xpos;
	data->lasty = ypos;
	return 0;
}
int cursor(int windowID, int xpos, int ypos) {
	windowData *data;
	wsGetWindowData(windowID, (void **)&data);
	if (data->pressed) {
		if ((int)xpos != data->lastx || ypos != data->lasty) {
			if (data->lastx >= 0)
				wsSetWindowPos(windowID, (xpos - data->lastx), (ypos - data->lasty), WS_SWP_RELATIVE);
			else data->lastx = xpos, data->lasty = ypos;
		}
	}
	else if (data->rpressed) {
		if ((int)xpos != data->lastx || (int)ypos != data->lasty) {
			if (data->lastx >= 0) {
				wsSetWindowSize(windowID, xpos - data->lastx, data->lasty - ypos, WS_SWSI_RELATIVE);
				data->lastx = (int)xpos;
			}
			else {
				data->lastx = (int)xpos, data->lasty = (int)ypos;
			}
		}
	}
	else data->lastx = (int)xpos, data->lasty = (int)ypos;
	return 0;
}

int mouse(int windowID, int button, int state, int) {
	windowData *data;
	wsGetWindowData(windowID, (void **)&data);
	if (button == WS_MOUSE_BUTTON_LEFT) {
		data->pressed = (state != WS_RELEASE);
	}
	else if (button == WS_MOUSE_BUTTON_RIGHT) {
		data->rpressed = (state != WS_RELEASE);
	}
	else if (state != WS_RELEASE)wsCloseWindow(windowID);
	return 0;
}

void enterC(int windowID, int) {
	windowData *data;
	wsGetWindowData(windowID, (void **)&data);
	data->pressed = data->rpressed = false;
}

wsWindowCallbacks wndCbk;

int createWindow(int, int button, int state, int) {
	if (button == WS_MOUSE_BUTTON_LEFT && state == WS_PRESS)
		wsCreateWindow(WS_STYLE_ALIGN_LD, "noName", &wndCbk, 128, 128, 256, 256, new windowData, WS_ROOT_WINDOW_ID);
	return 0;
}

void recycle(int windowID) {
	windowData *data;
	wsGetWindowData(windowID, (void **)&data);
	delete data;
}

int main() {
	int id, nid;
	if (!wsInit("wsDemo", 200, 200, 512, 512)) {
		unsigned err = wsLastError();
		printf("Init failed, error code: %d %s", err, wsErrorString(err));
		return -1;
	}
	wsSetWindowData(WS_ROOT_WINDOW_ID, new windowData);
	wsSetWindowStyle(WS_ROOT_WINDOW_ID, WS_STYLE_ALIGN_LD, WS_SWST_ADD);
#ifdef _DEBUG
	wsSetDebugMode(WS_SDM_FULL);
#endif
	wndCbk.windowCloseCallback = recycle;
	wndCbk.mouseCallback = createWindow;
	wndCbk.cursorMoveCallback = remPos;
	wsSetWindowCallbacks(WS_ROOT_WINDOW_ID, &wndCbk, WS_SWC_COVER);
	wndCbk.displayCallback = display;
	wndCbk.cursorMoveCallback = cursor;
	wndCbk.mouseCallback = mouse;
	wndCbk.cursorEnterCallback = enterC;
	id = wsCreateWindow(WS_STYLE_NO_BUFFER_RESIZE | WS_STYLE_ALIGN_LD, "Window1", &wndCbk, 128, 128, 256, 256, new windowData, WS_ROOT_WINDOW_ID);
	nid = wsCreateWindow(WS_STYLE_NO_BUFFER_RESIZE | WS_STYLE_ALIGN_RD, "Window2", &wndCbk, 64, 64, 128, 128, new windowData, id);
	wsCreateWindow(WS_STYLE_NO_BUFFER_RESIZE | WS_STYLE_NORESIZE, "Window3", &wndCbk, 32, 32, 64, 64, new windowData, nid);
	wsMainLoop();
	return 0;
}
