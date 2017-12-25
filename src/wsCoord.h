#ifndef __WS_COORD_H__
#define __WS_COORD_H__

template <class T>
struct wsCoord2 {
	T x, y;
	wsCoord2(const T x = 0, const T y = 0) : x(x), y(y) {}
};
template <class T>
struct wsCoord3 {
	T x, y, z;
	wsCoord3(const T x = 0, const T y = 0, const T z = 0) : x(x), y(y), z(z) {}
};

template <class T>
struct wsCoord4 {
	T x, y, z, w;
	wsCoord4(const T x = 0, const T y = 0, const T z = 0, const T w = 0) : x(x), y(y), z(z), w(w) {}
};

#endif