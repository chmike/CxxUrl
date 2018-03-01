cmake_minimum_required(VERSION 3.4)
project(CxxUrl)

set(CMAKE_CXX_STANDARD 11)

add_definitions(-D_GNU_SOURCE -Wall -Wextra)

#*********************************************************
# determine platform
#*********************************************************
set(PLATFORM UNKNOWN)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(PLATFORM LINUX)
    if (DEFINED ${ANDROID_PLATFORM})
        set(PLATFORM ANDROID)
        add_definitions(-DANDROID_PLATFORM)
    endif()
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    set(PLATFORM MAC_OS)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(PLATFORM WINDOWS)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    set(PLATFORM ANDROID)
    add_definitions(-DANDROID_PLATFORM)
endif()

message("Platform: " ${CMAKE_SYSTEM_NAME} "-" ${PLATFORM})

set(HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/url.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/string.hpp)

set(SOURCES
        ${HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/url.cpp)

add_library(CxxUrl SHARED ${SOURCES})
set_target_properties(CxxUrl PROPERTIES SOVERSION 1)
set_target_properties(CxxUrl PROPERTIES PUBLIC_HEADER url.hpp)
