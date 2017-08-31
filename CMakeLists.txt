cmake_minimum_required(VERSION 2.4.0 FATAL_ERROR)

project(UNITdemo)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O0")
include_directories(${PROJECT_SOURCE_DIR}/externals/rapidjson/include ${PROJECT_SOURCE_DIR}/externals/curl)
link_libraries(curl)


set(SRC_FILES
    src/AccessToken.cpp
    src/Utterance.cpp
    src/base64.cpp
    src/main.cpp
    )

set(HEAD_FILES
    src/AccessToken.h
    src/Utterance.h
    src/base64.h
    )

set(SRCFILES ${SRC_FILES} ${HEAD_FILES})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_executable(${PROJECT_NAME} ${SRCFILES})
