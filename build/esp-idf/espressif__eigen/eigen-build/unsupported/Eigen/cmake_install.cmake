# Install script for directory: /home/shashwat/drone_flight_control/managed_components/espressif__eigen/eigen/unsupported/Eigen

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/shashwat/drone_flight_control/build/esp-idf/espressif__eigen/eigen-build/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/home/shashwat/.espressif/tools/xtensa-esp-elf/esp-13.2.0_20230928/xtensa-esp-elf/bin/xtensa-esp32-elf-objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/unsupported/Eigen" TYPE FILE FILES
    "/home/shashwat/drone_flight_control/managed_components/espressif__eigen/eigen/unsupported/Eigen/AdolcForward"
    "/home/shashwat/drone_flight_control/managed_components/espressif__eigen/eigen/unsupported/Eigen/AlignedVector3"
    "/home/shashwat/drone_flight_control/managed_components/espressif__eigen/eigen/unsupported/Eigen/ArpackSupport"
    "/home/shashwat/drone_flight_control/managed_components/espressif__eigen/eigen/unsupported/Eigen/AutoDiff"
    "/home/shashwat/drone_flight_control/managed_components/espressif__eigen/eigen/unsupported/Eigen/BVH"
    "/home/shashwat/drone_flight_control/managed_components/espressif__eigen/eigen/unsupported/Eigen/EulerAngles"
    "/home/shashwat/drone_flight_control/managed_components/espressif__eigen/eigen/unsupported/Eigen/FFT"
    "/home/shashwat/drone_flight_control/managed_components/espressif__eigen/eigen/unsupported/Eigen/IterativeSolvers"
    "/home/shashwat/drone_flight_control/managed_components/espressif__eigen/eigen/unsupported/Eigen/KroneckerProduct"
    "/home/shashwat/drone_flight_control/managed_components/espressif__eigen/eigen/unsupported/Eigen/LevenbergMarquardt"
    "/home/shashwat/drone_flight_control/managed_components/espressif__eigen/eigen/unsupported/Eigen/MatrixFunctions"
    "/home/shashwat/drone_flight_control/managed_components/espressif__eigen/eigen/unsupported/Eigen/MoreVectorization"
    "/home/shashwat/drone_flight_control/managed_components/espressif__eigen/eigen/unsupported/Eigen/MPRealSupport"
    "/home/shashwat/drone_flight_control/managed_components/espressif__eigen/eigen/unsupported/Eigen/NonLinearOptimization"
    "/home/shashwat/drone_flight_control/managed_components/espressif__eigen/eigen/unsupported/Eigen/NumericalDiff"
    "/home/shashwat/drone_flight_control/managed_components/espressif__eigen/eigen/unsupported/Eigen/OpenGLSupport"
    "/home/shashwat/drone_flight_control/managed_components/espressif__eigen/eigen/unsupported/Eigen/Polynomials"
    "/home/shashwat/drone_flight_control/managed_components/espressif__eigen/eigen/unsupported/Eigen/Skyline"
    "/home/shashwat/drone_flight_control/managed_components/espressif__eigen/eigen/unsupported/Eigen/SparseExtra"
    "/home/shashwat/drone_flight_control/managed_components/espressif__eigen/eigen/unsupported/Eigen/SpecialFunctions"
    "/home/shashwat/drone_flight_control/managed_components/espressif__eigen/eigen/unsupported/Eigen/Splines"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/unsupported/Eigen" TYPE DIRECTORY FILES "/home/shashwat/drone_flight_control/managed_components/espressif__eigen/eigen/unsupported/Eigen/src" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/shashwat/drone_flight_control/build/esp-idf/espressif__eigen/eigen-build/unsupported/Eigen/CXX11/cmake_install.cmake")

endif()

