// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from baseinterface_pkg:msg/Usrmsg.idl
// generated code does not contain a copyright notice
#include "baseinterface_pkg/msg/detail/usrmsg__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `name`
#include "rosidl_runtime_c/string_functions.h"

bool
baseinterface_pkg__msg__Usrmsg__init(baseinterface_pkg__msg__Usrmsg * msg)
{
  if (!msg) {
    return false;
  }
  // name
  if (!rosidl_runtime_c__String__init(&msg->name)) {
    baseinterface_pkg__msg__Usrmsg__fini(msg);
    return false;
  }
  // age
  // height
  return true;
}

void
baseinterface_pkg__msg__Usrmsg__fini(baseinterface_pkg__msg__Usrmsg * msg)
{
  if (!msg) {
    return;
  }
  // name
  rosidl_runtime_c__String__fini(&msg->name);
  // age
  // height
}

bool
baseinterface_pkg__msg__Usrmsg__are_equal(const baseinterface_pkg__msg__Usrmsg * lhs, const baseinterface_pkg__msg__Usrmsg * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // name
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->name), &(rhs->name)))
  {
    return false;
  }
  // age
  if (lhs->age != rhs->age) {
    return false;
  }
  // height
  if (lhs->height != rhs->height) {
    return false;
  }
  return true;
}

bool
baseinterface_pkg__msg__Usrmsg__copy(
  const baseinterface_pkg__msg__Usrmsg * input,
  baseinterface_pkg__msg__Usrmsg * output)
{
  if (!input || !output) {
    return false;
  }
  // name
  if (!rosidl_runtime_c__String__copy(
      &(input->name), &(output->name)))
  {
    return false;
  }
  // age
  output->age = input->age;
  // height
  output->height = input->height;
  return true;
}

baseinterface_pkg__msg__Usrmsg *
baseinterface_pkg__msg__Usrmsg__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  baseinterface_pkg__msg__Usrmsg * msg = (baseinterface_pkg__msg__Usrmsg *)allocator.allocate(sizeof(baseinterface_pkg__msg__Usrmsg), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(baseinterface_pkg__msg__Usrmsg));
  bool success = baseinterface_pkg__msg__Usrmsg__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
baseinterface_pkg__msg__Usrmsg__destroy(baseinterface_pkg__msg__Usrmsg * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    baseinterface_pkg__msg__Usrmsg__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
baseinterface_pkg__msg__Usrmsg__Sequence__init(baseinterface_pkg__msg__Usrmsg__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  baseinterface_pkg__msg__Usrmsg * data = NULL;

  if (size) {
    data = (baseinterface_pkg__msg__Usrmsg *)allocator.zero_allocate(size, sizeof(baseinterface_pkg__msg__Usrmsg), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = baseinterface_pkg__msg__Usrmsg__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        baseinterface_pkg__msg__Usrmsg__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
baseinterface_pkg__msg__Usrmsg__Sequence__fini(baseinterface_pkg__msg__Usrmsg__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      baseinterface_pkg__msg__Usrmsg__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

baseinterface_pkg__msg__Usrmsg__Sequence *
baseinterface_pkg__msg__Usrmsg__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  baseinterface_pkg__msg__Usrmsg__Sequence * array = (baseinterface_pkg__msg__Usrmsg__Sequence *)allocator.allocate(sizeof(baseinterface_pkg__msg__Usrmsg__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = baseinterface_pkg__msg__Usrmsg__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
baseinterface_pkg__msg__Usrmsg__Sequence__destroy(baseinterface_pkg__msg__Usrmsg__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    baseinterface_pkg__msg__Usrmsg__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
baseinterface_pkg__msg__Usrmsg__Sequence__are_equal(const baseinterface_pkg__msg__Usrmsg__Sequence * lhs, const baseinterface_pkg__msg__Usrmsg__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!baseinterface_pkg__msg__Usrmsg__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
baseinterface_pkg__msg__Usrmsg__Sequence__copy(
  const baseinterface_pkg__msg__Usrmsg__Sequence * input,
  baseinterface_pkg__msg__Usrmsg__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(baseinterface_pkg__msg__Usrmsg);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    baseinterface_pkg__msg__Usrmsg * data =
      (baseinterface_pkg__msg__Usrmsg *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!baseinterface_pkg__msg__Usrmsg__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          baseinterface_pkg__msg__Usrmsg__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!baseinterface_pkg__msg__Usrmsg__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
