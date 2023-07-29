cmake_minimum_required(VERSION 3.16.0)

#set(CMAKE_C_COMPILER clang)
#set(CMAKE_CXX_COMPILER clang++)

project(ZTableConvert CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ZTC_BUILD_SHARED_LIB "编译动态库" OFF)
option(ZTC_BUILD_STATIC_LIB "编译静态库" ON)
set(ZTC_SHARED_LIB_NAME "ZTableConvert")
set(ZTC_STATIC_LIB_NAME "ZTableConvert")

if (NOT MSVC)
    add_compile_options(-Werror)
    if (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)

    endif ()
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls)
    add_link_options(-fsanitize=address)
else ()
    add_compile_options(/Wall #[[/fsanitize=address]])
    add_link_options(#[[/fsanitize=address]])
endif ()
add_compile_definitions(DU_DEBUG)


set(ZTC_SRC_FILE src/TableAST.hpp src/Convert.hpp src/Pos.hpp)

if (ZTC_BUILD_SHARED_LIB)
    add_library(
            ${ZTC_SHARED_LIB_NAME}
            SHARED

            ${ZTC_SRC_FILE}
    )

    target_link_directories(${ZTC_SHARED_LIB_NAME} PRIVATE m)
    target_include_directories(${ZTC_SHARED_LIB_NAME} INTERFACE .)
endif (ZTC_BUILD_SHARED_LIB)


if (ZTC_BUILD_STATIC_LIB)
    add_library(
            ${ZTC_STATIC_LIB_NAME}
            OBJECT

            ${ZTC_SRC_FILE}
    )

    target_include_directories(${ZTC_STATIC_LIB_NAME} INTERFACE .)
    target_link_directories(${ZTC_STATIC_LIB_NAME} INTERFACE m)
endif (ZTC_BUILD_STATIC_LIB)

add_executable(
    test
    
    main.cpp
    ${ZTC_SRC_FILE}
)