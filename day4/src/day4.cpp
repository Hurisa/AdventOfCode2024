#include <algorithm>
#include <boost/optional.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

enum class Direction {
  North,
  South,
  East,
  West,
  NorthEast,
  SouthEast,
  SouthWest,
  NorthWest
};

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
  case Direction::NorthEast:
    return "NorthEast";
  case Direction::SouthEast:
    return "SouthEast";
  case Direction::SouthWest:
    return "SouthWest";
  case Direction::NorthWest:
    return "NorthWest";
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
  map[Direction::NorthEast] = std::make_pair(-1, 1);
  map[Direction::SouthEast] = std::make_pair(1, 1);
  map[Direction::SouthWest] = std::make_pair(1, -1);
  map[Direction::NorthWest] = std::make_pair(-1, -1);

  return map;
}

class Position {

public:
  Position(int x, int y, size_t xmax, size_t ymax)
      : x(x), y(y), xmax(xmax), ymax(ymax) {}

  int x;
  int y;
  size_t xmax;
  size_t ymax;

  // Define the equality operator
  bool operator==(const Position &other) const {
    return x == other.x && y == other.y && xmax == other.xmax &&
           ymax == other.ymax;
  }
  boost::optional<Position> travel(const Direction &direction) const {

    const auto direction_to_index = DirectionToIndex();
    const auto destination =
        Position(x + DirectionToIndex()[direction].first,
                 y + DirectionToIndex()[direction].second, xmax, ymax);

    if (!destination.is_valid()) {
      return boost::none;
    }
    return destination;
  }

  bool is_valid() const {
    return (0 <= x && x <= xmax) && (0 <= y && y <= ymax);
  }
};

bool CheckForLetterInDirection(const string &s, const Position &position,
                               const vector<vector<string>> &data,
                               const Direction &direction) {

  const auto candidate_position = position.travel(direction);

  if (candidate_position) {
    const auto pos = *candidate_position;
    if (data[pos.x][pos.y] == s) {
      return true;
    }
  }
  return false;
}

int main() {

  const string file_path = "/home/hugo/AoC/AdventOfCode2024/day4/data.txt";
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

  const auto xlimit = rows.size() - 1;
  const auto ylimit = columns.size() - 1;

  size_t total(0);
  for (const auto row : rows) {
    for (const auto column : columns) {
      if (data[row][column] == "X") {
        const auto pos = Position(row, column, xlimit, ylimit);

        for (const auto direction : possible_directions) {

          if (!CheckForLetterInDirection("M", pos, data, direction)) {
            continue;
          }
          const auto m_position = *pos.travel(direction);

          if (!CheckForLetterInDirection("A", m_position, data, direction)) {
            continue;
          }
          const auto a_position = *m_position.travel(direction);
          if (!CheckForLetterInDirection("S", a_position, data, direction)) {
            continue;
          }
          total++;
        }
      }
    }
  }

  cout << total << endl;

  size_t total_2(0);
  for (const auto row : rows) {
    for (const auto column : columns) {
      if (data[row][column] == "A") {
        cout << "found A at " << row << " " << column << endl;
        const auto pos = Position(row, column, xlimit, ylimit);

        const auto ne = pos.travel(Direction::NorthEast);
        const auto se = pos.travel(Direction::SouthEast);
        const auto nw = pos.travel(Direction::NorthWest);
        const auto sw = pos.travel(Direction::SouthWest);

        if (!ne || !se || !nw || !ne) {
          continue;
        }

        const auto ne_position = *ne;
        const auto sw_position = *sw;
        const auto se_position = *se;
        const auto nw_position = *nw;

        const auto ne_string = data[ne_position.x][ne_position.y];
        const auto se_string = data[se_position.x][se_position.y];
        const auto nw_string = data[nw_position.x][nw_position.y];
        const auto sw_string = data[sw_position.x][sw_position.y];

        bool option1 = ne_string == "M" && sw_string == "S" &&
                       nw_string == "M" && se_string == "S";
        bool option2 = ne_string == "S" && sw_string == "M" &&
                       nw_string == "S" && se_string == "M";
        bool option3 = ne_string == "S" && sw_string == "M" &&
                       nw_string == "M" && se_string == "S";
        bool option4 = ne_string == "M" && sw_string == "S" &&
                       nw_string == "S" && se_string == "M";

        if (option1 || option2 || option3 || option4) {
          total_2++;
        }
      }
    }
  }

  cout << total_2 << endl;
  return 0;
}