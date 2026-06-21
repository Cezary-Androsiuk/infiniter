cmake_minimum_required(VERSION 3.16)

project(Infiniter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Google Tests
include(FetchContent)
FetchContent_Declare(
  googletest
  # URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)
# Windows stuff for GTests
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


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





add_executable(RunInfiniterTests
    # executable files
    source/InfiniterMemory.cpp
    source/InfiniterException.cpp

    # tests files
    tests/InfiniterTestMemory.cpp
    tests/InfiniterTestCore.cpp
)

target_include_directories(RunInfiniterTests PRIVATE source)

target_link_libraries(RunInfiniterTests
    gtest_main
)

include(CTest)
add_test(NAME InfiniterMemoryTest COMMAND RunInfiniterTests)




