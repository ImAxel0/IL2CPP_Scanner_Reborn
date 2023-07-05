#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "globals.hpp"
#include "Search.hpp"
#include "GameObject.hpp"
#include "Component.hpp"
#include "Field.hpp"
#include "Logger.hpp"
#include "Property.hpp"
#include "Methods.hpp"
#include "Child.hpp"
#include "iconfont.hpp"
#include <boost/algorithm/string/predicate.hpp>
#include <iostream>
#include <algorithm>
#include <cctype>

using namespace Unity;

void TextCenteredXY(std::string text) {
	auto windowWidth = ImGui::GetContentRegionAvail().x;
	auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

	auto windowHeight = ImGui::GetContentRegionAvail().y;
	auto textHeight = ImGui::CalcTextSize(text.c_str()).y;

	ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
	ImGui::SetCursorPosY((windowHeight - textHeight) * 0.5f);
	ImGui::Text(text.c_str());
}

void TextCentered(std::string text) {
	auto windowWidth = ImGui::GetContentRegionAvail().x;
	auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

	ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
	ImGui::Text(text.c_str());
}

void InfoBox(std::string info)
{
	ImGui::BeginTooltip();
	ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
	ImGui::TextUnformatted(info.c_str());
	ImGui::PopTextWrapPos();
	ImGui::EndTooltip();
}

bool CompareStringCaseInsensitive(const std::string& lhs, const std::string& rhs) {

	std::string::size_type common_length = std::min<size_t>(lhs.length(), rhs.length());

	for (std::string::size_type i = 0; i < common_length; ++i) {
		if (toupper(lhs[i]) < toupper(rhs[i]))return true;
		if (toupper(lhs[i]) > toupper(rhs[i]))return false;
	}

	if (lhs.length() < rhs.length())return true;
	if (lhs.length() > rhs.length())return false;

	return false;
}

void GUI()
{
	Globals::Gui::style = &ImGui::GetStyle();
	Globals::Gui::style->WindowTitleAlign = ImVec2(0.5, 0.5);
	Globals::Gui::style->WindowBorderSize = 3.0f;
	Globals::Gui::style->WindowRounding = 8.0f;
	Globals::Gui::style->FrameRounding = 8.0f;
	Globals::Gui::style->ChildRounding = 8.0f;
	Globals::Gui::style->Colors[ImGuiCol_Border] = *Globals::Gui::theme;
	Globals::Gui::style->Colors[ImGuiCol_TitleBg] = *Globals::Gui::theme;
	Globals::Gui::style->Colors[ImGuiCol_TitleBgActive] = *Globals::Gui::theme;
	Globals::Gui::style->Colors[ImGuiCol_FrameBg] = ImColor(40, 40, 40);
	Globals::Gui::style->Colors[ImGuiCol_FrameBgActive] = ImColor(40, 40, 40);
	Globals::Gui::style->Colors[ImGuiCol_FrameBgHovered] = ImColor(40, 40, 40);
	Globals::Gui::style->Colors[ImGuiCol_Button] = ImColor(40, 40, 40);
	Globals::Gui::style->Colors[ImGuiCol_ButtonHovered] = ImColor(50, 50, 50);
	Globals::Gui::style->Colors[ImGuiCol_ChildBg] = ImColor(24, 24, 24);
	Globals::Gui::style->Colors[ImGuiCol_Separator] = ImColor(0, 0, 0, 0);
	Globals::Gui::style->Colors[ImGuiCol_SeparatorActive] = ImColor(0, 0, 0, 0);
	Globals::Gui::style->Colors[ImGuiCol_SeparatorHovered] = ImColor(0, 0, 0, 0);
	ImGui::SetNextWindowSize(ImVec2(1280, 720));
	ImGui::Begin("IL2CPP Scanner Reborn", &Globals::showMenu, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	ImGui::BeginChild("main", ImGui::GetContentRegionAvail(), false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	ImGui::Columns(2, nullptr, true);
	ImGui::SetColumnOffset(1, 400);

	if (ImGui::BeginCombo("Module", SearchClass::m_Module.c_str()))
	{
		size_t m_sAssembliesCount = 0U;
		Unity::il2cppAssembly** m_pAssemblies = IL2CPP::Domain::GetAssemblies(&m_sAssembliesCount);
		if (!m_pAssemblies || 0U >= m_sAssembliesCount) return;

		std::vector<std::string> names;
		for (size_t i = 0U; m_sAssembliesCount > i; ++i)
		{
			Unity::il2cppAssembly* m_pAssembly = m_pAssemblies[i];
			names.push_back(m_pAssembly->m_pImage->m_pNameNoExt);
		}
		std::sort(names.begin(), names.end(), CompareStringCaseInsensitive);
		for (auto modules : names)
		{
			if (ImGui::Selectable(modules.c_str()))
				SearchClass::m_Module = modules;
		}
		names.clear();
		ImGui::EndCombo();
	}

	ImGui::InputText("Namespace", &SearchClass::m_Namespace);
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
		InfoBox("leave blank to search in all namespaces");
	ImGui::InputText("Game Object", &SearchClass::m_GameObject);
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
		InfoBox("leave blank to search all GameObjects");
		
	if (ImGui::Button("Search " ICON_FA_MAGNIFYING_GLASS))
		(SearchClass::m_GameObject != "") ? GameObjectClass::GameObjectSearch() : GameObjectClass::GameObjectSearchAll();

	ImGui::SameLine();
	if (ImGui::Button("Clear"))
		SearchClass::ClearAll();

	TextCentered("Game Objects list");

	Globals::Gui::style->Colors[ImGuiCol_ChildBg] = ImColor(30, 30, 30);
	ImGui::BeginChild("gameobject list", ImGui::GetContentRegionAvail(), false);

	if (GameObjectClass::Gets().empty())
		TextCenteredXY("Nothing to show at the moment");

	ImGui::Separator();

	// GameObjects
	for (auto GameObject : GameObjectClass::Gets())
	{
		if (!GameObjectClass::IsValidGameObject(GameObject))
			break;

		if (ImGui::Button(GameObject->GetName()->ToString().c_str(), ImVec2(ImGui::GetContentRegionAvail().x, NULL)))
		{
			GameObjectClass::Set(GameObject);
			ComponentClass::ComponentSearchAll();
			ChildClass::ChildrenSearchAll();
			ChildClass::ClearParents();
			ChildClass::ClearParentPath();
			if (GameObjectClass::IsValidGameObject(GameObjectClass::Get()))
				ChildClass::PushParentPath(GameObject->GetName()->ToString());
			SearchClass::m_ComponentList.clear();
			Globals::Gui::window = "component";
		}
	}

	ImGui::EndChild();

	ImGui::NextColumn();

	ImGui::BeginChild("components/fields/log", ImGui::GetContentRegionAvail(), false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar);

	ImGui::BeginMenuBar();
	if (Globals::Gui::window == "component")
	{
		if (GameObjectClass::IsValidGameObject(GameObjectClass::Get()))
		{	
			Globals::Gui::style->Colors[ImGuiCol_Button] = ImColor(60, 60, 60);
			Globals::Gui::style->Colors[ImGuiCol_ButtonHovered] = ImColor(80, 80, 80);
			if (ImGui::SmallButton("Transform/Inspect"))
			{
				Globals::Gui::window = "transform";
			}
			Globals::Gui::style->Colors[ImGuiCol_Button] = ImColor(40, 40, 40);
			Globals::Gui::style->Colors[ImGuiCol_ButtonHovered] = ImColor(50, 50, 50);
			
			Globals::Gui::style->Colors[ImGuiCol_Text] = ImColor(0, 255, 0);
			std::string name = GameObjectClass::Get()->GetName()->ToString().c_str();
			name.append(" gameobject");
			TextCentered(name);	
			Globals::Gui::style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255);
		}

		ImGui::SetCursorPosX(570);
		Globals::Gui::style->Colors[ImGuiCol_FrameBg] = ImColor(80, 80, 80);
		Globals::Gui::style->Colors[ImGuiCol_FrameBgActive] = ImColor(80, 80, 80);
		Globals::Gui::style->Colors[ImGuiCol_FrameBgHovered] = ImColor(80, 80, 80);
		ImGui::PushItemWidth(230);
		ImGui::InputText("Filter", &SearchClass::m_ComponentList);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			InfoBox("name must be complete, case insensitive");
		ImGui::PopItemWidth();
		Globals::Gui::style->Colors[ImGuiCol_FrameBg] = ImColor(40, 40, 40);
		Globals::Gui::style->Colors[ImGuiCol_FrameBgActive] = ImColor(40, 40, 40);
		Globals::Gui::style->Colors[ImGuiCol_FrameBgHovered] = ImColor(40, 40, 40);
	}
	if (Globals::Gui::window == "field")
	{
		Globals::Gui::style->Colors[ImGuiCol_Button] = ImColor(60, 60, 60);
		Globals::Gui::style->Colors[ImGuiCol_ButtonHovered] = ImColor(80, 80, 80);
		if (ImGui::SmallButton("Methods Call"))
		{
			Globals::Gui::window = "method";
		}
		Globals::Gui::style->Colors[ImGuiCol_Button] = ImColor(40, 40, 40);
		Globals::Gui::style->Colors[ImGuiCol_ButtonHovered] = ImColor(50, 50, 50);
		
		if (ComponentClass::IsValidComponent(ComponentClass::Get()))
		{
			Globals::Gui::style->Colors[ImGuiCol_Text] = ImColor(0, 255, 0);
			std::string name = ComponentClass::Get()->m_Object.m_pClass->m_pName;
			name.append(" fields and properties");
			TextCentered(name);
			Globals::Gui::style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255);
		}

		ImGui::SetCursorPosX(570);
		Globals::Gui::style->Colors[ImGuiCol_FrameBg] = ImColor(80, 80, 80);
		Globals::Gui::style->Colors[ImGuiCol_FrameBgActive] = ImColor(80, 80, 80);
		Globals::Gui::style->Colors[ImGuiCol_FrameBgHovered] = ImColor(80, 80, 80);
		ImGui::PushItemWidth(230);
		ImGui::InputText("Filter", &SearchClass::m_FieldList);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			InfoBox("name must be complete, case insensitive");
		ImGui::PopItemWidth();
		Globals::Gui::style->Colors[ImGuiCol_FrameBg] = ImColor(40, 40, 40);
		Globals::Gui::style->Colors[ImGuiCol_FrameBgActive] = ImColor(40, 40, 40);
		Globals::Gui::style->Colors[ImGuiCol_FrameBgHovered] = ImColor(40, 40, 40);
	}
	if (Globals::Gui::window == "method")
	{
		if (ComponentClass::IsValidComponent(ComponentClass::Get()))
		{
			Globals::Gui::style->Colors[ImGuiCol_Text] = ImColor(0, 255, 0);
			std::string name = ComponentClass::Get()->m_Object.m_pClass->m_pName;
			name.append(" methods calling page");
			TextCentered(name);
			Globals::Gui::style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255);
		}
	}
	if (Globals::Gui::window == "transform")
	{
		if (GameObjectClass::IsValidGameObject(GameObjectClass::Get()))
		{
			Globals::Gui::style->Colors[ImGuiCol_Text] = ImColor(0, 255, 0);
			std::string name = GameObjectClass::Get()->GetName()->ToString().c_str();
			name.append(" transform and inspector");
			TextCentered(name);
			Globals::Gui::style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255);
		}
	}
	ImGui::EndMenuBar();

	if (Globals::Gui::window == "component")
	{
		ImGui::Columns(2, nullptr, true);
		ImGui::SetColumnOffset(1, 570);
	
		ImGui::BeginChild("components1", ImGui::GetContentRegionAvail(), true, ImGuiWindowFlags_MenuBar);
		auto scrollpos = ImGui::GetScrollY();

		ImGui::BeginMenuBar();
		TextCentered("Components");
		ImGui::EndMenuBar();

		if (ImGui::Checkbox("Include GameObject children components", &Globals::includeChildrenComp))
			ComponentClass::ComponentSearchAll();

		// Components
		for (auto Component : ComponentClass::Gets())
		{
			if (!ComponentClass::IsValidComponent(Component))
				break;

			if (boost::iequals(SearchClass::m_ComponentList, Component->m_Object.m_pClass->m_pName) || SearchClass::m_ComponentList.empty())
				if (ImGui::Button(Component->m_Object.m_pClass->m_pName, ImVec2(ImGui::GetContentRegionAvail().x, NULL)))
				{
					ComponentClass::Set(Component);
					FieldClass::FieldSearchAll();
					PropertyClass::PropertySearchAll();
					MethodClass::MethodSearchAll();
					SearchClass::m_FieldList.clear();
					Globals::Gui::window = "field";
				}		
		}
		ImGui::EndChild();
		ImGui::NextColumn();
		ImGui::SetNextWindowScroll(ImVec2(NULL, scrollpos));
		ImGui::BeginChild("namespaces", ImGui::GetContentRegionAvail(), true, ImGuiWindowFlags_MenuBar);

		ImGui::BeginMenuBar();
		TextCentered("Namespace");
		ImGui::EndMenuBar();

		ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
		ImGui::InputText("##dummyspace", new std::string(""), ImGuiInputTextFlags_ReadOnly);

		for (auto Component : ComponentClass::Gets())
		{
			if (!ComponentClass::IsValidComponent(Component))
				break;

			ImGui::PushID(Component);
			if (boost::iequals(SearchClass::m_ComponentList, Component->m_Object.m_pClass->m_pName) || SearchClass::m_ComponentList.empty())
			{
				ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
				ImGui::InputText("##", new std::string(Component->m_Object.m_pClass->m_pNamespace), ImGuiInputTextFlags_ReadOnly);
			}
			ImGui::PopID();
		}
		ImGui::EndChild();
	}

	if (Globals::Gui::window == "field")
	{
		ImGui::BeginChild("type filter", ImVec2(NULL, 38), ImGuiWindowFlags_NoMove);
		ImGui::Checkbox("int", &Globals::showInt);
		ImGui::SameLine(),
		ImGui::Checkbox("float", &Globals::showFloat);
		ImGui::SameLine(),
		ImGui::Checkbox("bool", &Globals::showBool);
		ImGui::SameLine(),
		ImGui::Checkbox("class", &Globals::showClass);
		ImGui::SameLine(),
		ImGui::Checkbox("properties", &Globals::showProperties);
		ImGui::EndChild();

		// Fields
		for (auto Field : FieldClass::Gets())
		{
			if (!ComponentClass::IsValidComponent(ComponentClass::Get()))
				break;

			if (boost::iequals(SearchClass::m_FieldList, Field->m_pName) || SearchClass::m_FieldList.empty())
				switch (Field->m_pType->m_uType)
				{
					case Type_Integer:
					{
						int i = ComponentClass::Get()->GetMemberValue<int>(Field->m_pName);

						if (Globals::showInt)
						{
							Globals::Gui::style->Colors[ImGuiCol_Text] = ImColor(23, 156, 255);
							ImGui::Text("INT"); ImGui::SameLine();
							Globals::Gui::style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255);

							ImGui::PushItemWidth(250);
							if (ImGui::InputInt(Field->m_pName, &i, NULL, NULL, ImGuiInputTextFlags_EnterReturnsTrue))
							{
								ComponentClass::Get()->SetMemberValue<int>(Field->m_pName, i);
							}
						}
						break;
					}

					case Type_Float:
					{
						float f = ComponentClass::Get()->GetMemberValue<float>(Field->m_pName);

						if (Globals::showFloat)
						{
							Globals::Gui::style->Colors[ImGuiCol_Text] = ImColor(0, 255, 0);
							ImGui::Text("FLOAT"); ImGui::SameLine();
							Globals::Gui::style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255);

							ImGui::PushItemWidth(250);
							if (ImGui::InputFloat(Field->m_pName, &f, NULL, NULL, "%.1f", ImGuiInputTextFlags_EnterReturnsTrue))
							{
								ComponentClass::Get()->SetMemberValue<float>(Field->m_pName, f);
							}
						}
						break;
					}

					case Type_Boolean:
					{
						bool b = ComponentClass::Get()->GetMemberValue<bool>(Field->m_pName);

						if (Globals::showBool)
						{
							Globals::Gui::style->Colors[ImGuiCol_Text] = ImColor(255, 0, 0);
							ImGui::Text("BOOL"); ImGui::SameLine();
							Globals::Gui::style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255);

							if (ImGui::Checkbox(Field->m_pName, &b))
							{
								(ComponentClass::Get()->GetMemberValue<bool>(Field->m_pName)) ? ComponentClass::Get()->SetMemberValue<bool>(Field->m_pName, false) : ComponentClass::Get()->SetMemberValue<bool>(Field->m_pName, true);
							}
						}
						break;
					}

					case Type_Class:
					{
						CComponent* component = ComponentClass::Get()->GetMemberValue<CComponent*>(Field->m_pName);

						if (Globals::showClass)
						{
							Globals::Gui::style->Colors[ImGuiCol_Text] = ImColor(167, 122, 221);
							ImGui::Text("CLASS"); ImGui::SameLine();
							Globals::Gui::style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255);

							if (ImGui::Button(Field->m_pName, ImVec2(ImGui::GetContentRegionAvail().x, NULL)))
							{
								if (ComponentClass::IsValidComponent(component))
								{
									ComponentClass::Set(component);
									FieldClass::FieldSearchAll();
									PropertyClass::PropertySearchAll();
									MethodClass::MethodSearchAll();
									SearchClass::m_FieldList.clear();
									Globals::Gui::window = "field";
								}
							}
						}
						break;
					}
				}
		}

		for (auto Property : PropertyClass::Gets())
		{
			if (!ComponentClass::IsValidComponent(ComponentClass::Get()))
				break;

			if (boost::iequals(SearchClass::m_FieldList, Property->m_pName) || SearchClass::m_FieldList.empty())
			{
				if (Globals::showProperties)
				{
					ImGui::Text(ICON_FA_WRENCH); ImGui::SameLine();
					if (ImGui::Button(Property->m_pName, ImVec2(ImGui::GetContentRegionAvail().x, NULL)))
					{
						// properties are found but can't do anything with them?
					}
				}
			}
		}
	}

	if (Globals::Gui::window == "method")
	{
		// Methods
		if (ImGui::BeginCombo("Methods", MethodArgs::MethodName.c_str()))
		{
			for (auto Method : MethodClass::Gets())
			{
				if (ImGui::Selectable(Method->m_pName))
					MethodArgs::MethodName = Method->m_pName;
			}
			ImGui::EndCombo();
		}

		ImGui::InputText("Method name", &MethodArgs::MethodName);
		ImGui::Combo("Return type", &Globals::MethodRetType_Current, Globals::MethodRetType, 4);
		ImGui::Combo("Argument type", &Globals::MethodArgType_Current, Globals::MethodArgType, 5);

		switch (Globals::MethodArgType_Current)
		{
			case ArgType::inT:
				ImGui::InputInt("Int value", &MethodArgs::MethodInt, NULL, NULL);
				break;
			case ArgType::floaT:
				ImGui::InputFloat("Float value", &MethodArgs::MethodFloat, NULL, NULL, "%.1f");
				break;
			case ArgType::booL:
				ImGui::Checkbox("True or False?", &MethodArgs::MethodBool);
				break;
			case ArgType::strinG:
				ImGui::InputText("String", &MethodArgs::MethodString);
				break;
		}

		if (ImGui::Button("Call Method"))
		{
			Globals::callMethod = true;
		}

		TextCentered("Log");
		ImGui::InputTextMultiline("##", &LoggerClass::GetContent(), ImGui::GetContentRegionAvail(), ImGuiInputTextFlags_ReadOnly);
	}

	if (Globals::Gui::window == "transform")
	{
		if (GameObjectClass::IsValidGameObject(GameObjectClass::Get()))
		{
			Globals::Gui::style->Colors[ImGuiCol_Text] = ImColor(0, 255, 0);
			ImGui::Text("Game Object:"); ImGui::SameLine(), Globals::Gui::style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255);
			ImGui::InputText("##", new std::string (GameObjectClass::Get()->GetName()->ToString()), ImGuiInputTextFlags_ReadOnly);
			ImGui::SameLine(),
			Globals::Gui::style->Colors[ImGuiCol_Button] = ImColor(183, 64, 64);
			Globals::Gui::style->Colors[ImGuiCol_ButtonHovered] = ImColor(193, 74, 74);
			if (ImGui::Button("Destroy"))
			{
				GameObjectClass::Get()->Destroy();
			}
			Globals::Gui::style->Colors[ImGuiCol_Button] = ImColor(40, 40, 40);
			Globals::Gui::style->Colors[ImGuiCol_ButtonHovered] = ImColor(50, 50, 50);
			ImGui::SameLine();
			if (ImGui::Checkbox("Is Active", new bool(GameObjectClass::Get()->GetActive())))
			{
				((GameObjectClass::Get()->GetActive())) ? GameObjectClass::Get()->SetActive(false) : GameObjectClass::Get()->SetActive(true);
			}

			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 38);
			ImGui::InputText("path", &ChildClass::GetChildrenPath(), ImGuiInputTextFlags_ReadOnly);
			ImGui::PopItemWidth();

			Globals::Gui::style->Colors[ImGuiCol_Separator] = ImColor(60, 60, 60, 255);
			ImGui::Separator();
			Globals::Gui::style->Colors[ImGuiCol_Separator] = ImColor(0, 0, 0, 0);
			
			Unity::Vector3 position = GameObjectClass::Get()->GetTransform()->GetPosition();
			if (ImGui::InputFloat3("Position", (float*)&position, "%.2f", ImGuiInputTextFlags_EnterReturnsTrue))
			{
				GameObjectClass::Get()->GetTransform()->SetPosition(position);
			}

			Unity::Vector3 localposition = GameObjectClass::Get()->GetTransform()->GetLocalPosition();
			if (ImGui::InputFloat3("Local position", (float*)&localposition, "%.2f", ImGuiInputTextFlags_EnterReturnsTrue))
			{
				GameObjectClass::Get()->GetTransform()->SetLocalPosition(localposition);
			}

			Unity::Vector3 scale = GameObjectClass::Get()->GetTransform()->GetLocalScale();
			if (ImGui::InputFloat3("Local scale", (float*)&scale, "%.2f", ImGuiInputTextFlags_EnterReturnsTrue))
			{
				GameObjectClass::Get()->GetTransform()->SetLocalScale(scale);
			}

			Unity::Quaternion rotation = GameObjectClass::Get()->GetTransform()->GetRotation();
			if (ImGui::InputFloat4("Rotation", (float*)&rotation, "%.2f", ImGuiInputTextFlags_EnterReturnsTrue))
			{
				GameObjectClass::Get()->GetTransform()->SetRotation(rotation);
			}

			ImGui::Separator();

			ImGui::Columns(2, nullptr, true);
			ImGui::SetColumnOffset(1, 420);

			ImGui::BeginChild("children", ImGui::GetContentRegionAvail(), true, ImGuiWindowFlags_MenuBar);

			ImGui::BeginMenuBar();
			if (ChildClass::GetParents().size() > 0)
			{
				if (ImGui::SmallButton(ICON_FA_ARROW_LEFT))
				{
					GameObjectClass::Set(ChildClass::GetParents().back());
					ComponentClass::ComponentSearchAll();
					ChildClass::ChildrenSearchAll();
					SearchClass::m_ComponentList.clear();
					ChildClass::PopParentPath();
					ChildClass::PopParent();
				}
			}
			TextCentered("Children Gameobjects");
			ImGui::EndMenuBar();

			for (auto child : ChildClass::Gets())
			{
				ImGui::PushID(child);
				bool onoff = child->GetActive();
				if (strcmp(child->m_Object.m_pClass->m_pName, "GameObject") != 0) { onoff = false; } // make sure it's a gameobject
				ImGui::Checkbox("##", &onoff);
				ImGui::SameLine();
				ImGui::Text(child->GetName()->ToString().c_str());
				if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
				{
					if (onoff)
					{
						ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
						if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
						{
							ChildClass::PushParentPath(child->GetName()->ToString());
							ChildClass::PushParent(GameObjectClass::Get());
							GameObjectClass::Set(child);
							ComponentClass::ComponentSearchAll();
							ChildClass::ChildrenSearchAll();
							SearchClass::m_ComponentList.clear();
						}
					}
				}
				ImGui::PopID();
			}
		}
		ImGui::EndChild();

		ImGui::NextColumn();

		ImGui::BeginChild("components2", ImGui::GetContentRegionAvail(), true, ImGuiWindowFlags_MenuBar);

		ImGui::BeginMenuBar();
		TextCentered("Components");
		ImGui::EndMenuBar();

		if (ImGui::Checkbox("Include children components", &Globals::includeChildrenComp))
			ComponentClass::ComponentSearchAll();
		ImGui::Separator();

		for (auto Component : ComponentClass::Gets())
		{
			if (!ComponentClass::IsValidComponent(Component))
				break;

			if (ImGui::Button(Component->m_Object.m_pClass->m_pName, ImVec2(ImGui::GetContentRegionAvail().x, NULL)))
			{
				ComponentClass::Set(Component);
				FieldClass::FieldSearchAll();
				PropertyClass::PropertySearchAll();
				MethodClass::MethodSearchAll();
				SearchClass::m_FieldList.clear();
				Globals::Gui::window = "field";
			}
			
		}
		ImGui::EndChild();
	}

	if (Globals::Gui::window == "log")
	{
		ImGui::Text("Log window");
		ImGui::InputTextMultiline("##", &LoggerClass::GetContent(), ImGui::GetContentRegionAvail(), ImGuiInputTextFlags_ReadOnly);
	}
	ImGui::EndChild();

	ImGui::EndChild();
	ImGui::End();
}