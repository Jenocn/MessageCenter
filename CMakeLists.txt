cmake_minimum_required(VERSION 3.30)

project("MessageCenter" CXX)
set(MESSAGE_CENTER "MessageCenter")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	add_compile_options("/Wall" "/wd5045" "/wd4711" "/wd4710")
	add_compile_options("/EHsc" "/permissive-")
	add_compile_options("/MT")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(CXXFLAGS "-Wall" "-Wextra" "-pedantic")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	set(CXXFLAGS "-Wall" "-Wextra" "-pedantic")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

file(GLOB_RECURSE SOURCES "./src/*.cpp" "./include/*.h")
add_library(${MESSAGE_CENTER} STATIC ${SOURCES})

add_executable("test" ${SOURCES} "./test/main.cpp")

include_directories("./include")