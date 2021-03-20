cmake_minimum_required(VERSION 3.5)
project(QuasarBell VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Help to find glfw3 config on Windows (May need to build it from source)
set(glfw3_DIR "C:/Program Files (x86)/glfw/lib/cmake/glfw3")

find_package(glfw3 3.3 REQUIRED CONFIG)
find_package(OpenAL REQUIRED)

add_executable(
    ${PROJECT_NAME}
    src/App.hpp
    src/SFXBasicEditor.hpp
    src/NodalEditor.hpp
    src/Synth.hpp
    src/gui/Gui.hpp
    src/gui/UserFileInput.hpp
    src/gui/InputEffect.hpp
    src/gui/Keyboard.hpp
    src/gui/Math.hpp
    src/gui/PostEffect.hpp
    src/gui/SignalEdit.hpp
    src/gui/imgui_impl_glfw.h
    src/gui/imgui_impl_opengl3.h
    src/io/json/json.hpp
    src/io/json/jsonrw.hpp
    src/io/FileIO.hpp
    src/io/SignalRW.hpp
    src/io/InputRW.hpp
    src/io/PostRW.hpp
    src/io/RwHelpers.hpp
    src/io/FileExplorer.hpp
    src/render/AudioRenderer.hpp
    src/render/Error.hpp
    src/render/Format.hpp
    src/signal/LinearSamplers.hpp
    src/signal/Modulator.hpp
    src/signal/PcmData.hpp
    src/signal/Signal.hpp
    src/signal/WavExporter.hpp
    src/Core/Vec2.h
    src/Font/DefaultFontMaker.h
    src/Font/FontMaker.h
    src/Font/GlyphRenderer.h
    src/GL/GlError.h
    src/GL/GlQuadShader.h
    src/GL/GlSpriteShader.h
    src/GL/GlQuad.h
    src/GL/GlSprite.h
    src/GL/RenderableTexture.h
    src/GL/RenderInterface.h
    src/UI/UiEvent.h
    src/UI/UiSystem.h
    src/UI/UiRect.h
    src/UI/UiNode.h
    src/UI/UiText.h
    src/UI/UiInput.h
    src/UI/UiContainer.h
    src/UI/UiList.h
    src/UI/UiButton.h
    src/UI/UiSlider.h
    src/UI/UiFrame.h
    src/UI/UiForm.h
    src/UI/UiPin.h
    src/UI/UiConnections.h
    
    src/main.cpp
    src/SFXBasicEditor.cpp
    src/NodalEditor.cpp
    src/Synth.cpp
    src/gui/Gui.cpp
    src/gui/UserFileInput.cpp
    src/gui/InputEffect.cpp
    src/gui/Keyboard.cpp
    src/gui/PostEffect.cpp
    src/gui/SignalEdit.cpp
    src/gui/imgui_impl_glfw.cpp
    src/gui/imgui_impl_opengl3.cpp
    src/io/json/json.cpp
    src/io/json/jsonrw.cpp
    src/io/FileIO.cpp
    src/io/SignalRW.cpp
    src/io/InputRW.cpp
    src/io/PostRW.cpp
    src/io/RwHelpers.cpp
    src/io/FileExplorer.cpp
    src/render/AudioRenderer.cpp
    src/signal/LinearSamplers.cpp
    src/signal/Modulator.cpp
    src/signal/PcmData.cpp
    src/signal/Signal.cpp
    src/signal/WavExporter.cpp
    src/Core/Vec2.cpp
    src/Font/DefaultFontMaker.cpp
    src/Font/FontMaker.cpp
    src/Font/GlyphRenderer.cpp
    src/GL/GlQuadShader.cpp
    src/GL/GlSpriteShader.cpp
    src/GL/GlQuad.cpp
    src/GL/GlSprite.cpp
    src/GL/RenderableTexture.cpp
    src/GL/RenderInterfaceGlfw.cpp
    src/UI/UiSystem.cpp
    src/UI/UiRect.cpp
    src/UI/UiNode.cpp
    src/UI/UiText.cpp
    src/UI/UiInput.cpp
    src/UI/UiContainer.cpp
    src/UI/UiList.cpp
    src/UI/UiButton.cpp
    src/UI/UiSlider.cpp
    src/UI/UiFrame.cpp
    src/UI/UiForm.cpp
    src/UI/UiPin.cpp
    src/UI/UiConnections.cpp
    
    thirdparty/imgui/imconfig.h
    thirdparty/imgui/imgui_demo.cpp
    thirdparty/imgui/imgui_draw.cpp
    thirdparty/imgui/imgui_widgets.cpp
    thirdparty/imgui/imgui.cpp
    thirdparty/imgui/imgui.h
    
    # this one is included in src/gui/Gui.cpp
    # thirdparty/glad/src/glad.c
    )

target_include_directories(${PROJECT_NAME} PRIVATE src)

target_include_directories(${PROJECT_NAME} PRIVATE thirdparty/imgui)
target_include_directories(${PROJECT_NAME} PRIVATE thirdparty/glad/include/)
target_include_directories(${PROJECT_NAME} PRIVATE ${OPENAL_INCLUDE_DIR})
    
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC ${OPENAL_LIBRARY})
target_link_libraries(${PROJECT_NAME} PUBLIC ${CMAKE_DL_LIBS})

add_definitions("-DIMGUI_IMPL_OPENGL_LOADER_GLAD")

