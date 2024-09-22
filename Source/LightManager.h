#pragma once

#include "Light.h"

#include <QObject>

namespace BSplineCurves3D
{
    class LightManager : public QObject
    {
        Q_OBJECT

    private:
        explicit LightManager(QObject* parent = nullptr);

    public:
        void AddLight(Light* light);
        void RemoveLight(Light* light);

        const QList<Light*>& GetLights() const;

        Light* GetActiveLight() const;
        void SetActiveLight(Light* newActiveLight);

        static LightManager* Instance();

    private:
        QList<Light*> mLights;
        Light* mActiveLight;
    };
}
