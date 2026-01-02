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

void rref(std::vector<std::vector<long long>>& mat, std::vector<std::size_t>& free) {
  std::size_t col=0, row=0;
  while(row<mat.size()) {
    while (col<mat[0].size()-1) {

      // check if we need to pivot
      if (mat[row][col]==0) {
        std::size_t irow=row+1;
        while (irow<mat.size()) {
          if (mat[irow][col]!=0) {
            // found pivot, swap row indices
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

int num_pushes(std::vector<std::vector<long long>>& mat, std::vector<std::size_t>& free, std::vector<int>& upper, int maxj) {

  int total_pushes=maxj*(static_cast<int>(mat[0].size())-1);

  // initial free variable guesses
  std::vector<long long> guesses(mat[0].size(),0);
  // initially upper and lower values for each guess
  std::vector<int> lower(free.size(),0), current(free.size(),0);

  // get bounds for guesses
  // when getting bounds for guesses, we have to keep in mind that if a reduced matrix row looks like
  // 1a -1b 1c 2
  // where indices 1 (-1) and 2 (1) are the free variables, that the inequality is c<=2+b!
  // so practically, first we should get the inequalities by taking the free variables out to get a matrix with rows corresponding to
  // fv1, fv2, ..., fvEnd, joltage
  // then we can remove all rows with no relevant fv information

  std::vector<std::vector<long long>> inequalities {};
  std::vector<std::size_t> becomes_single_inequality {};
  std::vector<double> free_sum(free.size());
  for (std::size_t rows=0; rows<mat.size(); rows++) {
    std::vector<long long> inequality_row(free.size()+1); // temporarily store row. Don't store permanently if number of non-zero values are 0 (meaningless) or 1 (single variable inequality, update bounds)
    int nonzero_counter=-1;
    std::size_t last_nonzero_loc=0, pivot_ind=0;
    for (std::size_t cols=0; cols<mat[rows].size()-1; cols++) {
      long long local = mat[rows][cols];
      if (local!=0) {
        if (nonzero_counter==-1) {
          pivot_ind = cols;
          nonzero_counter++;
        } else {
          for (std::size_t fv=0; fv<free.size(); fv++) {
            if (free[fv]==cols) {
              inequality_row[fv] = local;
              nonzero_counter++;
              free_sum[fv] += static_cast<double>(local)/static_cast<double>(mat[rows][pivot_ind]);
              last_nonzero_loc = fv;
            }
          }
        }
      }
    }
    inequality_row.back() = mat[rows].back();
    if (nonzero_counter==0) {
      continue; // meaningless, skip
    } else if (nonzero_counter==1) {
      // single variable inequality
      // pos+pos -> upper bound
      if (inequality_row[last_nonzero_loc]>0) {
        if (inequality_row.back()>=0) {
          upper[last_nonzero_loc] = std::min(upper[last_nonzero_loc],static_cast<int>(inequality_row.back()/inequality_row[last_nonzero_loc]));
        }
      }
      // neg+neg -> lower bound
      else {
        if (inequality_row.back()<=0) {
          lower[last_nonzero_loc] = std::max(lower[last_nonzero_loc],static_cast<int>(std::ceil(static_cast<double>(inequality_row.back())/static_cast<double>(inequality_row[last_nonzero_loc]))));
        }
      }
      // pos+neg||neg+pos should be impossible for a solution to actually exist
    } else { // multiple variable inequality; use recursion to make guesses that satisfy
      inequalities.push_back(inequality_row);
      becomes_single_inequality.push_back(last_nonzero_loc);
    }
  }
  // should use recursion to make free variable guesses
  // last free variable won't be a guess - just a simple button minimization
  // e.g. sum up the free variable column and see how many extra button pushes it causes other variables to have (using free_sum) - to determine if we should minimize or maximize the value of the last free variable
  // note that we can't just use free sum here because it is not necessarily true that we are fully in rref form for integers (e.g. 2 1 5, where 1 is a free variable)
  // note: also look at variable *equalities* solely in the free variables. Is this a concern? (yes - mainly for determining using free_sum?) could potentially use to determine co-prime values
  // theoretically, using all inequalities should mean that the equation solver will never return a negative number - check this!

  // if free_sum is 1, then any choice is valid
  // if free_sum is less than 1, then we want to choose the minimum valid value of the last free variable
  // if free_sum is greater than 1, then we want to choose the maximum valid value of the last free variable

  // instead of using recursion, could verify that each guess would satisfy inequalities?
  // for the last guess, would simply calculation if the minimum or maximum valid solution should be taken...

  // if (free.size()>0) {
  //   std::cout << "free var bounds " << free.size() << "\n";
  //   for (std::size_t fv=0; fv<free.size(); fv++) {
  //     std::cout << fv << ": " << lower[fv] << " " << upper[fv] << "\n";
  //   } 
  // }

  std::cout << "matrix\n";
  for (auto r : mat) {
    for (auto c : r) {
      std::cout << c << " ";
    }
    std::cout << "\n";
  }
  // std::cout << "Number of free variables: " << free.size() << "\n";
  // std::cout << "free variables location:\n";
  // for (auto fv : free) {
  //   std::cout << fv << " ";
  // }
  // std::cout << "\n";
  // std::cout << "free sum:\n";
  // for (auto fs : free_sum) {
  //   std::cout << fs << " ";
  // }
  // std::cout << "\n";
  // std::cout << "lower: ";
  // for (auto l : lower) {
  //   std::cout << l << " ";
  // }
  // std::cout << "\n";
  // std::cout << "upper: ";
  // for (auto u : upper) {
  //   std::cout << u << " ";
  // }
  // std::cout << "\n";
  // std::cout << "inequalities:\n";
  // for (auto in : inequalities) {
  //   for (auto i : in) {
  //     std::cout << i << " ";
  //   }
  //   std::cout << "\n";
  // }
  
  
  // loop over all possible guesses, find minimum number of pushes
  std::size_t adjust = 0; // set to 1 if eliminating free variable, otherwise set to 0
  std::vector<int> count(free.size(),0);
  while (true) {

    bool done_guessing = false;
    if (count.size()>0) { // otherwise, simply select the correct result - no free variables
      // create current using count, lower, and upper
      bool valid_guess = false;
      while (!valid_guess) {
        valid_guess = true;
        // start with copy of current inequalities
        std::vector<std::vector<long long>> local_inequalities {inequalities};
        int low_ineq, up_ineq, guess;
        for (std::size_t cur=0; cur<current.size(); cur++) {
          // set local inequalities
          low_ineq = lower[cur];
          up_ineq = upper[cur];
          // std::cout << "local ineq: " << low_ineq << " " << up_ineq << "\n";
          // update local inequalities
          if (cur!=0) { // skip update for first: already done previously
            std::size_t set_i = cur-1; // update local_inequalities with previous guess
            for (std::size_t i_num=0; i_num<local_inequalities.size(); i_num++) {
              local_inequalities[i_num].back() -= local_inequalities[i_num][set_i]*current[set_i]; 
              if (becomes_single_inequality[i_num]==cur) {
                // update inequality values!
                if (local_inequalities[i_num][cur]>0) {
                  if (local_inequalities[i_num].back()>=0) {
                    up_ineq = std::min(up_ineq,static_cast<int>(local_inequalities[i_num].back()/local_inequalities[i_num][cur]));
                  }
                } else {
                  if (local_inequalities[i_num].back()<=0) {
                    low_ineq = std::max(low_ineq,static_cast<int>(std::ceil(static_cast<double>(local_inequalities[i_num].back())/static_cast<double>(local_inequalities[i_num][cur]))));
                  }
                }
              }
            }
            // no valid guesses (only should occur for cur>0)
            if (low_ineq>up_ineq) {
              valid_guess = false;
              count[cur-1] += 1;
              for (std::size_t cur_after=cur; cur_after<current.size(); cur_after++) {
                count[cur_after] = 0;
              }
              break;
            }
          }
          // dimension reduction
          // must update in order to deal with, e.g., 2 -3 -5
          if (cur==current.size()-adjust) {
            if (free_sum.back()<=1) {
              current.back() = low_ineq;
            } else {
              current.back() = up_ineq;
            }
          } else { // otherwise make a guess
            guess = low_ineq + count[cur];
            if (guess>up_ineq) { // guess is too high, loop over
              // loop over the innermost one first, then set it to zero and increment the next innermost one
              if (cur==0) {
                done_guessing = true;
                break; // end loop when low_ineq + count[cur] is greater than up_ineq for cur=0
              }
              count[cur-1] += 1;
              for (std::size_t cur_after=cur; cur_after<current.size(); cur_after++) {
                count[cur_after] = 0;
              }
              valid_guess = false;
              break;
            } else { // guess is not too high
              current[cur] = guess;
            }
          }
        }
        if (done_guessing) break;
      }
      if (done_guessing) break; // invalid guess, have iterated through everything
      if (current.size()>adjust) {
        count[count.size()-1-adjust] += 1; // update current next loop by increasing count
      }
    }
    if (count.size()>adjust&&done_guessing) break;

    // assign guesses to current
    // std::cout << "guesses: ";
    for (std::size_t ind=0; ind<free.size(); ind++) {
      guesses[free[ind]] = current[ind];
      // std::cout << current[ind] << " ";
    }
    // std::cout << "\n";

    // get number of pushes
    int this_pushes=0;
    this_pushes = backward_sub(mat,guesses);

    // the issue appears to be that we cannot simply choose based off of free_sum when, for example, 8 7 83 (bounds 0-11). Need a further criteria
    // this is essentially the "equality" rule

    // 740 vs 119
    // 477 vs 72
    // 1118 vs 106
    // 1144 vs 134
    // 946 vs 103
    // 1166 vs 125
    // 930 vs 104
    
    if (this_pushes>=0) {
      total_pushes = std::min(total_pushes,this_pushes);
    }

    // if we only have one solution, we have solved it!
    if (current.size()<=adjust) break;
  }

  // std::cout << "total pushes: " << total_pushes << "\n";
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
    rref(mat,free_var);

    // std::cout << "Case " << Case << ": ";

    // set upper bound on free variables
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