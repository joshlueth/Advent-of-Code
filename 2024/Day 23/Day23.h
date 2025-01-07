#pragma once
#include <string>
#include <set>
#include <map>

int computerHash(std::string );
bool startsWith_T(int );
std::string reverseComputerHash(int comp);
std::set<int> BronKerbosch (std::set<int>& , std::set<int>& , std::set<int>& , std::multimap<int,int>& );

