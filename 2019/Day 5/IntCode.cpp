#include <vector>
#include <iostream>
#include <format>
#include <string>

// instruction structure
struct Instruction {
  int op_code;
  int p1, p2, p3;
};

// parse instruction into opcode and parameter modes
struct Instruction ParseInstruction(int x) {
  std::string str (std::format("{:05}",x));
  return Instruction {.op_code=std::stoi(str.substr(3,2)),.p1=str[2]-'0',.p2=str[1]-'0',.p3=str[0]-'0'};
}

std::size_t CalculateIncrement(int x) {
  switch (x) {
    case 1:
      return 4;
    case 2:
      return 4;
    case 3:
      return 2;
    case 4:
      return 2;
    case 5:
      return 3;
    case 6:
      return 3;
    case 7:
      return 4;
    case 8:
      return 4;
    case 99:
      return 0;
    case 100:
      return 0;
    default:
      std::cerr << "Instruction does not have a valid increment\n";
      std::exit(1);
  }
}

std::size_t CalculatePosition(int mode, std::size_t pos, std::vector<int>& Code) {
  switch (mode) {
    case 0:
      return static_cast<std::size_t>(Code[pos]);
    case 1:
      return pos;
    default:
      std::cerr << "Position does not have a valid code\n";
      std::exit(1);
  }
}

// passes in vector of variables used for IntCode computer, appropriately modifies vector
std::vector<int> IntCode(std::vector<int>& Code,int input) {
  bool stop {false};
  std::size_t increment {};
  std::vector<int> output {};
  // undergo IntCode operations
  for (std::size_t pos {0}; pos<Code.size(); pos+=increment) {
    if (stop) break;
    // get and parse current instruction
    int curIns {Code[pos]};
    Instruction ins1 = ParseInstruction(curIns);

    switch (ins1.op_code) {
      case 1:
        Code[CalculatePosition(ins1.p3,pos+3,Code)] = Code[CalculatePosition(ins1.p1,pos+1,Code)] + Code[CalculatePosition(ins1.p2,pos+2,Code)];
        break;
      case 2:
        Code[CalculatePosition(ins1.p3,pos+3,Code)] = Code[CalculatePosition(ins1.p1,pos+1,Code)] * Code[CalculatePosition(ins1.p2,pos+2,Code)];
        break;
      case 3:
        Code[CalculatePosition(ins1.p1,pos+1,Code)] = input;
        break;
      case 4:
        output.push_back(Code[CalculatePosition(ins1.p1,pos+1,Code)]);
        break;
      case 5:
        if (Code[CalculatePosition(ins1.p1,pos+1,Code)]!=0) {
          pos = static_cast<std::size_t>(Code[CalculatePosition(ins1.p2,pos+2,Code)]);
          ins1.op_code = 100;
        }
        break;
      case 6:
        if (Code[CalculatePosition(ins1.p1,pos+1,Code)]==0) {
          pos = static_cast<std::size_t>(Code[CalculatePosition(ins1.p2,pos+2,Code)]);
          ins1.op_code = 100;
        }
        break;
      case 7:
        if (Code[CalculatePosition(ins1.p1,pos+1,Code)]<Code[CalculatePosition(ins1.p2,pos+2,Code)]) {
          Code[CalculatePosition(ins1.p3,pos+3,Code)] = 1;
        } else {
          Code[CalculatePosition(ins1.p3,pos+3,Code)] = 0;
        }
        break;
      case 8:
        if (Code[CalculatePosition(ins1.p1,pos+1,Code)]==Code[CalculatePosition(ins1.p2,pos+2,Code)]) {
          Code[CalculatePosition(ins1.p3,pos+3,Code)] = 1;
        } else {
          Code[CalculatePosition(ins1.p3,pos+3,Code)] = 0;
        }
        break;
      case 99:
        stop = true;
        break;
      default:
        std::cerr << "Not a valid code: " << ins1.op_code << std::endl;
        std::exit(1);
    }

    increment = CalculateIncrement(ins1.op_code);
  }
  return output;
}
