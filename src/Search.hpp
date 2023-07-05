#pragma once
#include <string>

class SearchClass
{
public:
	static inline std::string m_Module = "Assembly-CSharp";
	static inline std::string m_GameObject;
	static inline std::string m_Namespace;

	static inline std::string m_ComponentList;
	static inline std::string m_FieldList;

	static inline void ClearAll()
	{
		m_Module.clear();
		m_GameObject.clear();
		m_Namespace.clear();
	}
};
