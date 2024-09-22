cmake_minimum_required(VERSION 3.25.1)

project(BSplineCurves3D VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "$ENV{Qt6_DIR}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(INCLUDE_DIR Dependencies/qtimgui/include/qtimgui
                Dependencies/qtimgui/include/imgui
                Dependencies/qtimgui/include/implot
                Dependencies/Eigen
)

set(LIBS_DIR Dependencies/qtimgui/lib)

set(LIBS imgui implot qt_imgui_widgets)

set(CPP_FILES Sources/Window.cpp
              Sources/Bezier.cpp
              Sources/Camera.cpp
              Sources/CameraManager.cpp
              Sources/Controller.cpp
              Sources/Curve.cpp
              Sources/CurveManager.cpp
              Sources/DummyCamera.cpp
              Sources/FreeCamera.cpp
              Sources/Helper.cpp
              Sources/Light.cpp
              Sources/LightManager.cpp
              Sources/Main.cpp
              Sources/Material.cpp
              Sources/Model.cpp
              Sources/ModelData.cpp
              Sources/ModelManager.cpp
              Sources/Node.cpp
              Sources/Point.cpp
              Sources/RendererManager.cpp
              Sources/ShaderManager.cpp
              Sources/Spline.cpp
              Sources/Ticks.cpp
)

set(HEADER_FILES Sources/Window.h
                 Sources/Bezier.h
                 Sources/Camera.h
                 Sources/CameraManager.h
                 Sources/Controller.h
                 Sources/Curve.h
                 Sources/CurveManager.h
                 Sources/DummyCamera.h
                 Sources/FreeCamera.h
                 Sources/Helper.h
                 Sources/Light.h
                 Sources/LightManager.h
                 Sources/Material.h
                 Sources/Model.h
                 Sources/ModelData.h
                 Sources/ModelManager.h
                 Sources/Node.h
                 Sources/Point.h
                 Sources/RendererManager.h
                 Sources/ShaderManager.h
                 Sources/Spline.h
                 Sources/Ticks.h
)

set(QRC_FILES BSplineCurves3D.qrc)

set(SOURCES ${CPP_FILES} ${HEADER_FILES} ${QRC_FILES})

find_package(Qt6 COMPONENTS Core Widgets OpenGL Gui Concurrent REQUIRED)

add_compile_options(/bigobj)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}" ${INCLUDE_DIR})

link_directories(${LIBS_DIR})

add_executable(BSplineCurves3D ${SOURCES})

target_link_libraries(BSplineCurves3D Qt6::Core Qt6::Widgets Qt6::OpenGL Qt6::Concurrent ${LIBS})

add_custom_command(TARGET BSplineCurves3D
    POST_BUILD COMMAND xcopy /E /Y 
    "\"$(SolutionDir)../Resources\""
    "\"$(OutDir)Resources/*\""
)

add_custom_command(TARGET BSplineCurves3D
    POST_BUILD COMMAND xcopy /E /Y 
    "\"$(SolutionDir)../Resources\""
    "\"$(BuildDir)Resources/*\""
)

add_custom_command(TARGET BSplineCurves3D
    POST_BUILD COMMAND "$ENV{Qt6_DIR}/bin/windeployqt.exe"
    "\"$(OutDir)$(TargetName)$(TargetExt)\""
)