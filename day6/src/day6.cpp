#include <algorithm>
#include <boost/optional.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <ranges>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

enum class Direction { North, South, East, West };

// Helper function to convert enum to string
std::string to_string(Direction d) {
  switch (d) {
  case Direction::North:
    return "North";
  case Direction::South:
    return "South";
  case Direction::East:
    return "East";
  case Direction::West:
    return "West";
  default:
    return "Unknown";
  }
}

std::map<Direction, std::pair<int, int>> DirectionToIndex() {
  std::map<Direction, std::pair<int, int>> map;

  map[Direction::North] = std::make_pair(-1, 0);
  map[Direction::South] = std::make_pair(1, 0);
  map[Direction::East] = std::make_pair(0, 1);
  map[Direction::West] = std::make_pair(0, -1);

  return map;
}

class Position {
public:
  int x;
  int y;

  // Define the < operator for use in std::set
  bool operator<(const Position &other) const {
    // Comparison logic: compare x first, then y
    if (x != other.x)
      return x < other.x;
    return y < other.y;
  }

  bool operator==(const Position &other) const {
    return y == other.y && x == other.x;
  }
};

class Guard {

public:
  Position position;
  Direction direction;
  std::vector<std::vector<string>> map;

  size_t xmax() const {
    const auto rows = std::views::iota(static_cast<size_t>(0), map.size());
    return rows.size() - 1;
  }

  size_t ymax() const {
    const auto columns =
        std::views::iota(static_cast<size_t>(0), map[0].size());
    return columns.size() - 1;
  }

  Position look_ahead(const Direction &direction) {

    const auto direction_to_index = DirectionToIndex();
    return Position(position.x + DirectionToIndex()[direction].first,
                    position.y + DirectionToIndex()[direction].second);
  }

  bool position_is_valid(Position position) const {
    return (0 <= position.x && position.x <= xmax()) &&
           (0 <= position.y && position.y <= ymax());
  }

  void rotate() {
    switch (direction) {
    case Direction::North:
      direction = Direction::East;
      break;
    case Direction::East:
      direction = Direction::South;
      break;
    case Direction::South:
      direction = Direction::West;
      break;
    case Direction::West:
      direction = Direction::North;
      break;
    }
  }

  std::set<Position> patrol() {
    std::vector<Position> visited;

    std::vector<std::pair<Direction, Position>> extra_visited;

    bool exit = false;
    while (!exit) {
      visited.emplace_back(position);
      const auto candidate_position = look_ahead(direction);
      if (!position_is_valid(candidate_position)) {
        exit = true;

      } else if (map[candidate_position.x][candidate_position.y] == "#") {
        const auto arrived = std::make_pair(direction, position);
        if (find(extra_visited.begin(), extra_visited.end(), arrived) !=
            extra_visited.end()) {
          // cout<<"returning empty"<<endl;
          return std::set<Position>();
        }
        extra_visited.emplace_back(arrived);
        rotate();
      } else {
        position = candidate_position;
      }
    }
    std::set<Position> unique_pairs(visited.begin(), visited.end());
    return unique_pairs;
  }

  void set_pose(const Position &pos, const Direction &dir) {
    direction = dir;
    position = pos;
  }
};

int main() {

  const string file_path = "/home/hugo/AoC/AdventOfCode2024/day6/data.txt";
  ifstream file(file_path);
  if (!file) {
    cerr << "Error: Unable to open file at " << file_path << endl;
    return 1;
  }

  // Read each line from the file
  string line;
  vector<vector<string>> data;
  while (std::getline(file, line)) {
    std::vector<std::string> row;

    // Directly iterate over characters in the line
    for (char c : line) {
      row.emplace_back(1, c); // Construct string from character and add to row
    }

    data.push_back(
        std::move(row)); // Avoid copying by moving the row into the vector
  }

  const auto rows = std::views::iota(static_cast<size_t>(0), data.size());
  const auto columns = std::views::iota(static_cast<size_t>(0), data[0].size());

  std::vector<Direction> possible_directions;
  const auto map = DirectionToIndex();
  possible_directions.reserve(map.size());
  std::transform(map.begin(), map.end(),
                 std::back_inserter(possible_directions),
                 [](const auto &pair) { return pair.first; });

  size_t total(0);
  Guard guard = Guard();
  for (const auto row : rows) {
    for (const auto column : columns) {
      // cout<<"data "<< data[row][column] << endl;
      if (data[row][column] == ">") {
        guard.position = Position(row, column);
        guard.map = data;
        guard.direction = Direction::East;
      }
      if (data[row][column] == "<") {
        guard.position = Position(row, column);
        guard.map = data;
        guard.direction = Direction::West;
      }
      if (data[row][column] == "v") {
        guard.position = Position(row, column);
        guard.map = data;
        guard.direction = Direction::South;
      }
      if (data[row][column] == "^") {
        guard.position = Position(row, column);
        guard.map = data;
        guard.direction = Direction::North;
      }
    }
  }
  const auto initial_position = guard.position;
  const auto initial_direction = guard.direction;
  const auto visited = guard.patrol();
  cout << visited.size() << endl;

  // cout<<"initial_position "<<initial_position.x<<"
  // "<<initial_position.y<<endl;

  size_t cycles = 0;
  size_t option_count = 0;
  for (const auto extra : visited) {
    cout << option_count << endl;
    // cout<<"extra "<<extra.x<<" "<<extra.y<<endl;
    auto updated_data = data;
    if (extra != initial_position) {
      updated_data[extra.x][extra.y] = "#";
    }
    guard.map = updated_data;
    guard.set_pose(initial_position, initial_direction);

    if (guard.patrol().empty()) {
      ++cycles;
    } else {
      // cout<<"no cycle"<< endl;
    }
    option_count++;
  }

  cout << "options :" << cycles << endl;
  return 0;
}