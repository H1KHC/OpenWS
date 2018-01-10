#include <openWS.h>

static char errString[][64] = {
	"WS_ERR_NO_ERROR",
	"WS_ERR_UNKNOWN_ERROR",

	"WS_ERR_NOT_INITIALIZED",
	"WS_ERR_GLFW_INITIALIZATION_FAILED",
	"WS_ERR_GLEW_INITIALIZATION_FAILED",
	"WS_ERR_THREAD_CREATE_FAILED",

	"WS_ERR_BUFFER_RESIZE_FAILED",
	"WS_ERR_WINDOW_NOT_FOUND",

	"WS_ERR_INVALID_VALUE",
	"WS_ERR_ILLEGAL_OPERATION",

	"WS_WARN_THREAD_HAS_EXITED",
};

extern int wsError;

void wsSetError(const int err) {
	wsError = err;
}

unsigned wsLastError() {
	return wsError;
}
const char * wsErrorString(const int err) {
	return errString[err];
}
