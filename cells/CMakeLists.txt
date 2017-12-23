cmake_minimum_required(VERSION 2.8.0 FATAL_ERROR) 
# Проверка версии CMake.
# Если версия установленой программы
# старее указаной, произайдёт аварийный выход.


project(cells)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories(cells)

add_executable(cells
        cells/Source.cpp)

target_link_libraries(cells ${OpenCV_LIBS} )
