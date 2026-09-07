cmake_minimum_required(VERSION 3.8)
project(tactile_nodes)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(tactile_interfaces REQUIRED)
find_package(std_msgs REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()


# 1. Add the Tactile Sensor Driver executable
add_executable(tactile_sensor_driver src/tactile_sensor_driver.cpp)
ament_target_dependencies(tactile_sensor_driver rclcpp tactile_interfaces)

# 2. Add the Slip Detector executable
add_executable(slip_detector src/slip_detector.cpp)
ament_target_dependencies(slip_detector rclcpp tactile_interfaces)

# 3. Install the executables so ROS2 can find them
install(TARGETS
  tactile_sensor_driver
  slip_detector
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
