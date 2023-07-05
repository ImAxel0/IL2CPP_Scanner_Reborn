#pragma once
#include "il2cppresolver/Il2CppResolver.hpp"
#include "GameObject.hpp"
#include <string>

using namespace Unity;

class ChildClass
{
	static inline std::vector<CGameObject*> m_Children;
	static inline std::vector<CGameObject*> m_Parent;
	static inline std::string m_ChildrenPath;

public:
	static inline void ChildrenSearchAll()
	{
		m_Children.clear();

		if (!GameObjectClass::IsValidGameObject(GameObjectClass::Get()))
			return;

		for (uintptr_t u{}; u < GameObjectClass::Get()->GetTransform()->GetChildCount(); ++u)
		{
			CGameObject* child = (CGameObject*)GameObjectClass::Get()->GetTransform()->GetChild(u);
			CGameObject* tmp = GameObject::Find(child->GetName()->ToString().c_str());		
			(tmp) ? m_Children.push_back(tmp) : m_Children.push_back(child);				
		}
	}

	static inline std::vector<CGameObject*> Gets()
	{
		return m_Children;
	}

	static inline void PushParent(CGameObject* ParentGameObject)
	{
		m_Parent.push_back(ParentGameObject);
	}

	static inline void PopParent()
	{
		m_Parent.pop_back();
	}

	static inline std::vector<CGameObject*> GetParents()
	{
		return m_Parent;
	}

	static inline void ClearParents()
	{
		m_Parent.clear();
	}

	static inline std::string& GetChildrenPath()
	{
		return m_ChildrenPath;
	}

	static inline void PushParentPath(std::string newpath)
	{
		newpath.pop_back(); // removes the end \0 which stop the string from appending
		m_ChildrenPath.append(newpath).append("\\");
	}

	static inline void PopParentPath()
	{
		int length = m_ChildrenPath.length();
		for (int i{}; i < length; ++i)
		{
			m_ChildrenPath.pop_back();
			if (m_ChildrenPath.back() == '\\')
				break;
		}
	}

	static inline void ClearParentPath()
	{
		m_ChildrenPath.clear();
	}
};