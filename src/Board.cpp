#include "Board.hpp"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <map>

#include <iostream>

Board::Board(std::istream& input)
  : m_contents()
  , m_parent(nullptr)
  , m_set(std::make_shared<std::set<Board>>())
  , m_distance(0)
  , m_max_stack_height(0)
{
  std::string line;
  std::getline(input, line);
  m_contents.resize(line.size());
  while (input.good()) {
    assert(line.size() == m_contents.size());
    m_max_stack_height += 1;
    for (size_t i = 0; i < line.size(); ++i) {
      if (line[i] != '0') {
        m_contents[i].insert(m_contents[i].begin(), line[i]);
      }
    }
    std::getline(input, line);
  }
}

Board::~Board() = default;

bool Board::is_solved() const {
  for (auto s : m_contents) {
    if (!s.empty() && !is_full_and_homogeneous(s)) {
      return false;
    }
  }
  return true;
}

std::vector<Edge> Board::generate_steps() const {
  std::vector<Edge> result;
  for (uint8_t i = 0; i < m_contents.size(); ++i) {
    for (uint8_t j = 0; j < m_contents.size(); ++j) {
      Edge e{i,j};
      if (is_valid(e)) {
        result.push_back(e);
      }
    }
  }
  return result;
}

INode const* Board::operator+(Edge const& e) const {
  auto result = m_set->emplace(*this, e);
  Board const& b = *result.first;
  if (result.second) {
    return &b;
  }
  if (m_distance + 1 < b.m_distance) {
#if 0
    std::cout << "I FOUND A THING!!!\n";
#else
    b.m_parent = this;
    b.m_distance = m_distance + 1;
#endif
  }
  return nullptr;
}

void Board::print() const {
  for (size_t i = 0; i < m_max_stack_height; ++i) {
    size_t r = (m_max_stack_height - 1) - i;
    for (size_t c = 0; c < m_contents.size(); ++c) {
      if (r < m_contents[c].size()) {
        std::cout << m_contents.at(c).at(r);
      } else {
        std::cout << ' ';
      }
    }
    std::cout << '\n';
  }
}

void Board::print_steps() const {
  if (m_parent) {
    m_parent->print_steps();
    std::cout << '\n';
  } else {
    std::cout << "Searched " << m_set->size() << " nodes\n";
  }
  print();
}

double Board::priority() const {
  std::map<char, size_t> counts;
  for (auto const& s : m_contents) {
    if (!s.empty()) {
      auto c = s.back();
      size_t count = 0;
      for (auto const& o : m_contents) {
        count += std::count(o.rbegin(), o.rend(), c);
      }
      counts[c] = count;
    }
  }
  double result = 0;
  for (auto const& kv : counts) {
    result += 1 << kv.second;
  }
  return result;
}

bool Board::operator<(Board const& o) const {
  return m_contents < o.m_contents;
}

Board::Board(Board const& o, Edge const& e)
  : m_contents(o.m_contents)
  , m_parent(&o)
  , m_set(o.m_set)
  , m_distance(o.m_distance + 1)
  , m_max_stack_height(o.m_max_stack_height)
{
  apply(e);
}

void Board::apply(Edge const& e) {
  m_contents.at(e.StackTo).push_back(m_contents.at(e.StackFrom).back());
  m_contents.at(e.StackFrom).pop_back();
}

bool Board::is_valid(Edge const& e) const {
  if (e.StackFrom == e.StackTo) { return false; }
  if (m_contents.at(e.StackFrom).empty()) { return false; }
  if (is_full(m_contents.at(e.StackTo))) { return false; }
  if (m_contents.at(e.StackTo).empty()) { return true; }
  if (is_full_and_homogeneous(m_contents[e.StackFrom])) { return false; }
  return m_contents[e.StackTo].back() == m_contents[e.StackFrom].back();
}

bool Board::is_homogeneous(std::vector<char> const& s) const {
  return std::all_of(s.begin(), s.end(), [&](char c){ return s.front() == c; });
}

bool Board::is_full(std::vector<char> const& s) const {
  return s.size() == m_max_stack_height;
}

bool Board::is_full_and_homogeneous(std::vector<char> const& s) const {
  return is_full(s) && is_homogeneous(s);
}
