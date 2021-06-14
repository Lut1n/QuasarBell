#include "Graphics/RenderInterface.h"

//--------------------------------------------------------------
// Append glad source code here
#include "../../thirdparty/glad/src/glad.c"

#include "Graphics/GlError.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <vector>

#include "Ui/UiSystem.h"

unsigned RenderInterface::s_currentTarget = 0;
unsigned RenderInterface::s_color = 0x000000FF;
float RenderInterface::s_thickness = 1.0f;
unsigned RenderInterface::s_precision = 1;

#include "Graphics/RenderableTexture.h"
#include "Graphics/GlQuad.h"
#include "Graphics/GlSprite.h"
#include "Graphics/GlQuadShader.h"
#include "Graphics/GlSpriteShader.h"
#include "Graphics/CustomProgram.h"

#include "imgui.h"
#include "Graphics/imgui_impl_glfw.h"
#include "Graphics/imgui_impl_opengl3.h"
#include "App/AppInterface.hpp"

struct GLfwTarget
{
    bool isWindow = false;
    unsigned width = 0;
    unsigned height = 0;
    GLFWwindow* window = nullptr;
    RenderableTexture* texture = nullptr;
};

static std::vector<GLfwTarget> s_targets;
static std::vector<GlCustomProgram*> s_CustomPrograms;

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void window_size_callback(GLFWwindow* window, int width, int height)
{
    for(auto& target : s_targets)
    {
        if(!target.isWindow) continue;
        target.width = width;
        target.height = height;
    }
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
 
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    
    UiEvent evn;
    evn.type = UiEvent::TYPE_KEY;
    if(action == GLFW_PRESS)
        evn.state = UiEvent::STATE_DOWN;
    else if(action == GLFW_RELEASE)
        evn.state = UiEvent::STATE_RELEASED;
    evn.input = key;
    UiSystem::instance()->onEvent(evn);
}

static void character_callback(GLFWwindow* window, unsigned int codepoint)
{
    UiEvent evn;
    evn.type = UiEvent::TYPE_CHAR;
    evn.input = codepoint;
    UiSystem::instance()->onEvent(evn);
}

static vec2 lastCursorPosition;

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    lastCursorPosition = vec2((float) xpos, (float) ypos);
    UiEvent evn;
    evn.type = UiEvent::TYPE_MOUSE_MOVE;
    evn.position = lastCursorPosition;
    UiSystem::instance()->onEvent(evn);
}


static UiEvent::State s_buttonStates[3] = {UiEvent::STATE_RELEASED,UiEvent::STATE_RELEASED,UiEvent::STATE_RELEASED};

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    // imgui has priority
    ImGuiHoveredFlags flags = ImGuiHoveredFlags_AnyWindow
                            | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem
                            | ImGuiHoveredFlags_AllowWhenBlockedByPopup;

    if(ImGui::IsWindowHovered(flags)) return;

    UiEvent evn;
    evn.type = UiEvent::TYPE_MOUSE_BUTTON;
    
    if (button == GLFW_MOUSE_BUTTON_LEFT)
        evn.input = UiEvent::INPUT_MOUSE_1;
    else if (button == GLFW_MOUSE_BUTTON_RIGHT)
        evn.input = UiEvent::INPUT_MOUSE_2;
    /*else if (button == GLFW_MOUSE_BUTTON_RIGHT)
        evn.input = UiEvent::INPUT_BUTTON_3;*/

    if(action == GLFW_PRESS)
        s_buttonStates[evn.input] = UiEvent::STATE_DOWN;
    else if(action == GLFW_RELEASE)
        s_buttonStates[evn.input] = UiEvent::STATE_RELEASED;
        
    evn.position = lastCursorPosition;
    evn.state = s_buttonStates[evn.input];
    
    if(action == GLFW_PRESS || action == GLFW_RELEASE)
    {
        UiEvent defocus;
        defocus.type = UiEvent::TYPE_DEFOCUS;
        defocus.position = lastCursorPosition;
        UiSystem::instance()->onEvent(defocus);
    }
    UiSystem::instance()->onEvent(evn);
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
}


static GlQuad* s_quad = nullptr;
static GlSprite* s_sprite = nullptr;
static GlSpriteShader* s_spriteShader = nullptr;
static GlQuadShader* s_quadShader = nullptr;
static GlLineShader* s_lineShader = nullptr;

void RenderInterface::deleteTarget(unsigned id)
{
    if(!s_targets[id].isWindow)
    {
        delete s_targets[id].texture;
        s_targets[id].texture = nullptr;
        GL_CHECKERROR("delete target");
    }
}

unsigned RenderInterface::createTarget(unsigned width, unsigned height, bool win, const std::string& title)
{
    GLfwTarget target;
    if(win)
    {
        glfwSetErrorCallback(error_callback);
    
        if (!glfwInit())
            exit(EXIT_FAILURE);
    
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    
        GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
    
        glfwSetWindowSizeCallback(window, window_size_callback);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetKeyCallback(window, key_callback);
        glfwSetCharCallback(window, character_callback);
        glfwSetCursorPosCallback(window, cursor_position_callback);
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        glfwSetScrollCallback(window, scroll_callback);
    
        glfwMakeContextCurrent(window);
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        glfwSwapInterval(1);
        target.width = width;
        target.height = height;
        target.window = window;
        target.isWindow = true;
        
        s_quad = new GlQuad();
        s_sprite = new GlSprite();
        s_spriteShader = new GlSpriteShader();
        s_quadShader = new GlQuadShader();
        s_lineShader = new GlLineShader();
        GL_CHECKERROR("create window");
        
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();
        io.WantCaptureMouse = false;

        // Setup Platform/Renderer bindings
        const char* glsl_version = "#version 130";
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
    }
    else
    {
        target.width = width;
        target.height = height;
        target.texture = new RenderableTexture(width, height);
        target.isWindow = false;
        
        GL_CHECKERROR("create render texture");
    }
    
    unsigned idx = (unsigned) s_targets.size();
    s_targets.push_back(target);
    return idx;
}

vec2 RenderInterface::getTargetSize(unsigned id)
{
    return vec2(s_targets[id].width, s_targets[id].height);
}

vec2 RenderInterface::getCurrentTargetSize()
{
    return getTargetSize(s_currentTarget);
}
    
void RenderInterface::setTarget(unsigned target)
{
    s_currentTarget = target;
    if(s_targets[s_currentTarget].isWindow)
        RenderableTexture::bindWindow();
    else
       s_targets[s_currentTarget].texture->bind();
    GL_CHECKERROR("set target");
}

void* RenderInterface::getTargetResource(unsigned target)
{
    if(!s_targets[target].isWindow)
        return reinterpret_cast<void*>(s_targets[target].texture->tex);
    return (void*)0;
}

qb::ImageData RenderInterface::downloadTargetImage(unsigned id)
{
    qb::ImageData ret;
    if(!s_targets[id].isWindow)
        ret = s_targets[id].texture->getImage();
    return ret;
}

float comp(unsigned color, int c)
{
    unsigned r = (color & 0xFF000000) >> 24;
    unsigned g = (color & 0x00FF0000) >> 16;
    unsigned b = (color & 0x0000FF00) >> 8;
    unsigned a = (color & 0x000000FF);
    
    if(c==0) return (float)r/255.0f;
    if(c==1) return (float)g/255.0f;
    if(c==2) return (float)b/255.0f;
    if(c==3) return (float)a/255.0f;
    return 0.0f;
}

void RenderInterface::clear(unsigned color)
{
    glViewport(0, 0, s_targets[s_currentTarget].width, s_targets[s_currentTarget].height);
    
    if(s_targets[s_currentTarget].isWindow)
    {
        glClearColor(comp(color,0),comp(color,1),comp(color,2),comp(color,3));
        glClear(GL_COLOR_BUFFER_BIT);
        GL_CHECKERROR("clear");
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }
    else
    {
        float colorf[4] = {comp(color,0),comp(color,1),comp(color,2),comp(color,3)};
        s_targets[s_currentTarget].texture->clear(colorf);
    }
}

void RenderInterface::setColor(unsigned color)
{
    s_color = color;
}

void RenderInterface::setThickness(float th)
{
    s_thickness = th;
}

void RenderInterface::setPrecision(unsigned precision)
{
    s_precision = precision;
}

void RenderInterface::line(float x1, float y1, float x2, float y2, bool smooth)
{
    line(vec2(x1,y1), vec2(x2,y2), smooth);
}

void RenderInterface::line(const vec2& p1, const vec2& p2, bool smooth)
{
    vec2 vp = vec2((float) s_targets[s_currentTarget].width, (float) s_targets[s_currentTarget].height);
    vec2 d = p2-p1;

    vec2 p1s = p1/vp * 2.0 - vec2(1.0,1.0);
    p1s = p1s * vec2(1.0,-1.0);
    vec2 p2s = p2/vp * 2.0 - vec2(1.0,1.0);
    p2s = p2s * vec2(1.0,-1.0);
    
    float colorf[4] = {comp(s_color,0), comp(s_color,1), comp(s_color,2), comp(s_color,3)};
    float p1p2f[4] = {p1s.x,p1s.y,p2s.x,p2s.y};
    float thicknessf[4] = {s_thickness/vp.x,s_thickness/vp.y,s_thickness/vp.x,s_thickness/vp.y};
    
    if(smooth)
        s_lineShader->setup(colorf, p1p2f, thicknessf);
    else
        s_quadShader->setup(colorf);
    GL_CHECKERROR("line::setup --- line::update");
    s_quad->update(p1, vec2(d.length(), s_thickness), vec2(0.0f, s_thickness*0.5f), d.angle(), vp);
    GL_CHECKERROR("line::update --- bindAttrib");
    if(smooth)
        s_lineShader->bindAttributes();
    else
        s_quadShader->bindAttributes();
    GL_CHECKERROR("line::bindAttrib --- draw");
    s_quad->draw();
    if(smooth)
        s_lineShader->unbindAttributes();
    else
        s_quadShader->unbindAttributes();
}
void RenderInterface::dot(const vec2& p)
{
    vec2 hThX(s_thickness * 0.5f, 0.f);
    line(p - hThX, p + hThX);
}
void RenderInterface::arc(const vec2& ctr, float r, float a1 , float a2)
{
    int c = 2 + s_precision;
    float la = a1;
    
    for(int i=1;i<c;++i)
    {
        float a = mix(a1, a2, i/(float)(c-1));
        RenderInterface::line(ctr+vec2::fromAngle(la)*r, ctr+vec2::fromAngle(a)*r);
        la = a;
    }
}
void RenderInterface::rect(const vec2& tl, const vec2& br)
{
    line(tl, vec2(br.x,tl.y));
    line(tl, vec2(tl.x,br.y));
    line(vec2(tl.x,br.y), br);
    line(vec2(br.x,tl.y), br);
}
void RenderInterface::fill(const vec2& tl, const vec2& br)
{
    vec2 vp = vec2((float) s_targets[s_currentTarget].width, (float) s_targets[s_currentTarget].height);
    float colorf[4] = {comp(s_color,0), comp(s_color,1), comp(s_color,2), comp(s_color,3)};
    s_quadShader->setup(colorf);
    GL_CHECKERROR("quad::setup --- quad::update");
    vec2 size = br-tl;
    vec2 orig = vec2(0.0, 0.0) * size;
    s_quad->update(tl, size, orig, 0.0, vp);
    GL_CHECKERROR("quad::update --- bindAttrib");
    s_quadShader->bindAttributes();
    s_quad->draw();
    s_quadShader->unbindAttributes();
}
void RenderInterface::copy(unsigned srcTarget, const Rect& srcRect, const Rect& dstRect, bool ySrcInverted, bool yDstInverted)
{
    if(s_targets[srcTarget].isWindow) return;
    
    vec2 srcSize = vec2((float) s_targets[srcTarget].width, (float) s_targets[srcTarget].height);
    vec2 uv1 = srcRect.p0 / srcSize;
    vec2 uv2 = srcRect.p1 / srcSize;
    
    if (ySrcInverted)
    {
        uv1.y = 1.f - uv1.y;
        uv2.y = 1.f - uv2.y;
    }
    
    vec2 vp = vec2((float) s_targets[s_currentTarget].width, (float) s_targets[s_currentTarget].height);
    float colorf[4] = {comp(s_color,0), comp(s_color,1), comp(s_color,2), comp(s_color,3)};
    s_spriteShader->setup(colorf, 0);
    GL_CHECKERROR("sprite::setup --- sprite::update");
    s_targets[srcTarget].texture->bindAsTexture();
    s_sprite->update(dstRect.p0, dstRect.size(), uv1, uv2, vp, yDstInverted);
    GL_CHECKERROR("sprite::update --- sprite::bindAttrib");
    s_spriteShader->bindAttributes();
    GL_CHECKERROR("sprite::bindAttrib --- sprite::draw");
    s_sprite->draw();
    s_spriteShader->unbindAttributes();
}
bool RenderInterface::begin()
{
    if(!s_targets[s_currentTarget].isWindow)
        return true;
    
    GLFWwindow* window = s_targets[s_currentTarget].window;
    if (!glfwWindowShouldClose(window))
    {
        return true;
    }
    return false;
}
void RenderInterface::end()
{
    if(s_targets[s_currentTarget].isWindow)
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        GLFWwindow* window = s_targets[s_currentTarget].window;
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
void RenderInterface::debugSave(const std::string& filename)
{
}

static bool firstUpdate = true;
static double startTime = 0.0;
static UiTime uiTime;

void RenderInterface::updateTime()
{
    if(firstUpdate)
    {
        startTime = glfwGetTime();
        firstUpdate = false;
    }
    double newFromStart = glfwGetTime() - startTime;
    uiTime.delta = newFromStart - uiTime.fromStart;
    uiTime.fromStart = newFromStart;
    UiSystem::instance()->update(uiTime);
}

double RenderInterface::getTime()
{
    return glfwGetTime();
}

void RenderInterface::shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

unsigned RenderInterface::createCustomProgram()
{
    unsigned id = s_CustomPrograms.size();
    s_CustomPrograms.push_back(new GlCustomProgram());
    GL_CHECKERROR("customProgram: create");
    return id;
}
void RenderInterface::destroyCustomProgram(unsigned id)
{
    delete s_CustomPrograms[id];
    GL_CHECKERROR("customProgram: delete");
    s_CustomPrograms[id] = nullptr;
}

void RenderInterface::updateCustomProgram(unsigned customId, const std::string& fragCode, bool bindUv)
{
    s_CustomPrograms[customId]->bindUv = bindUv;
    s_CustomPrograms[customId]->setFragCode(fragCode);
    GL_CHECKERROR("customProgram: update");
}

void RenderInterface::setInputCustomProgram(unsigned customId, size_t uniformId, const vec4& v4)
{
    s_CustomPrograms[customId]->setUniform(uniformId, v4);
}

void RenderInterface::setInputCustomProgram(unsigned customId, size_t uniformId, const Kernel& kernel)
{
    s_CustomPrograms[customId]->setUniform(uniformId, kernel.data);
}

void RenderInterface::setInputFrameCustomProgram(unsigned customId, size_t srcId, size_t textureUnit, size_t uniformId)
{
    s_targets[srcId].texture->bindAsTexture(textureUnit);
    s_CustomPrograms[customId]->setSampler(uniformId, textureUnit);
}

void RenderInterface::applyCustomProgram(unsigned customId, const vec2& tl, const vec2& br)
{
    bool yDstInverted = false;
    vec2 vp = vec2((float) s_targets[s_currentTarget].width, (float) s_targets[s_currentTarget].height);
    s_CustomPrograms[customId]->setup();
    GL_CHECKERROR("custom::setup --- custom::update");
    vec2 size = br-tl;
    s_sprite->update(tl, size, vec2(0.0f,0.0f), vec2(1.0f,1.0f), vp, yDstInverted);
    GL_CHECKERROR("custom::update --- bindAttrib");
    s_CustomPrograms[customId]->bindAttributes();
    GL_CHECKERROR("customProgram: bindAttrib");
    s_sprite->draw();
    GL_CHECKERROR("customProgram: draw");
    s_CustomPrograms[customId]->unbindAttributes();
    GL_CHECKERROR("customProgram: unbindAttrib");
}

void RenderInterface::resetCustomProgram(unsigned customId)
{
    s_CustomPrograms[customId]->bindUv = false;
    s_CustomPrograms[customId]->resetFragCode();
}

void RenderInterface::debugCheck(const char* msg)
{
    GL_CHECKERROR(msg);
}