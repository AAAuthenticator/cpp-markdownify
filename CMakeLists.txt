cmake_minimum_required(VERSION 3.14)
project(markdownify VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(MARKDOWNIFY_BUILD_EXAMPLES "Build examples" ON)
option(MARKDOWNIFY_BUILD_TESTS "Build tests" ON)
option(MARKDOWNIFY_USE_SYSTEM_TINYXML2 "Use system TinyXML2" OFF)

include(FetchContent)

if(NOT MARKDOWNIFY_USE_SYSTEM_TINYXML2)
    FetchContent_Declare(
        tinyxml2
        GIT_REPOSITORY https://github.com/leethomason/tinyxml2.git
        GIT_TAG 9.0.0
    )
    FetchContent_MakeAvailable(tinyxml2)
else()
    find_package(tinyxml2 REQUIRED)
endif()

set(MARKDOWNIFY_SOURCES
    src/converter.cpp
    src/text_utils.cpp
)

set(MARKDOWNIFY_HEADERS
    include/markdownify/markdownify.hpp
    include/markdownify/converter.hpp
    include/markdownify/options.hpp
    include/markdownify/text_utils.hpp
)

add_library(markdownify ${MARKDOWNIFY_SOURCES} ${MARKDOWNIFY_HEADERS})
target_include_directories(markdownify
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_link_libraries(markdownify PUBLIC tinyxml2::tinyxml2)

if(MARKDOWNIFY_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(MARKDOWNIFY_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

install(TARGETS markdownify
    EXPORT markdownify-targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/markdownify
    DESTINATION include
)

install(EXPORT markdownify-targets
    FILE markdownify-config.cmake
    NAMESPACE markdownify::
    DESTINATION lib/cmake/markdownify
)