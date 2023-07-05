#pragma once
#include "il2cppresolver/Il2CppResolver.hpp"
#include "Component.hpp"

using namespace Unity;

class FieldClass
{
	static inline std::vector<il2cppFieldInfo*> m_Fields;

public:

	static inline void FieldSearchAll()
	{
		m_Fields.clear();
		ComponentClass::Get()->FetchFields(&m_Fields, nullptr);
	}

	static inline std::vector<il2cppFieldInfo*> Gets()
	{
		return m_Fields;
	}
};