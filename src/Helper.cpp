#include "Helper.h"

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QQuaternion>
#include <QTextStream>
#include <QtMath>

#include <Dense>

BSplineCurves3D::Helper::Helper() {}

QByteArray BSplineCurves3D::Helper::GetBytes(QString path)
{
    QFile file(path);
    if (file.open(QFile::ReadOnly))
    {
        return file.readAll();
    }
    else
    {
        qWarning() << QString("Could not open '%1'").arg(path);
        return QByteArray();
    }
}

QList<BSplineCurves3D::Spline*> BSplineCurves3D::Helper::LoadCurveDataFromJson(const QString& filename)
{
    QJsonDocument document;

    // Read the file
    {
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly))
        {
            qCritical() << "Error occured while loading the file:" << filename;
            return QList<Spline*>();
        }

        document = QJsonDocument::fromJson(file.readAll());
        file.close();
    }

    QList<Spline*> curves;
    QJsonArray curvesArray = document.array();

    for (const auto& element : qAsConst(curvesArray))
    {
        Spline* curve = new Spline;

        QJsonObject curveObject = element.toObject();
        QJsonArray knotsArray = curveObject["knots"].toArray();
        float r = curveObject["r"].toDouble();
        int sectorCount = curveObject["sector_count"].toInt();
        curve->SetRadius(r);
        curve->SetSectorCount(sectorCount);

        for (const auto& knotElement : qAsConst(knotsArray))
        {
            QJsonObject knotObject = knotElement.toObject();
            QJsonObject positionObject = knotObject["position"].toObject();
            float x = positionObject["x"].toDouble();
            float y = positionObject["y"].toDouble();
            float z = positionObject["z"].toDouble();

            KnotPoint* knot = new KnotPoint(x, y, z);
            curve->AddKnotPoint(knot);
        }

        curves << curve;
    }

    return curves;
}

bool BSplineCurves3D::Helper::SaveCurveDataToJson(const QList<Spline*>& curves, const QString& filename)
{
    QJsonArray curvesArray;
    for (auto& curve : curves)
    {
        QJsonObject curveObject;
        QJsonArray knotsArray;
        QList<KnotPoint*> knots = curve->GetKnotPoints();

        for (auto& knot : knots)
        {
            QJsonObject knotObject;

            QJsonObject position;
            position.insert("x", knot->GetPosition().x());
            position.insert("y", knot->GetPosition().y());
            position.insert("z", knot->GetPosition().z());

            knotObject.insert("position", position);

            knotsArray << knotObject;
        }

        curveObject.insert("r", curve->GetRadius());
        curveObject.insert("sector_count", curve->GetSectorCount());
        curveObject.insert("knots", knotsArray);

        curvesArray << curveObject;
    }

    QFile file(filename);
    if (file.open(QIODevice::WriteOnly))
    {
        QJsonDocument document;
        document.setArray(curvesArray);
        QTextStream stream(&file);
        stream.setEncoding(QStringConverter::Encoding::Utf8);
        stream << document.toJson(QJsonDocument::Indented);
        stream.flush();
        file.close();
        return true;
    }
    else
    {
        qCritical() << Q_FUNC_INFO << "Couldn't write to file" << filename;
        return false;
    }
}

QQuaternion BSplineCurves3D::Helper::RotateX(float angleRadians)
{
    return QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), qRadiansToDegrees(angleRadians));
}

QQuaternion BSplineCurves3D::Helper::RotateY(float angleRadians)
{
    return QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), qRadiansToDegrees(angleRadians));
}

QQuaternion BSplineCurves3D::Helper::RotateZ(float angleRadians)
{
    return QQuaternion::fromAxisAndAngle(QVector3D(0, 0, 1), qRadiansToDegrees(angleRadians));
}

QVector3D BSplineCurves3D::Helper::ProjectOntoPlane(const QVector3D& normal, const QVector3D& point, const QVector3D& subject)
{
    Eigen::Vector3f normalEigen = Eigen::Vector3f(normal.x(), normal.y(), normal.z());
    Eigen::Vector3f pointEigen = Eigen::Vector3f(point.x(), point.y(), point.z());
    Eigen::Vector3f subjectEigen = Eigen::Vector3f(subject.x(), subject.y(), subject.z());
    Eigen::Hyperplane<float, 3> plane = Eigen::Hyperplane<float, 3>(normalEigen, -normalEigen.dot(pointEigen));
    Eigen::Vector3f projection = plane.projection(subjectEigen);

    return QVector3D(projection.x(), projection.y(), projection.z());
}