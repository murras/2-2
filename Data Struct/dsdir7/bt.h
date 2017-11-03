#ifndef BT_H
#define BT_H
#include <iostream>
#include <queue>
using namespace std;

template <class  K, class E>
struct Node {
	Node(K ky, E el, Node<K, E> *left = 0, Node<K, E> *right = 0) : key(ky), element(el), leftChild(left), rightChild(right) { }
	Node<K, E> *leftChild;
	Node<K, E> *rightChild;
	K key;
	E element;
	Node<K, E> *rightChild;
};

template <class K, class E>
class BST {
public:
	BST() { root = 0; } // empty BST
	void Insert(K &newkey, E &el) { Insert(root, newkey, el); }
	void Preorder() { Preorder(root); }
	void Inorder() { Inorder(root); }
	void Postorder() { Postorder(root); }
	void Levelorder();

private:
	void Visit(Node<K, E> *);
	void Insert(Node<K, E>* &, K &, E &);
	void Preorder(Node<K, E> *);
	void Inorder(Node<K, E> *);
	void Postorder(Node<K, E> *);

	Node<K, E> *root;
};

template <class K, class E>
void BST<K, E>::Visit(Node<K, E> *ptr) {
	cout << ptr->key << ":" << ptr->element << " ";
}

template <class K, class E>
void BST<K, E>::Insert(Node<K, E>* &ptr, K &newkey, E &el) {
	if (ptr == 0)
		ptr = new Node<K, E>(newkey, el);
	else if (newkey < ptr->key)
		Insert(ptr->leftChild, newkey, el);
	else if (newkey > ptr->key)
		Insert(ptr->righrChild, newkey, el);
	else ptr->element = el;
}

template <class K, class E>
void BST<K, E>::Preorder(Node<K, E> *currentNode) {
	if (currentNode) {
		Visit(currentNode);
		Preorder(currentNode->leftChild);
		Preorder(currentNode->rightChild);
	}
}

template <class K, class E>
void BST<K, E>::Inorder(Node<K, E> *currentNode) {
	if (currentNode) {
		Visit(currentNode);
		Inorder(currentNode->leftChild);
		Inorder(currentNode->rightChild);
	}
}
#endif
