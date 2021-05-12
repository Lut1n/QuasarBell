#include "Graphics/RenderableTexture.h"

#include "Graphics/GlError.h"

RenderableTexture::RenderableTexture(unsigned w, unsigned h)
    : width(w)
    , height(h)
{
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    GL_CHECKERROR("create frame buffer");

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        // erro
        std::cout << "fbo status is not complete" << std::endl;
        return;
    }
    
    GL_CHECKERROR("renderable texture ctor");
}

RenderableTexture::~RenderableTexture()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &fbo);
}

void RenderableTexture::clear(float* color)
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    GL_CHECKERROR("framebuffer bind");
    // GLenum bufferlist[] = {GL_COLOR_ATTACHMENT0};
    // glDrawBuffers(1, bufferlist);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    GL_CHECKERROR("framebuffer draw buffers");
    // glClearBufferfv(GL_COLOR, GL_DRAW_BUFFER0, color);
    glClearColor(color[0], color[1], color[2], color[3]);
    glClear(GL_COLOR_BUFFER_BIT);
    GL_CHECKERROR("framebuffer clear buffer");
}

void RenderableTexture::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    GL_CHECKERROR("framebuffer bind");
}

void RenderableTexture::bindAsTexture()
{
    glBindTexture(GL_TEXTURE_2D, tex);
    GL_CHECKERROR("texture bind");
}

void RenderableTexture::bindWindow()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
