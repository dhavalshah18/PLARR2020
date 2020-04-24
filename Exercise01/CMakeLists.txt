cmake_minimum_required(VERSION 3.3)
project(exercise01)

###########################################################################################

## OpenCV
find_package(OpenCV 3 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_definitions( -DDATA_DIR="${PROJECT_SOURCE_DIR}/data"  )

###########################################################################################


# TARGETS
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

