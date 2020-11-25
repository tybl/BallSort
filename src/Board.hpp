#ifndef BOARD_HPP
#define BOARD_HPP

#include "Edge.hpp"
#include "INode.hpp"

#include <memory>
#include <set>
#include <vector>

struct Board
  : public INode
{
  std::vector<std::vector<char>> m_contents;
  mutable Board const* m_parent;
  std::shared_ptr<std::set<Board>> m_set;
  mutable size_t m_distance;
  uint8_t m_max_stack_height;
public:

  Board(std::istream& input);

  Board(Board const& o, Edge const& e);

  ~Board() override;

  bool is_solved() const override;

  std::vector<Edge> generate_steps() const override;

  INode const* operator + (Edge const& e) const override;

  void print() const override;

  void print_steps() const override;

  double priority() const override;

  bool operator<(Board const& o) const;

private:

  void apply(Edge const& e);
  bool is_full(std::vector<char> const& s) const;
  bool is_full_and_homogeneous(std::vector<char> const& s) const;
  bool is_homogeneous(std::vector<char> const& s) const;
  bool is_valid(Edge const& e) const;

};

#endif // BOARD_HPP
