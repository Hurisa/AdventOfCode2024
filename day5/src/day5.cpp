
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

std::vector<int> get_relevant_values(std::vector<int> column1,
                                     std::vector<int> column2, int value) {
  auto view =
      std::views::iota(0ul, column1.size()) |
      std::views::filter(
          [&column1, value](size_t i) { return column1[i] == value; }) |
      std::views::transform([&column2](size_t i) { return column2[i]; });

  std::vector<int> result;
  std::ranges::copy(view, std::back_inserter(result));
  return result; // Return the populated vector
}

bool is_list_valid(std::vector<int> column1, std::vector<int> column2,
                   std::vector<int> list) {

  for (const auto &value : list) {

    const auto relevant_values = get_relevant_values(column1, column2, value);
    const auto it = std::find(list.begin(), list.end(), value);

    const auto is_invalid = [&relevant_values](const auto val) {
      return std::find(relevant_values.begin(), relevant_values.end(), val) !=
                 relevant_values.end() &&
             !relevant_values.empty();
    };

    if (std::any_of(list.begin(), it, is_invalid)) {
      return false;
    }
  }
  return true;
}

std::vector<int> correct_list(const std::vector<int> &column1,
                              const std::vector<int> &column2,
                              const std::vector<int> &list) {
  auto candidate_list = list;
  for (const auto &value : list) {

    const auto relevant_values = get_relevant_values(column1, column2, value);
    if (relevant_values.empty()) {
      continue;
    }

    auto it = std::find(candidate_list.begin(), candidate_list.end(), value);

    for (const auto val : relevant_values) {
      auto it2 = std::find(candidate_list.begin(), it, val);
      if (it2 != it) {
        std::iter_swap(it, it2);
        const std::vector<int> new_list(candidate_list.begin(), candidate_list.end());
        if (is_list_valid(column1, column2, new_list)) {
          return new_list;
        }
        return correct_list(column1, column2, new_list);
      }
    }
  }
  return candidate_list;
}

int main() {
  // File name
  std::string file_path = "/home/hugo/AoC/AdventOfCode2024/day5/data.txt";

  // Containers for the two parts
  std::vector<int> column1;
  std::vector<int> column2;
  std::vector<std::vector<int>> lists;

  // Open the file
  std::ifstream inputFile(file_path);
  if (!inputFile.is_open()) {
    std::cerr << "Failed to open the file." << std::endl;
    return 1;
  }

  std::string line;
  bool isSecondPart = false;

  // Read the file line by line
  while (std::getline(inputFile, line)) {
    // Trim leading/trailing whitespace
    line.erase(0, line.find_first_not_of(" \t"));
    line.erase(line.find_last_not_of(" \t") + 1);

    // Skip empty lines
    if (line.empty())
      continue;

    // Check if the line contains a comma (indicating the second part)
    if (line.find(',') != std::string::npos) {
      isSecondPart = true;
    }

    if (!isSecondPart) {
      // Parse columns (first part)
      std::istringstream iss(line);
      int a, b;
      char delimiter;
      if (iss >> a >> delimiter >> b && delimiter == '|') {
        column1.push_back(a);
        column2.push_back(b);
      } else {
        std::cerr << "Invalid pair format: " << line << std::endl;
      }
    } else {
      // Parse lists (second part)
      std::istringstream iss(line);
      std::vector<int> list;
      std::string number;
      while (std::getline(iss, number, ',')) {
        // Trim whitespace around each number
        number.erase(0, number.find_first_not_of(" \t"));
        number.erase(number.find_last_not_of(" \t") + 1);

        if (!number.empty()) {
          try {
            list.push_back(std::stoi(number));
          } catch (const std::invalid_argument &e) {
            std::cerr << "Invalid number format: " << number << std::endl;
          }
        }
      }
      lists.push_back(list);
    }
  }

  inputFile.close();

  size_t middle_page_sum = 0;
  size_t middle_page_sum_corrected = 0;
  for (const auto &list : lists) {

    const auto valid_list = is_list_valid(column1, column2, list);

    if (!valid_list) {
      const auto corrected_list = correct_list(column1, column2, list);
      const auto middle_page_index_corrected =
          (corrected_list.end() - corrected_list.begin()) / 2;
      middle_page_sum_corrected += corrected_list[middle_page_index_corrected];
    }

    if (valid_list) {
      const auto middle_page_index = (list.end() - list.begin()) / 2;
      middle_page_sum += list[middle_page_index];
    }
  }

  cout << "result 1: " << middle_page_sum << endl;
  cout << "result 2: " << middle_page_sum_corrected << endl;

  return 0;
}
