cmake_minimum_required(VERSION 2.8.12)
include(CMakeParseArguments)

get_filename_component(LIBPLUGIN_DIR ${CMAKE_CURRENT_LIST_DIR} DIRECTORY)

if(NOT COPPELIASIM_ROOT_DIR)
    if(NOT DEFINED ENV{COPPELIASIM_ROOT_DIR})
        if(EXISTS "${LIBPLUGIN_DIR}/../../programming/include")
            get_filename_component(COPPELIASIM_PROGRAMMING_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
        elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../../programming/include")
            get_filename_component(COPPELIASIM_PROGRAMMING_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
        else()
            message(FATAL_ERROR "Cannot find CoppeliaSim installation. Please set the COPPELIASIM_ROOT_DIR environment variable to point to the root of your CoppeliaSim installation.")
        endif()
        get_filename_component(COPPELIASIM_ROOT_DIR ${COPPELIASIM_PROGRAMMING_DIR} DIRECTORY)
        unset(COPPELIASIM_PROGRAMMING_DIR)
    else()
        set(COPPELIASIM_ROOT_DIR "$ENV{COPPELIASIM_ROOT_DIR}")
    endif()
endif()

file(TO_CMAKE_PATH "${COPPELIASIM_ROOT_DIR}" COPPELIASIM_ROOT_DIR)

if(EXISTS "${COPPELIASIM_ROOT_DIR}/programming/include" AND EXISTS "${COPPELIASIM_ROOT_DIR}/programming/common")
    set(COPPELIASIM_INCLUDE_DIR "${COPPELIASIM_ROOT_DIR}/programming/include")
    set(COPPELIASIM_COMMON_DIR "${COPPELIASIM_ROOT_DIR}/programming/common")
    if(NOT EXISTS "${COPPELIASIM_INCLUDE_DIR}/simLib.h")
        message(FATAL_ERROR "Cannot find simLib.h in ${COPPELIASIM_INCLUDE_DIR}.")
    endif()
    if(NOT EXISTS "${COPPELIASIM_INCLUDE_DIR}/simConst.h")
        message(FATAL_ERROR "Cannot find simConst.h in ${COPPELIASIM_INCLUDE_DIR}.")
    endif()
    if(NOT EXISTS "${COPPELIASIM_COMMON_DIR}/simLib.cpp")
        message(FATAL_ERROR "Cannot find simLib.cpp in ${COPPELIASIM_COMMON_DIR}.")
    endif()
    if(NOT COPPELIASIM_FIND_QUIETLY)
        message(STATUS "Found CoppeliaSim installation at ${COPPELIASIM_ROOT_DIR}.")
    endif()

    set(COPPELIASIM_VERSION_CHECK_SRC "${CMAKE_BINARY_DIR}/sim_version_check.cpp")
    set(COPPELIASIM_VERSION_CHECK_BIN "${CMAKE_BINARY_DIR}/sim_version_check")
    file(WRITE ${COPPELIASIM_VERSION_CHECK_SRC} "
#include <iostream>
#include <simConst.h>
int main() {
    char sep = ';';
    std::cout
        << SIM_PROGRAM_VERSION_NB/10000 << sep
        << SIM_PROGRAM_VERSION_NB/100%100 << sep
        << SIM_PROGRAM_VERSION_NB%100 << sep
        << SIM_PROGRAM_REVISION_NB << sep
        << 0 << std::endl;
}
    ")
    if(NOT COPPELIASIM_FIND_QUIETLY)
        message(STATUS "Checking CoppeliaSim header version...")
    endif()
    try_run(COPPELIASIM_VERSION_RUN_RESULT COPPELIASIM_VERSION_COMPILE_RESULT ${COPPELIASIM_VERSION_CHECK_BIN} ${COPPELIASIM_VERSION_CHECK_SRC} CMAKE_FLAGS -DINCLUDE_DIRECTORIES=${COPPELIASIM_INCLUDE_DIR} RUN_OUTPUT_VARIABLE COPPELIASIM_VERSION_CHECK_OUTPUT)
    if(${COPPELIASIM_VERSION_COMPILE_RESULT})
        if(${COPPELIASIM_VERSION_RUN_RESULT} EQUAL 0)
            list(GET COPPELIASIM_VERSION_CHECK_OUTPUT 0 COPPELIASIM_VERSION_MAJOR)
            list(GET COPPELIASIM_VERSION_CHECK_OUTPUT 1 COPPELIASIM_VERSION_MINOR)
            list(GET COPPELIASIM_VERSION_CHECK_OUTPUT 2 COPPELIASIM_VERSION_PATCH)
            list(GET COPPELIASIM_VERSION_CHECK_OUTPUT 3 COPPELIASIM_VERSION_TWEAK)
            set(COPPELIASIM_VERSION_COUNT 4)
            list(GET COPPELIASIM_VERSION_CHECK_OUTPUT 3 COPPELIASIM_REVISION)
            set(COPPELIASIM_VERSION "${COPPELIASIM_VERSION_MAJOR}.${COPPELIASIM_VERSION_MINOR}.${COPPELIASIM_VERSION_PATCH}.${COPPELIASIM_REVISION}")
            set(COPPELIASIM_VERSION_STR "${COPPELIASIM_VERSION_MAJOR}.${COPPELIASIM_VERSION_MINOR}.${COPPELIASIM_VERSION_PATCH} rev${COPPELIASIM_REVISION}")
            if(NOT COPPELIASIM_FIND_QUIETLY)
                message(STATUS "CoppeliaSim headers version ${COPPELIASIM_VERSION_STR}")
            endif()
            if(DEFINED COPPELIASIM_FIND_VERSION)
                if(${COPPELIASIM_VERSION} VERSION_LESS ${COPPELIASIM_FIND_VERSION})
                    message(FATAL_ERROR "Found CoppeliaSim version ${COPPELIASIM_VERSION} but ${COPPELIASIM_FIND_VERSION} required.")
                endif()
            endif()
        else()
            message(FATAL_ERROR "Failed to run CoppeliaSim version check program")
        endif()
    else()
        message(FATAL_ERROR "Failed to compile CoppeliaSim version check program")
    endif()

    set(COPPELIASIM_FOUND TRUE)
else()
    if(COPPELIASIM_FIND_REQUIRED)
        message(FATAL_ERROR "The specified COPPELIASIM_ROOT_DIR dir does not point to a valid CoppeliaSim installation.")
    endif()
endif()

set(COPPELIASIM_EXPORTED_SOURCES "${COPPELIASIM_COMMON_DIR}/simLib.cpp")

if(WIN32)
    add_definitions(-DWIN_SIM)
    add_definitions(-DNOMINMAX)
    add_definitions(-Dstrcasecmp=_stricmp)
    set(COPPELIASIM_LIBRARIES shlwapi)
elseif(UNIX AND NOT APPLE)
    add_definitions(-DLIN_SIM)
    set(COPPELIASIM_LIBRARIES "")
elseif(UNIX AND APPLE)
    add_definitions(-DMAC_SIM)
    set(COPPELIASIM_LIBRARIES "")
endif()

function(COPPELIASIM_GENERATE_STUBS GENERATED_OUTPUT_DIR)
    cmake_parse_arguments(COPPELIASIM_GENERATE_STUBS "" "XML_FILE;LUA_FILE" "" ${ARGN})
    if(NOT COPPELIASIM_FIND_QUIETLY)
        message(STATUS "Adding simStubsGen command...")
    endif()
    if("${COPPELIASIM_GENERATE_STUBS_LUA_FILE}" STREQUAL "")
        add_custom_command(OUTPUT ${GENERATED_OUTPUT_DIR}/stubs.cpp ${GENERATED_OUTPUT_DIR}/stubs.h
            COMMAND python ${LIBPLUGIN_DIR}/simStubsGen/generate.py --xml-file ${COPPELIASIM_GENERATE_STUBS_XML_FILE} --gen-all ${GENERATED_OUTPUT_DIR}
            DEPENDS ${COPPELIASIM_GENERATE_STUBS_XML_FILE})
    else()
        add_custom_command(OUTPUT ${GENERATED_OUTPUT_DIR}/stubs.cpp ${GENERATED_OUTPUT_DIR}/stubs.h ${GENERATED_OUTPUT_DIR}/lua_calltips.cpp
            COMMAND python ${LIBPLUGIN_DIR}/simStubsGen/generate.py --xml-file ${COPPELIASIM_GENERATE_STUBS_XML_FILE} --lua-file ${COPPELIASIM_GENERATE_STUBS_LUA_FILE} --gen-all ${GENERATED_OUTPUT_DIR}
            DEPENDS ${COPPELIASIM_GENERATE_STUBS_XML_FILE})
    endif()
    include_directories("${GENERATED_OUTPUT_DIR}")
    set(COPPELIASIM_EXPORTED_SOURCES ${COPPELIASIM_EXPORTED_SOURCES} "${GENERATED_OUTPUT_DIR}/stubs.cpp" PARENT_SCOPE)
endfunction(COPPELIASIM_GENERATE_STUBS)

find_package(Git)
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/.git AND GIT_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        OUTPUT_VARIABLE "BUILD_GIT_VERSION"
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE)
else()
    set(BUILD_GIT_VERSION "unknown")
endif()

include_directories(${LIBPLUGIN_DIR})
set(COPPELIASIM_EXPORTED_SOURCES ${COPPELIASIM_EXPORTED_SOURCES} ${LIBPLUGIN_DIR}/simPlusPlus/Plugin.cpp)
