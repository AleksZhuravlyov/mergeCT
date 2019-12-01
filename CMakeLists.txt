cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)

set(POJECT_NAME mergeCT)

project(${POJECT_NAME})

find_package(netCDF)

set(SOURCE_CODE main.cpp mergeNcFiles.cpp mergeNcFiles.h)

include_directories(.)

find_package(Boost COMPONENTS program_options REQUIRED)

add_executable(${POJECT_NAME} ${SOURCE_CODE})

target_link_libraries(${POJECT_NAME}
        /usr/local/lib/libnetcdf-cxx4.dylib
        Boost::program_options)