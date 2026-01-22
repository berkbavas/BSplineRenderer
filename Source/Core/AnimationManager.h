#pragma once

#include "Core/CurveContainer.h"
#include "Curve/Spline.h"

#include <QMatrix4x4>
#include <QVector3D>
#include <cmath>
#include <unordered_map>

namespace BSplineRenderer
{
    enum class AnimationType
    {
        None,
        Rotate,
        Pulse,
        Wave,
        Bounce,
        Spiral
    };

    class AnimationManager
    {
      public:
        static AnimationManager& Instance()
        {
            static AnimationManager instance;
            return instance;
        }

        void Update(float deltaTime, CurveContainer* container)
        {
            if (!mEnabled || mAnimationType == AnimationType::None)
                return;

            mTime += deltaTime * mSpeed;

            for (const auto& spline : container->GetCurves())
            {
                ApplyAnimation(spline);
            }
        }

        void ApplyAnimation(SplinePtr spline)
        {
            switch (mAnimationType)
            {
            case AnimationType::Rotate:
                ApplyRotation(spline);
                break;
            case AnimationType::Pulse:
                ApplyPulse(spline);
                break;
            case AnimationType::Wave:
                ApplyWave(spline);
                break;
            case AnimationType::Bounce:
                ApplyBounce(spline);
                break;
            case AnimationType::Spiral:
                ApplySpiral(spline);
                break;
            default:
                break;
            }
        }

        void SetEnabled(bool enabled) { mEnabled = enabled; }
        bool IsEnabled() const { return mEnabled; }

        void SetAnimationType(AnimationType type) { mAnimationType = type; }
        AnimationType GetAnimationType() const { return mAnimationType; }

        void SetSpeed(float speed) { mSpeed = speed; }
        float GetSpeed() const { return mSpeed; }

        void SetAmplitude(float amplitude) { mAmplitude = amplitude; }
        float GetAmplitude() const { return mAmplitude; }

        void Reset()
        {
            mTime = 0.0f;
            // Restore original positions
            for (auto& [spline, positions] : mOriginalPositions)
            {
                const auto& knots = spline->GetKnots();
                for (int i = 0; i < knots.size() && i < positions.size(); ++i)
                {
                    knots[i]->SetPosition(positions[i]);
                }
                spline->MakeDirty();
            }
        }

        void SaveOriginalPositions(CurveContainer* container)
        {
            mOriginalPositions.clear();
            for (const auto& spline : container->GetCurves())
            {
                QVector<QVector3D> positions;
                for (const auto& knot : spline->GetKnots())
                {
                    positions.append(knot->GetPosition());
                }
                mOriginalPositions[spline] = positions;
            }
        }

        const char* GetAnimationTypeName(AnimationType type) const
        {
            switch (type)
            {
            case AnimationType::None:
                return "None";
            case AnimationType::Rotate:
                return "Rotate";
            case AnimationType::Pulse:
                return "Pulse";
            case AnimationType::Wave:
                return "Wave";
            case AnimationType::Bounce:
                return "Bounce";
            case AnimationType::Spiral:
                return "Spiral";
            default:
                return "Unknown";
            }
        }

      private:
        AnimationManager() = default;

        void ApplyRotation(SplinePtr spline)
        {
            if (!mOriginalPositions.contains(spline))
                return;

            const auto& originalPositions = mOriginalPositions[spline];
            const auto& knots = spline->GetKnots();

            QMatrix4x4 rotation;
            rotation.rotate(mTime * 30.0f, 0, 1, 0); // Rotate around Y axis

            for (int i = 0; i < knots.size() && i < originalPositions.size(); ++i)
            {
                QVector3D newPos = rotation.map(originalPositions[i]);
                knots[i]->SetPosition(newPos);
            }
            spline->MakeDirty();
        }

        void ApplyPulse(SplinePtr spline)
        {
            if (!mOriginalPositions.contains(spline))
                return;

            const auto& originalPositions = mOriginalPositions[spline];
            const auto& knots = spline->GetKnots();

            float scale = 1.0f + mAmplitude * 0.2f * std::sin(mTime * 2.0f);

            for (int i = 0; i < knots.size() && i < originalPositions.size(); ++i)
            {
                QVector3D newPos = originalPositions[i] * scale;
                knots[i]->SetPosition(newPos);
            }
            spline->MakeDirty();
        }

        void ApplyWave(SplinePtr spline)
        {
            if (!mOriginalPositions.contains(spline))
                return;

            const auto& originalPositions = mOriginalPositions[spline];
            const auto& knots = spline->GetKnots();

            for (int i = 0; i < knots.size() && i < originalPositions.size(); ++i)
            {
                float offset = mAmplitude * std::sin(mTime * 2.0f + i * 0.5f);
                QVector3D newPos = originalPositions[i] + QVector3D(0, offset, 0);
                knots[i]->SetPosition(newPos);
            }
            spline->MakeDirty();
        }

        void ApplyBounce(SplinePtr spline)
        {
            if (!mOriginalPositions.contains(spline))
                return;

            const auto& originalPositions = mOriginalPositions[spline];
            const auto& knots = spline->GetKnots();

            float bounce = mAmplitude * std::abs(std::sin(mTime * 3.0f));

            for (int i = 0; i < knots.size() && i < originalPositions.size(); ++i)
            {
                QVector3D newPos = originalPositions[i] + QVector3D(0, bounce, 0);
                knots[i]->SetPosition(newPos);
            }
            spline->MakeDirty();
        }

        void ApplySpiral(SplinePtr spline)
        {
            if (!mOriginalPositions.contains(spline))
                return;

            const auto& originalPositions = mOriginalPositions[spline];
            const auto& knots = spline->GetKnots();

            for (int i = 0; i < knots.size() && i < originalPositions.size(); ++i)
            {
                float angle = mTime + i * 0.3f;
                float spiralOffset = mAmplitude * 0.5f * std::sin(angle);
                QMatrix4x4 rotation;
                rotation.rotate(mTime * 20.0f + i * 10.0f, 0, 1, 0);

                QVector3D rotatedPos = rotation.map(originalPositions[i]);
                rotatedPos += QVector3D(0, spiralOffset, 0);
                knots[i]->SetPosition(rotatedPos);
            }
            spline->MakeDirty();
        }

        bool mEnabled{ false };
        AnimationType mAnimationType{ AnimationType::None };
        float mSpeed{ 1.0f };
        float mAmplitude{ 1.0f };
        float mTime{ 0.0f };

        std::map<SplinePtr, QVector<QVector3D>> mOriginalPositions;
    };
}
