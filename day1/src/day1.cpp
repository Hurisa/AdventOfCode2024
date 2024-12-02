#include <algorithm>
#include <boost/range/combine.hpp>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>

using namespace std;
int main() {
  const string file_path = "/home/hugo/AoC/AdventOfCode2024/day1/data.txt";

  // Open the file
  ifstream file(file_path);
  if (!file) {
    cerr << "Error: Unable to open file at " << file_path << endl;
    return 1;
  }

  // Vectors to store the two columns
  vector<float> column1, column2;

  // Read each line from the file
  string line;
  while (getline(file, line)) {
    istringstream iss(line);
    float value1, value2;
    // Extract two float values from the line
    if (iss >> value1 >> value2) {
      column1.push_back(value1);
      column2.push_back(value2);
    } else {
      cerr << "Error: Malformed line: " << line << endl;
    }
  }

  file.close(); // Close the file

  sort(column1.begin(), column1.end());
  sort(column2.begin(), column2.end());

  const auto pairs = boost::combine(column1, column2);
  const auto result_part_1 =
      accumulate(pairs.begin(), pairs.end(), 0.0, [](int total, auto pair) {
        return total + std::abs(boost::get<0>(pair) - boost::get<1>(pair));
      });

  cout << "Result 1: " << long(result_part_1) << endl;

  const auto result_part_2 = accumulate(
      column1.begin(), column1.end(), 0, [&column2](int total, auto value) {
        return total +
               long(value * std::count(column2.begin(), column2.end(), value));
      });

  cout << "Result 2: " << result_part_2 << endl;

  return 0;
}
