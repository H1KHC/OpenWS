#include "wsWindow.h"
#include "wsWindowManager.h"
#include "wsClock.h"

wsClock *fpsClock;
wsBaseWindow *baseWindow;
wsWindow *currentWindow;
wsWindowManager windowManager;

int wsError, debugOutput, wsFPS;
int inited, fpsControl = false, wsNeedRedisplay = false;