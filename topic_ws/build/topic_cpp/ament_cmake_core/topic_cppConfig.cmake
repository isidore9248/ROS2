# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_topic_cpp_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED topic_cpp_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(topic_cpp_FOUND FALSE)
  elseif(NOT topic_cpp_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(topic_cpp_FOUND FALSE)
  endif()
  return()
endif()
set(_topic_cpp_CONFIG_INCLUDED TRUE)

# output package information
if(NOT topic_cpp_FIND_QUIETLY)
  message(STATUS "Found topic_cpp: 0.0.1 (${topic_cpp_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'topic_cpp' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${topic_cpp_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(topic_cpp_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${topic_cpp_DIR}/${_extra}")
endforeach()
