#include "gui utils.hpp"
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
#include <future>

using namespace Unity;

void GUI()
{
	MainTheme();

	ImGui::SetNextWindowSize(ImVec2(1280, 720));
	ImGui::SetNextWindowBgAlpha(1);
	ImGui::Begin("IL2CPP Scanner Reborn", &Globals::showMenu, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	ImGui::BeginChild("main", ImGui::GetContentRegionAvail(), false, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	Globals::Gui::style->Colors[ImGuiCol_Border] = Theme::borderOthers_Col;

	Globals::Gui::style->WindowBorderSize = 0;
	ImGui::BeginMainMenuBar();
	if (ImGui::Checkbox("Gui Tools", &Theme::gui_Tools));
	ImGui::EndMainMenuBar();
	Globals::Gui::style->WindowBorderSize = Theme::border_Size;

	if (Theme::gui_Tools)
		GuiTools();

	ImGui::Columns(2, nullptr, true);
	ImGui::SetColumnOffset(1, 400);

	ImGui::Dummy(ImVec2(0, 4));
	if (ImGui::BeginCombo("Module", SearchClass::m_Module.c_str(), ImGuiComboFlags_HeightLarge))
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

	ImGui::Dummy(ImVec2(0, 4));

	// Search buttons

	PushButtonCol(ImColor(30, 80, 159), Theme::buttonSearchHovered_Col);
	if (ImGui::Button("Search " ICON_FA_MAGNIFYING_GLASS))
		(SearchClass::m_GameObject != "") ? GameObjectClass::GameObjectSearch() : GameObjectClass::GameObjectSearchAll();
	PopButtonCol();

	ImGui::SameLine();

	PushButtonCol(ImColor(183, 64, 64), Theme::buttonSearchHovered_Col);
	if (ImGui::Button("Clear"))
		SearchClass::ClearAll();
	PopButtonCol();

	ImGui::Dummy(ImVec2(0, 4));

	PushChildBgCol(Theme::childBgGameObjects_Col);
	ImGui::BeginChild("gameobject list", ImGui::GetContentRegionAvail(), true, ImGuiWindowFlags_MenuBar);

	ImGui::BeginMenuBar();
	TextCentered("Game Objects list");
	ImGui::EndMenuBar();

	if (GameObjectClass::Gets().empty())
		TextCenteredXY("Nothing to show at the moment");

	ImGui::Separator();

	// GameObjects
	for (auto GameObject : GameObjectClass::Gets())
	{
		if (!GameObjectClass::IsValidGameObject(GameObject))
			continue;

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
	PopChildBgCol();

	ImGui::NextColumn();

	Globals::Gui::style->Colors[ImGuiCol_FrameBg] = ImColor(34, 34, 34);
	Globals::Gui::style->Colors[ImGuiCol_FrameBgActive] = ImColor(34, 34, 34);
	Globals::Gui::style->Colors[ImGuiCol_FrameBgHovered] = ImColor(34, 34, 34);
	PushChildBgCol(Theme::childBgOutside_Col);
	ImGui::BeginChild("components/fields/log", ImGui::GetContentRegionAvail(), true, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar);

	ImGui::BeginMenuBar();
	if (Globals::Gui::window == "component")
	{
		if (GameObjectClass::IsValidGameObject(GameObjectClass::Get()))
		{	
			PushButtonCol(Theme::buttonMenuBar_Col, Theme::buttonHoveredMenuBar_Col);
			if (ImGui::SmallButton("Transform/Inspect"))
			{
				Globals::Gui::window = "transform";
			}
			PopButtonCol();
			
			Globals::Gui::style->Colors[ImGuiCol_Text] = ImColor(0, 255, 0, 200);
			std::string name = GameObjectClass::Get()->GetName()->ToString().c_str();
			name.append(" gameobject");
			TextCentered(name);	
			Globals::Gui::style->Colors[ImGuiCol_Text] = Theme::text_Col;
		}
	}
	if (Globals::Gui::window == "field")
	{
		PushButtonCol(Theme::buttonMenuBar_Col, Theme::buttonHoveredMenuBar_Col);
		if (ImGui::SmallButton("Methods Call"))
		{
			Globals::Gui::window = "method";
		}
		PopButtonCol();
		
		if (ComponentClass::IsValidComponent(ComponentClass::Get()))
		{
			Globals::Gui::style->Colors[ImGuiCol_Text] = ImColor(0, 255, 0, 200);
			std::string name = ComponentClass::Get()->m_Object.m_pClass->m_pName;
			name.append(" fields and properties");

			if (ComponentClass::Get()->m_Object.m_pClass->m_pTypeDefinition)
			{
				il2cppClass* baseClass = (il2cppClass*)ComponentClass::Get()->m_Object.m_pClass->m_pTypeDefinition;
				name.append(" : ").append(baseClass->m_pName);
			}
			TextCentered(name);
			Globals::Gui::style->Colors[ImGuiCol_Text] = Theme::text_Col;
		}
	}
	if (Globals::Gui::window == "method")
	{
		PushFrameBgCol(ImColor(53, 53, 53));
		if (ImGui::RadioButton("Auto mode (unreliable)", Globals::MethodAutoMode))
			Globals::MethodAutoMode = !Globals::MethodAutoMode;
		PopFrameBgCol();

		if (ComponentClass::IsValidComponent(ComponentClass::Get()))
		{
			Globals::Gui::style->Colors[ImGuiCol_Text] = ImColor(0, 255, 0, 200);
			std::string name = ComponentClass::Get()->m_Object.m_pClass->m_pName;
			name.append(" methods calling page");
			TextCentered(name);
			Globals::Gui::style->Colors[ImGuiCol_Text] = Theme::text_Col;
		}
	}
	if (Globals::Gui::window == "transform")
	{
		if (GameObjectClass::IsValidGameObject(GameObjectClass::Get()))
		{
			Globals::Gui::style->Colors[ImGuiCol_Text] = ImColor(0, 255, 0, 200);
			std::string name = GameObjectClass::Get()->GetName()->ToString().c_str();
			name.append(" transform and inspector");
			TextCentered(name);
			Globals::Gui::style->Colors[ImGuiCol_Text] = Theme::text_Col;
		}
	}
	ImGui::EndMenuBar();

	if (Globals::Gui::window == "component")
	{
		PushChildBgCol(Theme::childBgComponents_Col);

		ImGui::PushItemWidth(230);
		ImGui::InputText("Filter", &SearchClass::m_ComponentList);
		auto v = ImGui::GetItemRectSize().y;
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			InfoBox("name must be complete, case insensitive");
		ImGui::PopItemWidth();

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
				continue;

			if (boost::iequals(SearchClass::m_ComponentList, Component->m_Object.m_pClass->m_pName) || SearchClass::m_ComponentList.empty())
				if (ImGui::Button(Component->m_Object.m_pClass->m_pName, ImVec2(ImGui::GetContentRegionAvail().x, NULL)))
				{
					ComponentClass::Set(Component);
					FieldClass::FieldSearchAll();
					PropertyClass::PropertySearchAll();
					MethodClass::MethodSearchAll();
					MethodClass::ClearContainers(); MethodClass::m_InitContainers = false;
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

		ImGui::Dummy(ImVec2(0, v)); // first namespace space

		for (auto Component : ComponentClass::Gets())
		{
			if (!ComponentClass::IsValidComponent(Component))
				continue;

			ImGui::PushID(Component);
			if (boost::iequals(SearchClass::m_ComponentList, Component->m_Object.m_pClass->m_pName) || SearchClass::m_ComponentList.empty())
			{
				ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
				ImGui::BeginDisabled();
				ImGui::InputText("##", new std::string(Component->m_Object.m_pClass->m_pNamespace), ImGuiInputTextFlags_ReadOnly);
				ImGui::EndDisabled();
			}
			ImGui::PopID();
		}
		ImGui::EndChild();
		PopChildBgCol();
	}

	if (Globals::Gui::window == "field")
	{
		PushChildBgCol(Theme::childBgFields_Col);

		ImGui::PushItemWidth(230);
		ImGui::InputText("Filter", &SearchClass::m_FieldList);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			InfoBox("name must be complete, case insensitive");
		ImGui::PopItemWidth();

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

		PopChildBgCol();

		PushChildBgCol(Theme::childBgFields_Col);
		ImGui::BeginChild("field container", ImGui::GetContentRegionAvail(), true);
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
							Globals::Gui::style->Colors[ImGuiCol_Text] = ImColor(23, 156, 255, 200);
							ImGui::Text("INT"); ImGui::SameLine();
							Globals::Gui::style->Colors[ImGuiCol_Text] = Theme::text_Col;

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
							Globals::Gui::style->Colors[ImGuiCol_Text] = ImColor(0, 255, 0, 200);
							ImGui::Text("FLOAT"); ImGui::SameLine();
							Globals::Gui::style->Colors[ImGuiCol_Text] = Theme::text_Col;

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
							Globals::Gui::style->Colors[ImGuiCol_Text] = ImColor(255, 0, 0, 200);
							ImGui::Text("BOOL"); ImGui::SameLine();
							Globals::Gui::style->Colors[ImGuiCol_Text] = Theme::text_Col;

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
							if (strcmp(Field->m_pName, "_instance") == 0 || strcmp(Field->m_pName, "_Instance") == 0) // exclude the get instance field
								break;

							Globals::Gui::style->Colors[ImGuiCol_Text] = ImColor(167, 122, 221, 200);
							ImGui::Text("CLASS"); ImGui::SameLine();
							Globals::Gui::style->Colors[ImGuiCol_Text] = Theme::text_Col;

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
			if (!ComponentClass::IsValidComponent(ComponentClass::Get()) || !Property->m_pGet)
				continue;

			if (boost::iequals(SearchClass::m_FieldList, Property->m_pName) || SearchClass::m_FieldList.empty())
			{
				if (Globals::showProperties)
				{
					switch (Property->m_pGet->m_pReturnType->m_uType)
					{
						case Type_Integer:
						{
							int i = ComponentClass::Get()->GetMemberValue<int>(Property->m_pName);

							ImGui::Text(ICON_FA_WRENCH); ImGui::SameLine(); ImGui::Text("int"); ImGui::SameLine();
							ImGui::PushItemWidth(250);
							if (!Property->m_pSet)
								ImGui::BeginDisabled(true);
							if (ImGui::InputInt(Property->m_pName, &i, NULL, NULL, ImGuiInputTextFlags_EnterReturnsTrue))
							{
								ComponentClass::Get()->SetMemberValue<int>(Property->m_pName, i);
							}
							if (!Property->m_pSet)
								ImGui::EndDisabled();
							break;
						}
						
						case Type_Float:
						{
							float f = ComponentClass::Get()->GetMemberValue<float>(Property->m_pName);

							ImGui::Text(ICON_FA_WRENCH); ImGui::SameLine(); ImGui::Text("float"); ImGui::SameLine();
							ImGui::PushItemWidth(250);
							if (!Property->m_pSet)
								ImGui::BeginDisabled(true);
							if (ImGui::InputFloat(Property->m_pName, &f, NULL, NULL, "%.1f", ImGuiInputTextFlags_EnterReturnsTrue))
							{
								ComponentClass::Get()->SetMemberValue<float>(Property->m_pName, f);
							}
							if (!Property->m_pSet)
								ImGui::EndDisabled();
							break;
						}

						case Type_Boolean:
						{
							bool b = ComponentClass::Get()->GetMemberValue<bool>(Property->m_pName);

							ImGui::Text(ICON_FA_WRENCH); ImGui::SameLine(); ImGui::Text("bool"); ImGui::SameLine();
							if (!Property->m_pSet)
								ImGui::BeginDisabled(true);
							if (ImGui::Checkbox(Property->m_pName, &b))
							{
								(ComponentClass::Get()->GetMemberValue<bool>(Property->m_pName)) ? ComponentClass::Get()->SetMemberValue<bool>(Property->m_pName, false) : ComponentClass::Get()->SetMemberValue<bool>(Property->m_pName, true);
							}
							if (!Property->m_pSet)
								ImGui::EndDisabled();
							break;
						}

						case Type_Class:
						{
							ImGui::Text(ICON_FA_WRENCH); ImGui::SameLine(); ImGui::Text("class"); ImGui::SameLine();
							if (ImGui::Button(Property->m_pName, ImVec2(ImGui::GetContentRegionAvail().x, NULL)))
							{
								PropertyClass::Set(Property);
								FieldClass::PropertyFieldSearchAll();
								PropertyClass::ClearAll();
								MethodClass::MethodSearchAll();
								SearchClass::m_FieldList.clear();
								Globals::Gui::window = "field";
							}
							break;
						}
					}
				}
			}
		}

		ImGui::EndChild();
		PopChildBgCol();
	}

	if (Globals::Gui::window == "method")
	{
		(!Globals::MethodAutoMode) ? PushChildBgCol(Theme::childBgMethods_Col) : PushChildBgCol(ImColor(46, 46, 46));
		ImGui::BeginChild("method container", ImGui::GetContentRegionAvail());

		if (!Globals::MethodAutoMode)
		{
			// Methods
			if (ImGui::BeginCombo("Methods", MethodArgs::MethodName.c_str()))
			{
				for (auto Method : MethodClass::Gets())
				{
					if (ImGui::Selectable(Method->m_pName))
					{
						MethodArgs::MethodName = Method->m_pName;
						Globals::MethodRetType_Current = MethodClass::GetMethodRetType(Method);
						Globals::MethodArgType_Current = MethodClass::GetMethodArgType(Method);
					}
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

			switch (Globals::MethodRetType_Current)
			{
			case RetType::Void:
				break;
			case RetType::Int:
				ImGui::BeginDisabled();
				ImGui::InputInt("Return value", &MethodArgs::RetInt, NULL, NULL, ImGuiInputTextFlags_ReadOnly);
				ImGui::EndDisabled();
				break;
			case RetType::Float:
				ImGui::BeginDisabled();
				ImGui::InputFloat("Return value", &MethodArgs::RetFloat, NULL, NULL, "%.1f", ImGuiInputTextFlags_ReadOnly);
				ImGui::EndDisabled();
				break;
			case RetType::Bool:
				ImGui::BeginDisabled();
				ImGui::Checkbox("Return value", &MethodArgs::RetBool);
				ImGui::EndDisabled();
				break;
			}

			if (ImGui::Button("Call Method"))
			{
				Globals::callMethod = true;
			}

			TextCentered("Log");
			ImGui::InputTextMultiline("##", &LoggerClass::GetContent(), ImGui::GetContentRegionAvail(), ImGuiInputTextFlags_ReadOnly);
		}

		if (Globals::MethodAutoMode)
		{
			if (!MethodClass::m_InitContainers)
			{
				MethodClass::ClearContainers();
				MethodArgs::MethodIntegers.resize(1000);
				MethodArgs::MethodFloats.resize(1000);
				MethodArgs::MethodBooleans.resize(1000);
				MethodArgs::MethodStrings.resize(1000);
				MethodArgs::RetIntegers.resize(1000);
				MethodArgs::RetFloats.resize(1000);
				MethodArgs::RetBooleans.resize(1000);
				MethodClass::m_InitContainers = true;
			}

			auto dummyhspace = (ImGui::GetContentRegionAvail().x - ImGui::GetContentRegionAvail().x + 4);
			for (uintptr_t u{}; u < MethodClass::Gets().size(); ++u)
			{
				ImGui::PushID(u);
				ImGui::Dummy(ImVec2(0, 1));
				ImGui::SetCursorPosX(dummyhspace);
				TextCol(MethodClass::Gets().at(u)->m_pName, ImColor(244, 202, 33)); ImGui::SameLine(); TextCol(MethodClass::GetMethodRetTypeAsText(MethodClass::Gets().at(u)), ImColor(23, 156, 255, 200));
				ImGui::Dummy(ImVec2(0, 4));
				bool unsupported = false;

				switch (MethodClass::Gets().at(u)->m_pParameters->m_pParameterType->m_uType)
				{
				case Type_Integer:
					ImGui::SetCursorPosX(dummyhspace);
					ImGui::InputInt("Int value", &MethodArgs::MethodIntegers.at(u), NULL, NULL);
					break;
				case Type_Float:
					ImGui::SetCursorPosX(dummyhspace);
					ImGui::InputFloat("Float value", &MethodArgs::MethodFloats.at(u), NULL, NULL, "%.1f");
					break;
				case Type_Boolean:
					ImGui::SetCursorPosX(dummyhspace);
					ImGui::Checkbox("True or False?", (bool*)&MethodArgs::MethodBooleans.at(u));
					break;
				case Type_String:
					ImGui::SetCursorPosX(dummyhspace);
					ImGui::InputText("String", &MethodArgs::MethodStrings.at(u));
					break;
				default:
					unsupported = true;
					ImGui::SetCursorPosX(dummyhspace);
					ImGui::BeginDisabled();
					ImGui::InputText("Unsupported argument type", new std::string(""), ImGuiInputTextFlags_ReadOnly);
					ImGui::EndDisabled();
					break;
				}

				switch (MethodClass::Gets().at(u)->m_pReturnType->m_uType)
				{
				case Type_Void:
					break;
				case Type_Integer:
					ImGui::BeginDisabled();
					ImGui::SetCursorPosX(dummyhspace);
					ImGui::InputInt("Return value", &MethodArgs::RetIntegers.at(u), NULL, NULL, ImGuiInputTextFlags_ReadOnly);
					ImGui::EndDisabled();
					break;
				case Type_Float:
					ImGui::BeginDisabled();
					ImGui::SetCursorPosX(dummyhspace);
					ImGui::InputFloat("Return value", &MethodArgs::RetFloats.at(u), NULL, NULL, "%.1f", ImGuiInputTextFlags_ReadOnly);
					ImGui::EndDisabled();
					break;
				case Type_Boolean:
					ImGui::BeginDisabled();
					ImGui::SetCursorPosX(dummyhspace);
					ImGui::Checkbox("Return value", (bool*)&MethodArgs::RetBooleans.at(u));
					ImGui::EndDisabled();
					break;
				default:
					unsupported = true;
					break;
				}

				ImGui::Dummy(ImVec2(0, 2));
				PushButtonCol(ImColor(30, 30, 30), Theme::buttonHovered_Col);
				if (unsupported) { ImGui::BeginDisabled(); }
				ImGui::SetCursorPosX(dummyhspace);
				if (ImGui::Button("Call Method"))
				{
					MethodArgs::MethodName = MethodClass::Gets().at(u)->m_pName;
					Globals::MethodRetType_Current = MethodClass::GetMethodRetType(MethodClass::Gets().at(u));
					Globals::MethodArgType_Current = MethodClass::GetMethodArgType(MethodClass::Gets().at(u));
					switch (Globals::MethodArgType_Current)
					{
					case ArgType::inT:
						MethodArgs::MethodInt = MethodArgs::MethodIntegers.at(u);
						break;
					case ArgType::floaT:
						MethodArgs::MethodFloat = MethodArgs::MethodFloats.at(u);
						break;
					case ArgType::booL:
						MethodArgs::MethodBool = MethodArgs::MethodBooleans.at(u);
						break;
					case ArgType::strinG:
						MethodArgs::MethodString = MethodArgs::MethodStrings.at(u);
						break;
					}

					Globals::callMethod = true;
					OnUpdate(); // just to force it to happen immediately so the the return value is in sync

					switch (Globals::MethodRetType_Current)
					{
					case RetType::Void:
						break;
					case RetType::Int:
						MethodArgs::RetIntegers.at(u) = MethodArgs::RetInt;
						break;
					case RetType::Float:
						MethodArgs::RetFloats.at(u) = MethodArgs::RetFloat;
						break;
					case RetType::Bool:
						MethodArgs::RetBooleans.at(u) = MethodArgs::RetBool;
						break;
					}
					
				}
				if (unsupported) { ImGui::EndDisabled(); }
				PopButtonCol();

				ImGui::PopID();

				Globals::Gui::style->Colors[ImGuiCol_Separator] = ImColor(80, 80, 80, 255);
				ImGui::Separator();
				Globals::Gui::style->Colors[ImGuiCol_Separator] = ImColor(0, 0, 0, 0);
			}
		}

		ImGui::EndChild();
		PopChildBgCol();
	}

	if (Globals::Gui::window == "transform")
	{
		PushChildBgCol(Theme::childBgTransform_Col);
		PushPopupBgCol(Theme::popupBgTransform_Col, Theme::headerHoveredTransform_Col);

		if (GameObjectClass::IsValidGameObject(GameObjectClass::Get()))
		{
			Globals::Gui::style->Colors[ImGuiCol_Text] = ImColor(0, 255, 0, 200);
			ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine(0);
			ImGui::Text("Game Object:"); ImGui::SameLine(), Globals::Gui::style->Colors[ImGuiCol_Text] = Theme::text_Col;
			ImGui::InputText("##", new std::string (GameObjectClass::Get()->GetName()->ToString()), ImGuiInputTextFlags_ReadOnly);
			ImGui::SameLine(),
			PushButtonCol(ImColor(183, 64, 64), ImColor(193, 74, 74));
			if (ImGui::Button("Destroy"))
			{
				GameObjectClass::Get()->Destroy();
			}
			PopButtonCol();
			ImGui::SameLine();
			if (ImGui::Checkbox("Is Active", new bool(GameObjectClass::Get()->GetActive())))
			{
				((GameObjectClass::Get()->GetActive())) ? GameObjectClass::Get()->SetActive(false) : GameObjectClass::Get()->SetActive(true);
			}

			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 48);
			ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine(0);
			ImGui::InputText("path", &ChildClass::GetChildrenPath(), ImGuiInputTextFlags_ReadOnly);
			ImGui::PopItemWidth();

			Globals::Gui::style->Colors[ImGuiCol_Separator] = ImColor(60, 60, 60, 230);
			ImGui::Separator();
			Globals::Gui::style->Colors[ImGuiCol_Separator] = ImColor(0, 0, 0, 0);
			
			Unity::Vector3 position = GameObjectClass::Get()->GetTransform()->GetPosition();
			ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine(0);
			if (ImGui::MyInputFloat3("Position", ICON_FA_X, ICON_FA_Y, ICON_FA_Z, ImColor(255, 0, 0, 200), ImColor(0, 255, 0, 200), ImColor(23, 156, 255, 200), (float*)&position, "%.2f", ImGuiInputTextFlags_EnterReturnsTrue))
			{
				GameObjectClass::Get()->GetTransform()->SetPosition(position);
			}

			Unity::Vector3 localposition = GameObjectClass::Get()->GetTransform()->GetLocalPosition();
			ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine(0);
			if (ImGui::MyInputFloat3("Local position", ICON_FA_X, ICON_FA_Y, ICON_FA_Z, ImColor(255, 0, 0, 200), ImColor(0, 255, 0, 200), ImColor(23, 156, 255, 200), (float*)&localposition, "%.2f", ImGuiInputTextFlags_EnterReturnsTrue))
			{
				GameObjectClass::Get()->GetTransform()->SetLocalPosition(localposition);
			}

			Unity::Vector3 scale = GameObjectClass::Get()->GetTransform()->GetLocalScale();
			ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine(0);
			if (ImGui::MyInputFloat3("Local scale", ICON_FA_X, ICON_FA_Y, ICON_FA_Z, ImColor(255, 0, 0, 200), ImColor(0, 255, 0, 200), ImColor(23, 156, 255, 200), (float*)&scale, "%.2f", ImGuiInputTextFlags_EnterReturnsTrue))
			{
				GameObjectClass::Get()->GetTransform()->SetLocalScale(scale);
			}

			Unity::Quaternion rotation = GameObjectClass::Get()->GetTransform()->GetRotation();
			ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine(0);
			ImGui::SetNextItemWidth(738);
			if (ImGui::MyInputFloat4("Rot.", ICON_FA_X, ICON_FA_Y, ICON_FA_Z, ICON_FA_W, ImColor(255, 0, 0, 200), ImColor(0, 255, 0, 200), ImColor(23, 156, 255, 200), ImColor(255, 0, 255, 200), (float*)&rotation, "%.2f", ImGuiInputTextFlags_EnterReturnsTrue))
			{
				GameObjectClass::Get()->GetTransform()->SetRotation(rotation);
			}

			ImGui::Columns(2, nullptr, true);
			ImGui::SetColumnOffset(1, 420);

			ImGui::BeginChild("children", ImGui::GetContentRegionAvail(), true, ImGuiWindowFlags_MenuBar);

			ImGui::BeginMenuBar();
			if (ChildClass::GetParents().size() > 0)
			{
				PushButtonCol(ImColor(60, 60, 60), ImColor(80, 80, 80));
				if (ImGui::SmallButton(ICON_FA_ARROW_LEFT))
				{
					GameObjectClass::Set(ChildClass::GetParents().back());
					ComponentClass::ComponentSearchAll();
					ChildClass::ChildrenSearchAll();
					SearchClass::m_ComponentList.clear();
					ChildClass::PopParentPath();
					ChildClass::PopParent();
				}
				PopButtonCol();
			}
			TextCentered("Children Gameobjects");
			ImGui::EndMenuBar();

			for (auto child : ChildClass::Gets())
			{
				ImGui::PushID(child);
				bool onoff = child->GetActive();
				if (!GameObjectClass::IsValidGameObject(child))
					onoff = false;

				ImGui::Checkbox("##", &onoff);
				ImGui::SameLine();
				if (ImGui::BeginMenu(child->GetName()->ToString().c_str(), onoff))
				{
					ChildClass::SubChildrenSearchAll(child);

					for (auto subchild : ChildClass::GetsSubChildren())
					{
						if (GameObjectClass::IsValidGameObject(subchild))
						{
							ImGui::Selectable(subchild->GetName()->ToString().c_str());
							if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
							{
								ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
								if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
								{
									ChildClass::PushParentPath(child->GetName()->ToString());
									ChildClass::PushParentPath(subchild->GetName()->ToString());
									ChildClass::PushParent(GameObjectClass::Get());
									ChildClass::PushParent(child);
									GameObjectClass::Set(subchild);
									ComponentClass::ComponentSearchAll();
									ChildClass::ChildrenSearchAll();
									SearchClass::m_ComponentList.clear();
								}
							}
						}
					}
					ImGui::EndMenu();
				}

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

				Globals::Gui::style->Colors[ImGuiCol_Separator] = ImColor(60, 60, 60, 230);
				ImGui::Separator();
				Globals::Gui::style->Colors[ImGuiCol_Separator] = ImColor(0, 0, 0, 0);
				
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
				continue;

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
		PopChildBgCol();
		PopPopupBgCol();
	}

	if (Globals::Gui::window == "log")
	{
		ImGui::Text("Log window");
		ImGui::InputTextMultiline("##", &LoggerClass::GetContent(), ImGui::GetContentRegionAvail(), ImGuiInputTextFlags_ReadOnly);
	}
	ImGui::EndChild();
	PopChildBgCol();

	ImGui::EndChild();
	ImGui::End();
}