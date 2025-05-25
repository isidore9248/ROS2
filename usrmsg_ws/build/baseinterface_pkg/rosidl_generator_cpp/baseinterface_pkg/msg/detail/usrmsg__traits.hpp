// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from baseinterface_pkg:msg/Usrmsg.idl
// generated code does not contain a copyright notice

#ifndef BASEINTERFACE_PKG__MSG__DETAIL__USRMSG__TRAITS_HPP_
#define BASEINTERFACE_PKG__MSG__DETAIL__USRMSG__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "baseinterface_pkg/msg/detail/usrmsg__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace baseinterface_pkg
{

namespace msg
{

inline void to_flow_style_yaml(
  const Usrmsg & msg,
  std::ostream & out)
{
  out << "{";
  // member: name
  {
    out << "name: ";
    rosidl_generator_traits::value_to_yaml(msg.name, out);
    out << ", ";
  }

  // member: age
  {
    out << "age: ";
    rosidl_generator_traits::value_to_yaml(msg.age, out);
    out << ", ";
  }

  // member: height
  {
    out << "height: ";
    rosidl_generator_traits::value_to_yaml(msg.height, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const Usrmsg & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: name
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "name: ";
    rosidl_generator_traits::value_to_yaml(msg.name, out);
    out << "\n";
  }

  // member: age
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "age: ";
    rosidl_generator_traits::value_to_yaml(msg.age, out);
    out << "\n";
  }

  // member: height
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "height: ";
    rosidl_generator_traits::value_to_yaml(msg.height, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const Usrmsg & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace baseinterface_pkg

namespace rosidl_generator_traits
{

[[deprecated("use baseinterface_pkg::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const baseinterface_pkg::msg::Usrmsg & msg,
  std::ostream & out, size_t indentation = 0)
{
  baseinterface_pkg::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use baseinterface_pkg::msg::to_yaml() instead")]]
inline std::string to_yaml(const baseinterface_pkg::msg::Usrmsg & msg)
{
  return baseinterface_pkg::msg::to_yaml(msg);
}

template<>
inline const char * data_type<baseinterface_pkg::msg::Usrmsg>()
{
  return "baseinterface_pkg::msg::Usrmsg";
}

template<>
inline const char * name<baseinterface_pkg::msg::Usrmsg>()
{
  return "baseinterface_pkg/msg/Usrmsg";
}

template<>
struct has_fixed_size<baseinterface_pkg::msg::Usrmsg>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<baseinterface_pkg::msg::Usrmsg>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<baseinterface_pkg::msg::Usrmsg>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // BASEINTERFACE_PKG__MSG__DETAIL__USRMSG__TRAITS_HPP_
