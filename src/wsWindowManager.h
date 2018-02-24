#ifndef __WS_WINDOWMANAGER_H__
#define __WS_WINDOWMANAGER_H__
#include <map>
#include <string.h> 
#include "wsWindow.h"
#include "wsList.h"

typedef class wsWindowManager {
	std::map<int, wsWindow *> windowMap;
	wsList<int> IDRecycler;
public:
	int genID() {
		if (!IDRecycler.isEmpty()) {
			int t = IDRecycler.front()->data;
			IDRecycler.deleteFront();
			return t;
		}
		if (windowMap.empty()) return 1;
		return (--(windowMap.end()))->first + 1;
	}
	void registerWindow(wsWindow *window) {
		windowMap[window->windowID = genID()] = window;
	}
	wsWindow *findWindow(int windowID) {
		std::map<int, wsWindow *>::iterator it = windowMap.find(windowID);
		if (it != windowMap.end()) return it->second;
		return nullptr;
	}
	wsWindow *findWindowByName(const char *name) {
		for (std::map<int, wsWindow *>::iterator it = windowMap.begin(); it != windowMap.end(); ++it)
			if (!strcmp(it->second->name, name)) return it->second;
		return nullptr;
	}
	void cancelWindow(wsWindow *window) {
		windowMap.erase(window->windowID);
		IDRecycler.addBack(window->windowID);
	}
} wsWindowManager;

extern wsWindowManager windowManager;

#endif