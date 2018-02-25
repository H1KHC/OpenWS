#ifndef __WINDOW_SYSTEM_H__
#define __WINDOW_SYSTEM_H__

#ifdef WS_STATIC
#	define WS_API
#else
#	if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#		ifdef WS_DLL_IMPLEMENT
#			define WS_API __declspec(dllexport)
#		else
#			define WS_API __declspec(dllimport)
#		endif
#	else
#		define WS_API __attribute__((visibility("default")))
#	endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

// The mouse and keyboard macros are copied from GLFW

// Mouse buttons
#define WS_MOUSE_BUTTON_1         0
#define WS_MOUSE_BUTTON_2         1
#define WS_MOUSE_BUTTON_3         2
#define WS_MOUSE_BUTTON_4         3
#define WS_MOUSE_BUTTON_5         4
#define WS_MOUSE_BUTTON_6         5
#define WS_MOUSE_BUTTON_7         6
#define WS_MOUSE_BUTTON_8         7
#define WS_MOUSE_BUTTON_LEFT      WS_MOUSE_BUTTON_1
#define WS_MOUSE_BUTTON_RIGHT     WS_MOUSE_BUTTON_2
#define WS_MOUSE_BUTTON_MIDDLE    WS_MOUSE_BUTTON_3
#define WS_MOUSE_BUTTON_LAST      WS_MOUSE_BUTTON_8

// Keyboard
#define WS_KEY_UNKNOWN            -1

#define WS_KEY_SPACE              32
#define WS_KEY_APOSTROPHE         39  /* ' */
#define WS_KEY_COMMA              44  /* , */
#define WS_KEY_MINUS              45  /* - */
#define WS_KEY_PERIOD             46  /* . */
#define WS_KEY_SLASH              47  /* / */
#define WS_KEY_0                  48
#define WS_KEY_1                  49
#define WS_KEY_2                  50
#define WS_KEY_3                  51
#define WS_KEY_4                  52
#define WS_KEY_5                  53
#define WS_KEY_6                  54
#define WS_KEY_7                  55
#define WS_KEY_8                  56
#define WS_KEY_9                  57
#define WS_KEY_SEMICOLON          59  /* ; */
#define WS_KEY_EQUAL              61  /* = */
#define WS_KEY_A                  65
#define WS_KEY_B                  66
#define WS_KEY_C                  67
#define WS_KEY_D                  68
#define WS_KEY_E                  69
#define WS_KEY_F                  70
#define WS_KEY_G                  71
#define WS_KEY_H                  72
#define WS_KEY_I                  73
#define WS_KEY_J                  74
#define WS_KEY_K                  75
#define WS_KEY_L                  76
#define WS_KEY_M                  77
#define WS_KEY_N                  78
#define WS_KEY_O                  79
#define WS_KEY_P                  80
#define WS_KEY_Q                  81
#define WS_KEY_R                  82
#define WS_KEY_S                  83
#define WS_KEY_T                  84
#define WS_KEY_U                  85
#define WS_KEY_V                  86
#define WS_KEY_W                  87
#define WS_KEY_X                  88
#define WS_KEY_Y                  89
#define WS_KEY_Z                  90
#define WS_KEY_LEFT_BRACKET       91  /* [ */
#define WS_KEY_BACKSLASH          92  /* \ */
#define WS_KEY_RIGHT_BRACKET      93  /* ] */
#define WS_KEY_GRAVE_ACCENT       96  /* ` */
#define WS_KEY_WORLD_1            161 /* non-US #1 */
#define WS_KEY_WORLD_2            162 /* non-US #2 */

#define WS_KEY_ESCAPE             256
#define WS_KEY_ENTER              257
#define WS_KEY_TAB                258
#define WS_KEY_BACKSPACE          259
#define WS_KEY_INSERT             260
#define WS_KEY_DELETE             261
#define WS_KEY_RIGHT              262
#define WS_KEY_LEFT               263
#define WS_KEY_DOWN               264
#define WS_KEY_UP                 265
#define WS_KEY_PAGE_UP            266
#define WS_KEY_PAGE_DOWN          267
#define WS_KEY_HOME               268
#define WS_KEY_END                269
#define WS_KEY_CAPS_LOCK          280
#define WS_KEY_SCROLL_LOCK        281
#define WS_KEY_NUM_LOCK           282
#define WS_KEY_PRINT_SCREEN       283
#define WS_KEY_PAUSE              284
#define WS_KEY_F1                 290
#define WS_KEY_F2                 291
#define WS_KEY_F3                 292
#define WS_KEY_F4                 293
#define WS_KEY_F5                 294
#define WS_KEY_F6                 295
#define WS_KEY_F7                 296
#define WS_KEY_F8                 297
#define WS_KEY_F9                 298
#define WS_KEY_F10                299
#define WS_KEY_F11                300
#define WS_KEY_F12                301
#define WS_KEY_F13                302
#define WS_KEY_F14                303
#define WS_KEY_F15                304
#define WS_KEY_F16                305
#define WS_KEY_F17                306
#define WS_KEY_F18                307
#define WS_KEY_F19                308
#define WS_KEY_F20                309
#define WS_KEY_F21                310
#define WS_KEY_F22                311
#define WS_KEY_F23                312
#define WS_KEY_F24                313
#define WS_KEY_F25                314
#define WS_KEY_KP_0               320
#define WS_KEY_KP_1               321
#define WS_KEY_KP_2               322
#define WS_KEY_KP_3               323
#define WS_KEY_KP_4               324
#define WS_KEY_KP_5               325
#define WS_KEY_KP_6               326
#define WS_KEY_KP_7               327
#define WS_KEY_KP_8               328
#define WS_KEY_KP_9               329
#define WS_KEY_KP_DECIMAL         330
#define WS_KEY_KP_DIVIDE          331
#define WS_KEY_KP_MULTIPLY        332
#define WS_KEY_KP_SUBTRACT        333
#define WS_KEY_KP_ADD             334
#define WS_KEY_KP_ENTER           335
#define WS_KEY_KP_EQUAL           336
#define WS_KEY_LEFT_SHIFT         340
#define WS_KEY_LEFT_CONTROL       341
#define WS_KEY_LEFT_ALT           342
#define WS_KEY_LEFT_SUPER         343
#define WS_KEY_RIGHT_SHIFT        344
#define WS_KEY_RIGHT_CONTROL      345
#define WS_KEY_RIGHT_ALT          346
#define WS_KEY_RIGHT_SUPER        347
#define WS_KEY_MENU               348

#define WS_MOD_SHIFT            0x0001
#define WS_MOD_CONTROL          0x0002
#define WS_MOD_ALT              0x0004
#define WS_MOD_SUPER            0x0008
#define WS_MOD_CAPS_LOCK        0x0010
#define WS_MOD_NUM_LOCK         0x0020

#define WS_KEY_LAST               WS_KEY_MENU

//	These are all the callbacks the window will use,
//	if the callback function returns a integer, it
//	means the user can choose whether to call next window's
//	callback (on 1) or stop transfering it (on 0).
typedef void(*wsDisplayCallback)		(int windowID, int currentFramebuffer);

typedef int(*wsMouseButtonCallback)		(int windowID, int button, int action, int mods);
typedef int(*wsCursorMoveCallback)		(int windowID, int xpos, int ypos);
typedef void(*wsCursorEnterCallback)	(int windowID, int entered);
typedef int(*wsScrollCallback)			(int windowID, double xoffset, double yoffset);
typedef int(*wsKeyboardCallback)		(int windowID, int key, int scancode, int action, int mods);
typedef int(*wsCharCallback)			(int windowID, unsigned int unicode, int mods);
typedef int(*wsFileDropCallback)		(int windowID, int count, const char **filename);

typedef void(*wsWindowMoveCallback)		(int windowID, int x, int y);
typedef void(*wsWindowResizeCallback)	(int windowID, int w, int h);
typedef void(*wsWindowCloseCallback)	(int windowID);
typedef void(*wsWindowFocusCallback)	(int windowID, int focused);
typedef void(*wsWindowIconifyCallback)	(int windowID, int iconified);
typedef struct wsCoord2 {
	int x, y;
} wsCoord2;

typedef struct wsWindowInfo {
	char *name;
	int windowID;
	wsCoord2 position, size;
	unsigned texture;
	wsCoord2 bufferSize;
	int focused, underCursor;
	wsCoord2 cursorPos;
	int styleMask;
	wsDisplayCallback displayCallback;
	wsMouseButtonCallback mouseButtonCallback;
	wsCursorMoveCallback cursorMoveCallback;
	wsCursorEnterCallback cursorEnterCallback;
	wsScrollCallback scrollCallback;
	wsKeyboardCallback keyboardCallback;
	wsCharCallback charCallback;
	wsFileDropCallback fileDropCallback;
	wsWindowMoveCallback windowMoveCallback;
	wsWindowResizeCallback windowResizeCallback;
	wsWindowCloseCallback windowCloseCallback;
	wsWindowFocusCallback windowFocusCallback;
	wsWindowIconifyCallback windowIconifyCallback;
	void *userData;
} wsWindowInfo;

// errors
enum {
	WS_ERR_NO_ERROR,
	WS_ERR_UNKNOWN_ERROR,

	//INIT
	WS_ERR_NOT_INITIALIZED,
	WS_ERR_GLFW_INITIALIZATION_FAILED,
	WS_ERR_GLEW_INITIALIZATION_FAILED,
	WS_ERR_THREAD_CREATE_FAILED,

	//WINDOW OPERATION
	WS_ERR_FRAMEBUFFER_GENERATE_FAILED,
	WS_ERR_BUFFER_RESIZE_FAILED,
	WS_ERR_WINDOW_NOT_FOUND,

	//FUNCTION CALL
	WS_ERR_INVALID_VALUE,
	WS_ERR_ILLEGAL_OPERATION,

	//FUNCTIONAL
	WS_ERR_UNIMPLEMENTED,

	//WARNINGS
	WS_WARN_THREAD_HAS_EXITED,
};

#ifdef __cplusplus
#define DEFAULT(x) =(x)
#else
#define DEFAULT(x)
#endif

// Just 0
#define WS_FALSE					0
// Just 1
#define WS_TRUE						1

// The button is released
#define WS_RELEASE					0
// The button is pressed
#define WS_PRESS					1
// The button is held to be repeated
#define WS_REPEAT					2

// The return value when you attempted to find a window that is not existed
#define WS_INVALID_WINDOW_ID		-1
// The root window's id
#define WS_ROOT_WINDOW_ID			0

// Initialize the openWS library
WS_API int wsSetWindowHint(int hint, int value);
WS_API int wsInit();
// Deinitialize the openWS library
WS_API int wsDeinit();

// Ignore move message
#define WS_STYLE_NOMOVE					0x00000001
// Ignore resize message
#define WS_STYLE_NORESIZE				0x00000002
// Ignore focus message
#define WS_STYLE_NOFOCUS				0x00000004
// Ignore display message
#define WS_STYLE_NODISPLAY				0x00000008

// Align mode when resizing the window
#define WS_STYLE_ALIGN_LU				0x00000000
#define WS_STYLE_ALIGN_LD				0x00000010
#define WS_STYLE_ALIGN_RU				0x00000020
#define WS_STYLE_ALIGN_RD				0x00000040
#define WS_STYLE_ALIGN_MASK				0x00000070
// Ignore buffer resize when resizing the window
#define WS_STYLE_NO_BUFFER_RESIZE		0x00000080

#define WS_STYLE_NO_FRAMEBUFFER			0x00000100
#define WS_STYLE_NO_DEPTHBUFFER			0x00000200
#define WS_STYLE_STATIC_WINDOW			WS_STYLE_NO_FRAMEBUFFER
#define WS_STYLE_2D_WINDOW				WS_STYLE_NO_DEPTHBUFFER

// Default style
#define WS_STYLE_DEFAULT				WS_STYLE_ALIGN_LU | WS_STYLE_NO_DEPTHBUFFER
WS_API int wsCreateWindow(const char *windowName, int x, int y, int width, int height, void *windowData, int windowStyle DEFAULT(WS_STYLE_DEFAULT), int fatherWindowID DEFAULT(WS_ROOT_WINDOW_ID));
WS_API int wsCloseWindow(int windowID);
WS_API int wsAttachWindow(int subwindowID, int fatherWindowID);
WS_API int wsFocusWindow(int windowID);

//Global get functions

WS_API int wsGetForegroundWindow();
WS_API int wsGetWindowUnderCursor();
WS_API int wsGetWindowOnPos(int x, int y);
WS_API int wsGetWindowByName(const char *name);

//Window-related get functions

WS_API int wsGetFatherWindow(int windowID);
WS_API int wsGetTopSubWindow(int windowID);
WS_API int wsGetPrevWindow(int windowID);
WS_API int wsGetNextWindow(int windowID);

//Window-based get functions

WS_API int wsGetWindowSize(int windowID, int *w, int *h);
WS_API int wsGetWindowPos(int windowID, int *x, int *y);
WS_API int wsGetWindowStyle(int windowID, int *style);
WS_API int wsGetWindowData(int windowID, void **data);
WS_API int wsGetWindowCursorPos(int windowID, int *x, int *y);
WS_API int wsGetWindowInfo(int windowID, wsWindowInfo *info);

//Global set functions

WS_API int wsPostRedisplay(int window);
WS_API int wsSetSwapInterval(int window, int interval);
WS_API int wsSetFPS(int window, int fps);

WS_API int wsSetJoystickConnectionCallback(void(*func)(int joystickID, int joystickState));
WS_API int wsSetWindowIconifyCallback(int windowID, wsWindowIconifyCallback callback);

#if defined(_DEBUG) || defined(__DEBUG__)
#define WS_SDM_CLOSE				0x00000000
#define WS_SDM_FOCUSWINDOW			0x00000001
#define WS_SDM_CURSORWINDOW			0x00000002
#define WS_SDM_NO_CURSORPOS			0x00000003
#define WS_SDM_CURSORPOS			0x00000004
#define WS_SDM_NO_CURSORWINDOW		0x00000005
#define WS_SDM_NO_FOCUSWINDOW		0x00000006
#define WS_SDM_FULL					0x00000007
WS_API int wsSetDebugMode(int mode DEFAULT(WS_SDM_CLOSE));
#else
#define wsSetDebugMode(...)
#endif

//Window-based set functions
WS_API int wsSetWindowDisplayCallback(int windowID, wsDisplayCallback callback);
WS_API int wsSetWindowMouseButtonCallback(int windowID, wsMouseButtonCallback callback);
WS_API int wsSetWindowCursorMoveCallback(int windowID, wsCursorMoveCallback callback);
WS_API int wsSetWindowCursorEnterCallback(int windowID, wsCursorEnterCallback callback);
WS_API int wsSetWindowScrollCallback(int windowID, wsScrollCallback callback);
WS_API int wsSetWindowKeyboardCallback(int windowID, wsKeyboardCallback callback);
WS_API int wsSetWindowCharCallback(int windowID, wsCharCallback callback);
WS_API int wsSetWindowFileDropCallback(int windowID, wsFileDropCallback callback);
WS_API int wsSetWindowMoveCallback(int windowID, wsWindowMoveCallback callback);
WS_API int wsSetWindowResizeCallback(int windowID, wsWindowResizeCallback callback);
WS_API int wsSetWindowCloseCallback(int windowID, wsWindowCloseCallback callback);
WS_API int wsSetWindowFocusCallback(int windowID, wsWindowFocusCallback callback);

#define WS_SWSI_RELATIVE		0x00000000
#define WS_SWSI_ABSOLUTE		0x00000001
#define WS_SWSI_PROPORTIONAL	0x00000002
#define WS_SWSI_MOVEMODE_MASK	0x0000000F

WS_API int wsSetWindowSize(int windowID, double w, double h, int mode DEFAULT(WS_SWSI_RELATIVE));
WS_API int wsSetWindowBufferSize(int windowID, double w, double h, int mode DEFAULT(WS_SWSI_RELATIVE));

#define WS_SWP_RELATIVE			0x00000000
#define WS_SWP_ABSOLUTE			0x00000001
#define WS_SWP_MOVEMODE_MASK	0x00000001
WS_API int wsSetWindowPos(int windowID, int x, int y, int mode DEFAULT(WS_SWP_RELATIVE));

//Totally cover the original settings
#define WS_SWST_COVER			0x00000000
//Only adjust those which were null
#define WS_SWST_ADD				0x00000001
//Only adjust those which were not null
#define WS_SWST_UPDATE			0x00000002
WS_API int wsSetWindowStyle(int windowID, int style, int mode DEFAULT(WS_SWST_COVER));

WS_API int wsSetWindowData(int windowID, void *data);


//Error handling

WS_API unsigned wsLastError();
WS_API const char * wsErrorString(int err);


//Main loop

WS_API void wsMainLoop();

WS_API int wsTerminate();

struct GLFWwindow;
/***
  * get glfwWindow of specificated wswindow
  * if windowID is WS_ROOT_WINDOW_ID, then returns the foreground one's
***/
WS_API GLFWwindow* wsGetGLFWWindow(int windowID DEFAULT(WS_ROOT_WINDOW_ID));

#ifdef __cplusplus
}
#endif

#undef DEFAULT

#endif
