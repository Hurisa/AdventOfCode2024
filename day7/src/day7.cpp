#include <cstdint> // For uint64_t
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

using namespace std;

void generatePermutations(int n, const std::string &current,
                          std::vector<std::string> &results) {
  if (current.size() == n) {
    results.push_back(current);
    return;
  }

  generatePermutations(n, current + "A", results);
  generatePermutations(n, current + "M", results);
  generatePermutations(n, current + "C", results);
}
uint64_t operate(uint64_t a, uint64_t b, char s) {
  std::string str(1, s);
  if (str == "A") {
    return a + b;
  }
  if (str == "M") {
    return a * b;
  }
  if (str == "C") {
    std::string str1 = std::to_string(a);
    std::string str2 = std::to_string(b);
    return stoull(str1 + str2);
  }
  return -1;
}

int main() {

  std::ifstream file_path("/home/hugo/AoC/AdventOfCode2024/day7/data.txt");
  if (!file_path) {
    std::cerr << "Error: Could not open the file!" << std::endl;
    return 1;
  }

  std::map<uint64_t, std::vector<uint64_t>> dataMap{};
  std::string line;

  while (std::getline(file_path, line)) {
    std::istringstream lineStream(line);
    cout << "line: " << line << endl;
    uint64_t key;
    char colon;
    lineStream >> key >> colon;

    std::vector<uint64_t> values;
    uint64_t value;
    while (lineStream >> value) {
      values.push_back(value);
    }

    dataMap[key] = values;
    cout << endl;
  }

  file_path.close();

  uint64_t test_values = 0;
  for (const auto [key, vector] : dataMap) {
    std::vector<std::string> permutations;
    generatePermutations(vector.size() - 1, "", permutations);
    for (const auto p : permutations) {
      uint64_t total = 0;
      for (size_t op = 0; op < p.size(); op++) {
        total = total == 0 ? total + operate(vector[op], vector[op + 1], p[op])
                           : operate(total, vector[op + 1], p[op]);
      }
      if (total == key) {
        test_values += total;
        break;
      }
    }
  }
  cout << test_values << endl;

  return 0;
}
