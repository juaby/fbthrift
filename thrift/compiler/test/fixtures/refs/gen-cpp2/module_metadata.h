/**
 * Autogenerated by Thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#pragma once

#include <thrift/lib/cpp2/gen/module_metadata_h.h>
#include "thrift/compiler/test/fixtures/refs/gen-cpp2/module_types.h"


namespace apache {
namespace thrift {
namespace detail {
namespace md {
using ThriftMetadata = ::apache::thrift::metadata::ThriftMetadata;
using ThriftServiceContext = ::apache::thrift::metadata::ThriftServiceContext;
using ThriftService = ::apache::thrift::metadata::ThriftService;

template <>
class EnumMetadata<::cpp2::TypedEnum> {
 public:
  static void gen(ThriftMetadata& metadata);
};
template <>
class StructMetadata<::cpp2::MyUnion> {
 public:
  static void gen(ThriftMetadata& metadata);
};
template <>
class StructMetadata<::cpp2::MyField> {
 public:
  static void gen(ThriftMetadata& metadata);
};
template <>
class StructMetadata<::cpp2::MyStruct> {
 public:
  static void gen(ThriftMetadata& metadata);
};
template <>
class StructMetadata<::cpp2::StructWithUnion> {
 public:
  static void gen(ThriftMetadata& metadata);
};
template <>
class StructMetadata<::cpp2::RecursiveStruct> {
 public:
  static void gen(ThriftMetadata& metadata);
};
template <>
class StructMetadata<::cpp2::StructWithContainers> {
 public:
  static void gen(ThriftMetadata& metadata);
};
template <>
class StructMetadata<::cpp2::StructWithSharedConst> {
 public:
  static void gen(ThriftMetadata& metadata);
};
template <>
class StructMetadata<::cpp2::Empty> {
 public:
  static void gen(ThriftMetadata& metadata);
};
template <>
class StructMetadata<::cpp2::StructWithRef> {
 public:
  static void gen(ThriftMetadata& metadata);
};
template <>
class StructMetadata<::cpp2::StructWithRefTypeUnique> {
 public:
  static void gen(ThriftMetadata& metadata);
};
template <>
class StructMetadata<::cpp2::StructWithRefTypeShared> {
 public:
  static void gen(ThriftMetadata& metadata);
};
template <>
class StructMetadata<::cpp2::StructWithRefTypeSharedConst> {
 public:
  static void gen(ThriftMetadata& metadata);
};
template <>
class StructMetadata<::cpp2::StructWithRefAndAnnotCppNoexceptMoveCtor> {
 public:
  static void gen(ThriftMetadata& metadata);
};
} // namespace md
} // namespace detail
} // namespace thrift
} // namespace apache
