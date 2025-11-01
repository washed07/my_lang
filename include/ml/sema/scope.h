#pragma once

#include "ml/basic/flags.h"
#include "ml/sema/cls.h"
#include "ml/sema/func.h"
#include "ml/sema/rec.h"
#include "ml/sema/type.h"
#include "ml/sema/var.h"
#include <string>
#include <vector>

namespace ml::sema {

enum class ScopeKind {
  Global,
  Block,
  Function,
  Loop,
  Class,
  Record,
};

class Scope {
public:
  const std::string name;
  ScopeKind kind;

private:
  std::vector<Variable> variables;
  std::vector<Function> functions;
  std::vector<Class> classes;
  std::vector<Record> records;
  std::vector<Type> primitives = {i8_ty,   i16_ty,    i32_ty,  i64_ty,  i128_ty,
                                  u8_ty,   u16_ty,    u32_ty,  u64_ty,  u128_ty,
                                  f16_ty,  f32_ty,    f64_ty,  f128_ty, bool_ty,
                                  char_ty, string_ty, void_ty, null_ty};

  std::shared_ptr<Scope> parent = nullptr;

public:
  Scope(const std::string &name, const ScopeKind kind,
        std::shared_ptr<Scope> parent)
      : name(name), kind(kind), parent(parent) {}

  Scope(const std::string &name, const ScopeKind kind)
      : name(name), kind(kind) {}

  inline void addVariable(const Variable &var) {
    this->variables.push_back(var);
  }

  inline void addFunction(const Function &func) {
    this->functions.push_back(func);
  }

  inline void addClass(const Class &cls) { this->classes.push_back(cls); }

  inline void addRecord(const Record &rec) { this->records.push_back(rec); }

  inline bool hasVariable(const std::string &var_name) const {
    for (const auto &var : this->variables) {
      if (var.name == var_name) {
        return true;
      }
    }
    if (this->parent) {
      return this->parent->hasVariable(var_name);
    }
    return false;
  }

  inline bool hasFunction(const std::string &func_name) const {
    for (const auto &func : this->functions) {
      if (func.name == func_name) {
        return true;
      }
    }
    if (this->parent) {
      return this->parent->hasFunction(func_name);
    }
    return false;
  }

  inline bool hasClass(const std::string &class_name) const {
    for (const auto &cls : this->classes) {
      if (cls.name == class_name) {
        return true;
      }
    }
    if (this->parent) {
      return this->parent->hasClass(class_name);
    }
    return false;
  }

  inline bool hasRecord(const std::string &record_name) const {
    for (const auto &rec : this->records) {
      if (rec.name == record_name) {
        return true;
      }
    }
    if (this->parent) {
      return this->parent->hasRecord(record_name);
    }
    return false;
  }

  inline const Variable getVariable(const std::string &var_name) const {
    for (const auto &var : this->variables) {
      if (var.name == var_name) {
        return var;
      }
    }
    if (this->parent) {
      return this->parent->getVariable(var_name);
    }
    return Variable();
  }

  inline const Function getFunction(const std::string &func_name) const {
    for (const auto &func : this->functions) {
      if (func.name == func_name) {
        return func;
      }
    }
    if (this->parent) {
      return this->parent->getFunction(func_name);
    }
    return Function();
  }

  inline const Class getClass(const std::string &class_name) const {
    for (const auto &cls : this->classes) {
      if (cls.name == class_name) {
        return cls;
      }
    }
    if (this->parent) {
      return this->parent->getClass(class_name);
    }
    return Class();
  }

  inline const Record getRecord(const std::string &record_name) const {
    for (const auto &rec : this->records) {
      if (rec.name == record_name) {
        return rec;
      }
    }
    if (this->parent) {
      return this->parent->getRecord(record_name);
    }
    return Record();
  }

  inline bool hasType(const std::string &type_name) const {
    for (const auto &prim : this->primitives) {
      if (prim.name == type_name) {
        return true;
      }
    }
    if (this->hasClass(type_name) || this->hasRecord(type_name)) {
      return true;
    }
    if (this->parent) {
      return this->parent->hasType(type_name);
    }
    return false;
  }

  inline const Type getType(const std::string &type_name) const {
    for (const auto &prim : this->primitives) {
      if (prim.name == type_name) {
        return prim;
      }
    }
    if (this->hasClass(type_name)) {
      return this->getClass(type_name);
    }
    if (this->hasRecord(type_name)) {
      return this->getRecord(type_name);
    }
    if (this->parent) {
      return this->parent->getType(type_name);
    }
    return Type();
  }

  inline const Variable getClassField(const std::string &class_name,
                                      const std::string &field_name,
                                      const basic::Accessor access) const {
    const Class &cls = this->getClass(class_name);
    return cls.getField(field_name, access);
  }

  inline const Function getClassMethod(const std::string &class_name,
                                       const std::string &method_name,
                                       const basic::Accessor access) const {
    const Class &cls = this->getClass(class_name);
    return cls.getMethod(method_name, access);
  }

  inline const Variable getRecordField(const std::string &record_name,
                                       const std::string &field_name,
                                       const basic::Accessor access) const {
    const Record &rec = this->getRecord(record_name);
    return rec.getField(field_name, access);
  }

  inline bool isValidType(const Type &type) const {
    if (type.isPrimitive() || type.isVoid() || type.isNull()) {
      return true;
    }
    if (type.kind == TypeKind::Class) {
      return this->hasClass(type.name);
    }
    if (type.kind == TypeKind::Record) {
      return this->hasRecord(type.name);
    }
    return false;
  }

  inline const std::shared_ptr<Scope> getParent() const { return this->parent; }
};

} // namespace ml::sema

ENABLE_BITMASKS(ml::sema::ScopeKind)