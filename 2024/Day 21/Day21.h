#pragma once
#include <string>
#include <utility>
#include <map>
#include <tuple>

long long buttonPresses(std::string , int , std::map<std::tuple<std::string,char,int>,long long>& );
long long numKP(char , char , int , std::map<std::tuple<std::string,char,int>,long long>& );
long long numdirKP(std::pair<int,int> , std::pair<int,int> , int , std::map<std::tuple<std::string,char,int>,long long>& );
long long robotKP(std::string , char , int , std::map<std::tuple<std::string,char,int>,long long>& );
long long myKP(std::string );
std::pair<int,int> buttonKPcoords(char );
char coordsKPbutton(std::pair<int,int> );
bool isbuttonKPavoid (std::pair<int,int> );
std::pair<int,int> directionalKPcoords(char );
char coordsKPdirectional(std::pair<int,int> );
bool isdirectionalKPavoid (std::pair<int,int> );
int factorial(int );
char movement2key(int , char );
int L1_distance(std::pair<int,int> , std::pair<int,int> );
bool getNumPath(std::string& , std::pair<int,int> , std::pair<int,int> , int , int );
bool getDirPath(std::string& , std::pair<int,int> , std::pair<int,int> , int , int );
