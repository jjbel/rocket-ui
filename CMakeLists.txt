cmake_minimum_required(VERSION 3.16) # At least CMake-3.16 is required

# Install deps using Conan
find_program(CONAN_EXE conan REQUIRED)
execute_process(
        # TODO conan args: make sure they're correct
        COMMAND ${CONAN_EXE} install .
        -b missing
        -vquiet
        -of ${CMAKE_BINARY_DIR}
        # -pr:b=default -pr=default
        -s build_type=${CMAKE_BUILD_TYPE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        # OUTPUT_QUIET
        # TODO output_quiet not working? using -vquiet
        # TODO how to silence output from individual lib files
    )

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

project(rocket-ui CXX) # Create Project

find_package(samarium CONFIG REQUIRED)
find_package(RapidJSON REQUIRED)

add_executable(rocket-ui src/main.cpp)
target_compile_features(rocket-ui PUBLIC cxx_std_20) # set C++ standard
target_link_libraries(rocket-ui samarium::samarium)
target_link_libraries(rocket-ui rapidjson)

# TODO for now not using this, just running it thru python
# if(CURRENT_EXAMPLE)
#     add_custom_target(
#         run ALL
#         COMMAND example
#         DEPENDS example
#         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#         USES_TERMINAL
#     )
# endif()

# conan creates its own preset...but cmake gives error deuplicate preset: default
file(REMOVE CMakeUserPresets.json)
