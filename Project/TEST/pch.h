#pragma once

//C & C++ headers
#include <cassert>

#include <algorithm>
#include <array>
#include <chrono>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>
#include <locale>
#include <codecvt>
#include <fstream>
#include <sstream>

//#include <filesystem>
#include <random>

//window specific
#include <crtdbg.h>
#define CRTDBG_MAP_ALLOC

#include <Windows.h>
#define WIN32_LEAN_AND_MEAN

//third party libs
#include <CodeAnalysis/Warnings.h>
#pragma warning(push)
#pragma warning (disable :ALL_CODE_ANALYSIS_WARNINGS)
#include <tinyXml/tinyxml2.h>
#pragma warning(pop)
#ifdef _DEBUG
#pragma comment(lib,"tinyXml/tinyXml2_debug.lib")
#else
#pragma comment(lib,"tinyXml/tinyXml2.lib")
#endif


//game engine specific
#include "Define.h"
#include "Struct.h"
#include "Func.h"
#include "Macro.h"
#include "Events.h"
