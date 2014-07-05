// Декартово дерево, или дуча, или treap


#include <iostream>
// Используем умные указатели auto_ptr, чтобы не засорять память
// http://www.cplusplus.com/reference/memory/auto_ptr/ 
#include <memory>

// Используем реализацию наивного дерева из задачи #2
#include "naive_tree.h"


class Treap {
	// Объявление этой структуры не видно извне.
	struct Node {
		int key;
		int priority;
		// "Умные" указатели сами вызовут деструктор объекта, на который указывают,
		// когда вызывают их деструктор. Это удобно.
		// Обычные указатели не имеют такого "встроенного" деструктора.
		// Минусы - умные указатели нельзя копировать (а также 
		// передавать по значению и возвращать из функции), иначе при деструкции копии
		// она "случайно" удалит и сам объект.
		std::auto_ptr<Node> left;
		std::auto_ptr<Node> right;
				
		Node(int k, int p) : key(k), priority(p) {;}
	};

	// Дерево - это указатель на корень
	std::auto_ptr<Node> root;
	
	
	// Хорошее описание операций слияния и деления декартовых деревьев
	// в этой статье: http://habrahabr.ru/post/101818/
	
	
	// Операция слияния 2 декартовых поддеревьев. Возвращаем новое дерево
	// В левом поддереве все ключи меньше, чем в правом.
	// Переданные в эту функцию указатели должны быть освобождены от auto_ptr.
	// В этом случае гарантируется, что возвращаемый указатель также освобождён от auto_ptr.
	Node* Merge(Node* left, Node* right) {
		// Тривиальный случай: если одно из деревьев пустое - возвращаем второе
		if( left == NULL || right == NULL ) {
			return left == NULL ? right : left;
		}
		// Если приоритет корня левого дерева больше => новым корнем будет
		// вершина левого дерева, потому что у неё максимальный приоритет среди всех
		if( left->priority > right->priority ) {
			// left - новый корень. Его надо вернуть.
			// Левый сын у него остаётся тем же.
			// Новый правый сын - результат слияния right и старого правого сына
			// Перед передачей указателя на правого сына, освобождаем его от auto_ptr,
			// чтобы им могли завладеть другие автоматические указатели
			// http://www.cplusplus.com/reference/memory/auto_ptr/release/
			Node* newRightChild = Merge(left->right.release(), right);
			left->right.reset(newRightChild);
			return left;
		}
		// Случай, когда новый корень - вершина right.
		// Правый ребёнок у неё не меняется.
		// Левый ребёнок - результат слияния left и старого левого сына
		Node* newLeftChild = Merge(left, right->left.release());
		right->left.reset(newLeftChild);
		return right;
	}
	
	
	// Операция деления декартова дерева.
	// На вход - декартово дерево и ключ key.
	// Задача разделить дерево на два поддерева, в одном все ключи меньше key, 
	// в другом все ключи больше key.
	// Передаваемый в эту функцию указатель cur должны быть освобожден от auto_ptr.
	// В этом случае гарантируется, что возвращаемые left и right будут освобождёнными.
	void Split(Node* cur, int key, Node*& left, Node*& right) {
		if( cur == NULL ) {
			// Пустое дерево можно разделить только на 2 пустых
			left = NULL;
			right = NULL;
		} else if( key >= cur->key ) {
			// Случай, когда ключ >= вершине дерева:
			// Вершина дерева (и её левый сын) уходят в left.
			left = cur;
			// Правый же сын вершины должен быть рекурсивно разделён.
			Node* tempLeft = NULL;
			// Правая часть от правого сына - сразу уходит в right
			Split(cur->right.release(), key, tempLeft, right);
			// Левая часть от правого сына - новый правый сын вершины
			cur->right.reset(tempLeft);
		} else {
			// Случай, когда ключ < вершины дерева:
			// Вершина дерева (и её правый сын) уходят в right.
			right = cur;
			// Левый же сын вершины должен быть рекурсивно разделён.
			Node* tempRight = NULL;
			// Левая часть от левого сына - сразу уходит в left
			Split(cur->left.release(), key, left, tempRight);
			// Правая часть от левого сына - новый левый сын вершины
			cur->left.reset(tempRight);
		}
	}
	
	
	// Найти по ключу, вернуть указатель на вершину дерева.
	// Если вершина не найдена - остановиться на ближайшей родительской
	Node* FindInternal(int key) {
		Node* cur = root.get();
		
		if (cur == NULL) {
			return NULL;
		}
		
		// Идём вправо, если key больше-равен значению, влево - если меньше
		while ((key >= cur->key && cur->right.get() != NULL)
				|| (key < cur->key && cur->left.get() != NULL)) {
				
			if (key >= cur->key) {
				cur = cur->right.get();
			} else {
				cur = cur->left.get();
			}
		}
		return cur;
	}


public:
	// Проверить, есть ли вершина с заданным ключом.
	// Если вершина не найдена - вернуть false.
	bool find(int key) {
		Node* cur = FindInternal(key);
		if (cur == NULL || cur->key != key) {
			return false;
		}
		return true;
	}
	
	// Алгоритм добавления узла из задания:
	// При добавлении узла выполняйте спуск по ключу до узла P с меньшим приоритетом. 
	// Затем разбивайте найденное поддерево. Получившиеся два дерева сделайте 
	// дочерними для нового узла. Новый узел вставьте на место узла P.
	void add(int key, int priority) {
		std::auto_ptr<Node> *cur = &root;
		// Спускаемся до первого узла, у которого приоритет меньше priority
		while (cur->get() != NULL && (*cur)->priority >= priority) {
			if (key >= (*cur)->key) {
				cur = &(*cur)->right;
			} else {
				cur = &(*cur)->left;
			}
		}
			
		// Возможны 2 варианта.
		if (cur->get() == NULL) {
			// 1) Вставляем вместо NULL
			cur->reset(new Node(key, priority));
		} else {
			// 2) Вершина имеет меньший приоритет, чем priority.
			// Разбить её на 2 дерева.
			Node *tempLeft, *tempRight;
			Split(cur->release(), key, tempLeft, tempRight);
			// Эти два дерева привязать к новому узлу.
			cur->reset(new Node(key, priority));
			(*cur)->left.reset(tempLeft);
			(*cur)->right.reset(tempRight);
		}
	}
	
	
public:
	// Напечатать все вершины в порядке in-order (по неубыванию ключа)
	void PrintInOrder() {
		PrintInOrderHelper(root.get());
	}
private:
	void PrintInOrderHelper(Node* cur) {
		// Обходим рекурсивно
		if (cur != NULL) {
			PrintInOrderHelper(cur->left.get());
			std::cout << cur->key << "-" << cur->priority << " ";
			PrintInOrderHelper(cur->right.get());
		}
	}
	
public:
	// Напечатать все вершины в порядке pre-order (сначала корень, потом поддеревья)
	void PrintPreOrder() {
		PrintPreOrderHelper(root.get());
	}
private:
	void PrintPreOrderHelper(Node* cur) {
		// Обходим рекурсивно
		if (cur != NULL) {
			std::cout << cur->key << "-" << cur->priority << " (";
			PrintPreOrderHelper(cur->left.get());
			std::cout << ", ";
			PrintPreOrderHelper(cur->right.get());
			std::cout << ") ";
		} else {
			std::cout << "()";
		}
	}
	
	
public:
	// Вычислить высоту дерев
	inline int CountHeight() {
		return CountHeightHelper(root.get());
	}
	
private:
	// По определению, высота дерева - максимальная длина пути до листа.
	// Листьями считаем указатели на NULL. (Дерево из одной вершины будет высотой 1)
	int CountHeightHelper(Node* cur) {
		if (cur == NULL) {
			return 0; // Высота листа = 0
		}
		
		int left_height = CountHeightHelper(cur->left.get());
		int right_height = CountHeightHelper(cur->right.get());
		if (left_height > right_height) {
			return left_height + 1;
		}
		return right_height + 1;
	}
	
};


int main()
{
	// comment this line for reading from stdin
	freopen("test.txt", "r", stdin);
	
	// Деревья. При выходе из программы будут автоматически уничтожены.
	NaiveTree NT;
	Treap Tr;
	
	// Размер массива чисел
	int n = 0;
	std::cin >> n;
	while( !std::cin.eof() && n > 0 ) {
		--n;
		int key = 0, priority = 0;
		std::cin >> key >> priority;
		// Вставить элемент в деревья
		NT.add(key);
		Tr.add(key, priority);
	}
	
	// Для теста
	// NT.PrintInOrder(); std::cout << std::endl;
	
	// Tr.PrintInOrder(); std::cout << std::endl;
	// Tr.PrintPreOrder(); std::cout << std::endl;
	
	// std::cout << NT.CountHeight() << " " << Tr.CountHeight() << std::endl;
	
	// Вывести разницу глубин деревьев.
	std::cout << NT.CountHeight() - Tr.CountHeight();
}