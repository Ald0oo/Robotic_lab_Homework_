cmake_minimum_required(VERSION 3.8)
project(arm_controller)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)


add_executable(arm_controller_node src/arm_controller_node.cpp)
 
ament_target_dependencies(arm_controller_node rclcpp sensor_msgs std_msgs)


ament_package()

install(TARGETS
  arm_controller_node
  DESTINATION lib/${PROJECT_NAME}
)
