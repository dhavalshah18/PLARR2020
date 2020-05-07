cmake_minimum_required(VERSION 3.3)
project(exercise_02)

###########################################################################################

## OpenCV
find_package(OpenCV 3.0 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_definitions( -DDATA_DIR="${PROJECT_SOURCE_DIR}/data")
add_definitions( -DOUTPUT_DIR="${PROJECT_SOURCE_DIR}/output")

###########################################################################################


# TARGETS
add_executable(${PROJECT_NAME}_bonus src/exercise2_bonus.cpp)
target_link_libraries(${PROJECT_NAME}_bonus ${OpenCV_LIBS})

add_executable(${PROJECT_NAME} src/exercise2.cpp)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
