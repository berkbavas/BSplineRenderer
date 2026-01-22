#pragma once

#include "Core/CurveContainer.h"
#include "Curve/Knot.h"
#include "Curve/Spline.h"
#include "Util/Macros.h"

#include <QObject>
#include <QString>
#include <QVariant>

namespace BSplineRenderer
{
    class RendererManager;

    enum class ThemeStyle
    {
        Dark,
        Light,
        Classic,
        Blue
    };

    class ImGuiWindow : public QObject
    {
        Q_OBJECT
      public:
        explicit ImGuiWindow(QObject* parent);

        void Draw();

        void SetRendererManager(RendererManager* manager);
        void SetCurveContainer(CurveContainer* container) { mCurveContainer = container; }

      signals:
        void CurveAdded(SplinePtr spline);
        void RequestCameraReset();
        void RequestCameraPreset(int preset);

      private:
        void DrawRenderSettings();
        void DrawCurvePanel();
        void DrawPresetShapesPanel();
        void DrawAnimationPanel();
        void DrawStatisticsPanel();
        void DrawFileOperationsPanel();
        void DrawThemePanel();
        void DrawCameraPanel();
        void DrawHelpPanel();

        void ApplyTheme(ThemeStyle style);

        int mNumberOfSegments;
        int mNumberOfSectors;

        RendererManager* mRendererManager;
        CurveContainer* mCurveContainer{ nullptr };

        ThemeStyle mCurrentTheme{ ThemeStyle::Dark };
        bool mShowStatistics{ false };
        bool mShowHelp{ false };

        // Preset şekil parametreleri
        float mPresetRadius{ 5.0f };
        float mPresetHeight{ 10.0f };
        int mPresetPoints{ 24 };
        int mPresetTurns{ 3 };

        // Animasyon parametreleri
        int mSelectedAnimationType{ 0 };
        float mAnimationSpeed{ 1.0f };
        float mAnimationAmplitude{ 1.0f };

        // Dosya işlemleri
        char mFilePath[256] = "curves.json";

        DEFINE_MEMBER(SplinePtr, SelectedCurve, nullptr);
        DEFINE_MEMBER(KnotPtr, SelectedKnot, nullptr);
    };
}