// Бинарный QuickSort
// Алгоритм: http://www.cs.princeton.edu/courses/archive/spr02/cs226/lectures/radix.4up.pdf
// Принцип обычного quicksort, только мы разделяем массив каждый раз n-му биту элементов

#include <iostream>
#include <vector>
#include <stdint.h>

// Биты считаются от 0 до 63 начиная со старшего
int inline GetBit(int64_t i, int digit) {
	return (i >> (64 - (digit + 1))) & 1;
}

// Сортирует массив от элемента 'from' до 'to' по 'bit' биту
void Partition(std::vector<int64_t> &v, int from, int to, int bit) {
	// Тривиальный случай - один элемент в массиве или bit >= 64
	if (bit >= 64 || to <= from) return;
	
	int i = from; // Итератор конца нулей
	for (int j = from; j <= to; j ++) {
		if (GetBit(v[j], bit) == 0) {
			// Меняем v[i] и v[j], прибавляем i
			int64_t temp = v[j];
			v[j] = v[i];
			v[i] = temp;
			++i;
		}
	}
	
	// Рекурсивно сортируем часть массива с 0 и с 1
	Partition(v, from, i - 1, bit + 1);
	Partition(v, i, to, bit + 1);
}

void Sort(std::vector<int64_t> &v) {
	Partition(v, 0, (int)v.size() - 1, 0);
}


int main()
{
	// Закомментировать строку для чтения с stdin
	freopen("numbers.txt", "r", stdin);
	
	// 64 битные числа
	std::vector<int64_t> v;
	
	// количество - до миллиона
	int n = 0;
	std::cin >> n;
	while (!std::cin.eof() && n > 0) {
		--n;
		int64_t i = 0;
		std::cin >> i;
		v.push_back(i);
	}
	
	// Для теста - вывести все введённые данные.
	/*
	for (std::vector<int64_t>::iterator it = v.begin(); it != v.end(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;
	*/
	
	Sort(v);
	
	// Контрольная отладка
	// for (int i = 1; i < n; i++) {
		// if (v[i - 1] > v[i]) {
			// std::cout << v[i - 1] << " " << v[i] << std::endl;
		// }
	// }
	
	// Вывести отсортированные данные
	for (std::vector<int64_t>::iterator it = v.begin(); it != v.end(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;
}