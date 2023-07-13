#include "imgui/imgui.h"
#include "gui utils.hpp"

void GuiTools()
{
	ImGui::Begin("Gui Tools", nullptr, ImGuiWindowFlags_NoCollapse);

	ImGui::SliderFloat("Border size", &Theme::border_Size, 0, 20, "%.1f");
	ImGui::SliderFloat("Rounding windows", &Theme::rounding_Windows, 0, 20, "%.1f");
	ImGui::SliderFloat("Rounding others", &Theme::rounding_Others, 0, 20, "%.1f");
	ImGui::ColorEdit3("Border Col", (float*)&Theme::border_Col.Value.x);
	ImGui::ColorEdit3("Border others Col", (float*)&Theme::borderOthers_Col.Value.x);
	ImGui::ColorEdit3("Child Bg Col", (float*)&Theme::childBg_Col.Value.x);
	ImGui::ColorEdit3("Child Bg outside Col", (float*)&Theme::childBgOutside_Col.Value.x);
	ImGui::ColorEdit3("Child Bg GameObjects Col", (float*)&Theme::childBgGameObjects_Col.Value.x);
	ImGui::ColorEdit3("Child Bg Components Col", (float*)&Theme::childBgComponents_Col.Value.x);
	ImGui::ColorEdit3("Child Bg Fields Col", (float*)&Theme::childBgFields_Col.Value.x);
	ImGui::ColorEdit3("Child Bg Methods Col", (float*)&Theme::childBgMethods_Col.Value.x);
	ImGui::ColorEdit3("Child Bg Transform Col", (float*)&Theme::childBgTransform_Col.Value.x);
	ImGui::ColorEdit3("Text Col", (float*)&Theme::text_Col.Value.x);
	ImGui::ColorEdit3("Frame Col", (float*)&Theme::frame_Col.Value.x);
	ImGui::ColorEdit3("Button Col", (float*)&Theme::button_Col.Value.x);
	ImGui::ColorEdit3("Button Hovered Col", (float*)&Theme::buttonHovered_Col.Value.x);
	ImGui::ColorEdit3("Search Button Col", (float*)&Theme::buttonSearch_Col.Value.x);
	ImGui::ColorEdit3("Search Button Hovered Col", (float*)&Theme::buttonSearchHovered_Col.Value.x);
	ImGui::ColorEdit3("Buttons Menu Bar Col", (float*)&Theme::buttonMenuBar_Col.Value.x);
	ImGui::ColorEdit3("Buttons Menu Bar Hovered Col", (float*)&Theme::buttonHoveredMenuBar_Col.Value.x);
	ImGui::ColorEdit3("Popup Bg Col", (float*)&Theme::popupBg_Col.Value.x);
	ImGui::ColorEdit3("Popup Bg TransformCol", (float*)&Theme::popupBgTransform_Col.Value.x);
	ImGui::ColorEdit3("Popup Hovered Bg TransformCol", (float*)&Theme::headerHoveredTransform_Col.Value.x);
	ImGui::ColorEdit3("Theme Col", (float*)&Theme::color.Value.x);

	ImGui::End();
}
