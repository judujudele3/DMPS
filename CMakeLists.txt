cmake_minimum_required(VERSION 3.16)

project(DMPS LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(DMPS main.cpp
    resources/.gitkeep
    src/.gitkeep
    docs/.gitkeep)

include(GNUInstallDirs)
install(TARGETS DMPS
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
