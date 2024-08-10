#include "argparse.hpp"
#include "version.h"

#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>

using Matrix = std::array<std::array<char, 3>, 3>;

std::ostream &operator<<(std::ostream &os, Matrix m) {
  for (auto &&row : m) {
    for (auto &&col : row) {
      os << col;
    }
    os << '\n';
  }

  return os;
}

enum class State { O_WIN, X_WIN, DRAW, CONTINUE };

std::ostream &operator<<(std::ostream &os, const State s) {
  switch (s) {
  case State::CONTINUE:
    os << "State::CONTINUE";
    break;
  case State::O_WIN:
    os << "State::O_WIN";
    break;
  case State::X_WIN:
    os << "State::X_WIN";
    break;
  case State::DRAW:
    os << "State::DRAW";
    break;
  }

  return os;
}

State check_state(const Matrix &m) {
  // проверяем строки
  for (auto &&row : m) {
    if (std::all_of(std::cbegin(row), std::cend(row), [](char elem) { return elem == 'o'; }))
      return State::O_WIN;
    if (std::all_of(std::cbegin(row), std::cend(row), [](char elem) { return elem == 'x'; }))
      return State::X_WIN;
  }

  // проверяем столбцы
  for (int j = 0; j < std::size(m.front()); j++) {
    if (constexpr char t = 'o'; m[0][j] == t && m[1][j] == t && m[2][j] == t)
      return State::O_WIN;

    if (constexpr char t = 'x'; m[0][j] == t && m[1][j] == t && m[2][j] == t)
      return State::X_WIN;
  }

  // проверяем диагонали
  if (constexpr char t = 'o';
      (m[0][0] == t && m[1][1] == t && m[2][2] == t) || (m[0][2] == t && m[1][1] == t && m[2][0] == t))
    return State::O_WIN;

  if (constexpr char t = 'x';
      (m[0][0] == t && m[1][1] == t && m[2][2] == t) || (m[0][2] == t && m[1][1] == t && m[2][0] == t))
    return State::X_WIN;

  auto have_free_space = std::any_of(std::cbegin(m), std::cend(m), [](auto raw) {
    return std::any_of(std::cbegin(raw), std::cend(raw), [](char elem) { return elem == '_'; });
  });

  if (have_free_space)
    return State::CONTINUE;

  return State::DRAW;
}

struct Role {
  char my;
  char op;
};

void next(Matrix &m, const Role &role) {
  const auto opponent_win = role.op == 'o' ? State::O_WIN : State::X_WIN;
  const auto my_win = role.my == 'o' ? State::O_WIN : State::X_WIN;

  // проверяем выигрываем ли мы на следующий ход
  for (int i = 0; i < std::size(m); ++i) {
    for (int j = 0; j < std::size(m.front()); ++j) {
      if (m[i][j] != '_')
        continue;

      m[i][j] = role.my;
      auto state = check_state(m);
      if (state == my_win) {
        return;
      }

      m[i][j] = '_';
    }
  }

  // проверим, что опп не выигрывает на следующий ход
  for (int i = 0; i < std::size(m); ++i) {
    for (int j = 0; j < std::size(m.front()); ++j) {
      if (m[i][j] != '_')
        continue;

      m[i][j] = role.op;
      auto state = check_state(m);
      if (state == opponent_win) {
        m[i][j] = role.my;
        return;
      }

      m[i][j] = '_';
    }
  }

  // тогда мы хотим сделать выигрывающий ход
  // если центр свободен, надо его занять
  if (m[1][1] == '_') {
    m[1][1] = role.my;
    return;
  }

  // хотим занять один из углов
  if (m[0][0] == '_') {
    m[0][0] = role.my;
    return;
  }
  if (m[0][2] == '_') {
    m[0][2] = role.my;
    return;
  }
  if (m[2][0] == '_') {
    m[2][0] = role.my;
    return;
  }
  if (m[2][2] == '_') {
    m[2][2] = role.my;
    return;
  }

  // все хорошие точки уже заняты, уже пофиг куда ходить
  for (int i = 0; i < std::size(m); ++i) {
    for (int j = 0; j < std::size(m.front()); ++j) {
      if (m[i][j] != '_')
        continue;
      m[i][j] = role.my;
      return;
    }
  }
}

void read(const std::filesystem::path &file_path, Matrix &m, Role &role) {
  // компьютер сделал ход
  std::ifstream is{file_path};
  is >> role.my;
  role.op = role.my == 'o' ? 'x' : 'o';

  for (auto &&row : m) {
    for (auto &&col : row) {
      is >> col;
    }
  }
}

struct FileWriter final {
  FileWriter(std::filesystem::path file_path, Matrix m, const Role &role)
      : m_{std::move(m)}, game_file_(std::move(file_path)) {
    game_file_ << role.my << '\n';
  }

  Matrix &get() noexcept { return m_; }
  const Matrix &get() const noexcept { return m_; }

  ~FileWriter() {

    game_file_ << m_;
    const auto state = check_state(m_);
    switch (state) {
    case State::CONTINUE:
      break;
    case State::DRAW:
      game_file_ << '-';
      break;
    case State::X_WIN:
      game_file_ << 'x';
      break;
    case State::O_WIN:
      game_file_ << 'o';
      break;
    }
  }

private:
  Matrix m_{};
  std::ofstream game_file_{};
};

int main(int argc, char *argv[]) {

  argparse::ArgumentParser program("tick", PROJECT_VERSION);
  program.add_argument("file_path").help("Файл, в котором будем играть").nargs(1).default_value("../files/field1.txt");
  program.add_argument("--verbose").default_value(false).implicit_value(true);

  try {
    program.parse_args(argc, argv);
  } catch (const std::exception &ex) {
    std::cerr << ex.what() << '\n';
    std::cerr << program;
    return 1;
  }

  std::filesystem::path file_path = program.get<std::string>("file_path");

  if (not exists(file_path)) {
    std::cerr << file_path << " не существует.\n";
    return 1;
  }

  Role role;
  Matrix matrix{};

  read(file_path, matrix, role);

  FileWriter writer(file_path, std::move(matrix), role);

  auto state = check_state(writer.get());
  if (state != State::CONTINUE) {
    std::cout << state;
    return 0;
  }

  next(writer.get(), role);

  if (program["--verbose"] == true) {
    std::cout << writer.get();
  }

}