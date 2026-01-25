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

add_executable(DMPS
    main.cpp
    resources/.gitkeep
    src/.gitkeep
    docs/.gitkeep
    src/module/IModule.hpp
    src/engine/IEngine.hpp
    src/data/IData.hpp
    src/engine/Engine.hpp
    src/engine/Engine.cpp
    src/data/TextData.hpp
    src/data/TextData.cpp
    src/module/TextLengthModule.hpp
    src/module/TextLengthModule.cpp
    src/core/ModuleResult.hpp
    src/core/DataType.hpp
    src/core/ModuleError.hpp
    src/ui/MainWindow.hpp
    src/ui/MainWindow.cpp
    src/ui/MainWindow.ui
    src/controller/Controller.hpp src/controller/Controller.cpp
    src/dataLoaderManager/DataLoaderManager.hpp src/dataLoaderManager/DataLoaderManager.cpp
    src/dataLoader/IDataLoader.hpp
    src/dataLoader/TextDataLoader.cpp
    src/dataLoader/TextDataLoader.hpp
    src/ui/display/IDisplayAdapter.hpp
    src/ui/display/TextDisplayAdapter.hpp src/ui/display/TextDisplayAdapter.cpp
)

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
