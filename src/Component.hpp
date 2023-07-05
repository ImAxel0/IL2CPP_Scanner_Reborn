#pragma once
#include "il2cppresolver/Il2CppResolver.hpp"
#include "Search.hpp"
#include "GameObject.hpp"
#include "globals.hpp"

using namespace Unity;

class ComponentClass
{
	static inline CComponent* m_Component;
	static inline std::vector<CComponent*> m_Components;

public:
	static inline void ChildrenComponentSearchAll(std::vector<il2cppClass*> m_vClasses)
	{
		for (auto ChildrenComponent : m_vClasses)
		{
			std::string namespac = ChildrenComponent->m_pNamespace;
			std::string ok = namespac.append(".").append(ChildrenComponent->m_pName);

			CComponent* tmp = GameObjectClass::Get()->GetComponentInChildren(ok.c_str());

			if (std::find(m_Components.begin(), m_Components.end(), tmp) != m_Components.end()) // skip if not a child component
				continue;

			if (tmp)
				m_Components.push_back(tmp);
		}
	}

	static inline void ComponentSearchAll()
	{
		m_Components.clear();
		std::vector<il2cppClass*> m_vClasses;
		IL2CPP::Class::FetchClasses(&m_vClasses, SearchClass::m_Module.c_str(), nullptr);

		for (auto Component : m_vClasses)
		{
			if (!GameObjectClass::IsValidGameObject(GameObjectClass::Get()))
				return;

			CComponent* tmp = GameObjectClass::Get()->GetComponent(Component->m_pName);
			if (tmp)
				m_Components.push_back(tmp);
		}

		if (Globals::includeChildrenComp)
			ChildrenComponentSearchAll(m_vClasses);
	}

	static inline CComponent* Get()
	{
		return m_Component;
	}

	static inline std::vector<CComponent*> Gets()
	{
		return m_Components;
	}

	static inline void Set(CComponent* Component)
	{
		m_Component = Component;
	}

	static inline bool IsValidComponent(CComponent* Component)
	{
		if (Component && Component->m_CachedPtr)
			return true;

		return false;
	}
};