#include "LightManager.h"
#include "Light.h"

BSplineCurves3D::LightManager::LightManager(QObject* parent)
    : QObject(parent)
    , mActiveLight(nullptr)
{}

void BSplineCurves3D::LightManager::AddLight(Light* light)
{
    mLights << light;
}

void BSplineCurves3D::LightManager::RemoveLight(Light* light)
{
    if (light)
    {
        if (light == mActiveLight)
        {
            mActiveLight = nullptr;
        }

        mLights.removeAll(light);
        light->deleteLater();
    }
}

BSplineCurves3D::Light* BSplineCurves3D::LightManager::GetActiveLight() const
{
    return mActiveLight;
}

void BSplineCurves3D::LightManager::SetActiveLight(Light* newActiveLight)
{
    mActiveLight = newActiveLight;
}

BSplineCurves3D::LightManager* BSplineCurves3D::LightManager::Instance()
{
    static LightManager instance;

    return &instance;
}

const QList<BSplineCurves3D::Light*>& BSplineCurves3D::LightManager::GetLights() const
{
    return mLights;
}