#pragma once

#define KBMO_VERSION "0.0"	

#include <stdint.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <queue>
#include <deque>
#include <stack>
#include <cassert>
#include <map>
#include <set>
#include <unordered_set>
#include <functional>
#include <chrono>
#include <filesystem>

#define _USE_MATH_DEFINES // for M_PI and similar
#include <cmath>

// Remove need for std namespace scope for very common datatypes
using std::vector;
using std::map;
using std::unordered_map;
using std::set;
using std::multiset;
using std::unordered_set;
using std::list;
using std::stack;
using std::deque;
using std::string;
using std::wstring;
using std::pair;

// Integer typedefs
typedef int8_t	int8;	typedef uint8_t	 uint8;
typedef int16_t int16;	typedef uint16_t uint16;
typedef int32_t int32;	typedef uint32_t uint32;
typedef int64_t int64;	typedef uint64_t uint64;
typedef uint8_t byte;

// Current millisecond time
#define CUR_MS() (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count())

#define MAX(a, b) ((a > b) ? a : b)
#define MIN(a, b) ((a < b) ? a : b)

#define CLAMP(val, min, max) MIN(MAX(val, min), max)

#define STR(s) ([&]{ std::stringstream __macroStream; __macroStream << s; return __macroStream.str(); }())

// Returns sign of number (1 if positive, -1 if negative, and 0 if 0)
#define SGN(val) ((val > 0) - (val < 0))

// Bakkesmod
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/wrappers/CanvasWrapper.h"
#include "bakkesmod/wrappers/GameObject/BallWrapper.h"
#include "bakkesmod/wrappers/Engine/WorldInfoWrapper.h"
#include "bakkesmod/wrappers/GameEvent/ServerWrapper.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"
#include "bakkesmod/plugin/PluginWindow.h"

static inline CVarManagerWrapper* g_CVarManager;
#define LOG(s) if (g_CVarManager) { g_CVarManager->log(STR(s)); }

// Debug logging
#if defined(_RELDEBUG) or defined(_DEBUG)
#define DLOG(s) LOG("[DBG] " << s)
#else
#define DLOG(s) {}
#endif

#define DEG2RAD(x) (x * (M_PI / 180))
#define RAD2DEG(x) (x * (180 / M_PI))

inline std::ostream& operator <<(std::ostream& stream, Vector v) {
	stream << "[ " << v.X << ", " << v.Y << ", " << v.Z << " ]";
	return stream;
}

#define C_RED		ImColor(255, 0, 0, 255)
#define C_YELLOW	ImColor(255, 255, 0, 255)
#define C_GREEN		ImColor(0, 255, 0, 255)
#define C_BLUE		ImColor(0, 150, 255, 255)
#define C_MAGENTA	ImColor(128, 0, 128, 255)
#define C_WHITE		ImColor(255, 255, 255, 255)
#define C_WHITE_DIM	ImColor(230, 230, 230, 255)
#define C_BLACK		ImColor(0, 0, 0, 255)
#define C_GRAY		ImColor(170, 170, 170, 255)
#define C_DARKGRAY	ImColor(60, 60, 60, 255)