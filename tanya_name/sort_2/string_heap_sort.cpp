// Пирамидальная сортировка строк
#include <iostream>
#include <vector>
#include <string>

// Модульность - это круто!
#include "heap_sort.h"

int main()
{
	// comment this line for reading from stdin
	freopen("strings.txt", "r", stdin);
	
	std::vector<std::string> v;
	
	// Считать количество строк ввода
	int n = 0;
	std::cin >> n;
	// read strings until end-of-file
	while(!std::cin.eof() && n > 0) {
		--n;
		std::string s;
		std::cin >> s;
		v.push_back(s);
	}
	
	Sort(v);
	
	// print an array
	for(std::vector<std::string>::iterator it = v.begin(); it != v.end(); ++it) {
		std::cout << *it << std::endl;
	}
}