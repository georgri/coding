// Сортировка пузырьком
#include <iostream>
#include <vector>

// Проходим много раз по массиву, проверяя порядок соседних элементов и меняя их
// местами, если порядок нарушен.
// Псевдокод: http://en.wikipedia.org/wiki/Bubble_sort#Pseudocode_implementation
void Sort(std::vector<int> &v) {
	int n = (int)v.size();
	bool swapped;
	do {
		swapped = false;
		for (int i = 1; i < n; i++) {
			// Если не в том порядке
			if (v[i - 1] > v[i]) {
				// Меняем местами и помечаем, что что-то поменялось
				int temp = v[i];
				v[i] = v[i - 1];
				v[i - 1] = temp;
				swapped = true;
			}
		}
	} while (swapped);
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