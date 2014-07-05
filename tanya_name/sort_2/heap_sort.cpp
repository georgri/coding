// Пирамидальная сортировка
#include <iostream>
#include <vector>

inline int left(int i) { return (i * 2) + 1; }
inline int right(int i) { return (i * 2) + 2; }
inline int parent(int i) { return (i - 1) / 2; }

// Первый проход - строим кучу (клёвая структура данных, кстати)
// Второй проход - переносим n раз максимум кучи в конец массива,
//   уменьшая размер кучи на 1, восстанавливаем свойство оставшейся кучи
// Алгоритм: http://ru.wikipedia.org/wiki/%D0%9F%D0%B8%D1%80%D0%B0%D0%BC%D0%B8%D0%B4%D0%B0%D0%BB%D1%8C%D0%BD%D0%B0%D1%8F_%D1%81%D0%BE%D1%80%D1%82%D0%B8%D1%80%D0%BE%D0%B2%D0%BA%D0%B0#.D0.90.D0.BB.D0.B3.D0.BE.D1.80.D0.B8.D1.82.D0.BC
template<typename T> 
inline void RestoreHeap(std::vector<T> &v, int p, int n) {
	// Восстанавливаем свойство кучи - родительские элементы должны быть больше дочерних
	while ((left(p) < n && v[p] < v[left(p)]) || // Левый элемент больше родительского 
				(right(p) < n && v[p] < v[right(p)])) { // Правый элемент в куче и больше род.
		
		// Мы попали в этот цикл => один из дочерних элементов гарантированно больше родительского
		int max = left(p);
		if (right(p) < n && v[right(p)] > v[max]) {
			max = right(p);
		}
		// Максимальный элемент перемещаем в родительский
		T temp = v[p];
		v[p] = v[max];
		v[max] = temp;
		// В следующей итерации цикла восстанавливаем порядок для новой вершины
		p = max;
	}
}

template<typename T> void Sort(std::vector<T> &v) {
	int n = (int)v.size();
	
	// Идём по дочерним вершинам с конца массива, переносим максимальное 
	// значение в родительскую вершину
	for (int i = n / 2; i >= 0; i--) {
		// Восстановить свойство i-й вершины
		RestoreHeap(v, i, n);
		
		// Отладка - проверка отношений родителя и детей
		// if (i != parent(left(i)) || i != parent(right(i))
				// || (i != right(parent(i)) && i != left(parent(i)) ) ) {
			// std::cout << i << " " << left(i) << " " << right(i) << " " << parent(left(i)) << " " << parent(right(i)) << std::endl;
		// }
	}
	
	// Отладка - проверка свойства кучи
	// for (int i = 0; i < n/2; i++) {
		// if (v[i] < v[left(i)] || v[i] < v[right(i)]) {
			// std::cout << v[i] << " " << v[left(i)] << " " << v[right(i)] << std::endl;
		// }
	// }
	
	// n раз Переносим вершину кучи (максимум) в конец массива, восстанавливаем свойство кучи
	for (int i = n - 1; i >= 1; i--) {
		// i - это новый размер кучи
		// Максимум (то есть, первый элемент кучи) меняем с последним элементом кучи
		T temp = v[0];
		v[0] = v[i];
		v[i] = temp;
		
		// Восстанавливаем свойство в максимуме
		RestoreHeap(v, 0, i);
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
/*