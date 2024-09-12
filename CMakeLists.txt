cmake_minimum_required(VERSION 3.30)

project("MessageCenter" CXX)

set(LIB_NAME ${PROJECT_NAME})
set(SRC_PATH "src")


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

aux_source_directory(${SRC_PATH} SRC_FILES)

add_library(${LIB_NAME} STATIC ${SRC_FILES})

include_directories(${SRC_PATH})