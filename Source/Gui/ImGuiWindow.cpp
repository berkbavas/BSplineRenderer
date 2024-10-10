#include "ImGuiWindow.h"

#include "Util/Logger.h"

#include <QtImGui.h>
#include <imgui.h>

BSplineRenderer::ImGuiWindow::ImGuiWindow(QObject* parent)
    : QObject(parent)
{
}

void BSplineRenderer::ImGuiWindow::Draw()
{
    ImGui::Begin("Controls");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}