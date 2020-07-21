cmake_minimum_required(VERSION 3.2)
project(python)

set(CMAKE_CXX_STANDARD  11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

set(CMAKE_CXX_FLAGS "-g -pthread ${CMAKE_CXX_FLAGS}")

find_package(Boost REQUIRED COMPONENTS
        python
        )

set(Python_INCLUDE_DIRS
        /usr/include/python2.7
        /usr/include/x86_64-linux-gnu/python2.7
        )

include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${Boost_INCLUDE_DIRS}
        ${Python_INCLUDE_DIRS}
)

add_library(oyoung_pyexec SHARED
        src/executor.cc
        )

target_link_libraries(oyoung_pyexec
        python2.7
        ${Boost_LIBRARIES}
        )

add_executable(app
        src/main.cc
        )

target_link_libraries(app
        oyoung_pyexec
        )

install(DIRECTORY
        ${PROJECT_SOURCE_DIR}/include/ DESTINATION include
        )

install(TARGETS oyoung_pyexec
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        )


