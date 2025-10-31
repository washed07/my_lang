/**
 * @file visitor.h
 * @brief Visitor pattern definitions for My Language.
 * @details Defines visitor and visitable interfaces and macros.
 * @copyright Copyright (c) 2025 Karson P. Califf
 */

#pragma once

namespace ml::basic {

/**
 * @class Visiting visitor.h
 * @brief Interface for visiting objects of type T.
 * @details Classes that implement this interface can visit objects of type T.
 */
template <typename T> class Visiting {
public:
  /**
   * @brief Visits an object of type T.
   * @param v The object to visit.
   */
  virtual void visit(T &v) = 0;

  virtual ~Visiting() = default;
};

/**
 * @class Visitor visitor.h
 * @brief Base class for visitors.
 * @details Provides a mechanism to visit different types of visitable objects.
 */
class Visitor {
public:
  /**
   * @brief Visits an object of type T.
   * @param v The object to visit.
   * @details Inherit from Visiting<T> to implement specific visit behavior.
   */
  template <typename T> void visit(T &v) {
    if (auto *visitor = dynamic_cast<Visiting<T> *>(this)) {
      visitor->visit(v);
    }
  }

  virtual ~Visitor() = default;
};

/**
 * @class Visitable visitor.h
 * @brief Interface for visitable objects of type T.
 * @details Classes that implement this interface can accept visitors.
 */
template <typename T> class Visitable {
public:
  virtual void accept(Visitor &v) = 0;

  virtual ~Visitable() = default;
};

/**
 * @brief Macro to enable visitor acceptance for a class.
 * @param ClassName The name of the class to enable visitors for.
 * @details This is the same as implementing the accept method manually.
 */
#define ENABLE_VISITORS(ClassName)                                             \
  void accept(basic::Visitor &v) override {                                    \
    v.visit(*static_cast<ClassName *>(this));                                  \
  }

} // namespace ml::basic
