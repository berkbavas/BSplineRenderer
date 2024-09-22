#pragma once

#include "Model.h"
#include <QObject>

namespace BSplineCurves3D
{
    class ModelManager : public QObject
    {
        Q_OBJECT

    private:
        explicit ModelManager(QObject* parent = nullptr);

    public:
        void AddModel(Model* model);
        void RemoveModel(Model* model);

        const QList<Model*>& GetModels() const;

        static ModelManager* Instance();

    private:
        QList<Model*> mModels;
    };
}
