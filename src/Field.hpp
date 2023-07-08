#pragma once
#include "il2cppresolver/Il2CppResolver.hpp"
#include "Component.hpp"
#include "Property.hpp"

using namespace Unity;

class FieldClass
{
	static inline std::vector<il2cppFieldInfo*> m_Fields;

public:

	static inline void FieldSearchAll()
	{
		m_Fields.clear();
		ComponentClass::Get()->FetchFields(&m_Fields, nullptr);
		std::vector<il2cppFieldInfo*> copy = m_Fields;

		if (ComponentClass::Get()->m_Object.m_pClass->m_pTypeDefinition) // base class e.g class : base class
		{
			IL2CPP::Class::FetchFields((il2cppClass*)ComponentClass::Get()->m_Object.m_pClass->m_pTypeDefinition, &m_Fields, nullptr);
			for (auto field : copy)
			{
				m_Fields.push_back(field);
			}
		}

	}

	static inline std::vector<il2cppFieldInfo*> Gets()
	{
		return m_Fields;
	}

	static inline void PropertyFieldSearchAll()
	{
		m_Fields.clear();

		if (!PropertyClass::Get() || !PropertyClass::Get()->m_CachedPtr)
			return;

		PropertyClass::Get()->FetchFields(&m_Fields, nullptr);
		std::vector<il2cppFieldInfo*> copy = m_Fields;

		if (PropertyClass::Get()->m_Object.m_pClass->m_pTypeDefinition) // base class e.g class : base class
		{
			IL2CPP::Class::FetchFields((il2cppClass*)PropertyClass::Get()->m_Object.m_pClass->m_pTypeDefinition, &m_Fields, nullptr);
			for (auto field : copy)
			{
				m_Fields.push_back(field);
			}
		}
		ComponentClass::Set((CComponent*)PropertyClass::Get());
	}
};