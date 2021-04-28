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
    src/app/App.hpp
    src/app/SFXBasicEditor.hpp
    src/app/NodalEditor.hpp
    src/app/Synth.hpp
    src/gui/Gui.hpp
    src/gui/UserFileInput.hpp
    src/gui/InputEffect.hpp
    src/gui/Keyboard.hpp
    src/gui/PostEffect.hpp
    src/gui/SignalEdit.hpp
    src/gui/NodeContextMenu.hpp
    src/gui/NodePropertiesEdit.hpp
    src/gui/nodal/SignalOperationNode.hpp
    src/gui/nodal/AddSignalNode.hpp
    src/gui/nodal/FloatSignalNode.hpp
    src/gui/nodal/CubicSamplerNode.hpp
    src/gui/nodal/OscillatorNode.hpp
    src/gui/nodal/AudioOutputNode.hpp
    src/gui/nodal/QuantizerNode.hpp
    src/gui/nodal/MixNode.hpp
    src/gui/nodal/EnvelopNode.hpp
    src/gui/nodal/StepNode.hpp
    src/gui/nodal/FilterNode.hpp
    src/gui/nodal/PitchNode.hpp
    src/gui/nodal/WaveformNode.hpp
    src/gui/nodal/KeySamplerNode.hpp
    src/gui/nodal/HarmonicsNode.hpp
    src/gui/nodal/PolynomialNode.hpp
    src/gui/imgui_impl_glfw.h
    src/gui/imgui_impl_opengl3.h
    src/json/json.hpp
    src/json/jsonrw.hpp
    src/readerwriter/FileIO.hpp
    src/readerwriter/SignalRW.hpp
    src/readerwriter/RwHelpers.hpp
    src/readerwriter/FileExplorer.hpp
    src/readerwriter/SignalOperationsRW.hpp
    src/audio/AudioRenderer.hpp
    src/audio/Error.hpp
    src/audio/Format.hpp
    src/signal/LinearSamplers.hpp
    src/signal/Modulator.hpp
    src/signal/PcmData.hpp
    src/signal/Signal.hpp
    src/signal/WavExporter.hpp
    src/signal/operations/OperationType.hpp
    src/signal/operations/SignalOperation.hpp
    src/signal/operations/Operations.hpp
    src/signal/operations/Oscillator.hpp
    src/signal/operations/Quantizer.hpp
    src/signal/operations/MixOperation.hpp
    src/signal/operations/EnvelopOperation.hpp
    src/signal/operations/StepOperation.hpp
    src/signal/operations/FilterOperation.hpp
    src/signal/operations/PitchSelector.hpp
    src/signal/operations/Waveform.hpp
    src/signal/operations/KeySampler.hpp
    src/signal/operations/Harmonics.hpp
    src/core/Vec2.h
    src/core/Factory.h
    src/core/Math.hpp
    src/core/PseudoRand.hpp
    src/font/DefaultFontMaker.h
    src/font/FontMaker.h
    src/font/GlyphRenderer.h
    src/graphics/GlError.h
    src/graphics/GlQuadShader.h
    src/graphics/GlSpriteShader.h
    src/graphics/GlQuad.h
    src/graphics/GlSprite.h
    src/graphics/RenderableTexture.h
    src/graphics/RenderInterface.h
    src/ui/UiEvent.h
    src/ui/UiSystem.h
    src/ui/UiRect.h
    src/ui/UiNode.h
    src/ui/UiNodeBoard.h
    src/ui/UiText.h
    src/ui/UiInput.h
    src/ui/UiContainer.h
    src/ui/UiList.h
    src/ui/UiButton.h
    src/ui/UiSlider.h
    src/ui/UiFrame.h
    src/ui/UiForm.h
    src/ui/UiPin.h
    src/ui/UiConnections.h
    
    src/main.cpp
    src/app/SFXBasicEditor.cpp
    src/app/NodalEditor.cpp
    src/app/Synth.cpp
    src/gui/Gui.cpp
    src/gui/UserFileInput.cpp
    src/gui/InputEffect.cpp
    src/gui/Keyboard.cpp
    src/gui/PostEffect.cpp
    src/gui/SignalEdit.cpp
    src/gui/nodal/SignalOperationNode.cpp
    src/gui/nodal/AddSignalNode.cpp
    src/gui/nodal/FloatSignalNode.cpp
    src/gui/nodal/CubicSamplerNode.cpp
    src/gui/nodal/OscillatorNode.cpp
    src/gui/nodal/AudioOutputNode.cpp
    src/gui/nodal/QuantizerNode.cpp
    src/gui/nodal/MixNode.cpp
    src/gui/nodal/EnvelopNode.cpp
    src/gui/nodal/StepNode.cpp
    src/gui/nodal/FilterNode.cpp
    src/gui/nodal/PitchNode.cpp
    src/gui/nodal/WaveformNode.cpp
    src/gui/nodal/KeySamplerNode.cpp
    src/gui/nodal/HarmonicsNode.cpp
    src/gui/nodal/PolynomialNode.cpp
    src/gui/NodeContextMenu.cpp
    src/gui/NodePropertiesEdit.cpp
    src/gui/imgui_impl_glfw.cpp
    src/gui/imgui_impl_opengl3.cpp
    src/json/json.cpp
    src/json/jsonrw.cpp
    src/readerwriter/FileIO.cpp
    src/readerwriter/SignalRW.cpp
    src/readerwriter/RwHelpers.cpp
    src/readerwriter/FileExplorer.cpp
    src/readerwriter/SignalOperationsRW.cpp
    src/audio/AudioRenderer.cpp
    src/signal/LinearSamplers.cpp
    src/signal/Modulator.cpp
    src/signal/PcmData.cpp
    src/signal/Signal.cpp
    src/signal/WavExporter.cpp
    src/signal/operations/OperationType.cpp
    src/signal/operations/SignalOperation.cpp
    src/signal/operations/Operations.cpp
    src/signal/operations/Oscillator.cpp
    src/signal/operations/Quantizer.cpp
    src/signal/operations/MixOperation.cpp
    src/signal/operations/EnvelopOperation.cpp
    src/signal/operations/StepOperation.cpp
    src/signal/operations/FilterOperation.cpp
    src/signal/operations/PitchSelector.cpp
    src/signal/operations/Waveform.cpp
    src/signal/operations/KeySampler.cpp
    src/signal/operations/Harmonics.cpp
    src/core/Vec2.cpp
    src/core/Factory.cpp
    src/core/PseudoRand.cpp
    src/font/DefaultFontMaker.cpp
    src/font/FontMaker.cpp
    src/font/GlyphRenderer.cpp
    src/graphics/GlQuadShader.cpp
    src/graphics/GlSpriteShader.cpp
    src/graphics/GlQuad.cpp
    src/graphics/GlSprite.cpp
    src/graphics/RenderableTexture.cpp
    src/graphics/RenderInterfaceGlfw.cpp
    src/ui/UiSystem.cpp
    src/ui/UiRect.cpp
    src/ui/UiNode.cpp
    src/ui/UiNodeBoard.cpp
    src/ui/UiText.cpp
    src/ui/UiInput.cpp
    src/ui/UiContainer.cpp
    src/ui/UiList.cpp
    src/ui/UiButton.cpp
    src/ui/UiSlider.cpp
    src/ui/UiFrame.cpp
    src/ui/UiForm.cpp
    src/ui/UiPin.cpp
    src/ui/UiConnections.cpp
    
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

