#pragma once
#include <vector>

void rotate_right(int&, int&, char&);
void rotate_left(int&, int&, char&);
void move(int&, int&, char);
bool isOut(int, int);
int checkObstruction(std::vector<std::vector<char>>, int, int, char);
