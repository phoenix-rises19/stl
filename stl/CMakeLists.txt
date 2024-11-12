CMAKE_MINIMUM_REQUIRED(VERSION 3.27)
PROJECT(stl)

SET(CMAKE_CXX_STANDARD 23)

INCLUDE_DIRECTORIES(./)
include_directories(${GTEST_INCLUDE_DIRS})
find_package(GTest REQUIRED)

ENABLE_TESTING()

SET(VAI_SRC
        vai/vector.hpp vai/map.hpp vai/list.hpp)

ADD_EXECUTABLE(main
        ${VAI_SRC}
        src/main.cpp)

ADD_EXECUTABLE(vectorTest test/vector_test.cpp)
ADD_EXECUTABLE(listTest test/list_test.cpp)
TARGET_LINK_LIBRARIES(vectorTest GTest::GTest GTest::Main)
TARGET_LINK_LIBRARIES(listTest GTest::GTest GTest::Main)
ADD_TEST(NAME VectorTest COMMAND runTests)
ADD_TEST(NAME ListTest COMMAND listTest)


