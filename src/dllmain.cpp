#include <iostream>
#include <Windows.h>
#include "kiero/kiero.h"
#include "il2cppresolver/Il2CppResolver.hpp"
#include "globals.hpp"
#include "gui.hpp"
#include "Methods.hpp"

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	printf("Initializing IL2CPP...\n");

	Globals::il2cppStatus = IL2CPP::Initialize(true);

	if (!Globals::il2cppStatus) {
		printf("[-] Can't Initialize IL2CPP!\n\n");
		exit(1);
	}

	printf("[+] IL2CPP Initialized!\n\n");

	bool init_hook = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			IL2CPP::Callback::Initialize();
			IL2CPP::Callback::OnUpdate::Add(OnUpdate);
			kiero::bind(8, (void**)&Globals::Gui::oPresent, hkPresent);
			init_hook = true;

			printf("[+] IL2CPP Scanner Reborn loaded");
		}
	} while (!init_hook);
	return TRUE;
}


BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}