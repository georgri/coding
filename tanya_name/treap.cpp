#include <cstdio>
#include <iostream>

struct CTreapNode {
    int Key;
	int Priority;
    CTreapNode* Left;
	CTreapNode* Right;

	CTreapNode(int key, int priority) : Key(key), Priority(priority), Left(0), Right(0) {}
};


CTreapNode* Merge(CTreapNode* left, CTreapNode* right)
{
	if( left == 0 || right == 0 ) {
		return left == 0 ? right : left;
	}
	if( left->Priority > right->Priority ) {
		left->Right = Merge(left->Right, right);
		return left;
    }
	right->Left = Merge(left, right->Left);
	return right;
}

void Split(CTreapNode* currentNode, int key, CTreapNode*& left, CTreapNode*& right)
{
	if( currentNode == 0 ) {
		left = 0;
		right = 0;
	} else if( currentNode->Key <= key ) {
		// Сокращенный вариант - Split(currentNode->Right, key, currentNode->Right, right);
		CTreapNode* tempLeft = 0;
		CTreapNode* tempRight = 0;
		Split(currentNode->Right, key, tempLeft, tempRight);
		right = tempRight;
		left = currentNode;
		left->Right = tempLeft;
    } else {
		// Сокращенный вариант - Split(currentNode->Left, key, left, currentNode->Left);
		CTreapNode* tempLeft = 0;
		CTreapNode* tempRight = 0;
		Split(currentNode->Left, key, tempLeft, tempRight);//a, t->l);
		left = tempLeft;
		right = currentNode;
		right->Left = tempRight;
    }
}

CTreapNode* Insert(CTreapNode* root, int key, int priority) {
	CTreapNode* node = new CTreapNode(key, priority);
	CTreapNode* splitLeft = 0;
	CTreapNode* splitRight = 0;
	Split(root, key, splitLeft, splitRight);
    return Merge(Merge(splitLeft, node), splitRight);
}

CTreapNode* Delete(CTreapNode* root, int key)
{
	CTreapNode* splitLeft = 0;
	CTreapNode* splitRight = 0;
	Split(root, key - 1, splitLeft, splitRight);
	CTreapNode* splitRightLeft = 0;
	CTreapNode* splitRightRight = 0;
    Split(splitRight, key, splitRightLeft, splitRightRight);
    delete splitRightLeft;
	return Merge(splitLeft, splitRightRight);
}

int main()
{
	freopen("001.dat", "r", stdin);

	CTreapNode* root = 0;

	while( true ) {
		int key = 0;
		int priority = 0;
		std::cin >> key >> priority;
		if( std::cin.eof() ) {
			break;
		}
		if( root == 0 ) {
			root = new CTreapNode(key, priority);
		} else {
			root = Insert(root, key, priority);
		}
	}

	while( root != 0 ) {
		root = Delete(root, root->Key);
	}
}