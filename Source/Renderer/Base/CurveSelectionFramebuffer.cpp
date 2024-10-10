#include "CurveSelectionFramebuffer.h"

#include "Util/Logger.h"

BSplineRenderer::CurveSelectionFramebuffer::CurveSelectionFramebuffer(int width, int height)
    : mWidth(width)
    , mHeight(height)
{
    initializeOpenGLFunctions();

    // Generate framebuffer
    glGenFramebuffers(1, &mFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);

    // Generate texture
    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32I, mWidth, mHeight, 0, GL_RGBA_INTEGER, GL_INT, NULL);

    // Bind texture to the framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        BR_EXIT_FAILURE("CurveSelectionFramebuffer::CurveSelectionFramebuffer: Could not create framebuffer!");
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

BSplineRenderer::CurveSelectionFramebuffer::~CurveSelectionFramebuffer()
{
    if (mFramebuffer != 0)
    {
        glDeleteFramebuffers(1, &mFramebuffer);
    }

    if (mTexture != 0)
    {
        glDeleteTextures(1, &mTexture);
    }
}

void BSplineRenderer::CurveSelectionFramebuffer::Clear()
{
    glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);
    glViewport(0, 0, mWidth, mHeight);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void BSplineRenderer::CurveSelectionFramebuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);
}

BSplineRenderer::CurveQueryInfo BSplineRenderer::CurveSelectionFramebuffer::Query(const QPoint& queryPoint)
{
    CurveQueryInfo info;
    Bind();
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glReadPixels(queryPoint.x(), mHeight - queryPoint.y(), 1, 1, GL_RGBA_INTEGER, GL_INT, &info);

    return info;
}
