// Скользящий максимум
// Описание алгоритма: http://leetcode.com/2011/01/sliding-window-maximum.html

// Заметим, что нам нужны не все k элементов окна.
// Если в окне были элементы [ 1, 4, 5, 2, 9, 10, 8], а новый элемент 11 - нам становятся
// ненужны все остальные элементы окна, потому что 11 больше всех их.
// В реализованном алгоритме используется структура данных двойная очередь.
// Время работы алгоритма O(n), что асимптотически не превосходит O(n*log(n)), 
// что удовлетворяет требованию задачи.

#include <iostream>
#include <vector>
#include <deque>


void PrintSlidingMax(std::vector<int> &A, int w) {
	int n = (int)A.size();
	
	std::deque<int> Q;
	for (int i = 0; i < w; i++) {
		while (!Q.empty() && A[i] >= A[Q.back()]) {
			Q.pop_back();
		}
		Q.push_back(i);
	}
	for (int i = w; i < n; i++) {
		std::cout << A[Q.front()] << " ";
		while (!Q.empty() && A[i] >= A[Q.back()]) {
			Q.pop_back();
		}
		if (!Q.empty() && (Q.front() == i - w)) {
			Q.pop_front();
		}
		Q.push_back(i);
	}
	std::cout << A[Q.front()] << std::endl;
}

int main()
{
	// comment this line for reading from stdin
	freopen("test.txt", "r", stdin);
	
	
	// Размер массива чисел
	int n = 0;
	std::cin >> n;
	std::vector<int> v;
	v.reserve(n); // Будет n чисел
	while( !std::cin.eof() && n > 0 ) {
		--n;
		int i = 0;
		std::cin >> i;
		v.push_back(i);
	}
	// Размер окна
	int k = 0;
	std::cin >> k;
	
	// Вывод входных данных
	// std::cout << n << " " << k << std::endl;
	// for(std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
		// std::cout << *it << " ";
	// }
	// std::cout << std::endl;
	
	// Вычисляет и сразу печатает скользящий максимум
	PrintSlidingMax(v, k);

}