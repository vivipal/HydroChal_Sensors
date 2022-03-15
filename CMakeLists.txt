cmake_minimum_required(VERSION 3.0) # setting this is required
project(HC_sensors)            # this sets the project name


add_subdirectory(RXFromRC_cpp/raspberry-sbus)


add_executable(IMU_exe IMU/main.c)
set_target_properties(IMU_exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../executable/")
add_executable(logger_exe Logger/main.c)
set_target_properties(logger_exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../executable/")
add_executable(WS_exe WeatherStation/main.c)
set_target_properties(WS_exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../executable/")
add_executable(rx_exe RXFromRC_cpp//main.cpp)
set_target_properties(rx_exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../executable/")
target_link_libraries(rx_exe PUBLIC libsbus)
