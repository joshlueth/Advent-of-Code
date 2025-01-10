#pragma once
#include <vector>
#include <string>
#include <map>
#include <tuple>
#include <utility>
#include <set>

std::vector<std::string> topological_sort (std::set<std::pair<std::string,std::string>> , std::set<std::string> );
void logic_gates (std::map<std::string,std::tuple<std::string,std::string,std::string>> , std::vector<std::string> , std::map<std::string,int>& );
int single_gate(int p1, int p2, std::string );