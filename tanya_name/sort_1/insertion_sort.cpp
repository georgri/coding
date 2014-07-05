// Сортировка вставками
#include <iostream>
#include <vector>

// Идём с начала массива, для каждого элемента пытаемся найти, куда его вставить
// в уже отсортированную часть.
// Псевдокод: http://www.ee.ryerson.ca/~courses/coe428/sorting/insertionsort.html
void Sort(std::vector<int> &v) {
	int size = (int)v.size();
	for (int i = 0; i < size; i++) {
		int key = v[i];
		int j = i - 1;
		while (j >= 0 && v[j] > key) {
			v[j + 1] = v[j];
			--j;
		}
		v[j + 1] = key;
	}
}

int main()
{
	// comment this line for reading from stdin
	freopen("numbers.txt", "r", stdin);
	
	std::vector<int> v;
	
	// read numbers until end-of-file
	while( !std::cin.eof() ) {
		int i = 0;
		std::cin >> i;
		v.push_back(i);
	}
	
	Sort(v);

	// Контрольная отладка
	// int n = (int)v.size();
	// for (int i = 1; i < n; i++) {
		// if (v[i - 1] > v[i]) {
			// std::cout << v[i - 1] << " " << v[i] << std::endl;
		// }
	// }
	
	// print an array
	for(std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;
}