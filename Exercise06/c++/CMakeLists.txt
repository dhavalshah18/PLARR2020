cmake_minimum_required(VERSION 3.3)
project(exercise06)

###########################################################################################

## Open3D
find_package(Open3D REQUIRED)
include_directories(${Open3D_INCLUDE_DIRS})

add_definitions( -DDATA_DIR="${PROJECT_SOURCE_DIR}/data"  )


###########################################################################################


# TARGETS
add_executable(${PROJECT_NAME}_01 src/Exercise06_1/main.cpp)
target_link_libraries(${PROJECT_NAME}_01 ${Open3D_LIBRARIES})

add_executable(${PROJECT_NAME}_02 src/Exercise06_2/main.cpp)
target_link_libraries(${PROJECT_NAME}_02 ${Open3D_LIBRARIES})

add_executable(${PROJECT_NAME}_03 src/Exercise06_3/main.cpp)
target_link_libraries(${PROJECT_NAME}_03 ${Open3D_LIBRARIES})

