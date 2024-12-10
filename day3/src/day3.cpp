#include <boost/optional.hpp>
#include <fstream>
#include <iostream>
#include <numeric>
#include <regex>
#include <string>
#include <vector>

using namespace std;

vector<size_t> get_indices_for_pattern(const regex pattern, const string str) {
  std::smatch matches;
  std::vector<size_t> indices;
  auto begin = str.cbegin();
  auto end = str.cend();
  while (std::regex_search(begin, end, matches, pattern)) {
    const auto position = begin - str.cbegin() + matches.position(0);
    indices.push_back(position);
    begin = matches.suffix().first;
  }
  return indices;
}

boost::optional<size_t>
get_relevant_action_index_for_position(const vector<size_t> action_indices,
                                       size_t position) {
  if (action_indices.size() == 0) {
    return boost::none;
  }

  auto it =
      std::lower_bound(action_indices.begin(), action_indices.end(), position);

  if (*it == *action_indices.data()) {
    return boost::none;
  }

  --it;
  if (find(action_indices.begin(), action_indices.end(), *it) ==
      action_indices.end()) {
    return boost::none;
  }

  return *it;
}

long process_string(const std::string &str, const bool &part1) {

  std::regex do_pattern(R"(do\(\))");
  const auto do_indices = get_indices_for_pattern(do_pattern, str);

  std::regex dont_pattern(R"(don't\(\))");
  const auto dont_indices = get_indices_for_pattern(dont_pattern, str);

  std::regex pattern(R"(mul\((\d{1,3}),(\d{1,3})\))");
  std::smatch matches;

  auto begin = str.cbegin();
  auto end = str.cend();

  vector<float> parcels;
  while (std::regex_search(begin, end, matches, pattern)) {
    float d1 = std::stoi(matches[1].str());
    float d2 = std::stoi(matches[2].str());

    const auto position = begin - str.cbegin() + matches.position(0);

    const auto do_index =
        get_relevant_action_index_for_position(do_indices, position);
    const auto dont_index =
        get_relevant_action_index_for_position(dont_indices, position);

    auto compute = part1 || do_index && dont_index && *dont_index < *do_index ||
                   do_index && !dont_index || !do_index && !dont_index;

    if (compute) {
      parcels.push_back(long(d1 * d2));
    }
    begin = matches.suffix().first;
  }

  return long(accumulate(parcels.begin(), parcels.end(), 0.0));
}

int main() {

  const string file_path = "/home/hugo/AoC/AdventOfCode2024/day3/data.txt";
  ifstream file(file_path);
  if (!file) {
    cerr << "Error: Unable to open file at " << file_path << endl;
    return 1;
  }

  string line;
  vector<string> v;
  while (getline(file, line)) {
    v.push_back(line);
  }

  const auto full_string = std::accumulate(v.begin(), v.end(), std::string());
  cout << "Part 1: " << long(process_string(full_string, true)) << endl;
  cout << "Part 2: " << long(process_string(full_string, false)) << endl;

  return 0;
}