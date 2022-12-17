cmake_minimum_required(VERSION 3.23)
project(PenInspector VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
    Charts
    QuickWidgets
    Test
    Widgets
)

get_target_property(QMAKE_EXECUTABLE Qt6::qmake IMPORTED_LOCATION)
get_filename_component(_QT_BIN_DIR_ "${QMAKE_EXECUTABLE}" DIRECTORY)
find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_QT_BIN_DIR_}")
find_program(LINUXDEPLOY_EXECUTABLE linuxdeploy linuxdeploy-x86_64.AppImage HINTS "${_QT_BIN_DIR_}")
find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_QT_BIN_DIR_}")
find_program(CREATE_DMG_EXECUTABLE create-dmg)

add_subdirectory(tests)
add_subdirectory(src/app)
