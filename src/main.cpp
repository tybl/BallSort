#include <array>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <vector>

template <typename T>
struct FixedStack {
  static constexpr size_t MAX_SIZE = 4;
  std::array<T, MAX_SIZE> m_stack;
  size_t m_count;
public:

  size_t size() const { return m_count; }

  T top() const { return at(m_count - 1); }

  T base() const { return at(0); }

  size_t count_from_top(char val) const {
    size_t count = 0;
    while (count < m_count && m_stack[m_count - count - 1] == val) { count += 1; }
    return count;
  }

  size_t count_from_base() const {
    size_t count = 0;
    while (count < m_count && m_stack[count] == top()) { count += 1; }
    return count;
  }

  T at(size_t i) const {
    if (size() <= i)
      throw std::runtime_error("FixedStack::at");
    return m_stack[i];
  }

  bool is_full() const { return MAX_SIZE == m_count; }

  bool is_empty() const { return 0 == m_count; }

  bool is_full_and_same() const {
    return is_full() && m_stack[0] == m_stack[1]
                     && m_stack[1] == m_stack[2]
                     && m_stack[2] == m_stack[3];
  }

  T pop_top() {
    if (is_empty())
      throw std::runtime_error("Cannot pop from empty FixedStack");
    m_count -= 1;
    return m_stack[m_count];
  }

  void add_top(T val) {
    if (is_full())
      throw std::runtime_error("Cannot push onto full FixedStack");
    m_stack[m_count] = val; m_count += 1;
  }

  bool operator < (FixedStack const& o) const {
    for (size_t i = 0; i < m_count && i < o.m_count; ++i) {
      if (m_stack[i] < o.m_stack[i]) return true;
      if (o.m_stack[i] < m_stack[i]) return false;
    }
    return m_count < o.m_count;
  }

};

struct Move {
  char Value;
  uint8_t ColFrom;
  uint8_t ColTo;
};

struct Board {
  std::vector<FixedStack<char>> m_board;
  std::vector<Move> m_moves;
public:

  Board(size_t num_stacks)
    : m_board(num_stacks) { }

  bool operator < (Board const& o) const {
    return m_board < o.m_board;
  }

  size_t size() const { return m_board.size(); }

  bool is_solved() const {
    for (auto const& fs : m_board) {
      if (!fs.is_empty() && !fs.is_full_and_same()) {
        return false;
      }
    }
    return true;
  }

  void print_moves() const {
    int step_num = 1;
    for (auto const& move : m_moves) {
      std::cout << step_num++ << ": Move " << move.Value << " from column " << static_cast<size_t>(move.ColFrom) + 1 << " to column " << static_cast<size_t>(move.ColTo) + 1 << "\n";
    }
  }

  void print() const {
    std::cout << std::setw(m_board.size()) << std::setfill('*') << "\n";
    for (size_t r = 0; r < 4; ++r) {
      for (size_t c = 0; c < m_board.size(); ++c) {
        if ((3 - r) < m_board[c].size()) {
          std::cout << m_board[c].at(3 - r);
        } else {
          std::cout << ' ';
        }
      }
      std::cout << "\n";
    }
    std::cout << "  value: " << calc_value() << "\n";
  }

  bool is_valid_move(size_t col_from, size_t col_to) const {
    if (m_board.at(col_from).is_empty()) { return false; }
    if (m_board.at(col_to).is_full()) { return false; }
    if (m_board.at(col_to).is_empty()) { return true; }
    if (m_board[col_from].is_full_and_same()) { return false; }
    return m_board[col_to].top() == m_board[col_from].top();
  }

  void move(size_t col_from, size_t col_to) {
    m_moves.push_back({m_board[col_from].top(), static_cast<uint8_t>(col_from), static_cast<uint8_t>(col_to)});
    m_board[col_to].add_top(m_board[col_from].pop_top());
  }

  size_t calc_value() const {
    std::map<char,int> counts;
    for (auto const& s : m_board) {
      if (!s.is_empty()) {
        auto c = s.top();
        size_t count = 0;
        for (auto const& o : m_board) {
          count += o.count_from_top(c);
        }
        counts[c] = count;
      }
    }
    size_t result = 0;
    for (auto const& kv : counts) {
      result += 1 << kv.second;
    }
    return result;
  }

  size_t num_moves() const {
    return m_moves.size();
  }

};

struct BoardCompare {
  bool operator()(Board const& lhs, Board const& rhs) {
    auto lhs_val = lhs.calc_value();
    auto rhs_val = rhs.calc_value();
    return (lhs_val < rhs_val) || ((lhs_val == rhs_val) && (lhs.num_moves() > rhs.num_moves()));
  }
};

struct Executor {
  std::priority_queue<Board, std::vector<Board>, BoardCompare> m_tasks;
  std::set<Board> m_found;
public:
  Executor()
    : m_tasks(BoardCompare()) {}

  void run() {
    size_t count = 0;
    while (!m_tasks.empty()) {
      count += 1;
      auto b = m_tasks.top();
      m_tasks.pop();
      if (b.is_solved()) {
        b.print_moves();
        std::cout << "Inspected " << count << " boards\n";
        return;
      }
      for (size_t i = 0; i < b.size(); ++i) {
        for (size_t j = 0; j < b.size(); ++j) {
          if (i != j) {
            if (b.is_valid_move(i, j)) {
              Board c = b;
              c.move(i,j);
              if (0 == m_found.count(c)) {
                m_tasks.emplace(c);
                m_found.insert(c);
              }
            }
          }
        } // for
      } // for
    } // while (!empty)
    std::cout << "Error: Could not progress beyond these boards:\n";
    for (auto const& b : m_found) {
      b.print();
    }
  }
};

int main() {
  size_t num_stacks = 0;
  //std::cout << "How many stacks? ";
  std::cin >> num_stacks;
  Board board(num_stacks);
  for (size_t i = 0; i < num_stacks; ++i) {
    char value = '\0';
    //std::cout << "Please enter the contents of stack " << i + 1 << "\n";
    for (int j = 0; j < 4; ++j) {
      std::cin >> value;
      if ('0' == value) { break; }
      board.m_board[i].add_top(value);
    }
  }
  board.print();
  Executor e;
  e.m_tasks.emplace(board);
  e.m_found.insert(board);
  e.run();
}

