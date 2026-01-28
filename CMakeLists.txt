cmake_minimum_required(VERSION 3.16)
project(DMPS LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# AJOUT : Activer l'automoc et l'autouic pour Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# AJOUT : Trouver Qt (essayez Qt6 d'abord, sinon Qt5)
find_package(Qt6 COMPONENTS Core Gui Widgets QUIET)
if(NOT Qt6_FOUND)
    find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)
    set(QT_VERSION_MAJOR 5)
else()
    set(QT_VERSION_MAJOR 6)
endif()

if(WIN32)
    set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/resources/app.rc")
endif()

include_directories(include)

file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "src/*.hpp"
    "src/*.ui"
    "src/include/stb_image.h"
    "src/test_data/*.jpeg"
    "src/test_data/*.csv"
)



add_executable(DMPS main.cpp ${SOURCES}
${APP_ICON_RESOURCE_WINDOWS})

# AJOUT : Lier les bibliothèques Qt
target_link_libraries(DMPS PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
)

include(GNUInstallDirs)
install(TARGETS DMPS
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)


