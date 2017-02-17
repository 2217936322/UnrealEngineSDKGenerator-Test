#pragma once

template<typename T, typename U>
bool HookVFunction(uintptr_t instance, size_t tableIndex, T hookFunction, U **originalFunction)
{
	auto address = *reinterpret_cast<uintptr_t*>(instance) + (tableIndex * sizeof(uintptr_t));

	DWORD protection;
	if (VirtualProtect(reinterpret_cast<void*>(address), sizeof(uintptr_t), PAGE_EXECUTE_READWRITE, &protection))
	{
		*originalFunction = *reinterpret_cast<U**>(address);

		*reinterpret_cast<uintptr_t*>(address) = reinterpret_cast<uintptr_t>(hookFunction);

		VirtualProtect(reinterpret_cast<void*>(address), sizeof(uintptr_t), protection, &protection);

		return true;
	}

	return false;
}