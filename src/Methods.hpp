#pragma once
#include "il2cppresolver/Il2CppResolver.hpp"
#include "Component.hpp"

using namespace Unity;

void OnUpdate();

namespace MethodArgs
{
	inline std::string MethodName;
	inline int MethodInt;
	inline float MethodFloat;
	inline bool MethodBool;
	inline std::string MethodString;
}

enum RetType
{
	Void = 0,
	Int = 1,
	Float = 2,
	Bool = 3
};

enum ArgType
{
	inT = 0,
	floaT = 1,
	booL = 2,
	strinG = 3,
	noArg = 4
};

class MethodClass
{
	static inline std::vector<il2cppMethodInfo*> m_Methods;

public:
	static inline void MethodSearchAll()
	{
		m_Methods.clear();
		ComponentClass::Get()->FetchMethods(&m_Methods, nullptr);
	}

	static inline std::vector<il2cppMethodInfo*> Gets()
	{
		return m_Methods;
	}
};
