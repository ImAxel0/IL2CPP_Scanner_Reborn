#pragma once
#include "imgui/imgui.h"
#include "globals.hpp"
#include <string>

void GuiTools();

namespace Theme
{
	inline ImColor color(26, 26, 26);
	inline ImColor* theme = &color;
	inline float border_Size = 3.0f;
	inline float rounding_Windows = 5.0f;
	inline float rounding_Others = 4.0f;
	inline ImColor text_Col = ImColor(255, 255, 255, 230);
	inline ImColor border_Col = ImColor(24, 24, 24);
	inline ImColor borderOthers_Col = ImColor(115, 115, 115);
	inline ImColor frame_Col = ImColor(24, 24, 24);
	inline ImColor button_Col = ImColor(48, 48, 48);
	inline ImColor buttonHovered_Col = ImColor(36, 36, 36);
	inline ImColor buttonSearch_Col = ImColor(75, 75, 75);
	inline ImColor buttonSearchHovered_Col = ImColor(45, 45, 45);
	inline ImColor buttonMenuBar_Col = ImColor(30, 80, 159);
	inline ImColor buttonHoveredMenuBar_Col = ImColor(55, 55, 55);
	inline ImColor childBg_Col = ImColor(56, 56, 56);
	inline ImColor childBgOutside_Col = ImColor(75, 75, 75);
	inline ImColor childBgGameObjects_Col = ImColor(24, 24, 24);
	inline ImColor childBgComponents_Col = ImColor(24, 24, 24);
	inline ImColor childBgFields_Col = ImColor(24, 24, 24);
	inline ImColor childBgMethods_Col = ImColor(75, 75, 75);
	inline ImColor childBgTransform_Col = ImColor(24, 24, 24);
	inline ImColor separator_Col = ImColor(0, 0, 0, 0);
	inline ImColor popupBg_Col = ImColor(16, 16, 16);
	inline ImColor popupBgTransform_Col = ImColor(16, 16, 16);
	inline ImColor headerHovered_Col = ImColor(26, 26, 26);
	inline ImColor headerHoveredTransform_Col = ImColor(38, 38, 38);

	inline bool gui_Tools = false;
}

inline void MainTheme()
{
	Globals::Gui::style = &ImGui::GetStyle();
	ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)Theme::childBg_Col);
	Globals::Gui::style->WindowTitleAlign = ImVec2(0.5, 0.5);
	Globals::Gui::style->WindowBorderSize = Theme::border_Size;
	Globals::Gui::style->WindowRounding = Theme::rounding_Windows;
	Globals::Gui::style->FrameRounding = Theme::rounding_Others;
	Globals::Gui::style->ChildRounding = Theme::rounding_Others;
	Globals::Gui::style->Colors[ImGuiCol_Text] = Theme::text_Col;
	Globals::Gui::style->Colors[ImGuiCol_Border] = Theme::border_Col;
	Globals::Gui::style->Colors[ImGuiCol_TitleBg] = *Theme::theme;
	Globals::Gui::style->Colors[ImGuiCol_TitleBgActive] = *Theme::theme;
	Globals::Gui::style->Colors[ImGuiCol_FrameBg] = Theme::frame_Col;
	Globals::Gui::style->Colors[ImGuiCol_FrameBgActive] = Theme::frame_Col;
	Globals::Gui::style->Colors[ImGuiCol_FrameBgHovered] = Theme::frame_Col;
	Globals::Gui::style->Colors[ImGuiCol_Button] = Theme::button_Col;
	Globals::Gui::style->Colors[ImGuiCol_ButtonHovered] = Theme::buttonHovered_Col;
	Globals::Gui::style->Colors[ImGuiCol_ChildBg] = Theme::childBg_Col;
	Globals::Gui::style->Colors[ImGuiCol_Separator] = Theme::separator_Col;
	Globals::Gui::style->Colors[ImGuiCol_SeparatorActive] = Theme::separator_Col;
	Globals::Gui::style->Colors[ImGuiCol_SeparatorHovered] = Theme::separator_Col;
	Globals::Gui::style->Colors[ImGuiCol_PopupBg] = Theme::popupBg_Col;
	Globals::Gui::style->Colors[ImGuiCol_HeaderHovered] = Theme::headerHovered_Col;
}

inline void PushButtonCol(ImColor color, ImColor hovered)
{
	Globals::Gui::style->Colors[ImGuiCol_Button] = color;
	Globals::Gui::style->Colors[ImGuiCol_ButtonHovered] = hovered;
}

inline void PopButtonCol()
{
	Globals::Gui::style->Colors[ImGuiCol_Button] = Theme::button_Col;
	Globals::Gui::style->Colors[ImGuiCol_ButtonHovered] = Theme::buttonHovered_Col;
}

inline void PushChildBgCol(ImColor color)
{
	Globals::Gui::style->Colors[ImGuiCol_ChildBg] = color;
}

inline void PopChildBgCol()
{
	Globals::Gui::style->Colors[ImGuiCol_ChildBg] = Theme::childBg_Col;
}

inline void PushPopupBgCol(ImColor color, ImColor hovered)
{
	Globals::Gui::style->Colors[ImGuiCol_PopupBg] = color;
	Globals::Gui::style->Colors[ImGuiCol_HeaderHovered] = hovered;
}

inline void PopPopupBgCol()
{
	Globals::Gui::style->Colors[ImGuiCol_PopupBg] = Theme::popupBg_Col;
	Globals::Gui::style->Colors[ImGuiCol_HeaderHovered] = Theme::headerHovered_Col;
}

inline void PushFrameBgCol(ImColor color)
{
	Globals::Gui::style->Colors[ImGuiCol_FrameBg] = color;
	Globals::Gui::style->Colors[ImGuiCol_FrameBgHovered] = color;
	Globals::Gui::style->Colors[ImGuiCol_FrameBgActive] = color;
}

inline void PopFrameBgCol()
{
	Globals::Gui::style->Colors[ImGuiCol_FrameBg] = Theme::frame_Col;
	Globals::Gui::style->Colors[ImGuiCol_FrameBgHovered] = Theme::frame_Col;
	Globals::Gui::style->Colors[ImGuiCol_FrameBgActive] = Theme::frame_Col;
}

inline void TextCol(const char* txt, ImColor color)
{
	Globals::Gui::style->Colors[ImGuiCol_Text] = color;
	ImGui::Text(txt);
	Globals::Gui::style->Colors[ImGuiCol_Text] = Theme::text_Col;
}

inline void TextCenteredXY(std::string text) {
	auto windowWidth = ImGui::GetContentRegionAvail().x;
	auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

	auto windowHeight = ImGui::GetContentRegionAvail().y;
	auto textHeight = ImGui::CalcTextSize(text.c_str()).y;

	ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
	ImGui::SetCursorPosY((windowHeight - textHeight) * 0.5f);
	ImGui::Text(text.c_str());
}

inline void TextCentered(std::string text) {
	auto windowWidth = ImGui::GetWindowContentRegionWidth();
	auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

	ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
	ImGui::Text(text.c_str());
}

inline void InfoBox(std::string info)
{
	ImGui::BeginTooltip();
	ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
	ImGui::TextUnformatted(info.c_str());
	ImGui::PopTextWrapPos();
	ImGui::EndTooltip();
}

inline bool CompareStringCaseInsensitive(const std::string& lhs, const std::string& rhs) {

	std::string::size_type common_length = std::min<size_t>(lhs.length(), rhs.length());

	for (std::string::size_type i = 0; i < common_length; ++i) {
		if (toupper(lhs[i]) < toupper(rhs[i]))return true;
		if (toupper(lhs[i]) > toupper(rhs[i]))return false;
	}

	if (lhs.length() < rhs.length())return true;
	if (lhs.length() > rhs.length())return false;

	return false;
}
