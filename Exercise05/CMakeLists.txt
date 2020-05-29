cmake_minimum_required(VERSION 3.3)
project(exercise_05)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

###########################################################################################
## OpenCV
FIND_PACKAGE(OpenCV REQUIRED)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})

## Eigen3
FIND_PACKAGE(Eigen3 REQUIRED)

###########################################################################################
# TARGETS
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} Eigen3::Eigen ${OpenCV_LIBS})
target_compile_definitions(${PROJECT_NAME} PUBLIC -DDATA_DIR="${PROJECT_SOURCE_DIR}/data")