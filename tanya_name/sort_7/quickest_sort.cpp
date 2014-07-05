// Быстрейшая сортировка
// По условию, даны числа до миллиарда из псевдорандомной последовательности.
// Если последовательность действительно псевдорандомная, то можно было бы отсортировать
// её за линейное время Bucket Sort-ом.
// Так как в задаче говорится про оптимизацию выбора опорного элемента, то 
// предполагается, что мы будем сортировать с помощью Quick Sort.
//
// Оптимизация ввода - читать кусками с помощью низкоуровневого read
// http://stackoverflow.com/questions/9371238/why-is-reading-lines-from-stdin-much-slower-in-c-than-python	std::sync_with_stdio(false); // Говорит потокам читать/писать быстро
// Оптимизация вывода - пишем кусками с помощью write
// Оптимизация выбора опорного элемента - будем выбирать медианный элемент из первого,
// последнего и среднего элементов сортируемой части массива
// Оптимизация концевой рекурсии - последний рекурсивный вызов преобразуем в цикл.

#include <vector>
#include <iostream>

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>


// Для оптимизированного чтения
int const MAX_BUF = 16 * 1024;
char buf[MAX_BUF];
int buf_it = 0; // Позиция чтения в буфере
int chars_in_buf = 0; // Размер полезной информации в буфере

// Специальная функция для чтения чисел от 0 до 10^9 с stdin - возвращает цифру от 0 до 9,
// -1 если на входе не числовой символ, -2 в случае конца файла
// После чтения всех данных из файла функция гарантирует выполнение
// условия (buf_it == 0 && chars_in_buf == 0)
int GetDigit() {
	if (buf_it >= chars_in_buf) {
		buf_it = 0; // итератор на начало
		// Читать новые символы со входа
		if ((chars_in_buf = read(0, buf, MAX_BUF)) <= 0) {
			return -2; // Считали 0 символов или ошибка - считаем, что конец ввода
		}
	}
	
	char c = buf[buf_it++];
	if (c >= '0' && c <= '9') {
		return c - '0';
	} else return -1;
}

// Специальная фунция для чтения неотрицательного числа до миллиарда.
// Возвращает -1, если конец файла.
int GetNumber() {
	int i;
	// Читаем, пока не будет цифра или конец ввода.
	while((i = GetDigit()) == -1) {;}
	// Конец ввода
	if (i == -2) return -1;
	
	int r = 0;
	do {
		r = r * 10 + i;
	} while((i = GetDigit()) >= 0);
	
	return r;
}


// Писать в stdout всё, что скопилось в буфере, обнулить buf_it
void WriteBuf() {
	if (buf_it <= 0) return;
	
	int it = buf_it;
	buf_it = 0;
	int writen = 0;
	int bytes = 0;
	while (writen < it) {
		bytes = write(1, buf + writen, it - writen);
		if (bytes < 0) return; // Произошла ошибка - выходим
		writen += bytes;
	}
}


// Используем тот же механизм для вывода, что и для ввода
// копим символы в буфере, как только он переполняется - пишем его в stdout
void PutChar(char c) {
	if (buf_it >= MAX_BUF) {
		WriteBuf();
	}
	
	buf[buf_it++] = c;
}

// Писать в stdout каждые step чисел вектора (числа неотрицательные)
void WriteVector(std::vector<int> &v, int step) {
	if (step <= 0) return; // Не поддерживается такой шаг
	
	int n = (int)v.size();
	
	for (int i = step - 1; i < n; i += step) {
		int num = v[i];
		
		// Пишем во временный буфер цифры в обратном порядке
		char temp[10]; // числа до миллиарда - 10 знаков гарантированно хватит
		int idx = 0;
		do {
			temp[idx++] = (char)('0' + (num % 10));
			num /= 10;
		} while (num != 0 && idx < 10);
		
		// Печатаем цифры в восстановленном порядке
		while (idx > 0) {
			PutChar(temp[--idx]);
		}
		PutChar(' ');
	}
	
	// Дозаписать что осталось в буфере
	WriteBuf();
}


// Оптимизация для сортировки малого количества элементов
// http://stackoverflow.com/questions/2786899/fastest-sort-of-fixed-length-6-int-array
// Генератор сравнений http://pages.ripco.net/~jgamble/nw.html

// Сколько элементов сортируем "вручную"
#define MIN_LIST_SIZE 6

#define min(x, y) (x<y?x:y)
#define max(x, y) (x<y?y:x) 
#define SWAP(x,y) { const int a = min(d[x], d[y]); const int b = max(d[x], d[y]); d[x] = a; d[y] = b;}
static inline void sort6(int * d) {
	SWAP(1, 2);
    SWAP(4, 5);
    SWAP(0, 2);
    SWAP(3, 5);
    SWAP(0, 1);
    SWAP(3, 4);
    SWAP(2, 5);
    SWAP(1, 4);
    SWAP(0, 3);
    SWAP(2, 4);
    SWAP(1, 3);
    SWAP(2, 3);
}
static inline void sort5(int * d) {
	SWAP(3, 4);
	SWAP(0, 1);
	SWAP(2, 4);
	SWAP(2, 3);
	SWAP(1, 4);
	SWAP(0, 3);
	SWAP(0, 2)
	SWAP(1, 3);
	SWAP(1, 2);
}
static inline void sort4(int * d) {
	SWAP(0, 1);
	SWAP(2, 3);
	SWAP(0, 2);
	SWAP(1, 3);
	SWAP(1, 2);
}
static inline void sort3(int * d) {
	SWAP(1, 2);
	SWAP(0, 2);
	SWAP(0, 1);
}
static inline void sort2(int * d) {
	SWAP(0, 1);
}
#undef SWAP
#undef max
#undef min

// Быстрая сортировка с оптимизацией выбора опорного элемента (и хвостовой рекурсии)
// http://www.cs.utexas.edu/users/lavender/courses/EE360C/lectures/lecture-22.pdf
// (там на слайдах в коде ошибка в строке "int i = low, j = high;" (должно быть high - 1)

template<class T>
void quickSort (std::vector<T> &v, int low, int high) {
#define swap(x,y) { const int a = v[(x)]; v[(x)] = v[(y)]; v[(y)] = a;}
	
	do {
		if (low >= high) return; // Тривиальный случай - один элемент
		
		if (low + MIN_LIST_SIZE > high) {
			// Оптимизация для сортировки малых кусков массива
			int * d = &v[low];
			switch (high - low + 1) {
				case 2: sort2(d); break;
				case 3: sort3(d); break;
				case 4: sort4(d); break;
				case 5: sort5(d); break;
				case 6: sort6(d); break;
			}
			
			// Отладка
			// for (int i = low + 1; i <= high; ++i) {
				// if (v[i - 1] > v[i]) {
					// std::cout << v[i - 1] << " " << v[i] << " " << high - low + 1 << std::endl;
				// }
			// }
			
			return;
		}
		
		int mid = (low + high) / 2;
		if (v[high] < v[mid]) swap (mid, high);
		if (v[high] < v[low]) swap (low, high);
		if (v[mid] < v[low]) swap (low, mid);
		
		// Отладка
		// if (v[low] > v[mid] || v[mid] > v[high]) {
			// std::cout << v[low] << " " << v[mid] << " " << v[high] << std::endl;
		// }
		
		T pivot = v[mid];
		swap (mid, high - 1); // вставляем опорный элемент прямо перед a[high]
		int i = low, j = high - 1;
		while (true) {
			while (v[++i] < pivot) {;} // сканируем вправо, пока не a[i] >= pivot
			while (v[--j] > pivot) {;} // сканируем влево, пока не a[j] <= pivot
			if (i < j) {
				swap (i, j);
			} else {
				break;
			}
		}
		swap (i, high - 1); // восстанавливаем позицию опорного элемента
		
		// Отладка
		// if (v[low] > v[i] || v[i] > v[high]) {
			// std::cout << v[low] << " " << v[mid] << " " << v[high] << std::endl;
		// }
		
		// Оптимизация хвостовой рекурсии для сортировки правой части массива: 
		// задание новых значений low и high
		// Делаем оптимизацию хвостового вызова для бОльшей части массива
		if ((i - 1) - low < high - (i + 1)) {
			quickSort(v, low, i - 1); // Рекурсивная ортировка левой части массива
			low = i + 1; // Задание low для хвостовой рекурсии
		} else {
			quickSort(v, i + 1, high); // Рекурсивная сортировка правой части массива
			high = i - 1; // Задание high для хвостовой рекурсии
		}
	} while (true); // Возврат из функции будет, когда дойдём до размера <= 6
	
#undef swap
}

/* call qsort to start the sort */
template<class T> inline void Sort (std::vector<T> &v) { 
	quickSort(v, 0, (int)v.size() - 1); 
	
	// Контрольная отладка
	// int n = (int)v.size();
	// for (int i = 1; i < n; i++) {
		// if (v[i - 1] > v[i]) {
			// std::cout << v[i - 1] << " " << v[i] << std::endl;
		// }
	// }
}


int main()
{
	// Закомментировать эту строку и раскомментировать следующую для чтения с stdin
	freopen("numbers.txt", "rb", stdin);
	// 	freopen(NULL, "rb", stdin);

	std::vector<int> v;
	v.reserve(64 * 1024); // Если ожидается до 25 миллионов чисел, сразу устанавливаем ёмкость
				// вектора хотя бы на 64 тысячи чисел - избежим 10+ перевыделений памяти
	int i = 0;
	while ((i = GetNumber()) != -1) {
		v.push_back(i);
	}
	
	// Для теста - вывести все введённые числа
	//WriteVector(v, 1);
	
	Sort(v);

	// Для теста - вывести все отсортированные числа
	//WriteVector(v, 1);
	
	// Вывести отсортированные данные - каждые 10 чисел
	WriteVector(v, 10);
}