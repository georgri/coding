// Задача Современники
#include <iostream>
#include <vector>
#include <list>
#include <stdint.h>
#include "stdio.h"


#include "merge_sort.h"

// Дату мы перегоним в формат "20140608" - число от 0 до <= 31 + 12 * 100 + 9999 * 10000 < 100.000.000
// 100 миллионов даже влезут в знаковый int

// Мы можем схитрить - не писать класс для хранения пар дат, а хранить
// дату рождения в старших 32 битах, а дату смерти в младших 32 битах 64 битного числа

// Достать дату рождения - левые (старшие) 32 бита
inline int birth(int64_t pair) { 
	return (int)(pair >> 32);
}
// Достать дату смерти - правые (младшие) 32 бита
inline int death(int64_t pair) {
	return (int)(pair & 0x00000000FFFFFFFF);
}

inline bool inside(int date, int from, int to) {
	return (to >= from && date == from) || (from <= date && date < to);
}

bool intersect(int64_t pair1, int64_t pair2) {
	return (birth(pair1) + 180000 <= death(pair1)
			&& birth(pair2) + 180000 <= death(pair2)) // Оба дожили до 18 летия
		&& (inside(birth(pair1) + 180000, birth(pair2) + 180000, death(pair2)) 
			|| inside(birth(pair2) + 180000, birth(pair1) + 180000, death(pair1)));
}


// Надо найти максимальное количество пересекающихся интервалов в отсортированном массиве
// Ключевые моменты, которые надо проверять на пересечения - когда кто-то рождается.
int GetMaxIntersect(std::vector<int64_t> &v) {
	int n = (int)v.size();
	
	int max = 0; // Будем обновлять, если найдём пересечения
	
	// Храним в списке актуальные интервалы, с которыми ещё может быть пересечение
	std::list<int64_t> prev;
	
	// Идём по интервалам, ищем пересечения даты рождения с предыдущими интервалами
	for (int i = 0; i < n; i++) {
		int cur = 0; // Текущий счётчик пересечений - интервал может не пересекаться с собой
		prev.push_back(v[i]); // Запихиваем текущий интервал в список для поиска

		// Считаем пересечения с актуальными интервалами в списке
		for (std::list<int64_t>::iterator it = prev.begin(); it != prev.end();) {
			// Если интервал устарел - удаляем из списка
			if (intersect(v[i], *it)) {
				++cur; // Увеличиваем счётчик пересекающихся интервалов
				++it; // Переходим к следующему элементу списка
			} else {
				it = prev.erase(it); // Удаляем элемент, переприсваивается итератор
			}
		}
		
		// Нашли больше пересекающихся интервалов, чем было?
		if (cur > max) {
			max = cur;
		}
	}
	
	return max;
}

int main()
{
	// comment this line for reading from stdin
	freopen("dates.txt", "r", stdin);
	
	std::vector<int64_t> v;
	
	int n = 0;
	std::cin >> n;
	while( !std::cin.eof() && n > 0 ) {
		--n;
		// формат: день месяц год день месяц год
		int d, m, y, birthday, dayOfDeath;
		std::cin >> d >> m >> y;
		birthday = d + m * 100 + y * 10000;
		std::cin >> d >> m >> y;
		dayOfDeath = d + m * 100 + y * 10000;
		int64_t i = ((int64_t)birthday << 32) + dayOfDeath;
		v.push_back(i);
	}
	
	// Отсортировать по дате рождения
	Sort<int64_t>(v);
	
	// Напечатать для тестовых целей, что получилось после сортировки
	/*
	for(std::vector<int64_t>::iterator it = v.begin(); it != v.end(); ++it) {
		std::cout << birth(*it) << " " << death(*it) << std::endl;
	}
	*/
	
	std::cout << GetMaxIntersect(v);
	
}