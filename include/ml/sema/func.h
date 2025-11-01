#pragma once

#include "ml/sema/type.h"
#include "ml/sema/var.h"
#include <string>
#include <vector>

namespace ml::sema {

class Function : public Type {
public:
  Type return_type = none_ty;
  std::vector<Variable> parameters = {};
  basic::Accessor access = basic::Accessor::Public;
  basic::Modifier modifier = basic::Modifier::None;

  Function(const std::string &name, const Type &return_type,
           const std::vector<Variable> &parameters, basic::Accessor access,
           basic::Modifier modifier)
      : Type(TypeKind::Function, name), return_type(return_type),
        parameters(parameters), access(access), modifier(modifier) {}

  Function(const std::string &name, const Type &return_type,
           const std::vector<Variable> &parameters)
      : Type(TypeKind::Function, name), return_type(return_type),
        parameters(parameters) {}

  Function(const std::string &name, const Type &return_type)
      : Type(TypeKind::Function, name), return_type(return_type) {}

  Function(const std::string &name) : Type(TypeKind::Function, name) {}

  Function() : Type(TypeKind::Function) {}

  inline bool isValidArguments(const std::vector<Type> &arg_types) const {
    if (arg_types.size() != this->parameters.size()) {
      return false;
    }

    for (size_t i = 0; i < arg_types.size(); i++) {
      // Allow more flexible type matching for numeric types
      if (arg_types[i].isSimilarTo(this->parameters[i].type)) {
        continue; // Exact match
      } else if (arg_types[i].isNumeric() &&
                 this->parameters[i].type.isNumeric()) {
        continue; // Allow numeric conversions (int to float, etc.)
      } else {
        return false; // No conversion possible
      }
    }

    return true;
  }

  inline bool isValid() const { return this->name.empty() == false; }

  virtual ~Function() = default;
};

} // namespace ml::sema
