cmake_minimum_required(VERSION 3.5)

project(3DViewer_2_app VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets OpenGLWidgets OpenGL)

qt6_add_resources(SHADER_RSC_ADDED
    3DViewer_2/shaders.qrc
)

qt6_add_resources(DEFAULT_RSC_ADDED
    3DViewer_2/defaults.qrc
)

include_directories(3DViewer_2 3DViewer_2/gif_image/3rdParty/giflib 3DViewer_2/gif_image/gifimage)

set(PROJECT_SOURCES
    3DViewer_2/main.cpp
    3DViewer_2/mainwindow.cc
    3DViewer_2/oglwidget.cc
    3DViewer_2/oglwidget.h
    3DViewer_2/mainwindow.h
    3DViewer_2/mainwindow.ui
    3DViewer_2/viewersettings.h
    3DViewer_2/viewersettings.cc
    3DViewer_2/imagesaver.cc
    3DViewer_2/imagesaver.h
    parser/parser.cc
    parser/parser.h
    affine_transformations/matrix4X4.h
    affine_transformations/matrix4X4.cc
    affine_transformations/matrix4X4_transform.cc
    controller.h
    3DViewer_2/gif_image/3rdParty/giflib/dgif_lib.c
    3DViewer_2/gif_image/3rdParty/giflib/egif_lib.c
    3DViewer_2/gif_image/3rdParty/giflib/gif_err.c
    3DViewer_2/gif_image/3rdParty/giflib/gif_font.c
    3DViewer_2/gif_image/3rdParty/giflib/gif_hash.c
    3DViewer_2/gif_image/3rdParty/giflib/gifalloc.c
    3DViewer_2/gif_image/3rdParty/giflib/quantize.c
    3DViewer_2/gif_image/3rdParty/giflib/gif_hash.h
    3DViewer_2/gif_image/3rdParty/giflib/gif_lib.h
    3DViewer_2/gif_image/3rdParty/giflib/gif_lib_private.h
    3DViewer_2/gif_image/gifimage/qgifimage.cpp
    3DViewer_2/gif_image/gifimage/qgifglobal.h
    3DViewer_2/gif_image/gifimage/qgifimage.h
    3DViewer_2/gif_image/gifimage/qgifimage_p.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(3DViewer_2_app
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${SHADER_RSC_ADDED}
        ${DEFAULT_RSC_ADDED}
    )

# Define target properties for Android with Qt 6 as:
# set_property(TARGET 3DViewer_2_app APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
# ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(3DViewer_2_app SHARED
            ${PROJECT_SOURCES}
            ${SHADER_RSC_ADDED}
            ${DEFAULT_RSC_ADDED}
        )

    # Define properties for Android with Qt 5 after find_package() calls as:
    # set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(3DViewer_2_app
            ${PROJECT_SOURCES}
            ${SHADER_RSC_ADDED}
            ${DEFAULT_RSC_ADDED}
        )
    endif()
endif()

target_link_libraries(3DViewer_2_app PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::OpenGLWidgets Qt${QT_VERSION_MAJOR}::OpenGL ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

set_target_properties(3DViewer_2_app PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(3DViewer_2_app)
endif()
