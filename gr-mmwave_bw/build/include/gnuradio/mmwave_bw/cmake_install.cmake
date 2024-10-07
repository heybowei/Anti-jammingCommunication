# Install script for directory: /home/cnss/gr-mmwave_bw/include/gnuradio/mmwave_bw

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
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
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

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/gnuradio/mmwave_bw" TYPE FILE FILES
    "/home/cnss/gr-mmwave_bw/include/gnuradio/mmwave_bw/api.h"
    "/home/cnss/gr-mmwave_bw/include/gnuradio/mmwave_bw/tx_qam_source_rand.h"
    "/home/cnss/gr-mmwave_bw/include/gnuradio/mmwave_bw/tx_freq_frame.h"
    "/home/cnss/gr-mmwave_bw/include/gnuradio/mmwave_bw/tx_ifft.h"
    "/home/cnss/gr-mmwave_bw/include/gnuradio/mmwave_bw/tx_time_frame.h"
    "/home/cnss/gr-mmwave_bw/include/gnuradio/mmwave_bw/rx_frame_detect.h"
    "/home/cnss/gr-mmwave_bw/include/gnuradio/mmwave_bw/rx_fft.h"
    "/home/cnss/gr-mmwave_bw/include/gnuradio/mmwave_bw/rx_frame_equalization_csi.h"
    "/home/cnss/gr-mmwave_bw/include/gnuradio/mmwave_bw/rx_deqam.h"
    )
endif()

