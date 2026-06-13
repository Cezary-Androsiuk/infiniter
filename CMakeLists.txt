cmake_minimum_required(VERSION 3.16)

project(Infiniter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    main.cpp
    source/InfiniterMemory.cpp
    source/InfiniterException.cpp
)

set(HEADERS
    source/InfiniterShared.hpp
    source/InfiniterException.hpp
    source/InfiniterMemory.hpp
    source/InfiniterCore.hpp
    source/InfiniterBit.hpp
    source/InfiniterArithmetic.hpp
    source/InfiniterIO.hpp
    source/InfiniterUtility.hpp
    source/Infiniter.hpp
)

set(TEMPLATES
    source/InfiniterCore.tpp
    source/InfiniterBit.tpp
    source/InfiniterArithmetic.tpp
    source/InfiniterIO.tpp
    source/InfiniterUtility.tpp
    source/Infiniter.tpp
)

add_executable(Infiniter
    ${SOURCES}
    ${HEADERS}
    ${TEMPLATES}
)

include(GNUInstallDirs)
install(TARGETS Infiniter
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
