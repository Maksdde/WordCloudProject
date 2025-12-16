cmake_minimum_required(VERSION 3.16)
project(WordCloudApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui)

add_executable(WordCloud
    main.cpp
    WordCloudGenerator.cpp
    WordCloudGenerator.h
)

target_link_libraries(WordCloud PRIVATE Qt6::Core Qt6::Gui)

enable_testing()

set(GTEST_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/google/googletest-1.14.0/googletest)

add_library(gtest STATIC
    ${GTEST_SRC_DIR}/src/gtest-all.cc
)

target_include_directories(gtest PUBLIC
    ${GTEST_SRC_DIR}
    ${GTEST_SRC_DIR}/include
)

add_library(gtest_main STATIC
    ${GTEST_SRC_DIR}/src/gtest_main.cc
)

target_link_libraries(gtest_main PRIVATE gtest)

add_executable(WordCloudTests
    tests/test_wordcloud.cpp
    WordCloudGenerator.cpp
    WordCloudGenerator.h
)

target_include_directories(WordCloudTests PRIVATE
    ${GTEST_SRC_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(WordCloudTests PRIVATE
    Qt6::Core
    Qt6::Gui
    gtest_main
)

add_test(NAME WordCloudTests COMMAND WordCloudTests)