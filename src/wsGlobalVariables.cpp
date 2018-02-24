#include "wsWindow.h"
#include "wsWindowManager.h"

// window local needed

std::map<GLFWwindow*, wsBaseWindow*> baseWindows;
thread_local wsWindow *currentWindow, *currentBaseWindow;
wsWindowManager windowManager;

int wsError, debugOutput;
int inited;