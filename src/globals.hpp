#pragma once
#include <dxgi.h>
#include "imgui/imgui.h"
#include <string>

typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

namespace Globals
{
	inline bool il2cppStatus = false;
	inline bool showMenu = false;
	inline bool callMethod = false;
	inline bool includeChildrenComp = false;

	inline const char* MethodRetType[] = { "void", "int", "float", "bool"};
	inline int MethodRetType_Current = 0;
	inline const char* MethodArgType[] = { "int", "float", "bool", "string", "no argument"};
	inline int MethodArgType_Current = 0;

	inline bool showInt = true;
	inline bool showFloat = true;
	inline bool showBool = true;
	inline bool showClass = true;
	inline bool showProperties = true;

	namespace Gui
	{
		inline Present oPresent;
		inline ImGuiStyle* style;
		inline std::string window = "component";
		inline ImColor color(62, 63, 65);
		inline ImColor* theme = &color;
	}
}