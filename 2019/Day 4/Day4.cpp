#include "Day4.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <algorithm>

int main()
{

// argc is the number of command line arguments
// argv is the value of command line arguments
// currently accept argc==1 (no additional) or argc==2 (1 additional, filename w/ no .txt)


	auto t2 {std::chrono::high_resolution_clock::now()};

  std::string start {"372037"}, finish {"905157"};
  int Nsol1 {0}, Nsol2 {0};
  for (int i {std::stoi(start)}; i<=std::stoi(finish); i++) {
    std::string str{std::to_string(i)}, str2{std::to_string(i)};
    std::sort(str.begin(),str.end());
    if (str != str2) continue;
    int count = 0;
    char prev = '\0';
    bool is_two = false, g_two = false;
    for (char c : str) {
      if (c == prev) {
        ++count;
      } else {
        prev = c;
        g_two = g_two || count >= 2;
        is_two = is_two || count == 2;
        count = 1;
      }
    }
    g_two = g_two || count >= 2;
    is_two = is_two || count == 2;
    if (g_two) Nsol1 +=1;
    if (is_two) Nsol2 += 1;
  }
  
  std::cout << Nsol1 << "\n";
  std::cout << Nsol2 << "\n";

  auto t3 {std::chrono::high_resolution_clock::now()};



  auto t4 {std::chrono::high_resolution_clock::now()};

	std::cout << "Program took, in microseconds:" << "\n";
	std::cout << "  Total Time:               " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t2).count() << "\n";
	std::cout << "  Problem Solving (Part 1): " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count() << "\n";
	std::cout << "  Problem Solving (Part 2): " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t3).count() << "\n";


	return 0;
}
