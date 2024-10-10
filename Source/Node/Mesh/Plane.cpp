#include "Plane.h"

#include "Util/Logger.h"

#include <QVector3D>

BSplineRenderer::Plane::Plane(float size)
{
    LOG_DEBUG("Plane::Plane: Constructing OpenGL stuff for Plane.");

    mVertices << Vertex{ QVector3D(-size, 0, size), QVector3D(0, 1, 0), QVector2D(0, 0) };
    mVertices << Vertex{ QVector3D(-size, 0, -size), QVector3D(0, 1, 0), QVector2D(0, 1) };
    mVertices << Vertex{ QVector3D(size, 0, -size), QVector3D(0, 1, 0), QVector2D(1, 1) };

    mVertices << Vertex{ QVector3D(-size, 0, size), QVector3D(0, 1, 0), QVector2D(0, 0) };
    mVertices << Vertex{ QVector3D(size, 0, -size), QVector3D(0, 1, 0), QVector2D(0, 1) };
    mVertices << Vertex{ QVector3D(size, 0, size), QVector3D(0, 1, 0), QVector2D(1, 0) };

    initializeOpenGLFunctions();

    glGenVertexArrays(1, &mVertexArray);
    glBindVertexArray(mVertexArray);

    glGenBuffers(1, &mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.constData(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) (offsetof(Vertex, normal)));
    glEnableVertexAttribArray(1);

    if (mVertexArray == 0 || mVertexBuffer == 0)
    {
        BR_EXIT_FAILURE("Plane::Plane: OpenGL handle(s) could not be created!");
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    LOG_DEBUG("Plane::Plane: OpenGL stuff for Plane has been constructed.");
}

BSplineRenderer::Plane::~Plane()
{
    if (mVertexArray)
    {
        glDeleteVertexArrays(1, &mVertexArray);
        mVertexArray = 0;
    }

    if (mVertexBuffer)
    {
        glDeleteBuffers(1, &mVertexBuffer);
        mVertexBuffer = 0;
    }
}

void BSplineRenderer::Plane::Render()
{
    glBindVertexArray(mVertexArray);
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
    glBindVertexArray(0);
}
