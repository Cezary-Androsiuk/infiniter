cmake_minimum_required(VERSION 3.16)

project(Infiniter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Infiniter
    main.cpp
    source/InfiniterShared.hpp
    source/InfiniterMemory.hpp source/InfiniterMemory.cpp
    source/InfiniterCore.hpp source/InfiniterCore.cpp
    source/InfiniterIO.hpp source/InfiniterIO.cpp
    source/InfiniterBit.hpp source/InfiniterBit.cpp
    source/InfiniterArithmetic.hpp source/InfiniterArithmetic.cpp
    source/InfiniterUtility.hpp source/InfiniterUtility.cpp
    source/Infiniter.hpp source/Infiniter.cpp
)

include(GNUInstallDirs)
install(TARGETS Infiniter
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
