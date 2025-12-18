#include "Day10.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <sstream>
#include <cmath>

int main(int argc, char* argv[])
{

// argc is the number of command line arguments
// argv is the value of command line arguments
// currently accept argc==1 (no additional) or argc==2 (1 additional, filename w/ no .txt)

  std::string fileName {"sample.txt"}, extensionDefault {".txt"};
  if (argc<=1) {
  } else if (argc==2) {
    fileName = argv[1];
    if (fileName.find('.')==std::string::npos) {
      fileName += extensionDefault;
    } 
  } else {
    std::cerr << "Received " << argc-1 << " additional command line arguments" << "\n";
    std::cerr << "Only accepts up to 1 additional argument:" << "\n";
    std::cerr << "  input filename (default \"" << fileName << "\", extension defaults to \"" << extensionDefault << "\")" << "\n";
  }

	auto t1 {std::chrono::high_resolution_clock::now()};

	std::ifstream inputFile {fileName};
	if (!inputFile)
	{
		std::cerr << "Input file \"" << fileName << "\" could not be opened\n";
		return 1;
	}

	std::string inputStr {};
  int part1{0}, part2{0};

  std::vector<std::vector<bool>> on {};
  std::vector<std::vector<std::vector<bool>>> buttons {};
  std::vector<std::vector<int>> joltage {};

	while (std::getline(inputFile,inputStr))
	{
    std::size_t bracketL {inputStr.find_first_of('[')};
    std::size_t bracketR {inputStr.find_first_of(']')};
    std::size_t braceL {inputStr.find_first_of('{')};
    std::size_t braceR {inputStr.find_first_of('}')};

    std::string bracket {inputStr.substr(bracketL+1,bracketR-bracketL-1)};
    std::string brace {inputStr.substr(braceL+1,braceR-braceL-1)};
    std::istringstream parens {inputStr.substr(bracketR+2,braceL-bracketR-3)};

    std::vector<bool> on_sub {};
    std::size_t sz = 0;
    for (auto ch : bracket) {
      if (ch=='.') {
        on_sub.push_back(false);
      } else {
        on_sub.push_back(true);
      }
      sz++;
    }
    on.push_back(on_sub);

    // parse into std::vector<std::vector<std::vector<bool>>> buttons; where we have buttons[case][button_num][affects_light]
    std::string button{};
    std::vector<std::vector<bool>> casenum{};
    while (parens >> button) {
      std::istringstream button_noparen {button.substr(1,static_cast<std::size_t>(button.end()-button.begin()-2))};
      std::string num{};
      std::vector<bool> blist(sz) ;
      while (std::getline(button_noparen,num,',')) {
        std::size_t nn = static_cast<std::size_t>(std::stoi(num));
        blist[nn] = true;
      }
      casenum.push_back(blist);
    }
    buttons.push_back(casenum);

    std::vector<int> jolt_sub {};
    std::string jolt {};
    for (auto ch : brace) {
      if (ch==',') {
        jolt_sub.push_back(std::stoi(jolt));
        jolt = "";
      } else {
        jolt += ch;
      }
    }
    jolt_sub.push_back(std::stoi(jolt));
    joltage.push_back(jolt_sub);
	}

  // for (auto j : joltage) {
  //   for (auto jj : j) {
  //     std::cout << " " << jj;
  //   }
  //   std::cout << "\n";
  // }
  // for (auto c : buttons) {
  //   for (auto b : c) {
  //     for (bool tf : b) {
  //       std::cout << tf;
  //     }
  //     std::cout << "\n";
  //   }
  // }
  // for (auto o : on) {
  //   for (auto oo : o) {
  //     std::cout << " " << oo;
  //   }
  //   std::cout << "\n";
  // }

	inputFile.close();

	auto t2 {std::chrono::high_resolution_clock::now()};

  // now that we have verified that the problem is appropriately parsed
  for (std::size_t Case=0; Case<on.size(); Case++) {
    std::vector<bool> goal = on[Case];
    std::vector<std::vector<bool>> presses = buttons[Case]; // [this button][affects these lights]
    std::vector<int> jolts = joltage[Case];

    std::size_t total_combinations = static_cast<std::size_t>(static_cast<double>(presses.size())*(std::pow(presses.size(),presses.size())-1)/static_cast<double>(presses.size()-1));

    // FF:F, TF:T, TT:F, FT:T - use bitwise xor (^)
    bool found = false;
    std::size_t options=0;
    std::vector<bool> start(goal.size(),false);
    while (!found) {
      // try buttons: every single combination
      // restart with initially making everything false
      std::vector<bool> current {start};
      std::vector<std::size_t> these_buttons {};
      // set these_buttons
      std::size_t current_counter = options;
      do {
        these_buttons.push_back(current_counter%presses.size());
        current_counter/=presses.size();
      } while (current_counter>0);

      // try pushing the buttons in these_buttons, this_button at a time
      for (std::size_t this_button=0; this_button<these_buttons.size(); this_button++) {
        // element-wise use xor to push buttons
        for (std::size_t b=0; b<goal.size(); b++) {
          current[b] = current[b] ^ presses[these_buttons[this_button]][b];
        }
      }

      if (current==goal) {
        found = true;
        part1 += static_cast<int>(these_buttons.size());

        // output to make sure that it works...
        std::cout << "case num: " << Case << " size: " << these_buttons.size() << " buttons:";
        for (auto it : these_buttons) {
          std::cout << " " << it;
        }
        std::cout << "\n";

        break; // break for loop; found solution
      } else {
        options++;
      }
      if (options>=total_combinations) {
        std::cerr << "no solution found...\n";
        return 0;
      }
    }
  }

  // 1, 2, 3, 4, 5; 12, 13, 14, 15; 23, 24, 25; 34, 35; 45, 123, 124 125 134 135 145 234 235 245 345 

  auto t3 {std::chrono::high_resolution_clock::now()};



  auto t4 {std::chrono::high_resolution_clock::now()};

	std::cout << "Program took, in microseconds:" << "\n";
	std::cout << "  Total Time:               " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t1).count() << "\n";
	std::cout << "  Reading in Input File:    " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << "\n";
	std::cout << "  Problem Solving (Part 1): " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count() << "\n";
	std::cout << "  Problem Solving (Part 2): " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t3).count() << "\n";

  std::cout << "\nPart 1: " << part1 << "\n";
  std::cout << "Part 2: " << part2 << "\n";

	return 0;
}
