#include <stdio.h>
#include <GL/gl.h>
#include <openWS.h>

int rootWindow;

struct windowData {
	float theta, theta1, theta2;
	int leftButton, rightButton, firstPress;
	int x, y;
	windowData() : theta(0), theta1(0), theta2(0),
		leftButton(0), rightButton(0), firstPress(0),
		x(0), y(0) {}
};

void display(int windowID, int) {
	windowData *data;
	wsGetWindowUserPointer(windowID, (void **)&data);
	glClearColor(0.0f, 0.1f, 0.3f, 0.5f);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, -1, 1);
	glPushMatrix();
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

int mouseButton(int windowID, int button, int action, int mods) {
	windowData *data;
	wsGetWindowUserPointer(windowID, (void **)&data);
	if(windowID) {
	 	if(button == WS_MOUSE_BUTTON_LEFT) data->leftButton = data->firstPress = action;
	 	else if(button == WS_MOUSE_BUTTON_RIGHT) data->rightButton = data->firstPress = action;
	//	else if(button == WS_MOUSE_BUTTON_MIDDLE) wsCloseWindow(windowID);
	} else if(button == WS_MOUSE_BUTTON_LEFT && action) {
	//	int id = wsCreateWindow(WS_STYLE_DEFAULT, "", &wndCbk, 128, 128, 256, 256, rootWindow);
	//	wsSetWindowUserPointer(id, new windowData);
	}
	printf("Mouse.button: Window %d, button %d, action %d, mods %d\n", windowID, button, action, mods);
	return 0;
}

int cursorMove(int windowID, int xpos, int ypos) {
	if(windowID) {
		windowData *data;
		wsGetWindowUserPointer(windowID, (void **)&data);
		if(data->firstPress) {
			data->firstPress = false;
			data->x = xpos, data->y = ypos;
		} else {
			if(data->leftButton)
				wsSetWindowPos(windowID, xpos - data->x, ypos - data->y, WS_SWP_RELATIVE);
			else if(data->rightButton) {
				wsSetWindowSize(windowID, xpos - data->x, ypos - data->y, WS_SWP_RELATIVE);
				data->x = xpos;
				data->y = ypos;
			}
		}
	}
	printf("cousor.move: Window %d, xpos %d, ypos %d\n", windowID, xpos, ypos);
	return 0;
}

void cursorEnter(int windowID, int entered) {
	printf("cursor.enter: Window %d, entered %d\n", windowID, entered);
	if(windowID && !entered) {
		windowData *data;
		wsGetWindowUserPointer(windowID, (void **)&data);
		data->leftButton = (data->rightButton = 0);
	}
}

int scroll(int windowID, double xoffset, double yoffset) {
	printf("scroll: window %d, xoffset %.2lf, yoffset %.2lf\n", windowID, xoffset, yoffset);
	return 0;
}

int key(int windowID, int key, int scancode, int action, int mods) {
	printf("key: window %d, key %d, scancode %d, action %d, mods %d\n", windowID, key, scancode, action, mods);
	return 0;
}

int charC(int windowID, unsigned int unicode, int mods) {
	printf("char: window %d, unicode %d, mods %d\n", windowID, unicode, mods);
	return 0;
}

int fileDrop(int windowID, int count, const char **filename) {
	printf("files: window %d, count %d,\n", windowID, count);
	for(int i = 0; i < count; ++i)
		printf("\t#%2d: %s\n", i + 1, filename[i]);
	return 0;
}

void windowMove(int windowID, int x, int y) {
	printf("window.move: window %d, x %d, y %d\n", windowID, x, y);
}

void windowResize(int windowID, int w, int h) {
	printf("window.resize: window %d, w %d, h %d\n", windowID, w, h);
}

void windowClose(int windowID) {
	printf("window.close: window %d\n", windowID);
}

void windowFocus(int windowID, int focused) {
	printf("window.focus: window %d, focused %d\n", windowID, focused);
}

int main() {
	if (!wsInit()){
		unsigned err = wsLastError();
		printf("Init failed, error code: %d %s", err, wsErrorString(err));
		return -1;
	}
	rootWindow = wsCreateWindow("messages", 200, 200, 512, 512, new windowData);
	wsSetWindowDisplayCallback(rootWindow, display);
	wsSetMouseButtonCallback(rootWindow, mouseButton);
	wsSetCursorPosCallback(rootWindow, cursorMove);
	wsSetCursorEnterCallback(rootWindow, cursorEnter);
	wsSetScrollCallback(rootWindow, scroll);
	wsSetKeyCallback(rootWindow, key);
	wsSetCharModsCallback(rootWindow, charC);
	wsSetDropCallback(rootWindow, fileDrop);
	wsSetWindowPosCallback(rootWindow, windowMove);
	wsSetWindowSizeCallback(rootWindow, windowResize);
	wsSetWindowCloseCallback(rootWindow, windowClose);
	wsSetWindowFocusCallback(rootWindow, windowFocus);
	wsSetDebugMode(WS_SDM_FULL);
	wsSetWindowUserPointer(rootWindow, new windowData);
	int id = wsCreateWindow("", 128, 128, 256, 256, new windowData, WS_STYLE_DEFAULT, rootWindow);
	wsSetWindowDisplayCallback(id, display);
	wsSetMouseButtonCallback(id, mouseButton);
	wsSetCursorPosCallback(id, cursorMove);
	wsSetCursorEnterCallback(id, cursorEnter);
	wsSetScrollCallback(id, scroll);
	wsSetKeyCallback(id, key);
	wsSetCharModsCallback(id, charC);
	wsSetDropCallback(id, fileDrop);
	wsSetWindowPosCallback(id, windowMove);
	wsSetWindowSizeCallback(id, windowResize);
	wsSetWindowCloseCallback(id, windowClose);
	wsSetWindowFocusCallback(id, windowFocus);
	wsMainLoop();
}