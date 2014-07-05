// Порядок обхода

// Используем АВЛ дерево из предыдущей задачи, расширим его полем "вес" -
// количество узлов у каждого поддерева. Будем обновлять это поле вместе с высотой.
// Реализуем операцию получения k-го по порядку элемента дерева при помощи спуска
// в одну из листовых узлов. Высота АВЛ дерева O(lg(n)), значит, операция выполнится
// за O(lg(n)) времени.
// Таким образом, время каждого запроса (вставка/удаление/запрос k-го элемента)
// не будет превышать O(lg(n)).

#include <iostream>
// Используем умные указатели auto_ptr, чтобы не засорять память
// http://www.cplusplus.com/reference/memory/auto_ptr/ 
#include <memory>


class AVLTree {
	// Объявление этой структуры не видно извне.
	struct Node {
		int key;
		int height;
		// Новое поле - количество вершин в поддереве или "вес".
		int weight; // (с английского "вес")
		// "Умные" указатели сами вызовут деструктор объекта, на который указывают,
		// когда вызывают их деструктор. Это удобно.
		// Обычные указатели не имеют такого "встроенного" деструктора.
		// Минусы - умные указатели нельзя копировать (а также 
		// передавать по значению и возвращать из функции), иначе при деструкции копии
		// она "случайно" удалит и сам объект.
		std::auto_ptr<Node> left;
		std::auto_ptr<Node> right;
		
		// Конструктор
		Node(int k, int h = 1, int w = 1) : key(k), height(h), weight(w) {;}
		
		// Скопировать данные из другого узла дерева
		// (только данные, не трогать структуру дерева)
		void CopyData(std::auto_ptr<Node> &from) {
			key = from->key; // У нас данных - один только ключ. Повезло. :)
		}
	};
	
	// Дерево - это указатель на корень
	std::auto_ptr<Node> root;
	
	
	// Найти по ключу, вернуть указатель на вершину дерева.
	// Используем возврат по указателю (*), потому что operator= для автоматических
	// указателей имеет совсем другую семантику, чем можно было бы подумать.
	std::auto_ptr<Node>* FindInternal(int key) {
		std::auto_ptr<Node>* cur = &root;
		// Идём вправо, если key больше-равен значению, влево - если меньше
		while (cur->get() != NULL) {
			if (key >= cur->get()->key) {
				cur = &(cur->get()->right);
			} else {
				cur = &(cur->get()->left);
			}
		}
		return cur;
	}


public:
	// Проверить, есть ли узел с заданным ключом.
	// Если узел не найден - вернуть false.
	bool find(int key) {
		return (FindInternal(key)->get() != NULL);
	}
	

private:
	inline int GetHeight(const std::auto_ptr<Node> &cur) {
		return cur.get() == NULL ? 0 : cur->height;
	}
	
	// Баланс узла - высота левого сына минус высота правого
	inline int GetBalance(const std::auto_ptr<Node> &cur) {
		return cur.get() == NULL ? 0 : GetHeight(cur->left) - GetHeight(cur->right);
	}
	
	// Получить вес поддерева
	inline int GetWeight(const std::auto_ptr<Node> &cur) {
		return cur.get() == NULL ? 0 : cur->weight;
	}
	
	// Обновить высоту (и вес) узла по его двум детям
	void UpdateNodeHeight(const std::auto_ptr<Node> &cur) {
		if (cur.get() == NULL) return;
		int leftH = GetHeight(cur->left);
		int rightH = GetHeight(cur->right);
		cur->height = leftH > rightH ? leftH + 1 : rightH + 1;
		
		// Вес поддерева - это сумма весов детей +1 (собственный вес узла)
		cur->weight = GetWeight(cur->left) + GetWeight(cur->right) + 1;
	}
	
	// Вращение ребра дерева влево
	// http://en.wikipedia.org/wiki/Tree_rotation
	void LeftRotate(std::auto_ptr<Node> &cur) {
		// Необходимо, чтобы у узла был правый сын.
		if (cur.get() != NULL && cur->right.get() != NULL) {
			// Правый сын становится вместо вершины.
			Node* ex_peak = cur.release();
			Node* ex_right = ex_peak->right.release();
			cur.reset(ex_right);
			// Левый сын текущей вершины становится правым сыном бывшей вершины.
			Node* ex_left = ex_right->left.release();
			ex_peak->right.reset(ex_left);
			// Бывшая вершина становится левым сыном текущей вершины
			ex_right->left.reset(ex_peak);
			
			// Обновить высоты переставленных узлов дерева
			UpdateNodeHeight(cur->left);
			UpdateNodeHeight(cur); // Новая вершина - обновляем последней
		}
	}
	
	
	// Вращение ребра дерева вправо
	// http://en.wikipedia.org/wiki/Tree_rotation
	void RightRotate(std::auto_ptr<Node>& cur) {
		// Необходимо, чтобы у узла был левый сын.
		if (cur.get() != NULL && cur->left.get() != NULL) {
			// Левый сын становится вместо вершины.
			Node* ex_peak = cur.release();
			Node* ex_left = ex_peak->left.release();
			cur.reset(ex_left);
			// Правый сын текущей вершины становится левым сыном бывшей вершины.
			Node* ex_right = ex_left->right.release();
			ex_peak->left.reset(ex_right);
			// Бывшая вершина становится правым сыном текущей вершины
			ex_left->right.reset(ex_peak);
			
			// Обновить высоты переставленных узлов дерева
			UpdateNodeHeight(cur->right);
			UpdateNodeHeight(cur);
		}
	}
	
	
	// Перебалансировать вершину, если её баланс 2 или -2
	// Картинка: http://en.wikipedia.org/wiki/AVL_tree#mediaviewer/File:AVL_Tree_Rebalancing.svg
	void RebalanceNode(std::auto_ptr<Node> &cur) {
		int balance = GetBalance(cur);
		if (balance == 2) {
			// Левая сторона перевешивает правую.
			// 1) Проверить, есть ли "зигзаг" между дедом, отцом и сыном, распрямить
			if (GetBalance(cur->left) == -1) {
				LeftRotate(cur->left);
			}
			// 2) Согнуть "прямую линию" по середине
			RightRotate(cur);
		} else if (balance == -2) {
			// Правая сторона перевешивает левую - симметрично
			if (GetBalance(cur->right) == 1) {
				RightRotate(cur->right);
			}
			LeftRotate(cur);
		}
	}
	
	
public:
	// Вставка узла в АВЛ дерево.
	// http://en.wikipedia.org/wiki/AVL_tree#Insertion
	inline void add(int key) {
		AddInternal(root, key); 
	}
private:	
	void AddInternal(std::auto_ptr<Node> &cur, int key) {
		if (cur.get() == NULL) {
			// Тривиальный случай - вставляем новый узел в лист
			cur.reset(new Node(key));
			return;
		}
		
		// Рекурсивно вставить узел, как в наивное дерево
		if (key < cur->key) {
			AddInternal(cur->left, key);
		} else {
			AddInternal(cur->right, key);
		}
		// Добавили ребёнка - обновим высоту
		UpdateNodeHeight(cur);
		
		// На пути назад перебалансировать узлы
		RebalanceNode(cur);
	}
	

	// "НАСТОЯЩЕЕ" УДАЛЕНИЕ УЗЛОВ С ОЧИСТКОЙ ПАМЯТИ ПРОИСХОДИТ ТОЛЬКО ТУТ!!!
	// Вставить единственного ребёнка вместо себя (себя удалить)
	void ReplaceByChild(std::auto_ptr<Node> &cur) {
		// При cur.reset() неявно вызывается деструктор текущего узла.
		// Так как мы перед этим освобождаем указатель единственного 
		// ребёнка из-под удаляемого узла (с помощью release), то каскад удаления 
		// не пойдёт дальше этого единственного удаляемого узла.
		if (cur->left.get() == NULL) {
			cur.reset(cur->right.release());
		} else if (cur->right.get() == NULL) {
			cur.reset(cur->left.release());
		}
	}
	
	
	// Получить узел со следующим значением ключа.
	// (Спускаемся в левого ребёнка, дальше до упора вправо.
	std::auto_ptr<Node>* GetSuccessor(std::auto_ptr<Node> &cur) {
		if (cur.get() == NULL || cur->left.get() == NULL) {
			return &cur; // Тривиальный случай - нет последователя - берём себя
		}
		
		// Спускаемся всё время вправо по левому ребёнку
		std::auto_ptr<Node> *succ = &(cur->left);
		while (succ->get()->right.get() != NULL) {
			succ = &(succ->get()->right);
		}
		return succ;
	}

	
public:
	// Удаление узла из АВЛ дерева
	// http://en.wikipedia.org/wiki/AVL_tree#Deletion
	void del(int key) {
		DelInternal(root, key);
	}
private:
	void DelInternal(std::auto_ptr<Node> &cur, int key) {
		if (cur.get() == NULL) {
			// Тривиальный случай - не нашли узла, который надо удалять
			return;
		}
		
		// Рекурсивно спускаемся, пока не найдём узел с нужным ключом
		if (key < cur->key) {
			DelInternal(cur->left, key);
		} else if (key > cur->key) {
			DelInternal(cur->right, key);
		} else {
			// Добрались до вершины с нужным ключом
			if (cur->left.get() == NULL || cur->right.get() == NULL) {
				// Если нет детей или всего один ребёнок => вставляем ребёнка вместо себя
				ReplaceByChild(cur);
				return;
			}
			
			// У нас проблемы - случай, когда есть оба ребёнка.
			// Найти вершину - последователя (следующую по значению ключа)
			std::auto_ptr<Node> *succ = GetSuccessor(cur);
			// Скопировать данные последователя в текущую вершину.
			// Фактически, текущая вершина уже на этом шаге становится удалённой,
			// потому что её данных нигде больше нет.
			cur->CopyData(*succ);
			// Теперь рекурсивно удалим узел-последователь из левого поддерева. 
			// У него гарантировано не больше одного ребёнка - а это гарантирует
			// тривиальный случай для рекурсии (без второго поиска последователя). 
			DelInternal(cur->left, succ->get()->key);
		}
		
		// Удалили сына - обновим высоту (и вес)
		UpdateNodeHeight(cur);
		
		// На обратном пути из рекурсии - восстанавливаем баланс узлов
		RebalanceNode(cur);
	}

	
public:
	// Новая функция - получение k-го элемента (считая с 0)
	int GetKeyByOrder(int k) {
		return GetKeyByOrderInternal(root, k);
	}
private:
	int GetKeyByOrderInternal(const std::auto_ptr<Node> &cur, int k) {
		if (cur.get() == NULL) {
			return -1; // Ошибка - такого не должно было случиться
						// Возможно, дали номер, больший, чем размер дерева
		}
		
		int leftW = GetWeight(cur->left);
		if (leftW == k) {
			// Тривиальный случай - мы нашли свой элемент
			return cur->key;
		} else if (k < leftW) {
			// Ищем в левом поддереве
			return GetKeyByOrderInternal(cur->left, k);
		} else {
			// Наш номер строго больше, чем размер левого поддерева.
			// Спускаемся в правое поддерево, ищем элемент с номером k - leftW - 1
			return GetKeyByOrderInternal(cur->right, k - leftW - 1);
		}
	}
	
	
public:
	// Напечатать все вершины в порядке in-order (по неубыванию ключа)
	inline void PrintInOrder() {
		PrintInOrderHelper(root);
	}
private:
	void PrintInOrderHelper(const std::auto_ptr<Node> &cur) {
		// Обходим рекурсивно
		if (cur.get() != NULL) {
			PrintInOrderHelper(cur->left);
			std::cout << cur->key << " ";
			PrintInOrderHelper(cur->right);
		}
	}
	
public:
	// Напечатать все вершины в порядке pre-order (сначала корень, потом поддеревья)
	inline void PrintPreOrder() {
		PrintPreOrderHelper(root);
	}
private:
	void PrintPreOrderHelper(const std::auto_ptr<Node> &cur) {
		// Обходим рекурсивно
		if (cur.get() != NULL) {
			std::cout << cur->key;
			if (cur->left.get() != NULL || cur->right.get() != NULL) {
				std::cout << " [";
				PrintPreOrderHelper(cur->left);
				std::cout << ", ";
				PrintPreOrderHelper(cur->right);
				std::cout << "]";
			}
		} else {
			std::cout << "()";
		}
	}
	
public:
	// Вычислить высоту дерева - у нас уже есть такое поле
	inline int CountHeight() {
		return GetHeight(root);
	}
	
	// Вычислить вес дерева - взять поле у корня
	inline int CountWeight() {
		return GetWeight(root);
	}
	
};


int main()
{
	// comment this line for reading from stdin
	freopen("test.txt", "r", stdin);
	
	// Дерево. При выходе из программы будет автоматически уничтожено.
	AVLTree AT;
	
	int n = 0;
	std::cin >> n;
	while (!std::cin.eof() && n > 0) {
		--n;
		int key = 0, k = 0;
		std::cin >> key >> k;
		// Вставить, если ключ положительный; удалить, если отрицательный
		if (key > 0) {
			AT.add(key);
		} else {
			AT.del(-key);
		}
		// Для теста
		// std::cout << key << " " << k << std::endl;
		// AT.PrintInOrder(); std::cout << std::endl;
		// AT.PrintPreOrder(); std::cout << std::endl;
		// std::cout << AT.CountWeight() << std::endl;
		
		// Вывести k-й по порядку элемент
		std::cout << AT.GetKeyByOrder(k) << std::endl;
	}

}