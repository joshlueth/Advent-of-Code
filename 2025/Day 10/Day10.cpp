#include "Day10.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <sstream>
#include <cmath>
#include <numeric>
#include <cstdlib>
#include <algorithm>

void rref(std::vector<std::vector<long long>>& mat, std::vector<size_t>& free) {
  std::size_t col=0, row=0;
  while(row<mat.size()) {
    while (col<mat[0].size()-1) {
      // check if we need to pivot
      if (mat[row][col]==0) {
        std::size_t irow=row+1;
        while (irow<mat.size()) {
          if (mat[irow][col]!=0) {
            // found pivot, swap rows
            for (std::size_t icol=0; icol<mat[row].size(); icol++) {
              long long swap = mat[row][icol];
              mat[row][icol] = mat[irow][icol];
              mat[irow][icol] = swap;
            }
            irow = mat.size();
          } else {
            irow++;
          }
        }
        // if we can't pivot, this is a free variable and we need to go to the next column
        if (mat[row][col]==0) {
          free.push_back(col);
          col++;
          continue;
        }
      }

      // ensure that pivot index is positive
      if (mat[row][col]<0) {
        for (std::size_t icol=col; icol<mat[row].size(); icol++) {
          mat[row][icol] *= -1;
        }
      }

      // eliminate other rows
      long long current = std::abs(mat[row][col]);
      for (std::size_t down=row+1; down<mat.size(); down++) {
        long long next = std::abs(mat[down][col]);
        if (next!=0) {
          // make sure we can do integer math when reducing
          for (std::size_t icol=col; icol<mat[row].size(); icol++) {
            mat[row][icol] *= next;
            mat[down][icol] *= current;
          }
          // need to update current
          current = std::abs(mat[row][col]);

          // reduce 
          long long sign_store = mat[down][col]/std::abs(mat[down][col]);
          for (std::size_t icol=col; icol<mat[row].size(); icol++) {
            mat[down][icol] -= sign_store * mat[row][icol];
          }
          // std::cout << "reduced\n";
        }
      }
      // further eliminate rows above
      for (std::size_t up=0; up<row; up++) {
        long long prev = std::abs(mat[up][col]);
        if (prev!=0) {
          // make sure we can do integer math when reducing
          for (std::size_t icol=0; icol<mat[row].size(); icol++) {
            mat[row][icol] *= prev/std::gcd(prev,current);
            mat[up][icol] *= current/std::gcd(prev,current);
          }
          // need to update current
          current = std::abs(mat[row][col]);
          // reduce
          long long sign_store = mat[up][col]/std::abs(mat[up][col]);
          for (std::size_t icol=col; icol<mat[row].size(); icol++) {
            mat[up][icol] -= sign_store * mat[row][icol];
          }
        }
      }

      // succeeded in pivoting first row and column
      row++; col++;
      if (row>=mat.size()) {
        while (col<mat[0].size()-1) {
          free.push_back(col++);
        }
      }
    }
    if (row<mat.size()) {
      // at this point, the final remaining rows should be all zero
      for (std::size_t icol=0; icol<mat[row].size(); icol++) {
        if (mat[row][icol]!=0) {
          std::cout << row << " Should be zero!\n";
        }
      }
      mat.erase(mat.begin()+static_cast<int>(row)); // erase entry with all zeros
    }
  }
  // iterate over each row; make sure that the initial pivot is 1!
  for (std::size_t r=0; r<mat.size(); r++) {
    bool found_pivot = false;
    long long div_val = 0;
    for (std::size_t c=0; c<mat[0].size(); c++) {
      if (mat[r][c]!=0) {
        if (!found_pivot) {
          div_val = std::abs(mat[r][c]);
          found_pivot = true;
        } else {
          div_val = std::gcd(std::abs(mat[r][c]),div_val);
        }
      }
    }
    
    if (div_val!=0) {
      for (std::size_t c=0; c<mat[0].size(); c++) {
        mat[r][c] /= div_val;
      }
    }
  }
  return;
}

int backward_sub(std::vector<std::vector<long long>>& mat, std::vector<long long>& csol) {
  for (std::size_t rowt=mat.size(); rowt>0; rowt--) {
    std::size_t row = rowt-1;
    bool first = false;
    long long first_val;
    std::size_t first_loc;
    for (std::size_t col=0; col<mat[row].size()-1; col++) {
      if (mat[row][col]==0) continue;
      if (!first) {
        first_val = mat[row][col];
        first_loc = col;
        csol[first_loc] = mat[row][mat[row].size()-1]; // error
        first = true;
      } else {
        csol[first_loc] -= mat[row][col]*csol[col]; // error
      }
    }
    if (std::lcm(csol[first_loc],first_val)!=csol[first_loc]) return -1;
    csol[first_loc] /= first_val; // error
    if (csol[first_loc]<0) {
      return -1;
    }
  }
  long long presses = 0;
  for (long long cs : csol) {
    presses += cs;
  }
  return static_cast<int>(presses);
}

int num_pushes(std::vector<std::vector<long long>>& mat, std::vector<std::size_t>& free, std::vector<int> jolts) {

  int maxj = *std::max_element(jolts.begin(),jolts.end());
  int total_pushes=maxj*(static_cast<int>(mat[0].size())-1);

  // initial free variable guesses
  std::vector<long long> guesses(mat[0].size(),0);
  // initially upper and lower values for each guess
  std::vector<int> lower(free.size(),0), upper(free.size(),maxj), current(free.size(),0);

  // get bounds for guesses
  // for (std::size_t rows=0; rows<mat.size(); rows++) {
  //   for (std::size_t fr=0; fr<free.size(); fr++) {
  //     if (mat[rows].back()>0) {
  //       if (mat[rows][free[fr]]>0) {
  //         upper[fr] = std::min(upper[fr],static_cast<int>(mat[rows].back()/mat[rows][free[fr]]));
  //       } else if (mat[rows][free[fr]]<0) {
  //         lower[fr] = std::max(lower[fr],static_cast<int>( ));
  //       }
  //     }
  //   }
  // }

  // eliminate dimension // don't eliminate dimension
  // std::vector<int> sums(mat[0].size()+1,1);
  // for (std::size_t iter=0; iter<mat.size(); iter++) { // sum down over rows
  //   for (std::size_t iter2=0; iter2<mat[0].size(); iter2++) { // add sum from columns
  //     sums[iter2] -= mat[iter][iter2];
  //   }
  //   sums.back() += -mat[iter].back() - 1;
  // }
  // if we add up every row, we get (a+b+...) = sum(last row) - a_extra - b_extra - ...
  // therefore we have, for a total number of buttons, a way to choose the requisite free variables

  // loop over all possible guesses, find minimum number of pushes
  std::size_t adjust = 0; // set to 1 if eliminating free variable, otherwise set to 0
  current = lower;
  while (true) {

    // assign guesses to current
    for (std::size_t ind=0; ind<free.size(); ind++) {
      guesses[free[ind]] = current[ind];
    }

    // update current by iterating through possibilities
    if (current.size()>0) current[0]+=1;
    for (std::size_t ind=0; ind+1+adjust<current.size(); ind++) {
      if (current[ind]>upper[ind]) {
        current[ind] = lower[ind];
        current[ind+1] +=1;
      } else {
        break;
      }
    }

    // get number of pushes
    int this_pushes=0;
    this_pushes = backward_sub(mat,guesses);
    
    if (this_pushes>=0) {
      total_pushes = std::min(total_pushes,this_pushes);
    }

    // if we only have one solution, we have solved it!
    if (current.size()<=adjust) break;
    // otherwise, this represents the complete iteration
    else if (current[current.size()-1-adjust]>upper[current.size()-1-adjust]) break;
  }

  return total_pushes;
}

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

  // PRINT input to check
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
  // /*
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
  // */

  auto t3 {std::chrono::high_resolution_clock::now()};

  // write linear programming solver
  // we have an equation of the form Ax=b
  // A is the adjacency matrix with the i-th column being the lights that the i-th button pushes
  // x is the number of button presses: we want to minimize sum(x) - that is, in the L1 norm
  // b is the result state
  // note that for part 2, Ax must directly equal b, while for part 1, Ax must only equal b mod 2

  // also note that it is possible for A to be over-determined or under-determined
  // if A is over-determined, we don't need to do anything special
  // if A is under-determined, we need to massage the matrix into something solvable

  for (std::size_t Case=0; Case<on.size(); Case++) {

    std::vector<int> jolts = joltage[Case];
    std::vector<std::vector<long long>> mat (buttons[Case][0].size(),std::vector<long long>(buttons[Case].size(),0));

    // create mat[row][col]
    for (std::size_t ii=0; ii<buttons[Case].size(); ii++) {
      for (std::size_t jj=0; jj<buttons[Case][ii].size(); jj++) {
        if (buttons[Case][ii][jj]) {
          mat[jj][ii] = 1;
        } 
      }  
    }
    for (std::size_t jj=0; jj<buttons[Case][0].size(); jj++) {
      mat[jj].push_back(jolts[jj]);
    }

    std::vector<std::size_t> free_var{};
    rref(mat,free_var);

    std::cout << "Case " << Case << ":\n";
    int this_pushes = num_pushes(mat,free_var,jolts);
    std::cout << "  Pushes for this case: " << this_pushes << "\n";
    part2 += this_pushes;
  }

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
