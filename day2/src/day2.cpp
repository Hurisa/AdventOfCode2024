#include <algorithm>
#include <boost/optional.hpp>
#include <boost/range/combine.hpp>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <ranges>
#include <sstream>
#include <vector>

using namespace std;

vector<int> remove_level(const vector<int> &sequence, int level) {
  auto new_sequence = sequence;
  new_sequence.erase(new_sequence.begin() + level);
  return new_sequence;
}

bool is_sequence_safe(const vector<int> &sequence, const bool use_dampener) {
  std::vector<int> prev(sequence.begin(), sequence.end() - 1);
  std::vector<int> next(sequence.begin() + 1, sequence.end());

  boost::optional<string> direction = boost::none;
  bool is_unsafe = false;

  const auto pairs = boost::combine(prev, next);

  for (const auto pair : pairs) {
    const auto first = boost::get<0>(pair);
    const auto last = boost::get<1>(pair);

    if (!direction) {
      direction = last > first ? "increasing" : "decreasing";
    }
    const auto new_direction = last > first ? "increasing" : "decreasing";

    is_unsafe = (last == first) || (*direction != new_direction) ||
                abs(last - first) > 3;

    if (is_unsafe && !use_dampener) {
      return false;
    }
    if (is_unsafe && use_dampener) {
      break;
    }
  }

  if (is_unsafe) {
    return std::ranges::any_of(
        std::views::iota(static_cast<std::size_t>(0), sequence.size()),
        [&](size_t index) {
          return is_sequence_safe(remove_level(sequence, index), false);
        });
  }

  return true;
}

int main() {
  const string file_path = "/home/hugo/AoC/AdventOfCode2024/day2/data.txt";

  // Open the file
  ifstream file(file_path);
  if (!file) {
    cerr << "Error: Unable to open file at " << file_path << endl;
    return 1;
  }

  // Read each line from the file
  string line;
  vector<vector<int>> sequences;
  while (getline(file, line)) {
    std::stringstream ss(line);
    vector<int> sequence;
    int number;
    while (ss >> number) {        // Read numbers from the stringstream
      sequence.push_back(number); // Add each number to the row
    }
    sequences.push_back(sequence);
  }

  int count(0);
  for (const auto &s : sequences) {
    if (is_sequence_safe(s, true)) {
      ++count;
    }
  }
  cout << "safe: " << count << endl;
}
