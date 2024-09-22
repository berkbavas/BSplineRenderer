#include "ModelManager.h"

BSplineCurves3D::ModelManager::ModelManager(QObject* parent)
    : QObject(parent)
{}

void BSplineCurves3D::ModelManager::AddModel(Model* model)
{
    mModels << model;
}

void BSplineCurves3D::ModelManager::RemoveModel(Model* model)
{
    if (model)
    {
        mModels.removeAll(model);
        model->deleteLater();
    }
}

const QList<BSplineCurves3D::Model*>& BSplineCurves3D::ModelManager::GetModels() const
{
    return mModels;
}

BSplineCurves3D::ModelManager* BSplineCurves3D::ModelManager::Instance()
{
    static ModelManager instance;
    return &instance;
}