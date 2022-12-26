cmake_minimum_required(VERSION 3.5)
project(caculate)


# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()



if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()
include_directories(
  include
  ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}
)
# find dependencies
find_package(ament_cmake REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rclcpp REQUIRED)
# ament_auto_find_build_dependencies()
# ament_auto_add_executable(
#     caculate
#     src/get_score.cpp
# )
add_executable(caculate src/get_score.cpp)
ament_target_dependencies(caculate rclcpp std_msgs)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()
install(TARGETS
  caculate
  DESTINATION lib/${PROJECT_NAME}/
)
ament_package()
