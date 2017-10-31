#include <iostream>
#include "list.h"
ostream& operator<<(ostream& os, IntList& il) {
	Node* ptr = il.first;
	while (ptr != 0) {
		os << ptr->data << " ";
		ptr = ptr->link;
	}
	os << endl;
	return os;
}

void IntList::Push_Back(int e)
{
	if (!first)
		first = last = new Node(e);
	else {
		last->link = new Node(e);
		last = last->link;
	}
}

int IntList::Front() {
	//맨 앞성분 return
	return first->data;
}

void IntList::Pop_Front() {
	// 맨 앞 노드 제거
	first = first->link;
}

void IntList::Push_Front(int e) {
	// 맨 앞 노드 추가
	Node *p = new Node(e);
	p->link = first;
	first = p;
}

void IntList::Insert(int e) {
	// 정렬된 리스트에 새 노드 추가   
	if (!first)
		Push_Back(e);
	else if (last->data < e)
		Push_Back(e);
	else if (e < first->data)
		Push_Front(e);
	else if (first->data < e) {
		Node *q1 = first;
		Node *p = first->link;

		while (1) {
			if (p->data == e)
				break;
			if (p->data > e) {
				Node *q = new Node(e);
				q1->link = q;
				q->link = p;
				break;
			}
			if (p->data < e) {
				q1 = p;
				p = p->link;
				continue;
			}
		}
	}
}
