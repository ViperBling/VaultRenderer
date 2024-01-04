include_directories(${PROJECT_SOURCE_DIR}/Source)

find_package(GTest CONFIG REQUIRED)

set(GTestLib GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
set(MainFile MainTest.cpp)

add_executable(EngineTest ${MainFile} EngineTest.cpp)
target_link_libraries(EngineTest ${GTestLib} FrameworkLib)

target_include_directories(EngineTest PUBLIC ${PROJECT_SOURCE_DIR}/Source)