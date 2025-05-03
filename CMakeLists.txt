cmake_minimum_required(VERSION 3.14)
project(pigment VERSION 0.0.1 LANGUAGES CXX)

# -----------------------------------------------------------------------------
# 1) Create the header‐only INTERFACE library
# -----------------------------------------------------------------------------
add_library(pigment INTERFACE)

# Allow users to link via `pigment::pigment`
add_library(pigment::pigment ALIAS pigment)

# -----------------------------------------------------------------------------
# 2) Public include path
# -----------------------------------------------------------------------------
# This makes <pigment/base_type.h> etc. resolve to include/pigment/...
target_include_directories(pigment
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# -----------------------------------------------------------------------------
# 3) Installation & Export for find_package()
# -----------------------------------------------------------------------------
include(GNUInstallDirs)

# 3a) Install headers
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# 3b) Install and export the INTERFACE target
install(TARGETS pigment EXPORT pigmentTargets)

# 3c) Install the export file so that `find_package(pigment)` works
install(EXPORT pigmentTargets
  FILE pigmentTargets.cmake
  NAMESPACE pigment::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pigment
)

# 3d) Also export it for in‐source FetchContent usage
export(EXPORT pigmentTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/pigmentTargets.cmake"
  NAMESPACE pigment::
)

