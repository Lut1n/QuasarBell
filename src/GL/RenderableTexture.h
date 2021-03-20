#ifndef GL_RENDERABLE_TEXTURE_H
#define GL_RENDERABLE_TEXTURE_H

struct RenderableTexture
{
    unsigned fbo = 0;
    unsigned tex = 0;
    unsigned width = 0;
    unsigned height = 0;
    
    RenderableTexture(unsigned w, unsigned h);
    ~RenderableTexture();
    
    void clear(float* color);
    
    void bind();
    void bindAsTexture();
    
    static void bindWindow();
};

#endif // GL_RENDERABLE_TEXTURE_H
