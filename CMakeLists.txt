cmake_minimum_required(VERSION 3.10)

# Set the project name
project(GenericCryptanalyzer)

# Specify the C++ standard
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Boost
find_package(Boost REQUIRED COMPONENTS unit_test_framework)

# Check if boost was found
if(NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find boost!")
endif()

# Ensure that dynamic_bitset is present
find_path(BOOST_DYNAMIC_BITSET_INCLUDE_DIR boost/dynamic_bitset.hpp)

if(NOT BOOST_DYNAMIC_BITSET_INCLUDE_DIR)
    message(FATAL_ERROR "Could not find boost::dynamic_bitset!")
endif()

include_directories(${Boost_INCLUDE_DIRS})

# Add all the source files to the library
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")
add_library(GenericCryptanalyzerLib ${SOURCES} ${HEADERS})

# Include directories for the library
target_include_directories(GenericCryptanalyzerLib PUBLIC ${PROJECT_SOURCE_DIR}/src)

# Add the test files
file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
foreach(test_src ${TEST_SOURCES})
    # Get the filename without extension
    get_filename_component(test_name ${test_src} NAME_WE)

    # Add executable
    add_executable(${test_name} ${test_src})

    # Link the libraries
    target_link_libraries(${test_name} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} GenericCryptanalyzerLib)

    # Enable testing
    enable_testing()

    # Add the test
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()

# Add the examples
file(GLOB_RECURSE EXAMPLE_SOURCES "examples/*.cpp")
file(GLOB_RECURSE HEADERS "examples/*.h")
foreach(example_src ${EXAMPLE_SOURCES})
    # Get the filename without extension
    get_filename_component(example_name ${example_src} NAME_WE)

    # Add executable
    add_executable(${example_name} ${example_src})

    # Link the library
    target_link_libraries(${example_name} GenericCryptanalyzerLib)
endforeach()
