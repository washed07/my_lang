#pragma once

#include "ml/sema/func.h"
#include "ml/sema/rec.h"
#include "ml/sema/type.h"
#include "ml/sema/var.h"
#include <string>
#include <vector>

namespace ml::sema {

class Class : public Record {
public:
  std::vector<Function> methods = {};

  Class(const std::string &name, const std::vector<Variable> &fields,
        const std::vector<Function> &methods)
      : Record(TypeKind::Class, name, fields), methods(methods) {}

  Class(const std::string &name, const std::vector<Variable> &fields)
      : Record(TypeKind::Class, name, fields) {}

  Class(const std::string &name) : Record(TypeKind::Class, name) {}

  Class() : Record(TypeKind::Class) {}

  inline bool hasMethod(const std::string &method_name,
                        const basic::Accessor access) const {
    for (const auto &method : this->methods) {
      if (method.name == method_name) {
        return basic::canAccess(method.access, access);
      }
    }
    return false;
  }

  inline const Function getMethod(const std::string &method_name,
                                  const basic::Accessor access) const {
    for (const auto &method : this->methods) {
      if (method.name == method_name) {
        if (basic::canAccess(method.access, access)) {
          return method;
        } else {
          throw std::runtime_error("Access denied to method: " + method_name);
        }
      }
    }
    return Function();
  }

  inline bool isValid() const { return this->name.empty() == false; }
};

} // namespace ml::sema
