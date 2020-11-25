#ifndef NODEWEIGHT_HPP
#define NODEWEIGHT_HPP

#include "INode.hpp"

#include <stdexcept>

struct NodeWeight {

  bool operator () (INode const* lhs, INode const* rhs) {
    if (!lhs) throw std::runtime_error("Invalid lhs");
    if (!rhs) throw std::runtime_error("Invalid rhs");
    return lhs->priority() < rhs->priority();
  }

}; // struct NodeWeight

#endif // NODEWEIGHT_HPP
