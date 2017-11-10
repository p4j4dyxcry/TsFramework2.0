#pragma once

#include <algorithm>
#include <atomic>
#include <cfloat>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <ctime>
#include <cmath>
#include <functional>
#include <new>
#include <future>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include <vector>

//! externals
#include <Windows.h>

#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib,"d3dcompiler.lib")

//! TsFramework
#include "Code/Core/ForwardDefinition.h"
#include "Code/Utility/Serialize.h"
#include "Code/Utility/StringUtility.h"
#include "Code/Core/Develop.h"
#include "Code/Core/Time.h"
#include "Code/Core/Memory/MemorySystem.h"
#include "Code/Core/Memory/Pointer.h"

//! Gfx
#include "Code/Gfx/GfxCommon.h"
#include "Code/Gfx/IGfxResource.h"

#include "Code/Gfx/GfxDevice.h"
#include "Code/Gfx/GfxCore.h"