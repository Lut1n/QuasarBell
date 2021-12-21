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
    src/App/GeometryNode.hpp
    src/App/TypedGeometryNode.hpp
    src/App/BaseAttributes.hpp
    src/App/BaseOperation.hpp
    src/App/BaseOperationNode.hpp
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
    src/SignalOperation/SignalAttributes.hpp
    src/SignalOperation/SignalOperations.hpp
    src/SignalOperation/PcmBuilder.hpp
    src/ImageOperation/ImageAttributes.hpp
    src/ImageOperation/ImageOperations.hpp
    src/ImageOperation/ImageOperationType.hpp
    src/ImageOperation/GlslBuilder.hpp
    src/ImageOperation/TextureBuilder.hpp
    src/ImageOperation/FrameRenderer.hpp
    src/GeometryOperation/GeometryAttributes.hpp
    src/GeometryOperation/GeometryOperationType.hpp
    src/SdfOperation/SdfOperations.hpp
    src/SdfOperation/VoxelExporter.hpp
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
    src/App/GeometryNode.cpp
    src/App/TypedGeometryNode.cpp
    src/App/BaseAttributes.cpp
    src/App/BaseOperation.cpp
    src/App/BaseOperationNode.cpp
    src/Core/Vec2.cpp
    src/Core/Quat.cpp
    src/Core/PseudoRand.cpp
    src/Font/DefaultFontMaker.cpp
    src/Font/FontMaker.cpp
    src/Font/GlyphRenderer.cpp
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
    src/SignalOperation/SignalAttributes.cpp
    src/SignalOperation/OperationType.cpp
    src/SignalOperation/SignalOperations.cpp
    src/SignalOperation/PcmBuilder.cpp
    src/ImageOperation/ImageAttributes.cpp
    src/ImageOperation/ImageOperationType.cpp
    src/ImageOperation/ImageOperations.cpp
    src/ImageOperation/GlslBuilder.cpp
    src/ImageOperation/TextureBuilder.cpp
    src/ImageOperation/FrameRenderer.cpp
    src/GeometryOperation/GeometryAttributes.cpp
    src/GeometryOperation/GeometryOperationType.cpp
    src/SdfOperation/SdfOperations.cpp
    src/SdfOperation/VoxelExporter.cpp
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

