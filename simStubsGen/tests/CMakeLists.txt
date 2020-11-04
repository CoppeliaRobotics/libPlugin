cmake_minimum_required(VERSION 3.7)
project(simStubsGenTests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_MACOSX_RPATH 1)

if(NOT LIBPLUGIN_DIR)
    if(DEFINED ENV{COPPELIASIM_ROOT_DIR})
        set(LIBPLUGIN_DIR $ENV{COPPELIASIM_ROOT_DIR}/programming/libPlugin)
    else()
        set(LIBPLUGIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../libPlugin)
    endif()
endif()
set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
    ${LIBPLUGIN_DIR}/cmake)
find_package(CoppeliaSim 4.1.0.0 REQUIRED)
find_package(Boost REQUIRED)

include_directories(${COPPELIASIM_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR}/generated)
include_directories(${Boost_INCLUDE_DIRS})

coppeliasim_generate_stubs(${CMAKE_CURRENT_BINARY_DIR}/generated XML_FILE ${CMAKE_CURRENT_SOURCE_DIR}/callbacks.xml)
add_library(simStubsGenTests SHARED simStubsGenTests.cpp ${COPPELIASIM_EXPORTED_SOURCES})
target_link_libraries(simStubsGenTests ${Boost_LIBRARIES} ${COPPELIASIM_LIBRARIES})

enable_testing()
list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
add_test(
    NAME build_tests
    COMMAND "${CMAKE_COMMAND}" --build "${CMAKE_BINARY_DIR}" --target "${target}" --config "$<CONFIG>"
)
set_tests_properties(build_tests PROPERTIES FIXTURES_SETUP test_fixture)
add_test(
    NAME tests
    COMMAND "${CMAKE_COMMAND}"
        -E env
        "COPPELIASIM_CONSOLE_LOG_FORMAT={verbosity}:\t{message}"
        "${COPPELIASIM_EXECUTABLE}"
        -h # run in headless mode
        -vnone # will be raised after add-on is loaded
        "-a${CMAKE_CURRENT_SOURCE_DIR}/simStubsGenTests.lua" # load add-on script
        "-GsimStubsGenTests.module=$<TARGET_FILE:simStubsGenTests>"
        -GsimStubsGen.debug=0
        -GsimStubsGenTests.verbosity=infos
)
set_tests_properties(tests PROPERTIES FIXTURES_REQUIRED test_fixture)

add_custom_target(test-verbose
    "${CMAKE_CTEST_COMMAND}" -C "$<CONFIG>" -VV
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

