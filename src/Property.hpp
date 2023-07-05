#pragma once
#include "il2cppresolver/Il2CppResolver.hpp"
#include "Component.hpp"

using namespace Unity;

class PropertyClass
{
	static inline std::vector<il2cppPropertyInfo*> m_Properties;
	static inline CObject* m_Property;

public:
	static inline void PropertySearchAll()
	{
		m_Properties.clear();
		IL2CPP::Class::FetchProperties(ComponentClass::Get()->m_Object.m_pClass, &m_Properties, nullptr);
	}

	static inline std::vector<il2cppPropertyInfo*> Gets()
	{
		return m_Properties;
	}

	static inline CObject* Get()
	{
		return m_Property;
	}

	static inline void Set(il2cppPropertyInfo* property)
	{
		m_Property = ComponentClass::Get()->GetPropertyValue<CObject*>(property->m_pName);
	}
};