// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from baseinterface_pkg:msg/Usrmsg.idl
// generated code does not contain a copyright notice

#ifndef BASEINTERFACE_PKG__MSG__DETAIL__USRMSG__STRUCT_H_
#define BASEINTERFACE_PKG__MSG__DETAIL__USRMSG__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'name'
#include "rosidl_runtime_c/string.h"

/// Struct defined in msg/Usrmsg in the package baseinterface_pkg.
typedef struct baseinterface_pkg__msg__Usrmsg
{
  rosidl_runtime_c__String name;
  int64_t age;
  double height;
} baseinterface_pkg__msg__Usrmsg;

// Struct for a sequence of baseinterface_pkg__msg__Usrmsg.
typedef struct baseinterface_pkg__msg__Usrmsg__Sequence
{
  baseinterface_pkg__msg__Usrmsg * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} baseinterface_pkg__msg__Usrmsg__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // BASEINTERFACE_PKG__MSG__DETAIL__USRMSG__STRUCT_H_
