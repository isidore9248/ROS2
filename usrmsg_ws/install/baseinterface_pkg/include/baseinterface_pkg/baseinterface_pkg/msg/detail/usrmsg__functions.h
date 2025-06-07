// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from baseinterface_pkg:msg/Usrmsg.idl
// generated code does not contain a copyright notice

#ifndef BASEINTERFACE_PKG__MSG__DETAIL__USRMSG__FUNCTIONS_H_
#define BASEINTERFACE_PKG__MSG__DETAIL__USRMSG__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "baseinterface_pkg/msg/rosidl_generator_c__visibility_control.h"

#include "baseinterface_pkg/msg/detail/usrmsg__struct.h"

/// Initialize msg/Usrmsg message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * baseinterface_pkg__msg__Usrmsg
 * )) before or use
 * baseinterface_pkg__msg__Usrmsg__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_baseinterface_pkg
bool
baseinterface_pkg__msg__Usrmsg__init(baseinterface_pkg__msg__Usrmsg * msg);

/// Finalize msg/Usrmsg message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_baseinterface_pkg
void
baseinterface_pkg__msg__Usrmsg__fini(baseinterface_pkg__msg__Usrmsg * msg);

/// Create msg/Usrmsg message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * baseinterface_pkg__msg__Usrmsg__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_baseinterface_pkg
baseinterface_pkg__msg__Usrmsg *
baseinterface_pkg__msg__Usrmsg__create();

/// Destroy msg/Usrmsg message.
/**
 * It calls
 * baseinterface_pkg__msg__Usrmsg__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_baseinterface_pkg
void
baseinterface_pkg__msg__Usrmsg__destroy(baseinterface_pkg__msg__Usrmsg * msg);

/// Check for msg/Usrmsg message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_baseinterface_pkg
bool
baseinterface_pkg__msg__Usrmsg__are_equal(const baseinterface_pkg__msg__Usrmsg * lhs, const baseinterface_pkg__msg__Usrmsg * rhs);

/// Copy a msg/Usrmsg message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_baseinterface_pkg
bool
baseinterface_pkg__msg__Usrmsg__copy(
  const baseinterface_pkg__msg__Usrmsg * input,
  baseinterface_pkg__msg__Usrmsg * output);

/// Initialize array of msg/Usrmsg messages.
/**
 * It allocates the memory for the number of elements and calls
 * baseinterface_pkg__msg__Usrmsg__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_baseinterface_pkg
bool
baseinterface_pkg__msg__Usrmsg__Sequence__init(baseinterface_pkg__msg__Usrmsg__Sequence * array, size_t size);

/// Finalize array of msg/Usrmsg messages.
/**
 * It calls
 * baseinterface_pkg__msg__Usrmsg__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_baseinterface_pkg
void
baseinterface_pkg__msg__Usrmsg__Sequence__fini(baseinterface_pkg__msg__Usrmsg__Sequence * array);

/// Create array of msg/Usrmsg messages.
/**
 * It allocates the memory for the array and calls
 * baseinterface_pkg__msg__Usrmsg__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_baseinterface_pkg
baseinterface_pkg__msg__Usrmsg__Sequence *
baseinterface_pkg__msg__Usrmsg__Sequence__create(size_t size);

/// Destroy array of msg/Usrmsg messages.
/**
 * It calls
 * baseinterface_pkg__msg__Usrmsg__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_baseinterface_pkg
void
baseinterface_pkg__msg__Usrmsg__Sequence__destroy(baseinterface_pkg__msg__Usrmsg__Sequence * array);

/// Check for msg/Usrmsg message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_baseinterface_pkg
bool
baseinterface_pkg__msg__Usrmsg__Sequence__are_equal(const baseinterface_pkg__msg__Usrmsg__Sequence * lhs, const baseinterface_pkg__msg__Usrmsg__Sequence * rhs);

/// Copy an array of msg/Usrmsg messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_baseinterface_pkg
bool
baseinterface_pkg__msg__Usrmsg__Sequence__copy(
  const baseinterface_pkg__msg__Usrmsg__Sequence * input,
  baseinterface_pkg__msg__Usrmsg__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // BASEINTERFACE_PKG__MSG__DETAIL__USRMSG__FUNCTIONS_H_
