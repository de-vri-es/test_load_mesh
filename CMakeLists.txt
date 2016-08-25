project(test_load_mesh)
cmake_minimum_required(VERSION 2.8.3)

find_package(catkin REQUIRED COMPONENTS
	roscpp
	shape_msgs
	geometric_shapes
)

catkin_package()

include_directories(SYSTEM ${catkin_INCLUDE_DIRS})

add_executable(test_load_mesh src/main.cpp)
target_link_libraries(test_load_mesh ${catkin_LIBRARIES})
