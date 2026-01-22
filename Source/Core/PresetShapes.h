#pragma once

#include "Curve/Spline.h"

#include <QVector3D>
#include <cmath>

namespace BSplineRenderer
{
    class PresetShapes
    {
      public:
        // Create circle shape
        static SplinePtr CreateCircle(float radius = 5.0f, int numPoints = 12, const QVector3D& center = QVector3D(0, 0, 0))
        {
            auto spline = std::make_shared<Spline>();

            for (int i = 0; i < numPoints; ++i)
            {
                float angle = 2.0f * M_PI * i / numPoints;
                float x = center.x() + radius * std::cos(angle);
                float y = center.y() + radius * std::sin(angle);
                float z = center.z();
                spline->AddKnot(x, y, z);
            }

            // Close the shape by adding the first point again
            float x = center.x() + radius;
            float y = center.y();
            float z = center.z();
            spline->AddKnot(x, y, z);

            spline->SetColor(QVector4D(0.2f, 0.8f, 0.4f, 1.0f));
            return spline;
        }

        // Create spiral shape
        static SplinePtr CreateSpiral(float radius = 3.0f, float height = 10.0f, int turns = 3, int pointsPerTurn = 8,
                                       const QVector3D& center = QVector3D(0, 0, 0))
        {
            auto spline = std::make_shared<Spline>();
            int totalPoints = turns * pointsPerTurn;

            for (int i = 0; i <= totalPoints; ++i)
            {
                float t = static_cast<float>(i) / totalPoints;
                float angle = 2.0f * M_PI * turns * t;
                float x = center.x() + radius * std::cos(angle);
                float y = center.y() + height * t - height / 2.0f;
                float z = center.z() + radius * std::sin(angle);
                spline->AddKnot(x, y, z);
            }

            spline->SetColor(QVector4D(0.8f, 0.4f, 0.2f, 1.0f));
            return spline;
        }

        // Create heart shape
        static SplinePtr CreateHeart(float scale = 3.0f, int numPoints = 24, const QVector3D& center = QVector3D(0, 0, 0))
        {
            auto spline = std::make_shared<Spline>();

            for (int i = 0; i <= numPoints; ++i)
            {
                float t = 2.0f * M_PI * i / numPoints;
                // Heart parametric equation
                float x = scale * 16.0f * std::pow(std::sin(t), 3) / 16.0f;
                float y = scale * (13.0f * std::cos(t) - 5.0f * std::cos(2 * t) - 2.0f * std::cos(3 * t) - std::cos(4 * t)) / 16.0f;
                float z = 0.0f;

                spline->AddKnot(center.x() + x, center.y() + y, center.z() + z);
            }

            spline->SetColor(QVector4D(0.9f, 0.2f, 0.3f, 1.0f));
            return spline;
        }

        // Create star shape
        static SplinePtr CreateStar(int numPoints = 5, float outerRadius = 5.0f, float innerRadius = 2.5f,
                                     const QVector3D& center = QVector3D(0, 0, 0))
        {
            auto spline = std::make_shared<Spline>();

            for (int i = 0; i <= numPoints * 2; ++i)
            {
                float angle = M_PI * i / numPoints - M_PI / 2.0f;
                float radius = (i % 2 == 0) ? outerRadius : innerRadius;
                float x = center.x() + radius * std::cos(angle);
                float y = center.y() + radius * std::sin(angle);
                float z = center.z();
                spline->AddKnot(x, y, z);
            }

            spline->SetColor(QVector4D(1.0f, 0.85f, 0.0f, 1.0f));
            return spline;
        }

        // Create wave shape
        static SplinePtr CreateWave(float amplitude = 2.0f, float frequency = 2.0f, float length = 20.0f, int numPoints = 32,
                                     const QVector3D& start = QVector3D(-10, 0, 0))
        {
            auto spline = std::make_shared<Spline>();

            for (int i = 0; i <= numPoints; ++i)
            {
                float t = static_cast<float>(i) / numPoints;
                float x = start.x() + length * t;
                float y = start.y() + amplitude * std::sin(2.0f * M_PI * frequency * t);
                float z = start.z();
                spline->AddKnot(x, y, z);
            }

            spline->SetColor(QVector4D(0.2f, 0.6f, 0.9f, 1.0f));
            return spline;
        }

        // Create Lissajous curve
        static SplinePtr CreateLissajous(float a = 3.0f, float b = 2.0f, float A = 5.0f, float B = 5.0f, float delta = M_PI / 2.0f,
                                          int numPoints = 64, const QVector3D& center = QVector3D(0, 0, 0))
        {
            auto spline = std::make_shared<Spline>();

            for (int i = 0; i <= numPoints; ++i)
            {
                float t = 2.0f * M_PI * i / numPoints;
                float x = center.x() + A * std::sin(a * t + delta);
                float y = center.y() + B * std::sin(b * t);
                float z = center.z();
                spline->AddKnot(x, y, z);
            }

            spline->SetColor(QVector4D(0.7f, 0.3f, 0.9f, 1.0f));
            return spline;
        }

        // Create 3D Helix
        static SplinePtr CreateHelix(float radius = 3.0f, float pitch = 2.0f, int turns = 4, int pointsPerTurn = 12,
                                      const QVector3D& center = QVector3D(0, 0, 0))
        {
            auto spline = std::make_shared<Spline>();
            int totalPoints = turns * pointsPerTurn;

            for (int i = 0; i <= totalPoints; ++i)
            {
                float t = static_cast<float>(i) / pointsPerTurn;
                float angle = 2.0f * M_PI * t;
                float x = center.x() + radius * std::cos(angle);
                float y = center.y() + pitch * t;
                float z = center.z() + radius * std::sin(angle);
                spline->AddKnot(x, y, z);
            }

            spline->SetColor(QVector4D(0.4f, 0.8f, 0.8f, 1.0f));
            return spline;
        }

        // Create Trefoil Knot
        static SplinePtr CreateTrefoilKnot(float scale = 3.0f, int numPoints = 96, const QVector3D& center = QVector3D(0, 0, 0))
        {
            auto spline = std::make_shared<Spline>();

            for (int i = 0; i <= numPoints; ++i)
            {
                float t = 2.0f * M_PI * i / numPoints;
                float x = center.x() + scale * (std::sin(t) + 2.0f * std::sin(2.0f * t));
                float y = center.y() + scale * (std::cos(t) - 2.0f * std::cos(2.0f * t));
                float z = center.z() + scale * (-std::sin(3.0f * t));
                spline->AddKnot(x, y, z);
            }

            spline->SetColor(QVector4D(0.9f, 0.5f, 0.1f, 1.0f));
            return spline;
        }

        // Create Butterfly curve
        static SplinePtr CreateButterfly(float scale = 2.0f, int numPoints = 128, const QVector3D& center = QVector3D(0, 0, 0))
        {
            auto spline = std::make_shared<Spline>();

            for (int i = 0; i <= numPoints; ++i)
            {
                float t = 12.0f * M_PI * i / numPoints;
                float r = std::exp(std::cos(t)) - 2.0f * std::cos(4.0f * t) - std::pow(std::sin(t / 12.0f), 5);
                float x = center.x() + scale * r * std::sin(t);
                float y = center.y() + scale * r * std::cos(t);
                float z = center.z();
                spline->AddKnot(x, y, z);
            }

            spline->SetColor(QVector4D(0.95f, 0.6f, 0.8f, 1.0f));
            return spline;
        }
    };
}
