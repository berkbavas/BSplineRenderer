#pragma once

#include <QOpenGLExtraFunctions>

namespace BSplineRenderer
{
    class Quad : protected QOpenGLExtraFunctions
    {
      public:
        Quad();

        void Render();

      private:
        GLuint mVertexArray;
        GLuint mVertexBuffer;
    };
}
