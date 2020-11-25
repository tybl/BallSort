#include "Executor.hpp"

#include "Board.hpp"

Executor::Executor(std::ifstream& file)
  : m_nodes(NodeWeight())
  , m_root(std::make_unique<const Board>(file))
{
  m_nodes.push(m_root.get());
}

INode const* Executor::solve() {
  while(!m_nodes.empty()) {
    auto node = m_nodes.top();
    m_nodes.pop();
    if (node->is_solved()) {
      return node;
    }
    auto edges = node->generate_steps();
    for (auto edge : edges) {
      auto* n = *node + edge;
      if (n) {
        m_nodes.push(n);
      }
    }
  } // while (!empty)
  return nullptr;
}
