// Сортировка выбором
#include <iostream>
#include <vector>

// Многократно находим в неотсортированной части массива минимальный элемент,
// подставляем его в конец отсортированной части.
void Sort(std::vector<int> &v) {
	int n = (int)v.size();
	
	// [0 .. i - 1] - отсортированная часть массива.
	for (int i = 0; i < n - 1; i++) {
		int min = i;
		for (int j = i + 1; j < n; j++) {
			if (v[j] < v[min]) {
				min = j;
			}
		}
		// Меняем местами найденный минимальный элемент и v[i]
		int temp = v[i];
		v[i] = v[min];
		v[min] = temp;
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