#include "Day25.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>

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
  std::vector<std::vector<std::string>> keys {}, locks {};
  std::vector<std::string> single_data {};
  std::string keyStr {"....."}, lockStr {"#####"};
  bool data {false}, kl {false};
  int size {5}, counter {0};
	while (std::getline(inputFile,inputStr))
	{
    if (!data) {
      if (counter!=0) {
        counter -= 1;
        continue;
      }
      if (inputStr==keyStr) {
        counter = size;
        kl = true;
        data = true;
        single_data = {};
      } else if (inputStr==lockStr) {
        counter = size;
        kl = false;
        data = true;
        single_data = {};
      }
    } else {
      single_data.push_back(inputStr);
      counter -= 1;
      if (counter==0) {
        data = false;
        counter = 2;
        if (kl) {
          keys.push_back(single_data);
        } else {
          locks.push_back(single_data);
        }
      }
    }

	}

	inputFile.close();

  // for (auto entry : keys) {
  //   for (auto line : entry) {
  //     std::cout << line << "\n";
  //   }
  //   std::cout << "\n";
  // }

  // for (auto entry : locks) {
  //   for (auto line : entry) {
  //     std::cout << line << "\n";
  //   }
  //   std::cout << "\n";
  // }

	auto t2 {std::chrono::high_resolution_clock::now()};

  int length {size};
  char looking_for {};

  // for keys, we are looking for the first '#' or the last '.'
  // note that we have to do size-it2 in order to account for distance being measured from the bottom to the top
  std::vector<std::vector<int>> keys_l {};
  looking_for = '#';
  for (std::size_t k {0}; k<keys.size(); ++k) {
    std::vector<int> into {};
    for (std::size_t it1 {0}; it1<length; ++it1) {
      for (std::size_t it2 {0}; it2<length; ++it2) {
        if (keys[k][it2][it1] == looking_for) {
          into.push_back(length-it2);
          break;
        }
        if (it2==length-1) into.push_back(0);
      }
    }
    keys_l.push_back(into);
  }

  // for locks, we are looking for the first '.' or the last '#'
  // distance is measured from the top to the bottom - we can just take it2 straight up
  std::vector<std::vector<int>> locks_l {};
  looking_for = '.';
  for (std::size_t l {0}; l<locks.size(); ++l) {
    std::vector<int> into {};
    for (std::size_t it1 {0}; it1<length; ++it1) {
      for (std::size_t it2 {0}; it2<length; ++it2) {
        if (locks[l][it2][it1] == looking_for) {
          into.push_back(it2);
          break;
        }
        if (it2==length-1) into.push_back(length);
      }
    }
    locks_l.push_back(into);
  }

  // std::cout << "keys\n";
  // for (auto entry : keys_l) {
  //   for (auto e : entry) {
  //     std::cout << e << " ";
  //   }
  //   std::cout << "\n";
  // }
  // std::cout << "locks\n";
  // for (auto entry : locks_l) {
  //   for (auto e : entry) {
  //     std::cout << e << " ";
  //   }
  //   std::cout << "\n";
  // }

  int fitting {0};
  for (std::size_t itk {0}; itk<keys_l.size(); ++itk) {
    for (std::size_t itl{0}; itl<locks_l.size(); ++itl) {
      bool success {true};
      for (std::size_t count {0}; count<length; ++count) {
        if (keys_l[itk][count]+locks_l[itl][count]>5) {
          success = false;
          break;
        }
      }
      if (success) fitting += 1;
    }
  }

  auto t3 {std::chrono::high_resolution_clock::now()};



  auto t4 {std::chrono::high_resolution_clock::now()};

  std::cout << "Number of unique fitting lock-key pairs: " << fitting << "\n";

	std::cout << "Program took, in microseconds:" << "\n";
	std::cout << "  Total Time:               " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t1).count() << "\n";
	std::cout << "  Reading in Input File:    " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << "\n";
	std::cout << "  Problem Solving (Part 1): " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count() << "\n";
	std::cout << "  Problem Solving (Part 2): " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t3).count() << "\n";


	return 0;
}
