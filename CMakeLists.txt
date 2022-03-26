cmake_minimum_required(VERSION 3.18)

project (Concat)

set (CMAKE_CXX_STANDARD 17)

set (linker "")

set (OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIRECTORY}")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")

if (CMAKE_BUILD_TYPE MATCHES Debug)
    message  ("Building Debug")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    add_compile_definitions(PRINT_DEBUG_TOKENS)
elseif (CMAKE_BUILD_TYPE MATCHES Profile)
    message  ("Building Profile")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -no-pie -pg -g")
    add_compile_definitions(PRINT_DEBUG_TOKENS)
else ()
    message ("Building Release")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wno-unknown-pragmas")

add_subdirectory(vendor)

file (GLOB_RECURSE cppFiles CONFIGURE_DEPENDS "src/*.cpp")
add_executable(${PROJECT_NAME} ${cppFiles})

target_include_directories(${PROJECT_NAME} PUBLIC
    vendor/cxxopts/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${linker})
