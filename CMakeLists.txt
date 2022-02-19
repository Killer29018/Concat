cmake_minimum_required(VERSION 3.18)

project (SBIMCL)

set (CMAKE_CXX_STANDARD 17)

set (linker "")

set (CMAKE_BINARY_DIR "/bin")
set (OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}${CMAKE_BINARY_DIR}")

if (CMAKE_BUILD_TYPE MATCHES Debug)
    message  ("Building Debug")
    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIRECTORY}/debug")
else ()
    message ("Building Release")
    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIRECTORY}/release")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-unknown-pragmas")

file (GLOB_RECURSE cppFiles CONFIGURE_DEPENDS "src/*.cpp")
add_executable(${PROJECT_NAME} ${cppFiles})

# target_include_directories(${PROJECT_NAME} public)

target_link_libraries(${PROJECT_NAME} PRIVATE ${linker})
