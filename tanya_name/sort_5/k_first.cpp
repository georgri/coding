// Найти k минимальных чисел из очень длинной последовательности
#include <iostream>
#include <vector>

#include "merge_sort.h"

// По условию, все n чисел могут не помещаться в память, мы будем читать только 
// k из них за раз, и дополнительно хранить только k минимальных. (Это k + k ячеек памяти)
// После чтения очередных k элементов будем сортировать их сортировкой слияниями. 
// Требуемое место для сортировки - O(k). Время каждой такой сортировки - O(k lgk).
// Потом сольём орсортированный массив с уже отсортированным массивом из k минимальных
// элементов. При слиянии будет использовано k дополнительных ячеек памяти.
// Слияние занимает линейное время от суммы размеров сливаемых списков - O(k + k) = O(k).
//
// Итого, мы используем максимум 2k + O(k) + k = O(k) ячеек оперативной памяти.
// 
// Сортировка каждых k элементов занимает O(k lgk) времени.
// Сортировка n/k групп по k элементов займёт (n/k) * O(k lgk) = O(n/k * k lgk) = O(n lgk)
// Остальные затраты алгоритма (чтение, слияние) линейны по времени.
//
// В итоге, данный алгоритм удовлетворяет требованиям задания по использованию
// оперативной памяти и процессорного времени. (И использует слияния)


// Слить два отсортированных списка в один список из k минимальных элементов.
// Вернуть результат в первом параметре функции (для удобства).
void Merge(std::vector<int> &a, std::vector<int> &b, int k) {
	int n = (int)a.size();
	int m = (int)b.size();
	
	// Тривиальный случай - один из списков пустой (так, например, в начале алгоритма)
	if (n == 0 || m == 0) {
		if (n == 0) {
			a.swap(b); // Меняет содержимое 2 векторов за константное время
		}
		return;
	}
	
	std::vector<int> r; // для хранения результата из k минимальных элементов
	r.reserve(k); // Сразу говорим, что ожидается не меньше k элементов.
	
	int i = 0; // Итератор для a
	int j = 0; // Итератор для b
	while(i < n && j < m && i + j < k) {
		if (a[i] < b[j]) {
			r.push_back(a[i]);
			++i;
		} else {
			r.push_back(b[j]);
			++j;
		}
	}
	// Дозаполняем результат из оставшегося списка
	for (; i + j < k && i < n; ++i) {
		r.push_back(a[i]);
	}
	for (; i + j < k && j < m; ++j) {
		r.push_back(b[j]);
	}
	
	// Возвращаем результат в первом параметре функции
	a.swap(r); // Меняет содержимое 2 векторов за константное время http://www.cplusplus.com/reference/vector/vector/swap/
}


// Считать максимум k чисел из оставшихся n из входного потока данных stream
void ReadK(std::vector<int> &v, int n, int k, std::istream& stream) {
	v.clear(); // Очищаем куда будем читать
	
	// Нечего читать - выходим
	if (n <= 0 || stream.eof()) return;
	
	v.reserve(k); // Если входные данные будут достаточно длинными, то в подавляющем
				// большинстве случаев мы считаем не меньше k элементов.

	// Должны считать минимум из (k, n) чисел
	int min = (k < n) ? k : n;
	while (!stream.eof() && min > 0) {
		--min;
		int i = 0;
		stream >> i;
		v.push_back(i);
	}
}


int main()
{
	// Закомментировать строку для чтения с stdin
	freopen("test.txt", "r", stdin);
	
	
	int n = 0;
	std::cin >> n;
	int k = 0;
	std::cin >> k;

	std::vector<int> v;
	std::vector<int> buf;
	// Читаем по k элементов, сортируем, сливаем с существующими k минимальными
	while (!std::cin.eof() && n > 0) {
		ReadK(buf, n, k, std::cin);
		n -= (int)buf.size();
		
		Sort(buf);
		Merge(v, buf, k);
	}
	
	// Вывести первые k чисел в отсортированном порядке
	for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
		std::cout << *it << " ";
	}
}