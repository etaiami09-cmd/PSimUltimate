 cmake_minimum_required(VERSION 3.15)
 project(PSimModule LANGUAGES CXX C)

 set(CMAKE_CXX_STANDARD 26)
 set(CMAKE_CXX_STANDARD_REQUIRED ON)

 set(SOURCE_FILES module.cpp)
 set(HEADER_FILES include/psim_module_api.hpp)

 include(FetchContent)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

 FetchContent_Declare(
         raylib
         GIT_REPOSITORY https://github.com/raysan5/raylib.git
         GIT_TAG 5.0
         GIT_SHALLOW TRUE
 )

set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)

 FetchContent_MakeAvailable(raylib)

 add_library(psim_module_lib SHARED ${SOURCE_FILES} ${HEADER_FILES})

 target_include_directories(psim_module_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
 target_link_libraries(psim_module_lib PRIVATE raylib)