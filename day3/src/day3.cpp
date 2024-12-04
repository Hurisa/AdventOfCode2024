#include <fstream>
#include <iostream>
#include <numeric>
#include <regex>
#include <string>
#include <vector>

using namespace std;

float process_string(const auto str) {
  std::regex pattern(R"(mul\((\d{1,3}),(\d{1,3})\))");
  std::smatch matches;

  auto begin = str.cbegin();
  auto end = str.cend();

  vector<float> parcels;
  while (std::regex_search(begin, end, matches, pattern)) {
    float d1 = std::stoi(matches[1].str());
    float d2 = std::stoi(matches[2].str());
    parcels.push_back(d1 * d2);
    std::cout << "Found match: mul(" << d1 << "," << d2 << ")" << std::endl;
    std::cout << "Extracted digits: d1 = " << d1 << ", d2 = " << d2
              << std::endl;

    begin = matches.suffix().first; // Move to the rest of the string
  }
  return long(accumulate(parcels.begin(), parcels.end(), 0.0));
}

int main() {

  const string file_path = "/home/hugo/AoC/AdventOfCode2024/day3/data.txt";
  // Open the file
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

  cout << "Result 1: "
       << long(accumulate(v.begin(), v.end(), 0.0,
                          [](double sum, const string &str) {
                            return sum + process_string(str);
                          }))
       << endl;

  return 0;
}