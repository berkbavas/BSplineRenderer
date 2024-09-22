#pragma once

#include "Spline.h"

#include <QString>

namespace BSplineCurves3D
{
    class Helper
    {
    private:
        Helper();

    public:
        static QByteArray GetBytes(QString path);
        static QList<Spline*> LoadCurveDataFromJson(const QString& filename);
        static bool SaveCurveDataToJson(const QList<Spline*>& curves, const QString& filename);
        static QQuaternion RotateX(float angleRadians);
        static QQuaternion RotateY(float angleRadians);
        static QQuaternion RotateZ(float angleRadians);
        static QVector3D ProjectOntoPlane(const QVector3D& normal, const QVector3D& point, const QVector3D& subject);
    };
}
