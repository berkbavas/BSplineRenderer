#pragma once

#include "Core/CurveContainer.h"
#include "Curve/Spline.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

namespace BSplineRenderer
{
    class CurveSerializer
    {
      public:
        // Save all curves to JSON file
        static bool SaveToFile(const QString& filePath, CurveContainer* container)
        {
            QJsonObject root;
            QJsonArray curvesArray;

            for (const auto& spline : container->GetCurves())
            {
                QJsonObject curveObj;
                curveObj["radius"] = spline->GetRadius();
                curveObj["ambient"] = spline->GetAmbient();
                curveObj["diffuse"] = spline->GetDiffuse();
                curveObj["specular"] = spline->GetSpecular();
                curveObj["shininess"] = spline->GetShininess();

                QJsonArray colorArray;
                colorArray.append(spline->GetColor().x());
                colorArray.append(spline->GetColor().y());
                colorArray.append(spline->GetColor().z());
                colorArray.append(spline->GetColor().w());
                curveObj["color"] = colorArray;

                QJsonArray knotsArray;
                for (const auto& knot : spline->GetKnots())
                {
                    QJsonObject knotObj;
                    knotObj["x"] = knot->GetPosition().x();
                    knotObj["y"] = knot->GetPosition().y();
                    knotObj["z"] = knot->GetPosition().z();
                    knotsArray.append(knotObj);
                }
                curveObj["knots"] = knotsArray;

                curvesArray.append(curveObj);
            }

            root["version"] = "1.0";
            root["curves"] = curvesArray;

            QJsonDocument doc(root);
            QFile file(filePath);

            if (!file.open(QIODevice::WriteOnly))
            {
                return false;
            }

            file.write(doc.toJson(QJsonDocument::Indented));
            file.close();
            return true;
        }

        // Load curves from JSON file
        static bool LoadFromFile(const QString& filePath, CurveContainer* container)
        {
            QFile file(filePath);

            if (!file.open(QIODevice::ReadOnly))
            {
                return false;
            }

            QByteArray data = file.readAll();
            file.close();

            QJsonDocument doc = QJsonDocument::fromJson(data);
            if (doc.isNull())
            {
                return false;
            }

            QJsonObject root = doc.object();
            QJsonArray curvesArray = root["curves"].toArray();

            for (const auto& curveVal : curvesArray)
            {
                QJsonObject curveObj = curveVal.toObject();

                auto spline = std::make_shared<Spline>();

                spline->SetRadius(curveObj["radius"].toDouble(0.25f));
                spline->SetAmbient(curveObj["ambient"].toDouble(0.25f));
                spline->SetDiffuse(curveObj["diffuse"].toDouble(0.5f));
                spline->SetSpecular(curveObj["specular"].toDouble(0.25f));
                spline->SetShininess(curveObj["shininess"].toDouble(4.0f));

                QJsonArray colorArray = curveObj["color"].toArray();
                if (colorArray.size() >= 4)
                {
                    spline->SetColor(QVector4D(colorArray[0].toDouble(), colorArray[1].toDouble(), colorArray[2].toDouble(),
                                               colorArray[3].toDouble()));
                }

                QJsonArray knotsArray = curveObj["knots"].toArray();
                for (const auto& knotVal : knotsArray)
                {
                    QJsonObject knotObj = knotVal.toObject();
                    float x = knotObj["x"].toDouble();
                    float y = knotObj["y"].toDouble();
                    float z = knotObj["z"].toDouble();
                    spline->AddKnot(x, y, z);
                }

                container->AddCurve(spline);
            }

            return true;
        }

        // Convert a single curve to JSON string
        static QString SplineToJson(SplinePtr spline)
        {
            QJsonObject curveObj;
            curveObj["radius"] = spline->GetRadius();
            curveObj["ambient"] = spline->GetAmbient();
            curveObj["diffuse"] = spline->GetDiffuse();

            QJsonArray colorArray;
            colorArray.append(spline->GetColor().x());
            colorArray.append(spline->GetColor().y());
            colorArray.append(spline->GetColor().z());
            colorArray.append(spline->GetColor().w());
            curveObj["color"] = colorArray;

            QJsonArray knotsArray;
            for (const auto& knot : spline->GetKnots())
            {
                QJsonObject knotObj;
                knotObj["x"] = knot->GetPosition().x();
                knotObj["y"] = knot->GetPosition().y();
                knotObj["z"] = knot->GetPosition().z();
                knotsArray.append(knotObj);
            }
            curveObj["knots"] = knotsArray;

            QJsonDocument doc(curveObj);
            return QString(doc.toJson(QJsonDocument::Compact));
        }
    };
}
