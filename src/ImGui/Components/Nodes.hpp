#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_internal.h>

namespace ImGui {

void BeginNode() {
  BeginGroup();
  PushID("Node" /* node->ID */);

  const float NODE_SLOT_RADIUS = 4.0f;
  const ImVec2 NODE_WINDOW_PADDING(8.0f, 8.0f);

  PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
  BeginChild("Nodes Canvas", ImVec2(0, 0), false,
             ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);

  PushItemWidth(120.0f);
  // Render node links

  // Render nodes

  // Render node content

  BeginGroup(); // Lock horizontal position
  Text("%s", "Name" /* node->Name */);
  Separator();
}

void EndNode() {
  // Render node background/box

  // Render node sockets
  EndGroup();
  PopItemWidth();
  EndChild();
  PopStyleColor();
  PopID();
  EndGroup();
}

} // namespace ImGui
