cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)

set(POJECT_NAME mergeCT)

project(${POJECT_NAME})

set(SOURCE_CODE main.cpp)

include_directories(/usr/local/Cellar/netcdf/4.6.3_1/include)

find_package(Boost 1.40 COMPONENTS program_options REQUIRED)

add_executable(${POJECT_NAME} ${SOURCE_CODE})

target_link_libraries(${POJECT_NAME}
        /usr/local/lib/libnetcdf-cxx4.dylib
        Boost::program_options)