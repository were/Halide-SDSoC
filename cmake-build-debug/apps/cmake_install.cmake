# Install script for directory: /Users/were/Halide-SDSoC/apps

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
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

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/were/Halide-SDSoC/cmake-build-debug/apps/bilateral_grid/cmake_install.cmake")
  include("/Users/were/Halide-SDSoC/cmake-build-debug/apps/blur/cmake_install.cmake")
  include("/Users/were/Halide-SDSoC/cmake-build-debug/apps/c_backend/cmake_install.cmake")
  include("/Users/were/Halide-SDSoC/cmake-build-debug/apps/camera_pipe/cmake_install.cmake")
  include("/Users/were/Halide-SDSoC/cmake-build-debug/apps/glsl/cmake_install.cmake")
  include("/Users/were/Halide-SDSoC/cmake-build-debug/apps/interpolate/cmake_install.cmake")
  include("/Users/were/Halide-SDSoC/cmake-build-debug/apps/local_laplacian/cmake_install.cmake")

endif()

