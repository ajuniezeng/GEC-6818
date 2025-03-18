cmake_minimum_required(VERSION 3.28)
project(
  GEC-6818 
  VERSION 0.1.0
  LANGUAGES C)

### Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wshadow -Wpedantic -pthread -Os")

add_subdirectory("${PROJECT_SOURCE_DIR}/app")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# target format
file (GLOB_RECURSE ALL_SRC_FILES *.h *.c)
add_custom_target (format "clang-format" -i ${ALL_SRC_FILES} COMMENT "Formatting source code...")