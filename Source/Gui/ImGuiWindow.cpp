#include "ImGuiWindow.h"

#include "Renderer/RendererManager.h"
#include "Util/Logger.h"

#include <QtImGui.h>
#include <imgui.h>

BSplineRenderer::ImGuiWindow::ImGuiWindow(QObject* parent)
    : QObject(parent)
{
}

void BSplineRenderer::ImGuiWindow::Draw()
{
    mNumberOfSegments = mRendererManager->GetNumberOfSegments();
    mNumberOfSectors = mRendererManager->GetNumberOfSectors();

    ImGui::Begin("Controls");
    if (ImGui::SliderInt("Number of Segments", &mNumberOfSegments, 1, 64))
    {
        mRendererManager->SetNumberOfSegments(mNumberOfSegments);
    }

    if (ImGui::SliderInt("Number of Sectors", &mNumberOfSectors, 1, 64))
    {
        mRendererManager->SetNumberOfSectors(mNumberOfSectors);
    }

    ImGui::Checkbox("Wireframe", mRendererManager->GetWireframe());

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}

void BSplineRenderer::ImGuiWindow::SetRendererManager(RendererManager* manager)
{
    mRendererManager = manager;
}
