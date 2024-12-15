#pragma once
#include <vector>
#include <utility>

void getPosition(std::pair<std::size_t,std::size_t>& , std::vector<std::vector<char>>& );
void moveRobot(std::pair<std::size_t,std::size_t>& , const char , std::vector<std::vector<char>>& );
void adjustPosition(std::pair<std::size_t,std::size_t>& , const char );
void reversePosition(std::pair<std::size_t,std::size_t>& , const char );
void moveBoxes(std::pair<std::size_t,std::size_t>& , const char , std::vector<std::vector<char>>& );
void moveBoxes2(std::pair<std::size_t,std::size_t>& , const char , std::vector<std::vector<char>>& );
std::size_t trackScore(const std::vector<std::vector<char>>& );

