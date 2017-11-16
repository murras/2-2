#include <iostream>
using namespace std;

template <class T>
class MyStack {
	int tos;
	T data[100];
public:
	MyStack();
	void push(T element);
	T pop();
	friend void popAll(MyStack<T>);
	friend void popAll_ref(MyStack<T>&);
};

template <class T>
MyStack<T>::MyStack() {
	tos = -1;
}

template <class T>
void MyStack<T>::push(T element) {
	if (tos == -99) {
		cout << "stack full";
		return;
	}
	tos++;
	data[tos] = element;
}

template <class T>
T MyStack<T>::pop() {
	T retdata;
	if (tos == -1) {
		cout << "stack empty";
		return 0;
	}
	retdata = data[tos--];
	return retdata;
}

//template <class T>
void popAll(MyStack<int> stack) {
	while (1) {
		if (stack.tos == -1) {
			cout << "stack end" << endl;
			break;
		}
		cout << stack.pop() << endl;
	}
}

//template <class T>
void popAll_ref(MyStack<int> &stack) {
	while (1) {
		if (stack.tos == -1) {
			cout << "stack end" << endl;
			break;
		}
		cout << stack.pop() << endl;
	}
}

int main() {
	MyStack<int> istack;
	istack.push(3);
	istack.push(9);
	istack.push(15);
	cout << istack.pop() << endl;

	MyStack<double> dstack;
	dstack.push(3.5);
	dstack.push(15.4);
	dstack.push(115.354);
	cout << dstack.pop() << endl;

	MyStack<char> *p = new MyStack<char>();
	p->push('m');
	p->push('u');
	p->push('r');
	cout << p->pop() << endl;
	delete p;
	
	cout << endl << endl;
	popAll(istack);
	popAll_ref(istack);
}
