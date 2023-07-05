#pragma once
#include "il2cppresolver/Il2CppResolver.hpp"
#include "Logger.hpp"
#include "Search.hpp"

using namespace Unity;

class GameObjectClass
{
	static inline CGameObject* m_GameObject;
	static inline std::vector<CGameObject*> m_GameObjects;
	static inline std::string OGname;

	static inline void GameObjectSearchInNamespace()
	{
		std::vector<il2cppClass*> m_vClasses;
		
		(SearchClass::m_Module.empty()) ? IL2CPP::Class::FetchClasses(&m_vClasses, "Assembly-CSharp", SearchClass::m_Namespace.c_str()) : IL2CPP::Class::FetchClasses(&m_vClasses, SearchClass::m_Module.c_str(), SearchClass::m_Namespace.c_str());

		for (auto GameObject : m_vClasses)
		{
			CGameObject* tmp = GameObject::Find(GameObject->m_pName);
			if (IsValidGameObject(tmp))
				if (tmp->GetName()->ToString() == IL2CPP::String::New(SearchClass::m_GameObject)->ToString())
				{
					m_GameObjects.push_back(tmp);
					break;
				}
		}
	}

public:
	static inline void GameObjectSearch()
	{
		m_GameObjects.clear();
		(SearchClass::m_Namespace != "") ? GameObjectSearchInNamespace() : m_GameObjects.push_back(GameObject::Find(SearchClass::m_GameObject.c_str()));
	}

	static inline void GameObjectSearchAll()
	{
		m_GameObjects.clear();
		std::vector<il2cppClass*> m_vClasses;

		if (SearchClass::m_Module.empty())
		{
			(SearchClass::m_Namespace != "") ? IL2CPP::Class::FetchClasses(&m_vClasses, "Assembly-CSharp", SearchClass::m_Namespace.c_str()) : IL2CPP::Class::FetchClasses(&m_vClasses, "Assembly-CSharp", nullptr);
		}
		else
		{
			(SearchClass::m_Namespace != "") ? IL2CPP::Class::FetchClasses(&m_vClasses, SearchClass::m_Module.c_str(), SearchClass::m_Namespace.c_str()) : IL2CPP::Class::FetchClasses(&m_vClasses, SearchClass::m_Module.c_str(), nullptr);
		}

		for (auto GameObject : m_vClasses)
		{
			CGameObject* tmp = GameObject::Find(GameObject->m_pName);
			if (IsValidGameObject(tmp))
				m_GameObjects.push_back(tmp);
		}
	}

	static inline CGameObject* Get()
	{
		return m_GameObject;
	}

	static inline std::vector<CGameObject*> Gets()
	{
		return m_GameObjects;
	}

	static inline void Set(CGameObject* GameObject)
	{
		OGname = GameObject->GetName()->ToString();
		m_GameObject = GameObject;
	}

	static inline bool IsValidGameObject(CGameObject* GameObject)
	{
		if (GameObject && GameObject->m_CachedPtr && GameObject->m_Object.m_pClass && strcmp(GameObject->m_Object.m_pClass->m_pName, "GameObject") == 0)
			return true;

		return false;
	}

	static inline std::string GetOGname()
	{
		return OGname;
	}

	static inline void SetNull()
	{
		m_GameObject = nullptr;
	}

	static inline void SetNullAll()
	{
		m_GameObjects.clear();
	}
};
