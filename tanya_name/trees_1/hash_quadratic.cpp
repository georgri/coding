// Хеш-таблица с разрешением коллизий методом открытой адресации
// с квадратичным пробированием
// i-ая проба:
// g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.

// Хранимые строки непустые и состоят из строчных латинских букв. 
// Начальный размер таблицы должен быть равным 8-ми. 
// Перехеширование в случае, когда коэффициент заполнения таблицы достигает 3/4.

// Для получения хеша из строки в виде целого числа используем функцию:
// h(c) = sum(xi * c^i) mod p, 
// где p = (2^61 - 1) - простое число
// xi - компоненты строки (по 4 байта)
// c - случайное нечётное число < p
// http://en.wikipedia.org/wiki/Universal_hashing#Hashing_strings

// Для хеширования целого числа выберем фунцию:
// h(a, b) = (unsigned) (a * x + b) >> (64 - M)
// Где a - случайное нечётное положительное целое, b - случайные положительное целое (в пределах одной таблицы)
// M - двоичный логарифм от размера таблицы
// http://en.wikipedia.org/wiki/Universal_hashing#Hashing_integers

// Для удаления используется алгоритм "ленивого удаления":
// http://en.wikipedia.org/wiki/Lazy_deletion


#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>     /* srand, rand */
#include <stdint.h>

// Возвращает целое нечётное случайное 64-бит число
inline uint64_t GetRand64() {
	return (((uint64_t)rand()) << 32) + (rand() | 1); // Насильно добавляем единичку в конец 
													// для гарантии нечётности
}

// Получает i mod (2^61 - 1) без операции умножения
// http://mersenneforum.org/showthread.php?t=1955
inline uint64_t modP(uint64_t i) {
	// Константа (2^61 - 1) (в ней 61 единичка)
	const uint64_t M61 = ((uint64_t)-1) >> 3;
	i = (i & M61) + (i >> 61);
	return (i & M61) + (i >> 61);
}




class StringHashTable {
// Начальная ёмкость таблицы. Должна быть степенью двойки.
#define INITIAL_CAPACITY 8

	std::vector<std::string> v;
	std::vector<bool> deleted; // признак удалённых строк
	std::vector<uint64_t> coeffs; // Коэффициенты для хеширования
	unsigned int capacity; // ёмкость таблицы
	unsigned int M; // lg(capacity)
	unsigned int mask; // битовая маска для быстрого выполнения mod (M единичек в младших битах)
	unsigned int size; // заполненность таблицы
	unsigned int limit; // Предел заполнения таблицы (3/4 от capacity)
	uint64_t a, b, c; // параметры хеш-функции
	
	bool containsEmpty; // Специальный случай - содержит ли таблица пустую строку
		
	// Первичное хеширование
	inline unsigned int FirstHash(const std::string &s) {
		unsigned int n = (unsigned int)s.size();
	
		// Строка => целое число
		// h(c) = sum(xi * c^i) mod p
		uint64_t h = 0;
		// Группируем буквы по 8 байт
		for (unsigned int i = 0; i <= ((n - 1) >> 3); i++) {
			uint64_t buf = 0;
			for (unsigned int j = (i << 3); j < (i << 3) + 7 && j < n; j++) {
				buf <<= 8; // освобождаем место в младших 8 битах
				buf += s[j]; // копируем туда символ
			}
			
			// Суммируем. Храним коэффициенты в массиве для экономии умножений
			if (i == coeffs.size()) {
				if (i == 0) {
					coeffs.push_back(c);
				} else {
					// Тут и далее в этой функции при умножении 64-бит чисел
					// мы теряем старшие 64 бита и получаем только младшие 64.
					// Это не совсем корректно с точки зрения теории, 
					// но для практических целей должно быть достаточно.
					coeffs.push_back(modP(coeffs[i - 1] * c));
				}
			}
			h = modP(h + modP(buf * coeffs[i]));
		}
		
		// Целое число => номер ячейки
		// h(a, b) = (unsigned) (a * x + b) >> (64 - M)
		return (unsigned int) ((a * h + b) >> (64 - M));
	}
	
	// Вторичное хеширование. i - номер итерации
	inline unsigned int SecondHash(unsigned int prevHash, unsigned int i) {
		// g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.
		return (prevHash + i) & mask;
	}
	
	// Удвоить ёмкость таблицы при заполнении на 3/4
	void DoubleCapacity() {
		unsigned int oldCapacity = capacity;
		capacity *= 2;
		M++;
		mask = capacity - 1; // M единичек в младших битах
		limit = (capacity >> 2) * 3; // Битово делим на 4 и умножаем на 3
		a = GetRand64();
		b = GetRand64();
		c = (GetRand64() >> 3) | 1; // Должно быть не больше 2^61 - 1
		
		size = 0;
		// Увеличить v и deleted в 2 раза
		std::vector<std::string> oldV(capacity);
		std::vector<bool> oldDeleted(capacity);
		// Меняем содержимое текущих векторов с новыми векторами oldV, oldDeleted 
		v.swap(oldV);
		deleted.swap(oldDeleted);
		
		// Копируем все неудалённые элементы из старого вектора в новый
		for (unsigned int i = 0; i < oldCapacity; i++) {
			if (!oldDeleted[i] && !oldV[i].empty()) {
				add(oldV[i]);
			}
		}
	}
	
	// Найти элемент (функция для внутреннего использования)
	// Возвращает позицию найденного элемента в таблице или capacity, если ничего не найдено
	// Параметр forInsert - выполняем поиск для вставки элемента в пустую или удалённую ячейку
	// http://en.wikipedia.org/wiki/Lazy_deletion
	unsigned int find_internal(const std::string &s, bool forInsert = false) {
		// Находим элемент, если встречаем по пути удалённый - меняем их местами
		unsigned int delPlace = capacity; // По-умолчанию - не встречено удалённого элемента
		
		unsigned int place = FirstHash(s);
		// Условие (!forInsert && deleted[place]) означает, что если мы ищем для вставки
		// и встретили удалённый элемент, то сразу завершаем поиск
		for (unsigned int i = 1; !v[place].empty() || (!forInsert && deleted[place]); i++) {
			if (deleted[place] && (delPlace == capacity)) {
				delPlace = place; // Запомнили место первого удалённого слота
			}
			
			if (v[place] == s) {
				// Нашли нужный элемент - меняем с удалённым, возвращаем новую позицию
				if (delPlace != capacity) {
					v[delPlace].swap(v[place]);
					deleted[delPlace] = false;
					deleted[place] = true;
					place = delPlace;
				}
				return place;
			}
			
			// Генерируем вторичный хеш с итерацией i
			place = SecondHash(place, i);
		}
		
		if (forInsert) {
			// Искали для вставки - возвращаем номер пустого или удалённого слота
			return place;
		} else {
			// Не нашли элемент - возвращаем capacity
			return capacity;
		}
	}
	
public:
	StringHashTable() {
		capacity = INITIAL_CAPACITY;
		limit = (capacity >> 2) * 3; // 3/4 размера таблицы
		
		// M - двоичный логарифм от capacity
		// capacity = 8  ==>  M = 3
		// capacity = 16 ==>  M = 4
		M = 0;
		for (unsigned int i = capacity;  i > 1;  i >>= 1, M++) {;}
		
		// capacity = 8  ==>  mask = 111b  = 7
		// capacity = 16 ==>  mask = 1111b = 15
		mask = capacity - 1; // M единичек в младших битах
		
		a = GetRand64();
		b = GetRand64();
		c = (GetRand64() >> 3) | 1; // Должно быть не больше 2^61 - 1
		
		size = 0; // Изначально в таблице нет элементов
		v.resize(INITIAL_CAPACITY);
		deleted.resize(INITIAL_CAPACITY);
		
		containsEmpty = false;
	}
	
	bool add(const std::string &s) {
		// Специальный случай - добавление пустой строки
		if (s.empty()) {
			if (containsEmpty) {
				return false; // Пустая строка уже содержится
			} else {
				containsEmpty = true;
				return true; // "Добавили", вернули true
			}
		}
	
		// В случае переполнения 3/4 ёмкости удваиваем таблицу
		if (size >= limit) {
			DoubleCapacity();
		}
		
		// Поиск, куда воткнуть элемент. Слоты с удалёнными строками считаются пустыми.
		unsigned int place = find_internal(s, true);
		
		// Если элемент есть, то возвращаем false.
		if (v[place] == s) {
			return false;
		}

		// Нашли пустой слот - вставляем туда строку, помечаем элемент как неудалённый
		size++;
		v[place] = s;
		deleted[place] = false;
		
		return true;
	}
	
	bool del(const std::string &s) {
		// Специальный случай - удаление пустой строки
		if (s.empty()) {
			if (containsEmpty) {
				containsEmpty = false;
				return true;
			} else {
				return false;
			}
		}
	
		// Находим элемент, стираем, помечаем как удалённый
		// При поиске слоты с удалёнными элементами считаем заполненными
		unsigned int place = find_internal(s);
		if (place != capacity) {
			// Нашли - удаляем
			deleted[place] = true;
			v[place].clear();
			return true;
		}
		
		// не нашли - возвращаем ЛОЖЬ
		return false;
	}
	
	// Возвращаем ПРАВДА, если элемент в таблице
	bool find(const std::string &s) {
		// Специальный случай - поиск пустой строки
		if (s.empty()) {
			return containsEmpty;
		}
		
		return find_internal(s) != capacity;
	}
	
};

int main()
{
	// comment this line for reading from stdin
	freopen("test.txt", "r", stdin);
	
	
	// Размер массива чисел
	char op;
	std::string word;
	StringHashTable t;
	while( !std::cin.eof()) {
		std::cin >> op >> word;
		// Для теста: вывод всех входных данных
		//std::cout << op << ", '" << word << "'" << std::endl;
		
		bool result;
		switch (op) {
			case '+': result = t.add(word); break;
			case '-': result = t.del(word); break;
			case '?': result = t.find(word); break;
			default: result = false; break;
		}
		std::cout << (result ? "OK" : "FAIL") << std::endl;
	}

}