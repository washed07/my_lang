#pragma once

#include "ml/basic/accessor.h"
#include "ml/basic/modifier.h"
#include "ml/sema/type.h"
#include <string>

namespace ml::sema {

class Variable : public Type {
public:
  const Type type = none_ty;
  const basic::Accessor accessor = basic::Accessor::Public;
  const basic::Modifier modifier = basic::Modifier::None;

  Variable(const std::string &name, const Type &type,
           const basic::Accessor &accessor, const basic::Modifier &modifier)
      : Type(TypeKind::Variable, name), type(type), accessor(accessor),
        modifier(modifier) {}

  Variable(const std::string &name, const Type &type)
      : Type(TypeKind::Variable, name), type(type) {}

  Variable(const std::string &name) : Type(TypeKind::Variable, name) {}

  Variable() : Type(TypeKind::Variable) {}

  inline bool isValid() const { return this->name.empty() == false; }

  virtual ~Variable() = default;
};

} // namespace ml::sema