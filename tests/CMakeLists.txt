include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(test_basic test_basic.cpp)
target_link_libraries(test_basic PRIVATE markdownify gtest_main)

add_executable(test_conversions test_conversions.cpp)
target_link_libraries(test_conversions PRIVATE markdownify gtest_main)

include(GoogleTest)
gtest_discover_tests(test_basic)
gtest_discover_tests(test_conversions)