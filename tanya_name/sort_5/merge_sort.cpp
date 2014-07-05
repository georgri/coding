// Сортировка слиянием
#include <iostream>
#include <vector>

// Рекурсивно сортируем две половины массива; сливаем две половины в одну.
// Псевдокод: https://www.princeton.edu/~achaney/tmve/wiki100k/docs/Merge_sort.html
template<typename T> void Sort(std::vector<T> &v) {
	int n = (int)v.size();
	
	// Тривиальный случай - один элемент в массиве - он уже отсортирован
	if (n <= 1) {
		return;
	}
	
	// Формируем два массива - левый и правый
	int mid = n / 2; // Делим массив примерно поровну
	std::vector<T> left(v.begin(), v.begin() + mid);
	std::vector<T> right(v.begin() + mid, v.end());
	// Сортируем оба
	Sort<T>(left);
	Sort<T>(right);
	
	// Сливаем в один (merge), идя с конца.
	n--; // Устанавливаем на последний элемент
	int i = (int)left.size() - 1;
	int j = (int)right.size() - 1;
	while (i >= 0 && j >= 0) {
		// Сравниваем конечные элементы массивов, берём максимальный, добавляем в конец
		if (left[i] > right[j]) {
			v[n] = left[i];
			i--;
		} else {
			v[n] = right[j];
			j--;
		}
		n--;
	}
	
	// Один из массивов уже пуст - отработает лишь один из циклов
	for (; i >= 0; i--) {
		v[i] = left[i];
	}
	for (; j >= 0; j--) {
		v[j] = right[j];
	}
}


// Раскомментировать для теста сортировки
/*
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
	
	Sort<int>(v);
	
	// print an array
	for(std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
		std::cout << *it << std::endl;
	}
}
*/