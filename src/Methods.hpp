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

	inline std::vector<int> MethodIntegers;
	inline std::vector<float> MethodFloats;
	inline std::vector<int> MethodBooleans;
	inline std::vector<std::string> MethodStrings;

	inline int RetInt;
	inline float RetFloat;
	inline bool RetBool;

	inline std::vector<int> RetIntegers;
	inline std::vector<float> RetFloats;
	inline std::vector<int> RetBooleans;
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
	static inline bool m_InitContainers = false;

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

	static inline int GetMethodRetType(il2cppMethodInfo* method)
	{
		switch (method->m_pReturnType->m_uType)
		{
		case Type_Void:
			return RetType::Void;
		case Type_Integer:
			return RetType::Int;
		case Type_Float:
			return RetType::Float;
		case Type_Boolean:
			return RetType::Bool;
		default:
			return RetType::Void;
		}
	}

	static inline const char* GetMethodRetTypeAsText(il2cppMethodInfo* method)
	{
		switch (method->m_pReturnType->m_uType)
		{
		case Type_Void:
			return "(void)";
		case Type_Integer:
			return "(int)";
		case Type_Float:
			return "(float)";
		case Type_Boolean:
			return "(bool)";
		default:
			return "(unsupported)";
		}
	}

	static inline int GetMethodArgType(il2cppMethodInfo* method)
	{
		switch (method->m_pParameters->m_pParameterType->m_uType)
		{
		case Type_Integer:
			return ArgType::inT;
		case Type_Float:
			return ArgType::floaT;
		case Type_Boolean:
			return ArgType::booL;
		case Type_String:
			return ArgType::strinG;
		default:
			return ArgType::noArg;
		}
	}

	static inline void ClearContainers()
	{
		MethodArgs::MethodIntegers.clear();
		MethodArgs::MethodFloats.clear();
		MethodArgs::MethodBooleans.clear();
		MethodArgs::MethodStrings.clear();

		MethodArgs::RetIntegers.clear();
		MethodArgs::RetFloats.clear();
		MethodArgs::RetBooleans.clear();
	}
};
