cmake_minimum_required(VERSION 3.4)
project(CxxUrl
    VERSION 0.3.0
)


#*********************************************************
# GLOBAL OPTIONS - you can change those
# Note: that options are cached, so in order to reavaluate options
# specified via CLI you need to remove the cmake folder
#*********************************************************

option(ENABLE_INSTALL "Flag to indicate if the install target should be available" ON)

if(${PROJECT_IS_TOP_LEVEL})
    # This will be executed if the current project is top level
    add_definitions(-D_GNU_SOURCE -Wall -Wextra)
endif()

#*********************************************************
# determine platform
#*********************************************************
set(PLATFORM UNKNOWN)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(PLATFORM LINUX)
    if (DEFINED ${ANDROID_PLATFORM})
        set(PLATFORM ANDROID)
    endif()
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    set(PLATFORM MAC_OS)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(PLATFORM WINDOWS)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    set(PLATFORM ANDROID)
endif()

message(DEBUG "Platform: " ${CMAKE_SYSTEM_NAME} "-" ${PLATFORM})

#*********************************************************
# Project Settings (no need to edit those)
#*********************************************************

include(GNUInstallDirs) # for CMAKE_INSTALL_FULL_INCLUDEDIR
set(INSTALL_HEADER_INCLUDE_DIR "${CMAKE_INSTALL_FULL_INCLUDEDIR}/${PROJECT_NAME}")
set(NAMESPACE chmike)
set(TARGET_NAME ${PROJECT_NAME})

set(HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/url.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/string.hpp)

set(SOURCES
        ${HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/url.cpp)

add_library(${TARGET_NAME} SHARED ${SOURCES})
add_library(${NAMESPACE}::${TARGET_NAME} ALIAS ${TARGET_NAME})

set_target_properties(${TARGET_NAME} PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED ON)
set_target_properties(${TARGET_NAME} PROPERTIES SOVERSION 1)
set_target_properties(${TARGET_NAME} PROPERTIES PUBLIC_HEADER "${HEADERS}")

if(${PLATFORM} STREQUAL ANDROID)
    message(INFO "${PROJECT_NAME} Platform Android detected")
    target_compile_definitions(${TARGET_NAME} PUBLIC ANDROID_PLATFORM)
endif()

# The following tells CMake to propagate the correct include directory if this project is linked via target_link_libraries
target_include_directories(CxxUrl
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${INSTALL_HEADER_INCLUDE_DIR}>
)

if(${ENABLE_INSTALL})
    set(PACKAGE_NAME ${PROJECT_NAME}) # the package name is what you write in find_package
    set(TARGETS_FILE_NAME     "${PACKAGE_NAME}Targets.cmake")
    set(TARGETS_FILE_PATH     "${CMAKE_CURRENT_BINARY_DIR}/${TARGETS_FILE_NAME}")
    set(VERSION_FILE_PATH     "${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_NAME}ConfigVersion.cmake")
    set(CONFIG_FILE_PATH      "${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_NAME}Config.cmake")
    set(PACKAGE_CONFIG_IN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CxxUrlConfig.cmake.in")

    install(
        TARGETS ${TARGET_NAME}
        EXPORT ${PACKAGE_NAME} DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${INSTALL_HEADER_INCLUDE_DIR}
    )

    install(
        EXPORT ${PACKAGE_NAME}
        FILE ${TARGETS_FILE_NAME}
        NAMESPACE "${NAMESPACE}::"
        DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}/cmake/${PACKAGE_NAME}"
    )

    include(CMakePackageConfigHelpers)
    configure_package_config_file(
        "${PACKAGE_CONFIG_IN_FILE}"
        ${CONFIG_FILE_PATH}
        INSTALL_DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}/cmake/${PACKAGE_NAME}"
    )

    message(INFO "\tExpanding CMAKE_PREFIX_PATH to include package build dir") # this enables find_package after add_subdirectory
    if(${PROJECT_IS_TOP_LEVEL})
        set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${CMAKE_CURRENT_BINARY_DIR}")
    else()
        set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${CMAKE_CURRENT_BINARY_DIR}" PARENT_SCOPE)
    endif()

    write_basic_package_version_file(
      ${VERSION_FILE_PATH}
      VERSION ${PROJECT_VERSION}
      COMPATIBILITY SameMinorVersion # TODO: once 1.0.0 is reached, change to SameMajorVersion
    )

    install(
        FILES ${VERSION_FILE_PATH} ${CONFIG_FILE_PATH}
        DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}/cmake/${PACKAGE_NAME}"
    )

    export(
        TARGETS ${TARGET_NAME}
        NAMESPACE "${NAMESPACE}::"
        FILE "${TARGETS_FILE_PATH}"
    )
endif()
