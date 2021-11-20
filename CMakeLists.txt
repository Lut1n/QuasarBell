cmake_minimum_required(VERSION 3.5)
project(QuasarBell VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Help to find glfw3 config on Windows (May need to build it from source)
set(glfw3_DIR "C:/Program Files (x86)/glfw/lib/cmake/glfw3")

find_package(glfw3 3.3 REQUIRED CONFIG)
find_package(OpenAL REQUIRED)

set(QB_SOURCES
    src/App/UiSignalNodeBoard.hpp
    src/App/AppInterface.hpp
    src/App/UserFileInput.hpp
    src/App/SignalNode.hpp
    src/App/TypedSignalNode.hpp
    src/App/ImageNode.hpp
    src/App/TypedImageNode.hpp
    src/App/SdfNode.hpp
    src/App/TypedSdfNode.hpp
    src/App/BaseOperationNode.hpp
    src/Json/Json.hpp
    src/Json/JsonReaderWriter.hpp
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
    src/SignalOperation/PcmBuilder.hpp
    src/ImageOperation/GlslBuilder.hpp
    src/ImageOperation/HighResOutput.hpp
    src/ImageOperation/ImageNoises.hpp
    src/ImageOperation/ImageSampling.hpp
    src/ImageOperation/ImageUvOperations.hpp
    src/ImageOperation/ImageBasicOperators.hpp
    src/ImageOperation/ImageOperation.hpp
    src/ImageOperation/ImageOperationType.hpp
    src/SdfOperation/SdfOperation.hpp
    src/SdfOperation/SdfOperationType.hpp
    src/SdfOperation/SdfPrimitives.hpp
    src/SdfOperation/SdfBasicOperations.hpp
    src/SdfOperation/SdfTransforms.hpp
    src/SdfOperation/SdfSampling.hpp
    src/SdfOperation/SdfVoxel.hpp
    src/SdfOperation/VoxelExporter.hpp
    src/Core/Version.hpp
    src/Core/Vec2.h
    src/Core/Factory.h
    src/Core/Quat.hpp
    src/Core/Math.hpp
    src/Core/ProgressiveWork.hpp
    src/Core/PseudoRand.hpp
    src/Font/DefaultFontMaker.h
    src/Font/FontMaker.h
    src/Font/GlyphRenderer.h
    src/Graphics/GlError.h
    src/Graphics/GlQuadShader.h
    src/Graphics/GlSpriteShader.h
    src/Graphics/GlQuad.h
    src/Graphics/GlSprite.h
    src/Graphics/CustomProgram.h
    src/Graphics/RenderableTexture.h
    src/Graphics/RenderInterface.h
    src/Graphics/TgaExporter.hpp
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
    src/App/ImageNode.cpp
    src/App/TypedImageNode.cpp
    src/App/SdfNode.cpp
    src/App/TypedSdfNode.cpp
    src/App/BaseOperationNode.cpp
    src/Json/Json.cpp
    src/Json/JsonReaderWriter.cpp
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
    src/SignalOperation/PcmBuilder.cpp
    src/ImageOperation/GlslBuilder.cpp
    src/ImageOperation/HighResOutput.cpp
    src/ImageOperation/ImageNoises.cpp
    src/ImageOperation/ImageSampling.cpp
    src/ImageOperation/ImageUvOperations.cpp
    src/ImageOperation/ImageBasicOperators.cpp
    src/ImageOperation/ImageOperation.cpp
    src/ImageOperation/ImageOperationType.cpp
    src/SdfOperation/SdfOperation.cpp
    src/SdfOperation/SdfOperationType.cpp
    src/SdfOperation/SdfPrimitives.cpp
    src/SdfOperation/SdfBasicOperations.cpp
    src/SdfOperation/SdfTransforms.cpp
    src/SdfOperation/SdfSampling.cpp
    src/SdfOperation/SdfVoxel.cpp
    src/SdfOperation/VoxelExporter.cpp
    src/Core/Vec2.cpp
    src/Core/Quat.cpp
    src/Core/PseudoRand.cpp
    src/Font/DefaultFontMaker.cpp
    src/Font/FontMaker.cpp
    src/Font/GlyphRenderer.cpp
    src/Graphics/GlQuadShader.cpp
    src/Graphics/GlSpriteShader.cpp
    src/Graphics/GlQuad.cpp
    src/Graphics/GlSprite.cpp
    src/Graphics/CustomProgram.cpp
    src/Graphics/RenderableTexture.cpp
    src/Graphics/RenderInterfaceGlfw.cpp
    src/Graphics/TgaExporter.cpp
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

add_executable(${PROJECT_NAME} ${QB_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE src)

target_include_directories(${PROJECT_NAME} PRIVATE thirdparty/imgui)
target_include_directories(${PROJECT_NAME} PRIVATE thirdparty/glad/include/)
target_include_directories(${PROJECT_NAME} PRIVATE ${OPENAL_INCLUDE_DIR})
    
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC ${OPENAL_LIBRARY})
target_link_libraries(${PROJECT_NAME} PUBLIC ${CMAKE_DL_LIBS})

add_definitions("-DIMGUI_IMPL_OPENGL_LOADER_GLAD")

