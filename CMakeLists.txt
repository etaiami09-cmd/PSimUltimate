cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(ParticleSimulatorUltimate VERSION 0.1.5 LANGUAGES CXX)

enable_testing()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        master
    GIT_SHALLOW    TRUE
)

set(BUILD_SHARED_LIBS ON CACHE BOOL "Forced shared library build" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(raylib)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/version.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.hpp
    @ONLY
)

include_directories(include)
include_directories(include/inlines)
include_directories(extern/imgui)
include_directories(extern/rlImGui)
include_directories(extern/nlohmann)
include_directories(extern/portable-file-dialogs)

file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "extern/imgui/*.cpp"
    "extern/rlImGui/*.cpp"    
)

file(GLOB_RECURSE ENGINE_SOURCES
    "src/*.cpp"
    "extern/imgui/*.cpp"
    "extern/rlImGui/*.cpp")
list(REMOVE_ITEM ENGINE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/core/main.cpp")
list(FILTER ENGINE_SOURCES EXCLUDE REGEX "example_module/.*")

add_library(PSimTestableLib STATIC ${ENGINE_SOURCES})
target_include_directories(PSimTestableLib PUBLIC 
    include
    include/inlines
    extern/imgui
    extern/rlImGui
    extern/nlohmann
    extern/portable-file-dialogs
    ${CMAKE_CURRENT_BINARY_DIR}
)
target_link_libraries(PSimTestableLib PUBLIC raylib stdc++exp)
target_compile_definitions(PSimTestableLib PRIVATE NO_FONT_AWESOME)

add_executable(PSimUlt src/core/main.cpp)
target_link_libraries(PSimUlt PRIVATE PSimTestableLib)

target_link_options(PSimUlt PRIVATE
        -static
        -static-libgcc
        -static-libstdc++
)

set_target_properties(PSimUlt PROPERTIES WIN32_EXECUTABLE $<CONFIG:Release>)

target_compile_definitions(PSimTestableLib PUBLIC PSIM_STATIC_BUILD)

target_link_options(PSimUlt PRIVATE
        $<$<PLATFORM_ID:Windows>:-mwindows$<COMMA>-Wl,--entry=mainCRTStartup>)

target_include_directories(PSimUlt PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS PSimUlt DESTINATION bin)

set(CPACK_PACKAGE_NAME "Particle Simulator")
set(CPACK_PACKAGE_VENDOR "etaiami09-cmd")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Multi-Purpose Particle Simulator")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CPACK_PACKAGE_EXECUTABLES "PSimUlt" "Particle Simulator")

set(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_BINARY_DIR};ParticleSimulatorUltimate;ALL;/")

set(CPACK_GENERATOR "NSIS")

include(CPack)
target_compile_definitions(PSimUlt PRIVATE NO_FONT_AWESOME)

target_link_libraries(PSimUlt PRIVATE raylib)

add_subdirectory(tests)