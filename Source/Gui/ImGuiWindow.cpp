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

    if (ImGui::CollapsingHeader("Render Settings", ImGuiTreeNodeFlags_DefaultOpen))
    {
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
    }

    ImGui::BeginDisabled(!mSelectedCurve);
    if (ImGui::CollapsingHeader("Curve", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (mSelectedCurve)
        {
            ImGui::Text("Pointer: 0x%p", mSelectedCurve.get());
            ImGui::Text("# of Knots: %zu", mSelectedCurve->GetKnots().size());
            ImGui::SliderFloat("Radius", &mSelectedCurve->GetRadius_NonConst(), 0.10f, 0.50f);
            ImGui::SliderFloat("Ambient", &mSelectedCurve->GetAmbient_NonConst(), 0.0f, 1.0f);
            ImGui::SliderFloat("Diffuse", &mSelectedCurve->GetDiffuse_NonConst(), 0.0f, 1.0f);
            ImGui::ColorEdit4("Color", &mSelectedCurve->GetColor_NonConst()[0]);

            if (mSelectedKnot)
            {
                ImGui::Text("Selected Knot: (%.2f, %.2f)", mSelectedKnot->GetPosition().x(), mSelectedKnot->GetPosition().y());
            }
        }
    }
    ImGui::EndDisabled();

    ImGui::End();
}

void BSplineRenderer::ImGuiWindow::SetRendererManager(RendererManager* manager)
{
    mRendererManager = manager;
}
