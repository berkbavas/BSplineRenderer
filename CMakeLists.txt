cmake_minimum_required(VERSION 3.25.1)

project(BSplineRenderer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "$ENV{Qt6_DIR}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(INCLUDE_DIR Libs/qtimgui/include
                Libs/Eigen)

set(LIBS_DIR Libs/qtimgui/lib)

set(LIBS imgui implot qt_imgui_widgets)

file(GLOB SOURCES Source/*.cpp Source/*.h *.qrc)

find_package(Qt6 COMPONENTS Core Widgets OpenGL Gui Concurrent REQUIRED)

add_compile_options(/bigobj)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}" ${INCLUDE_DIR})

link_directories(${LIBS_DIR})

add_executable(BSplineRenderer ${SOURCES})

target_link_libraries(BSplineRenderer Qt6::Core Qt6::Widgets Qt6::OpenGL Qt6::Concurrent ${LIBS})

add_custom_command(TARGET BSplineRenderer
    POST_BUILD COMMAND xcopy /E /Y 
    "\"$(SolutionDir)../Resources\""
    "\"$(OutDir)Resources/*\"")

add_custom_command(TARGET BSplineRenderer
    POST_BUILD COMMAND xcopy /E /Y 
    "\"$(SolutionDir)../Resources\""
    "\"$(BuildDir)Resources/*\"")

add_custom_command(TARGET BSplineRenderer
    POST_BUILD COMMAND "$ENV{Qt6_DIR}/bin/windeployqt.exe"
    "\"$(OutDir)$(TargetName)$(TargetExt)\"")