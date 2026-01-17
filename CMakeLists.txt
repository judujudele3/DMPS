cmake_minimum_required(VERSION 3.16)

project(DMPS LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(DMPS main.cpp
    resources/.gitkeep
    src/.gitkeep
    docs/.gitkeep
    src/module/IModule.hpp
    src/engine/IEngine.hpp
    src/data/IData.hpp
    src/engine/Engine.hpp src/engine/Engine.cpp
    src/data/TextData.hpp src/data/TextData.cpp
    src/module/TextLengthModule.hpp src/module/TextLengthModule.cpp
    src/core/ModuleResult.hpp
    src/core/DataType.hpp
    src/core/ModuleError.hpp)

include(GNUInstallDirs)
install(TARGETS DMPS
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
