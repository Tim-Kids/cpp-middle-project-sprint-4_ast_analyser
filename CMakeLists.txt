cmake_minimum_required(VERSION 3.25)
project(analyser VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Configure local githooks.
#execute_process(
#        COMMAND git config core.hooksPath .githooks
#        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
#        RESULT_VARIABLE GIT_COMMAND_RESULT
#)
cmake_policy(SET CMP0167 OLD)
find_package(GTest REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options)

# Add range-v3
#add_subdirectory(third_party/range-v3)

# 'analyzer' building (linking with libs it depends on).
add_executable(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/main.cpp
        include/helpers.hpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}
        PUBLIC
        metric_accumulator
        metric
        cmd_options
        file
        function
        #        range-v3 # Link against the imported range-v3 target
)

# Add directory with dependencies for 'analyser' target.
add_subdirectory(src)

# Helper func that prints out project's blueprint.
add_executable(print_dir_tree ${CMAKE_CURRENT_SOURCE_DIR}/third_party/print_dir_tree.cpp)

# Building unit tests.
include(CTest)
enable_testing()

