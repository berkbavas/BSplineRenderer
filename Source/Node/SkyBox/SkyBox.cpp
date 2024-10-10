#include "SkyBox.h"

#include "Util/Logger.h"

#include <QImage>

BSplineRenderer::SkyBox::SkyBox(const QString& folder, const QString& extension)
{
    initializeOpenGLFunctions();

    mTexture;
    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture);

    QStringList paths;
    paths << folder + "/" + "px" + extension;
    paths << folder + "/" + "nx" + extension;
    paths << folder + "/" + "py" + extension;
    paths << folder + "/" + "ny" + extension;
    paths << folder + "/" + "pz" + extension;
    paths << folder + "/" + "nz" + extension;

    QVector<int> targets;

    targets << GL_TEXTURE_CUBE_MAP_POSITIVE_X;
    targets << GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
    targets << GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
    targets << GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
    targets << GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
    targets << GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;

    for (unsigned int i = 0; i < 6; i++)
    {
        LOG_DEBUG("SkyBox::SkyBox: Loading texture from '{}'", paths[i].toStdString());

        QImage image = QImage(paths[i]).mirrored().convertToFormat(QImage::Format_RGBA8888);

        if (image.isNull())
        {
            BR_EXIT_FAILURE("SkyBox::SkyBox: Image is null.");
        }

        glTexImage2D(targets[i], 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    LOG_DEBUG("SkyBox::Initialize: Constructing OpenGL stuff for SkyBox");

    glGenVertexArrays(1, &mVertexArray);
    glBindVertexArray(mVertexArray);

    glGenBuffers(1, &mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), &VERTICES, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    if (mVertexArray == 0 || mVertexBuffer == 0)
    {
        BR_EXIT_FAILURE("SkyBox::Initialize: OpenGL handle(s) could not be created!");
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    LOG_DEBUG("SkyBox::Initialize: OpenGL stuff has been constructed.");
}

BSplineRenderer::SkyBox::~SkyBox()
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

void BSplineRenderer::SkyBox::Render()
{
    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(mVertexArray);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glEnable(GL_DEPTH_TEST);
}

const float BSplineRenderer::SkyBox::VERTICES[] = {
    -1.0f, 1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    -1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f
};
