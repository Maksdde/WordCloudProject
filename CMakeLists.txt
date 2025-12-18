cmake_minimum_required(VERSION 3.16)
project(WordCloudApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)

add_library(WordCloudLib
    WordCloudGenerator.cpp
    WordCloudGenerator.h
)

target_link_libraries(WordCloudLib PUBLIC Qt6::Core Qt6::Gui)

add_executable(WordCloud main.cpp)
target_link_libraries(WordCloud PRIVATE WordCloudLib)

FetchContent_MakeAvailable(googletest)

add_executable(WordCloudTests tests/test_wordcloud.cpp)
target_link_libraries(WordCloudTests PRIVATE WordCloudLib gtest_main)

