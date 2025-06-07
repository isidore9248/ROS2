// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from baseinterface_pkg:msg/Usrmsg.idl
// generated code does not contain a copyright notice

#ifndef BASEINTERFACE_PKG__MSG__DETAIL__USRMSG__BUILDER_HPP_
#define BASEINTERFACE_PKG__MSG__DETAIL__USRMSG__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "baseinterface_pkg/msg/detail/usrmsg__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace baseinterface_pkg
{

namespace msg
{

namespace builder
{

class Init_Usrmsg_height
{
public:
  explicit Init_Usrmsg_height(::baseinterface_pkg::msg::Usrmsg & msg)
  : msg_(msg)
  {}
  ::baseinterface_pkg::msg::Usrmsg height(::baseinterface_pkg::msg::Usrmsg::_height_type arg)
  {
    msg_.height = std::move(arg);
    return std::move(msg_);
  }

private:
  ::baseinterface_pkg::msg::Usrmsg msg_;
};

class Init_Usrmsg_age
{
public:
  explicit Init_Usrmsg_age(::baseinterface_pkg::msg::Usrmsg & msg)
  : msg_(msg)
  {}
  Init_Usrmsg_height age(::baseinterface_pkg::msg::Usrmsg::_age_type arg)
  {
    msg_.age = std::move(arg);
    return Init_Usrmsg_height(msg_);
  }

private:
  ::baseinterface_pkg::msg::Usrmsg msg_;
};

class Init_Usrmsg_name
{
public:
  Init_Usrmsg_name()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Usrmsg_age name(::baseinterface_pkg::msg::Usrmsg::_name_type arg)
  {
    msg_.name = std::move(arg);
    return Init_Usrmsg_age(msg_);
  }

private:
  ::baseinterface_pkg::msg::Usrmsg msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::baseinterface_pkg::msg::Usrmsg>()
{
  return baseinterface_pkg::msg::builder::Init_Usrmsg_name();
}

}  // namespace baseinterface_pkg

#endif  // BASEINTERFACE_PKG__MSG__DETAIL__USRMSG__BUILDER_HPP_
