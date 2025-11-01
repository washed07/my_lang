#pragma once

#include <iostream>
#include <string>

namespace ml::sema {
enum class TypeKind {
  None,
  Void,
  Null,
  Boolean,
  I8,
  I16,
  I32,
  I64,
  I128,
  U8,
  U16,
  U32,
  U64,
  U128,
  F16,
  F32,
  F64,
  F128,
  String,
  Character,
  Array,
  Class,
  Record,
  Variable,
  Function,
};

inline std::string typeKindStr(const TypeKind kind) {
  switch (kind) {
  case TypeKind::None:
    return "none";
  case TypeKind::Void:
    return "void";
  case TypeKind::Null:
    return "null";
  case TypeKind::Boolean:
    return "bool";
  case TypeKind::I8:
    return "i8";
  case TypeKind::I16:
    return "i16";
  case TypeKind::I32:
    return "i32";
  case TypeKind::I64:
    return "i64";
  case TypeKind::I128:
    return "i128";
  case TypeKind::U8:
    return "u8";
  case TypeKind::U16:
    return "u16";
  case TypeKind::U32:
    return "u32";
  case TypeKind::U64:
    return "u64";
  case TypeKind::U128:
    return "u128";
  case TypeKind::F16:
    return "f16";
  case TypeKind::F32:
    return "f32";
  case TypeKind::F64:
    return "f64";
  case TypeKind::F128:
    return "f128";
  case TypeKind::String:
    return "string";
  case TypeKind::Character:
    return "char";
  case TypeKind::Array:
    return "array";
  case TypeKind::Class:
    return "class";
  case TypeKind::Record:
    return "record";
  default:
    return "unknown";
  }
}

class Type {
public:
  const TypeKind kind = TypeKind::None;
  const std::string name = "";

  Type(TypeKind kind, const std::string &name) : kind(kind), name(name) {}

  Type(const TypeKind kind) : kind(kind) {}

  Type() : kind(TypeKind::None) {}

  inline bool isValid() const {
    return this->name.empty() == false && this->kind != TypeKind::None;
  }

  virtual ~Type() = default;

  inline virtual int64_t size() const {
    switch (this->kind) {
    case TypeKind::I8:
    case TypeKind::U8:
    case TypeKind::Boolean:
    case TypeKind::Character:
      return 1;
    case TypeKind::I16:
    case TypeKind::U16:
      return 2;
    case TypeKind::I32:
    case TypeKind::U32:
    case TypeKind::F32:
      return 4;
    case TypeKind::I64:
    case TypeKind::U64:
    case TypeKind::F64:
      return 8;
    case TypeKind::I128:
    case TypeKind::U128:
    case TypeKind::F128:
      return 16;
    default:
      return 0;
    }
  }

  inline bool isSimilarTo(const Type &other) const {
    return this->kind == other.kind;
  }

  inline bool isInteger() const {
    return this->kind == TypeKind::I8 || this->kind == TypeKind::I16 ||
           this->kind == TypeKind::I32 || this->kind == TypeKind::I64 ||
           this->kind == TypeKind::I128 || this->kind == TypeKind::U8 ||
           this->kind == TypeKind::U16 || this->kind == TypeKind::U32 ||
           this->kind == TypeKind::U64 || this->kind == TypeKind::U128;
  }

  inline bool isFloatingPoint() const {
    return this->kind == TypeKind::F16 || this->kind == TypeKind::F32 ||
           this->kind == TypeKind::F64 || this->kind == TypeKind::F128;
  }

  inline bool isNumeric() const {
    return this->isInteger() || this->isFloatingPoint();
  }

  inline bool isPointer() const {
    return this->kind == TypeKind::Array || this->kind == TypeKind::Class ||
           this->kind == TypeKind::Record || this->kind == TypeKind::String;
  }

  inline bool isTruthy() const {
    return this->kind != TypeKind::None && this->kind != TypeKind::Void &&
           this->kind != TypeKind::Null;
  }

  inline bool isNone() const { return this->kind == TypeKind::None; }

  inline bool isVoid() const { return this->kind == TypeKind::Void; }

  inline bool isNull() const { return this->kind == TypeKind::Null; }

  inline bool isPrimitive() const {
    return this->kind == TypeKind::I8 || this->kind == TypeKind::I16 ||
           this->kind == TypeKind::I32 || this->kind == TypeKind::I64 ||
           this->kind == TypeKind::I128 || this->kind == TypeKind::U8 ||
           this->kind == TypeKind::U16 || this->kind == TypeKind::U32 ||
           this->kind == TypeKind::U64 || this->kind == TypeKind::U128 ||
           this->kind == TypeKind::F16 || this->kind == TypeKind::F32 ||
           this->kind == TypeKind::F64 || this->kind == TypeKind::F128 ||
           this->kind == TypeKind::Boolean || this->kind == TypeKind::Character;
  }

  operator std::string() const { return this->name; }
  bool operator==(const Type &other) const { return this->name == other.name; }
  bool operator!=(const Type &other) const { return !(*this == other); }
};

inline const Type none_ty = Type(TypeKind::None, "none");
inline const Type void_ty = Type(TypeKind::Void, "void");
inline const Type null_ty = Type(TypeKind::Null, "null");
inline const Type bool_ty = Type(TypeKind::Boolean, "bool");
inline const Type i8_ty = Type(TypeKind::I8, "i8");
inline const Type i16_ty = Type(TypeKind::I16, "i16");
inline const Type i32_ty = Type(TypeKind::I32, "i32");
inline const Type i64_ty = Type(TypeKind::I64, "i64");
inline const Type i128_ty = Type(TypeKind::I128, "i128");
inline const Type u8_ty = Type(TypeKind::U8, "u8");
inline const Type u16_ty = Type(TypeKind::U16, "u16");
inline const Type u32_ty = Type(TypeKind::U32, "u32");
inline const Type u64_ty = Type(TypeKind::U64, "u64");
inline const Type u128_ty = Type(TypeKind::U128, "u128");
inline const Type f16_ty = Type(TypeKind::F16, "f16");
inline const Type f32_ty = Type(TypeKind::F32, "f32");
inline const Type f64_ty = Type(TypeKind::F64, "f64");
inline const Type f128_ty = Type(TypeKind::F128, "f128");
inline const Type char_ty = Type(TypeKind::Character, "char");
inline const Type string_ty = Type(TypeKind::String, "str");

inline Type promoteTypes(const Type &a, const Type &b) {
  if (a.isSimilarTo(b)) {
    return a;
  }

  if (a.isFloatingPoint() && b.isFloatingPoint()) {
    if (a.size() >= b.size()) {
      return a;
    } else {
      return b;
    }
  }

  if (a.isInteger() && b.isInteger()) {
    if (a.size() >= b.size()) {
      return a;
    } else {
      return b;
    }
  }

  if (a.isFloatingPoint() && b.isInteger()) {
    return a;
  }

  if (a.isInteger() && b.isFloatingPoint()) {
    return b;
  }

  return none_ty;
}

inline bool canAssignType(const Type &to, const Type &from) {
  if (to.isSimilarTo(from)) {
    return true;
  }

  if (to.isFloatingPoint() && from.isInteger()) {
    return true;
  }

  if (to.isInteger() && from.isInteger()) {
    if (from.size() <= to.size()) {
      return true;
    }
  }

  return false;
}

} // namespace ml::sema