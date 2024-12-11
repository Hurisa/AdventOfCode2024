#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;
int main() {
    // File name
    const std::string file_path = "/home/hugo/AoC/AdventOfCode2024/day5/data.txt";

    // Containers for the two parts
    std::vector<std::pair<int, int>> pairs;
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
        // Check if the line contains a comma (indicating the second part)
        if (line.find(',') != std::string::npos) {
            isSecondPart = true;
        }

        if (!isSecondPart) {
            // Parse pairs (first part)
            std::istringstream iss(line);
            int a, b;
            char delimiter;
            if (iss >> a >> delimiter >> b && delimiter == '|') {
                pairs.emplace_back(a, b);
            }
        } else {
            // Parse lists (second part)
            std::istringstream iss(line);
            std::vector<int> list;
            std::string number;
            while (std::getline(iss, number, ',')) {
                cout<<number<<endl;
                list.push_back(std::stoi(number));
            }
            lists.push_back(list);
        }
    }

    inputFile.close();

    

    // Debug: Print the contents of the containers
    std::cout << "Pairs (First Part):\n";
    for (const auto& p : pairs) {
        std::cout << p.first << " | " << p.second << std::endl;
    }

    std::cout << "\nLists (Second Part):\n";
    for (const auto& list : lists) {
        for (size_t i = 0; i < list.size(); ++i) {
            std::cout << list[i];
            if (i != list.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }

    return 0;
}
