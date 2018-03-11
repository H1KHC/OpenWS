#include "wsWindow.h"
#include "wsWindowManager.h"

wsWindow::wsWindow(const char * title, const wsCoord2 position,
				   const wsCoord2 size, const int style) :
	wsWindowInfo{
		nullptr,			//name
		0,					//windowID
		position, size,		//position, size
		0,					//texture
		size,				//bufferSize
		false, false,		//focused, underCursor
		wsCoord2{ 0, 0 },	//cursorPos
		style,				//styleMask
		nullptr,			//displayCallback
		nullptr,			//mouseButtonCallback
		nullptr,			//cursorMoveCallback
		nullptr,			//cursorEnterCallback
		nullptr,			//scrollCallback
		nullptr,			//keyCallback
		nullptr,			//charCallback
		nullptr,			//fileDropCallback
		nullptr,			//windowMoveCallback
		nullptr,			//windowResizeCallback
		nullptr,			//windowCloseCallback
		nullptr,			//windowFocusCallback
		nullptr,			//windowIconifyCallback
		nullptr				//userData
	},
	fpsClock(nullptr),
	fpsControl(false), FPS(0), needRedraw(false), needRedisplay(false),
	framebuffer(0), depthbuffer(0),
	fatherWindow(nullptr),
	topWindow(nullptr), topWindowChangeTag(false),
	thisWindowNode(nullptr),
	windowUnderCursor(nullptr)
{
	int len = 0;
	while (title[len]) ++len;
	name = new char[len + 1];
	while (~len) name[len] = title[len], --len;
	windowManager.registerWindow(this);
}

wsWindow::~wsWindow() {
}

void wsWindow::deleteWindow() {
	if (fatherWindow) {
		if (fatherWindow->windowUnderCursor &&
		  fatherWindow->windowUnderCursor->data == this)
			fatherWindow->windowUnderCursor = nullptr;
		fatherWindow->subWindow.deleteNode(this);
	} else {
		extern std::map<GLFWwindow*, wsBaseWindow*> baseWindows;
		baseWindows[((wsBaseWindow*)this)->glfwWindow] = nullptr;
		glfwSetWindowShouldClose(((wsBaseWindow*)this)->glfwWindow, 1);
	}
	windowManager.cancelWindow(this);
	delete[] name;
	extern thread_local wsBaseWindow* currentBaseWindow;
	if(topWindow != currentBaseWindow) {
		int wsInitGLEW();
		glfwMakeContextCurrent(topWindow->glfwWindow);
		wsInitGLEW();
	}
	if (framebuffer) glDeleteFramebuffers(1, &framebuffer);
	if (depthbuffer) glDeleteRenderbuffers(1, &depthbuffer);
	if (texture) glDeleteTextures(1, &texture);
	if(topWindow != currentBaseWindow) {
		int wsInitGLEW();
		glfwMakeContextCurrent(currentBaseWindow->glfwWindow);
		wsInitGLEW();
	}
	delete this;
}
