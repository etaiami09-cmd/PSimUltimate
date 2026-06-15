cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER "C:\\Program Files\\LLVM\\bin\\clang++.exe")
set(CMAKE_C_COMPILER "C:\\Program Files\\LLVM\\bin\\clang.exe")

project(ParticleSimulatorUltimate VERSION 0.1.3 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        master
    GIT_SHALLOW    TRUE
)

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

file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "extern/imgui/*.cpp"
    "extern/rlImGui/*.cpp"    
)

add_executable(PSimUlt ${SOURCES})

set_target_properties(PSimUlt PROPERTIES WIN32_EXECUTABLE $<CONFIG:Release>)

target_link_options(PSimUlt PRIVATE $<$<CONFIG:Release>:-Xlinker /entry:mainCRTStartup>)

target_include_directories(PSimUlt PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# 1. Define your own application installation rule
install(TARGETS PSimUlt DESTINATION bin)

# 2. Setup standard cross-platform CPack configurations
set(CPACK_PACKAGE_NAME "Particle Simulator")
set(CPACK_PACKAGE_VENDOR "etaiami09-cmd")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Multi-Purpose Particle Simulator")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CPACK_PACKAGE_EXECUTABLES "PSimUlt" "Particle Simulator")

# 3. Explicitly tell CPack to ONLY package your executable target, ignoring raylib's build files
set(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_BINARY_DIR};ParticleSimulatorUltimate;ALL;/")

# 4. Define the generic installer type
set(CPACK_GENERATOR "NSIS")

# 5. Correctly load the CPack module (Case-Sensitive!)
include(CPack)
target_compile_definitions(PSimUlt PRIVATE NO_FONT_AWESOME)

target_link_libraries(PSimUlt PRIVATE raylib)