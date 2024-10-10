#pragma once

#include "Util/Macros.h"

#include <QObject>
#include <QVariant>

namespace BSplineRenderer
{
    class CurveContainer;
    class RendererManager;

    class ImGuiWindow : public QObject
    {
        Q_OBJECT
      public:
        explicit ImGuiWindow(QObject* parent);

        void Draw();
    };
}