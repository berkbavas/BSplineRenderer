#pragma once

#include "Node/Mesh/Mesh.h"
#include "Node/Mesh/Texture.h"
#include "Util/Macros.h"

#include <QObject>
#include <QOpenGLExtraFunctions>
#include <QString>

namespace BSplineRenderer
{
    class SkyBox : public Mesh
    {
      public:
        SkyBox(const QString& folder, const QString& extension);
        ~SkyBox();

        void Render() override;

      private:
        GLuint mTexture;
        GLuint mVertexArray{ 0 };
        GLuint mVertexBuffer{ 0 };

        DEFINE_MEMBER(float, Brightness, 0.5f);

        static const float VERTICES[108];
    };

    using SkyBoxPtr = std::shared_ptr<SkyBox>;
}