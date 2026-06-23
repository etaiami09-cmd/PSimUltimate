cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER "C:\\Program Files\\LLVM\\bin\\clang++.exe")
set(CMAKE_C_COMPILER "C:\\Program Files\\LLVM\\bin\\clang.exe")

include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.2
)
FetchContent_MakeAvailable(Catch2)

file(GLOB_RECURSE TEST_SOURCES "*.cpp")

add_executable(
    PSimTestRunner
    ${TEST_SOURCES}
)

target_link_libraries(PSimTestRunner
    PRIVATE
        PSimTestableLib
        Catch2::Catch2WithMain
)

include(Catch)
catch_discover_tests(PSimTestRunner)