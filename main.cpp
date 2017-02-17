// Unreal Engine SDK Generator Test
// by KN4CK3R
// https://www.oldschoolhack.me

#include <windows.h>

#include "PatternFinder.hpp"
#include "Hook.hpp"

// Include the SDK header and add the needed cpp files to the project.
// This example uses the generated SDK for Unreal Tournament 3.
#include "C:/SDK_GEN/UT3/SDK.hpp"

using namespace Classes;

/// <summary>Initializes the objects store.</summary>
/// <returns>True if it succeeds, false if it fails.</returns>
bool InitializeObjectsStore()
{
	// Search for the pattern.
	auto address = FindPattern(GetModuleHandleW(nullptr), reinterpret_cast<const unsigned char*>("\xA1\x00\x00\x00\x00\x8B\x00\x00\x8B\x00\x00\x25\x00\x02\x00\x00"), "x????x??x??xxxxx");
	if (address == -1)
	{
		return false;
	}

	// Initialize the global objects array pointer.
	UObject::GObjects = reinterpret_cast<decltype(UObject::GObjects)>(*reinterpret_cast<uint32_t*>(address + 1));

	return UObject::GObjects != nullptr;
}

/// <summary>Initializes the names store.</summary>
/// <returns>True if it succeeds, false if it fails.</returns>
bool InitializeNamesStore()
{
	// Search for the pattern.
	auto address = FindPattern(GetModuleHandleW(nullptr), reinterpret_cast<const unsigned char*>("\x8B\x0D\x00\x00\x00\x00\x83\x3C\x81\x00\x74"), "xx????xxxxx");
	if (address == -1)
	{
		return false;
	}

	// Initialize the global names array pointer.
	FName::GNames = reinterpret_cast<decltype(FName::GNames)>(*reinterpret_cast<uint32_t*>(address + 2));

	return FName::GNames != nullptr;
}

typedef void(__fastcall *oProcessEvent)(UObject*, DWORD, UFunction*, void*, void*);
oProcessEvent pProcessEvent;

/// <summary>A process event hook which prints every caller.</summary>
void __fastcall Hook_ProcessEvent(UObject *caller, DWORD edx, UFunction *function, void *parameter, void *result)
{
	if (caller != nullptr && function != nullptr)
	{
		auto funcName = function->GetFullName();

		MessageBoxA(nullptr, funcName.c_str(), "Function", 0);
	}

	pProcessEvent(caller, edx, function, parameter, result);
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);

		if (!InitializeObjectsStore())
		{
			MessageBoxA(nullptr, "InitializeObjectsStore failed", "Error", 0);
			return FALSE;
		}

		if (!InitializeNamesStore())
		{
			MessageBoxA(nullptr, "InitializeNamesStore failed", "Error", 0);
			return FALSE;
		}

		auto gameViewportClient = UObject::FindObject<UObject>("UTGameViewportClient Transient.GameEngine_1.UTGameViewportClient_1");
		if (!HookVFunction((uintptr_t)gameViewportClient, 59, Hook_ProcessEvent, &pProcessEvent))
		{
			MessageBoxA(nullptr, "Hook ProcessEvent failed", "Error", 0);
			return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}