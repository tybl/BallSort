#ifndef NODEWEIGHT_HPP
#define NODEWEIGHT_HPP

#include "INode.hpp"

#include <cassert>
#include <stdexcept>

struct NodeWeight {

  bool operator () (INode const* lhs, INode const* rhs) {
    if (!lhs) throw std::runtime_error("Invalid lhs");
    if (!rhs) throw std::runtime_error("Invalid rhs");
    auto lp = lhs->priority();
    auto rp = rhs->priority();
    return (lp < rp) || (lp == rp && lhs->num_steps() > rhs->num_steps());
  }

}; // struct NodeWeight

#endif // NODEWEIGHT_HPP
