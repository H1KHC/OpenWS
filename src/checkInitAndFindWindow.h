#define checkInit(failReturnValue) \
do { \
	extern int inited; \
	if (!inited) { \
		wsSetError(WS_ERR_NOT_INITIALIZED); \
		return (failReturnValue); \
	} \
} while(0)

#define findWindow(window, id, failReturnValue) \
do{ \
	extern thread_local wsWindow* currentWindow; \
	if(currentWindow && currentWindow->windowID == (id)) \
			(window) = currentWindow; \
	else (window) = windowManager.findWindow(id); \
	if (!(window)) { \
		wsSetError(WS_ERR_WINDOW_NOT_FOUND); \
		return (failReturnValue); \
	} \
} while(0)

#define checkInitAndFindWindow(window, id, failReturnValue) \
do{ \
	checkInit(failReturnValue); \
	findWindow(window, id, failReturnValue); \
} while(0)
