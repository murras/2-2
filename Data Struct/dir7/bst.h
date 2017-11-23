//B411063 김형석 2학년
#ifndef TREE_H
#define TREE_H
#include <iostream>
#include <queue>
using namespace std;

template <class  K, class E>
struct Node {
	Node(K ky, E el, Node<K, E> *left = 0, Node<K, E> *right = 0, int size = 1) : key(ky), element(el), leftChild(left), rightChild(right) { leftSize = size; }
	Node<K, E> *leftChild;
	Node<K, E> *rightChild;
	K key;
	E element;
	int leftSize;
};

template <class K, class E>
class BST {
public:
	BST() { root = 0; } // empty BST
	void Insert(K &newkey, E &el) { Insert(root, newkey, el); }
	void Inorder() { Inorder(root); }
	void Postorder() { Postorder(root); }
	bool Get(const K&, E&);
	void Print();
	bool RankGet(int r, K& k, E& e);
	void Delete(K &oldkey) { Delete(root, oldkey); }
	void ThreeWayJoin(BST<K, E>& small, K midkey, E midel, BST<K, E>& big);
	void TwoWayJoin(BST<K, E>& small, BST<K, E>& big);
private:
	void Visit(Node<K, E> *);
	void Insert(Node<K, E>* &, K &, E &);
	void Inorder(Node<K, E> *);
	void Postorder(Node<K, E> *);
	void Delete(Node<K, E>*&, K &);
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
	else if (newkey < ptr->key) {
		Insert(ptr->leftChild, newkey, el);
		ptr->leftSize++;
	}
	else if (newkey > ptr->key)
		Insert(ptr->rightChild, newkey, el);
	else ptr->element = el;
}

template <class K, class E> // 중위 LVR
void BST<K, E>::Inorder(Node<K, E> *currentNode) {
	if (currentNode) {
		Inorder(currentNode->leftChild);
		Visit(currentNode);
		Inorder(currentNode->rightChild);
	}
}

template <class K, class E> // 후위 LRV
void BST<K, E>::Postorder(Node<K, E> *currentNode) {
	if (currentNode) {
		Postorder(currentNode->leftChild);
		Postorder(currentNode->rightChild);
		Visit(currentNode);
	}
}

template <class K, class E>
bool BST<K, E>::Get(const K& k, E& e) {
	Node<K, E> *ptr = root;
	while (ptr) {
		if (k < ptr->key)
			ptr = ptr->leftChild;
		else if (k > ptr->key)
			ptr = ptr->rightChild; //
		else {
			e = ptr->element;
			return true;
		}
	}
}

template <class K, class E>
void BST<K, E>::Print() {
	cout << endl << "Inorder traversal: "; Inorder();
	cout << endl << "Postorder traversal: "; Postorder();
	cout << endl;
}

template <class K, class E>
bool BST<K, E>::RankGet(int r, K &k, E &e) {
	Node<K, E>* currentNode = root;
	
	while (currentNode) {
		if (r < currentNode->leftSize)
			currentNode = currentNode->leftChild;
		else if (r > currentNode->leftSize) {
			r -= currentNode->leftSize;
			currentNode = currentNode->rightChild;
		}
		else {
			k = currentNode->key;
			e = currentNode->element;
			return true;
		}
	}
	return false;

}

template <class K, class E>
void BST<K, E>::Delete(Node<K, E>* &ptr, K &oldkey) {
	// ptr을 루트로 하는 트리에서 oldkey를 키로 하는 노드를 지운다

	Node<K, E> * tmpdaddyptr;
	Node<K, E> * tmpptr;

	if (ptr == 0) return;
	if (oldkey < ptr->key) {
		Delete(ptr->leftChild, oldkey);
	}
	else if (oldkey > ptr->key) {
		Delete(ptr->rightChild, oldkey);
	}
	else {
		if (!ptr->leftChild && !ptr->rightChild) { // 아들이 없는 경우
			delete ptr;
			ptr = 0;
			return;
		}
		else if (ptr->leftChild && !ptr->rightChild) { // 왼쪽 아들만 있을 경우
			tmpptr = ptr;
			ptr = ptr->leftChild;
			delete tmpptr;
			return;
		}
		else if (!ptr->leftChild && ptr->rightChild) { // 오른쪽 아들만 있을 경우
			tmpptr = ptr;
			ptr = ptr->rightChild;
			delete tmpptr;
			return;
		}
		else { // 두 아들 모두 있을 경우
			Node<K, E> *rc = ptr->rightChild; // rc가 루트인 subtree
			if (!rc->leftChild) { // rc가 왼쪽아들이 없으면 rc가 그 노드
				ptr->key = rc->key;
				ptr->element = rc->element;
				ptr->rightChild = rc->rightChild;
				
				delete rc;
				return;
			}
			else {  // rc의 왼쪽 아들이 있을때
				while (true) {
					
					tmpdaddyptr = rc;
					tmpptr = tmpdaddyptr->leftChild;
					rc = tmpptr;
					if (!tmpptr->leftChild)
						break;
				}
				ptr->element = tmpptr->element;
				ptr->key = tmpptr->key;
				tmpdaddyptr->leftChild= tmpptr->rightChild;
			}
		}
	}
}

template <class K, class E>
void BST<K, E>::ThreeWayJoin(BST<K, E>& small, K midkey, E midel, BST<K, E>& big) {
	root = new Node<K, E>(midkey, midel, small.root, big.root);
	small.root = big.root = 0;
}

template <class K, class E>
void BST<K, E>::TwoWayJoin(BST<K, E>& small, BST<K, E>& big) {
	if (!small.root) {
		root = big.root;
		big.root = 0;
		return;
	}
	if (!big.root) {
		root = small.root;
		small.root = 0;
		return;
	}
	BST<K, E> small2 = small;
	// small2를 수정하고 midkey와 midel을 알아내어 ThreeWayJoin을 호출한다
	Node<K, E>* tmp;
	tmp = small2.root;
	while (true) {
		if (!tmp->rightChild)
			break;
		tmp=tmp->rightChild;
	}
	E midel_ = tmp->element;
	K key_ = tmp->key;
	small2.Delete(key_);
	ThreeWayJoin(small2, key_, midel_, big);


	small.root = 0;
	big.root = 0;
}
#endif
