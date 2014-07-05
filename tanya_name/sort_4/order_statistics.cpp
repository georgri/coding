// Найти элемент с заданным порядком в неотсортированном массиве
#include <iostream>
#include <vector>
#include <stdlib.h> // rand()

// Partition куска массива с from по to (включая) с выбором случайного опорного элемента
// Возвращаем финальный индекс опорного элемента
int RandPart(std::vector<int> &v, int from, int to) {
	// Если маленький кусочек - не надо ничего разбивать
	if (to - from <= 0) {
		return from;
	}
	
	// Случайно выбираем опорный элемент
	// Качество сгенерированного таким образом случайного числа - достаточное для наших целей
	int p = from + (rand() % (to - from + 1));
	// Меняем опорный элемент с первым
	int temp = v[p];
	v[p] = v[from];
	v[from] = temp;
	
	// По варианту требуется, чтобы итераторы бегали с КОНЦА массива
	// i - элемент перед началом группы > опорного, j - элемент перед началом <= опорного
	int i = to;
	for (int j = to; j > from; --j) {
		if (v[j] > v[from]) {
			// меняем местами v[i], v[j]
			int swap = v[i];
			v[i] =  v[j];
			v[j] = swap;
			// сдвигаем i
			--i;
		}
	}
	
	// меняем v[i] и последний наш опорный элемент
	temp = v[from];
	v[from] = v[i];
	v[i] = temp;
	
	// Возвращаем финальное положение опорного элемента
	return i;
}

int GetByOrder(std::vector<int> &v, int order) {
	// Разбиваем массив вокруг опорного элемента
	int from = 0;
	int to = (int)v.size() - 1;
	
	do {
		int k = RandPart(v, from, to);
		// Если разбили по k = order => возвращаем
		if (k == order) {
			return v[k];
		} else if (k > order) {
			// Сдвигаем правую грань на позицию до опорного элемента
			to = k - 1;
		} else {
			// Сдвигаем левую грань на позицию сразу после опорного элемента
			from = k + 1;
		}
	} while (from < to);
	
	return v[from];
}

int main()
{
	// Закомментировать строку для чтения с stdin
	freopen("test.txt", "r", stdin);
	
	// По условию до миллиарда - хватит знакового целого
	std::vector<int> v;
	
	int n = 0;
	std::cin >> n;
	int order = 0;
	std::cin >> order;
	while (!std::cin.eof() && n > 0) {
		--n;
		int i = 0;
		std::cin >> i;
		v.push_back(i);
	}
	
	// Для теста - вывести все введённые данные.
	/*
	std::cout << n << " " << order << std::endl;
	for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;
	*/
	
	// Посчитать и вывести элемент с нужной порядковой статистикой
	std::cout << GetByOrder(v, order);
	
}