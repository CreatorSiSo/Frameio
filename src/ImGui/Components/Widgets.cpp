#include "ImGui/Components/Widgets.hpp"

// TODO Add all DragFloat and DragInt widgets

bool ImGui::DragFloat3(const char* label,
                       glm::vec3& value,
                       float reset_value,
                       float v_speed,
                       float v_min,
                       float v_max,
                       const char* format,
                       ImGuiSliderFlags flags)
{
  PushID(label);

  Columns(2, NULL, false);
  SetColumnWidth(0, GetWindowContentRegionMin().x + 80.0f);

  //* For debugging the column width
  // ImVec2 vMin = ImGui::GetWindowContentRegionMin() + ImVec2(80.0f, 0.0f);
  // ImVec2 vMax = ImGui::GetWindowContentRegionMax();

  // vMin.x += ImGui::GetWindowPos().x;
  // vMin.y += ImGui::GetWindowPos().y;
  // vMax.x += ImGui::GetWindowPos().x;
  // vMax.y += ImGui::GetWindowPos().y;

  // ImGui::GetForegroundDrawList()->AddRect(vMin, vMax,
  //                                         IM_COL32(255, 255, 0, 255));

  Text("%s", label);

  NextColumn();
  PushMultiItemsWidths(3, CalcItemWidth());

  float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
  ImVec2 buttonSize = { lineHeight + 2.0f, lineHeight };
  PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.0f, GImGui->Style.ItemSpacing.y });

  //* X Component
  { /* Reset Button X */
    //? TODO Move hardcoded colors into general color theme
    PushStyleColor(ImGuiCol_Button, ImVec4(0.72f, 0.24f, 0.24f, 1.0f));
    PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.82f, 0.26f, 0.26f, 1.0f));
    PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.67f, 0.20f, 0.20f, 1.0f));

    // TODO Find out how to dynamically set the component names by passing
    // TODO exactly one character to imgui and not a range:
    // TODO const char components[3] = "XYZ"
    // TODO &components[0] -> "XYZ" &components[1] -> "YZ" &components[2] -> "Z"
    if (ButtonEx("X", buttonSize, 0, ImDrawFlags_RoundCornersLeft)) value.x = reset_value;
    PopStyleColor(3);
  }
  SameLine();
  { /* Drag X */
    DragScalar("##X", ImGuiDataType_Float, &value.x, 0.1f, NULL, NULL, NULL, 0, ImDrawFlags_RoundCornersNone);
  }
  PopItemWidth();
  // X Component End

  SameLine();

  //* Y Component
  { /* Reset Button Y */
    //? TODO Move hardcoded colors into general color theme
    PushStyleColor(ImGuiCol_Button, ImVec4(0.40f, 0.66f, 0.24f, 1.0f));
    PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.44f, 0.72f, 0.28f, 1.0f));
    PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.36f, 0.61f, 0.20f, 1.0f));
    if (ButtonEx("Y", buttonSize, 0, ImDrawFlags_RoundCornersNone)) value.y = reset_value;
    PopStyleColor(3);
  }
  SameLine();
  { /* Drag Y */
    DragScalar("##Y", ImGuiDataType_Float, &value.y, 0.1f, NULL, NULL, NULL, 0, ImDrawFlags_RoundCornersNone);
  }
  PopItemWidth();
  // Y Component End

  SameLine();

  //* Z Component
  { /* Reset Button Z */
    //? TODO Move hardcoded colors into general color theme
    PushStyleColor(ImGuiCol_Button, ImVec4(0.24f, 0.52f, 0.72f, 1.0f));
    PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.28f, 0.56f, 0.82f, 1.0f));
    PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.18f, 0.46f, 0.66f, 1.0f));
    if (ButtonEx("Z", buttonSize, 0, ImDrawFlags_RoundCornersNone)) value.z = reset_value;
    PopStyleColor(3);
  }
  SameLine();
  { /* Drag Z */
    DragScalar("##Z", ImGuiDataType_Float, &value.z, 0.1f, NULL, NULL, NULL, 0, ImDrawFlags_RoundCornersRight);
  }
  PopItemWidth();
  // Z Component End

  PopStyleVar();
  Columns(1);
  PopID();

  return true;
}
