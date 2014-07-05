// Порядок обхода in-order (наивный порядок вставки)

#include <iostream>
// Используем умные указатели auto_ptr, чтобы не засорять память
// http://www.cplusplus.com/reference/memory/auto_ptr/ 
#include <memory>


class NaiveTree {
	// Объявление этой структуры не видно извне.
	struct Node {
		int key;
		// "Умные" указатели сами вызовут деструктор объекта, на который указывают,
		// когда вызывают их деструктор. Это удобно.
		// Обычные указатели не имеют такого "встроенного" деструктора.
		// Минусы - умные указатели нельзя копировать (а также 
		// передавать по значению и возвращать из функции), иначе при деструкции копии
		// она "случайно" удалит и сам объект.
		std::auto_ptr<Node> left;
		std::auto_ptr<Node> right;
		
		Node* parent; // Обычный указатель, потому что родитель сам себя уничтожит
		
		Node(int k, Node* p) {
			this->key = k;
			this->parent = p;
		}
	};

	// Дерево - это указатель на корень
	std::auto_ptr<Node> root;
	
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
	// Проверить, есить ли вершина с заданным ключом.
	// Если вершина не найдена - вернуть false.
	bool find(int key) {
		Node* cur = FindInternal(key);
		if (cur == NULL || cur->key != key) {
			return false;
		}
		return true;
	}
	
	// Добавить вершину в дерево. У нас может быть несколько вершин с одним ключом,
	// поэтому не возвращаем ничего. (в хеш таблице, например, возвращалось false,
	// если такой элемент уже присутствовал)
	void add(int key) {
		Node* cur = FindInternal(key);
		if (cur == NULL) {
			// Такое может быть, только если корень пуст
			root.reset(new Node(key, cur)); // Родитель корня - NULL
		} else if (key > cur->key) {
			cur->right.reset(new Node(key, cur));
		} else {
			cur->left.reset(new Node(key, cur));
		}
	}
	
	// Напечатать все вершины в порядке in-order (по неубыванию ключа)
	void PrintInOrder() {
		// Требуется обойти дерево нерекурсивно.
		// Используем вспомогательную переменную состояния
		// DOWN - опустились из родителя в одного из детей
		// FROM_LEFT - поднялись из левого ребёнка
		// FROM_RIGHT - поднялись из правого ребёнка
		enum {DOWN, FROM_LEFT, FROM_RIGHT};
		int state = DOWN;
		
		Node* cur = root.get();
		while (cur != NULL) {
			switch (state) {
				case DOWN: { // Отдельный блок, чтобы переменные не виделись в других case
					// Спустились из родителя. Спускаемся в левое поддерево
					Node* left_node = cur->left.get();
					if (left_node != NULL) {
						cur = left_node;
						state = DOWN;
					} else {
						// Не удалось спуститься - считаем, что поднялись
						state = FROM_LEFT;
					}
				} break;
				
				case FROM_LEFT: {
					// Поднялись из левого поддерева - печатаем себя, спускаемся
					// в правое поддерево.
					std::cout << cur->key << " ";
					
					Node* right_node = cur->right.get();
					if (right_node != NULL) {
						cur = right_node;
						state = DOWN;
					} else {
						// Не удалось спуститься - считаем, что поднялись
						state = FROM_RIGHT;
					}
				} break;
				
				case FROM_RIGHT: {
					// Поднялись из правого поддерева - поднимаемся в родителя
					Node* parent_node = cur->parent; // Обычный указатель, без get()
					if (parent_node != NULL) {
						if (cur == parent_node->left.get()) {
							state = FROM_LEFT;
						} else {
							state = FROM_RIGHT;
						}
					}
					
					// Если родитель пуст - значит, мы в корне и всё уже обошли.
					// Условие while завершит обход, если cur == NULL.
					cur = parent_node;
				} break;
			}
		}
	}
	

	// Вычислить высоту дерева	
	inline int CountHeight() {
		return CountHeightHelper(root.get());
	}
	
private:
	// По определению, высота дерева - максимальная длина пути до листа.
	// Листьями считаем указатели на NULL. (Дерево из одной вершины будет высотой 1)
	int CountHeightHelper(Node* cur) {
		if (cur == NULL) {
			return 0;
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
	freopen("numbers.txt", "r", stdin);
	
	// Дерево. При выходе из программы автоматически вызовется деструктор, который
	// по цепной реакции освободит все элементы из-под умных указателей.
	NaiveTree T;
	
	// Размер массива чисел
	int n = 0;
	std::cin >> n;
	while( !std::cin.eof() && n > 0 ) {
		--n;
		int i = 0;
		std::cin >> i;
		// Вставить элемент в дерево
		T.add(i);
	}
	
	// Вывести все элементы в порядке неубывания ключа
	T.PrintInOrder();
	
	//std::cout << std::endl << T.CountHeight();
}