#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H
#include <iostream>
struct Term {
	float coef;
	int exp;
};

class Polynomial {

public:
	Polynomial();
	Polynomial operator+(Polynomial&);
	void NewTerm(const float, const int);
	friend ostream& operator<<(ostream&, Polynomial&);
	friend istream& operator>>(istream&, Polynomial&);

private:
	Term *termArray;
	int capacity; // 1로 초기화
	int terms; // 저장된 항의수 0으로 초기화

};
#endif