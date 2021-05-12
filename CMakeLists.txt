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
    src/App/UiSignalNodeBoard.hpp
    src/App/AppInterface.hpp
    src/App/UserFileInput.hpp
    src/App/SignalNode.hpp
    src/App/TypedSignalNode.hpp
    src/Json/json.hpp
    src/Json/jsonrw.hpp
    src/ReaderWriter/FileIO.hpp
    src/ReaderWriter/RwHelpers.hpp
    src/ReaderWriter/FileExplorer.hpp
    src/ReaderWriter/SignalOperationsRW.hpp
    src/Audio/AudioRenderer.hpp
    src/Audio/Error.hpp
    src/Audio/Format.hpp
    src/Audio/PcmData.hpp
    src/Audio/Signal.hpp
    src/Audio/WavExporter.hpp
    src/SignalOperation/OperationType.hpp
    src/SignalOperation/SignalOperation.hpp
    src/SignalOperation/Operations.hpp
    src/SignalOperation/Oscillator.hpp
    src/SignalOperation/Quantizer.hpp
    src/SignalOperation/MixOperation.hpp
    src/SignalOperation/EnvelopOperation.hpp
    src/SignalOperation/StepOperation.hpp
    src/SignalOperation/FilterOperation.hpp
    src/SignalOperation/PitchSelector.hpp
    src/SignalOperation/Waveform.hpp
    src/SignalOperation/KeySampler.hpp
    src/SignalOperation/Harmonics.hpp
    src/Core/Vec2.h
    src/Core/Factory.h
    src/Core/Math.hpp
    src/Core/PseudoRand.hpp
    src/Font/DefaultFontMaker.h
    src/Font/FontMaker.h
    src/Font/GlyphRenderer.h
    src/Graphics/GlError.h
    src/Graphics/GlQuadShader.h
    src/Graphics/GlSpriteShader.h
    src/Graphics/GlQuad.h
    src/Graphics/GlSprite.h
    src/Graphics/RenderableTexture.h
    src/Graphics/RenderInterface.h
    src/Graphics/imgui_impl_glfw.h
    src/Graphics/imgui_impl_opengl3.h
    src/Ui/UiEvent.h
    src/Ui/UiSystem.h
    src/Ui/UiRect.h
    src/Ui/UiNode.h
    src/Ui/UiNodeBoard.h
    src/Ui/UiText.h
    src/Ui/UiInput.h
    src/Ui/UiContainer.h
    src/Ui/UiList.h
    src/Ui/UiButton.h
    src/Ui/UiSlider.h
    src/Ui/UiFrame.h
    src/Ui/UiForm.h
    src/Ui/UiPin.h
    src/Ui/UiConnections.h
    
    src/main.cpp
    src/App/UiSignalNodeBoard.cpp
    src/App/AppInterface.cpp
    src/App/UserFileInput.cpp
    src/App/SignalNode.cpp
    src/App/TypedSignalNode.cpp
    src/Json/json.cpp
    src/Json/jsonrw.cpp
    src/ReaderWriter/FileIO.cpp
    src/ReaderWriter/RwHelpers.cpp
    src/ReaderWriter/FileExplorer.cpp
    src/ReaderWriter/SignalOperationsRW.cpp
    src/Audio/AudioRenderer.cpp
    src/Audio/PcmData.cpp
    src/Audio/Signal.cpp
    src/Audio/WavExporter.cpp
    src/SignalOperation/OperationType.cpp
    src/SignalOperation/SignalOperation.cpp
    src/SignalOperation/Operations.cpp
    src/SignalOperation/Oscillator.cpp
    src/SignalOperation/Quantizer.cpp
    src/SignalOperation/MixOperation.cpp
    src/SignalOperation/EnvelopOperation.cpp
    src/SignalOperation/StepOperation.cpp
    src/SignalOperation/FilterOperation.cpp
    src/SignalOperation/PitchSelector.cpp
    src/SignalOperation/Waveform.cpp
    src/SignalOperation/KeySampler.cpp
    src/SignalOperation/Harmonics.cpp
    src/Core/Vec2.cpp
    src/Core/PseudoRand.cpp
    src/Font/DefaultFontMaker.cpp
    src/Font/FontMaker.cpp
    src/Font/GlyphRenderer.cpp
    src/Graphics/GlQuadShader.cpp
    src/Graphics/GlSpriteShader.cpp
    src/Graphics/GlQuad.cpp
    src/Graphics/GlSprite.cpp
    src/Graphics/RenderableTexture.cpp
    src/Graphics/RenderInterfaceGlfw.cpp
    src/Graphics/imgui_impl_glfw.cpp
    src/Graphics/imgui_impl_opengl3.cpp
    src/Ui/UiSystem.cpp
    src/Ui/UiRect.cpp
    src/Ui/UiNode.cpp
    src/Ui/UiNodeBoard.cpp
    src/Ui/UiText.cpp
    src/Ui/UiInput.cpp
    src/Ui/UiContainer.cpp
    src/Ui/UiList.cpp
    src/Ui/UiButton.cpp
    src/Ui/UiSlider.cpp
    src/Ui/UiFrame.cpp
    src/Ui/UiForm.cpp
    src/Ui/UiPin.cpp
    src/Ui/UiConnections.cpp
    
    thirdparty/imgui/imconfig.h
    thirdparty/imgui/imgui_demo.cpp
    thirdparty/imgui/imgui_draw.cpp
    thirdparty/imgui/imgui_widgets.cpp
    thirdparty/imgui/imgui.cpp
    thirdparty/imgui/imgui.h
    
    # this one is included in src/App/Gui.cpp
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

