#pragma once

#include "Curve/Knot.h"
#include "Curve/Spline.h"
#include "Util/Macros.h"

#include <QObject>
#include <QVariant>

namespace BSplineRenderer
{
    class RendererManager;

    class ImGuiWindow : public QObject
    {
        Q_OBJECT
      public:
        explicit ImGuiWindow(QObject* parent);

        void Draw();

        void SetRendererManager(RendererManager* manager);

      private:
        int mNumberOfSegments;
        int mNumberOfSectors;

        RendererManager* mRendererManager;

        DEFINE_MEMBER(SplinePtr, SelectedCurve, nullptr);
        DEFINE_MEMBER(KnotPtr, SelectedKnot, nullptr);
    };
}