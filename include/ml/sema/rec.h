#pragma once

#include "ml/sema/type.h"
#include "ml/sema/var.h"
#include <string>
#include <vector>

namespace ml::sema {

class Record : public Type {
public:
  std::vector<Variable> fields;

  Record(const std::string &name, const std::vector<Variable> &fields)
      : Type(TypeKind::Record, name), fields(fields) {}

  Record(const std::string &name) : Type(TypeKind::Record, name) {}

  Record() : Type(TypeKind::Record) {}

  // Protected constructor for derived classes to specify their own TypeKind
protected:
  Record(TypeKind kind, const std::string &name,
         const std::vector<Variable> &fields)
      : Type(kind, name), fields(fields) {}

  Record(TypeKind kind, const std::string &name) : Type(kind, name) {}

  Record(TypeKind kind) : Type(kind) {}

public:
  inline bool hasField(const std::string &field_name,
                       const basic::Accessor access) const {
    for (const auto &field : this->fields) {
      if (field.name == field_name) {
        return basic::canAccess(field.accessor, access);
      }
    }
    return false;
  }

  inline const Variable getField(const std::string &field_name,
                                 const basic::Accessor access) const {
    for (const auto &field : this->fields) {
      if (field.name == field_name) {
        if (basic::canAccess(field.accessor, access)) {
          return field;
        } else {
          throw std::runtime_error("Access denied to field: " + field_name);
        }
      }
    }
    return Variable();
  }

  inline bool isValid() const { return this->name.empty() == false; }

  virtual ~Record() = default;
};
} // namespace ml::sema