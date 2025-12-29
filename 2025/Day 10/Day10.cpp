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
// #include <sys>


// 0; 1; 2; 3; 4; 10; 20; 30; 40; 21; 31; 41; 32; 42; 43;
// 210; 310; 410; 320; 420; 430; 321; 421; 431; 432; 3210; 4210; 4310; 4320; 4321; 43210.
// so clearly the strategy is to increment the outer-most variable
// if that goes too high, increment the next-inner-most variable, set the outer-most variable to the value of the interior+1 (enforcing the sequence to be monotonically decreasing)
// if interior+1 is the maximum value, we have to look at interior+2
// if all interior values plus the offset are the maximum value, in reverse order add 0, then set the inner-most to 1, 2, 3, ... moving outwards
void increment_buttons(std::vector<std::size_t>& buttons, std::size_t max_b) {
  if (buttons.size()==0) {
    buttons.push_back(0);
    return;
  }
  buttons[0] += 1;
  if (buttons[0]==max_b) { // must find innermost variable that we can increment
    std::size_t increment=buttons.size();
    for (std::size_t jj=0; jj<buttons.size(); jj++) {
      if (buttons[jj]+jj+1<max_b) {
        increment = jj;
        buttons[increment] += 1;
        break;
      }
    }
    if (increment==buttons.size()) { // no options to increment was found
      buttons.push_back(0); // instead add zero to the outermost
      // IF the number of buttons to press is greater than the number of buttons in existence, then there is no solution...
      if (buttons.size()>max_b) {
        std::cerr << "no solution found...\n";
        std::exit(0);
      }
    }
    for (std::size_t jj=increment, ii=buttons[increment]; ; jj--,ii++) {
      buttons[jj] = ii;
      if (jj==0) break;
    }
  }
  return;
}

bool check_pushing_buttons(const std::vector<std::size_t>& buttons, const std::vector<std::vector<bool>>& presses, const std::vector<bool>& goal) {
  std::vector<bool> current(goal.size(),false);
  // try pushing the buttons in these_buttons, this_button at a time
  for (std::size_t this_button=0; this_button<buttons.size(); this_button++) {
    // element-wise use xor to push buttons
    for (std::size_t b=0; b<goal.size(); b++) {
      current[b] = current[b] ^ presses[buttons[this_button]][b];
    }
  }

  return (current==goal);
}

void rref(std::vector<std::vector<long long>>& mat, std::vector<std::size_t>& free, std::vector<std::size_t>& pivot) {
  std::size_t col=0, row=0;
  while(row<mat.size()) {
    while (col<mat[0].size()-1) {

      // check if we need to pivot
      if (mat[row][col]==0) {
        std::size_t irow=row+1;
        while (irow<mat.size()) {
          if (mat[irow][col]!=0) {
            // found pivot, swap pivot indices
            std::size_t swap_pivot = pivot[row];
            pivot[row] = pivot[irow];
            pivot[irow] = swap_pivot;
            // swap row indices
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
          // guarantee we can do integer math when reducing
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
        }
      }

      // further eliminate rows above
      for (std::size_t up=0; up<row; up++) {
        long long prev = std::abs(mat[up][col]);
        if (prev!=0) {
          // guarantee we can do integer math when reducing
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

int backward_sub(const std::vector<std::vector<long long>>& mat, std::vector<long long>& csol) {
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
        csol[first_loc] = mat[row][mat[row].size()-1];
        first = true;
      } else {
        csol[first_loc] -= mat[row][col]*csol[col];
      }
    }
    if (std::lcm(csol[first_loc],first_val)!=csol[first_loc]) return -1; // if this is the case, then the given solution is not an integer solution!
    csol[first_loc] /= first_val;
    if (csol[first_loc]<0) { // if any values are less than zero, return false. Only solutions are positive solutions
      return -1;
    }
  }
  long long presses = 0;
  for (long long cs : csol) {
    presses += cs;
  }
  return static_cast<int>(presses);
}


// optimizations:
// use inequalities to further constrain free variables
// reduce dimensionality of search space!

int num_pushes(std::vector<std::vector<long long>>& mat, std::vector<std::size_t>& free, std::vector<int>& upper, int maxj) {

  int total_pushes=maxj*(static_cast<int>(mat[0].size())-1);

  // initial free variable guesses
  std::vector<long long> guesses(mat[0].size(),0);
  // initially upper and lower values for each guess
  std::vector<int> lower(free.size(),0), current(free.size(),0);

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
    // otherwise, this represents the complete iteration over all possible solutions
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

  std::vector<std::size_t> t_buttons{};

  // now that we have verified that the problem is appropriately parsed
  for (std::size_t Case=0; Case<on.size(); Case++) {
    std::vector<bool> goal = on[Case];
    std::vector<std::vector<bool>> presses = buttons[Case]; // [this button][affects these lights]


    // FF:F, TF:T, TT:F, FT:T - use bitwise xor (^)
    bool found = false;
    std::vector<std::size_t> these_buttons {};
    while (!found) {
      
      // increment_buttons should only give combinations
      increment_buttons(these_buttons,presses.size());

      found = check_pushing_buttons(these_buttons, presses, goal);
      if (found) {
        part1 += static_cast<int>(these_buttons.size());

        // output to make sure that it works...
        // std::cout << "case num: " << Case << " size: " << these_buttons.size() << " buttons:";
        // for (auto it : these_buttons) {
        //   std::cout << " " << it;
        // }
        // std::cout << "\n";

        break; // break for loop; found solution
      }
    }
  }

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
          mat[jj][ii] = 1; // so mat is [lights][affected by button]
        } 
      }  
    }
    for (std::size_t jj=0; jj<buttons[Case][0].size(); jj++) {
      mat[jj].push_back(jolts[jj]);
    }

    std::vector<std::size_t> free_var{};
    std::vector<std::size_t> pivot(mat.size(),0);
    for (std::size_t ii=0; ii<pivot.size(); ii++) {
      pivot[ii] = ii;
    }
    rref(mat,free_var,pivot);

    // std::cout << "Case " << Case << ": ";

    // use pivot to set upper bound on free variables
    int maxj = *std::max_element(jolts.begin(),jolts.end()); // max joltage overall
    std::vector<int> upper(free_var.size(),maxj);
    for (std::size_t fv=0; fv<free_var.size(); fv++) { // index denoting free variable
      for (std::size_t loop=0; loop<buttons[Case][0].size(); loop++) { // go down matrix column
        if (buttons[Case][free_var[fv]][loop]==1) { // if button triggers light
          upper[fv] = std::min(upper[fv],jolts[loop]); // then maximum value of free variable button is the minimum value of all joltages
        }
      }
    }

    int this_pushes = num_pushes(mat,free_var,upper,maxj);
    part2 += this_pushes;

    // std::cout << this_pushes << " pushes\n";
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