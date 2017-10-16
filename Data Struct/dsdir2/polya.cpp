#include <iostream>
using namespace std;
#include "polya.h"

istream& operator>>(istream& is, Polynomial& p){
// #terms and (coefficient, exponent)의 pair 들을 읽어 들인다.
// 높은 차수의 항부터 저장었다고 가정한다.

    int noofterms;
    float coef;
    int exp;
    is >> noofterms;
    for(int i=0; i<noofterms; i++){
        is >> coef >> exp;
        p.NewTerm(coef, exp);
    }
    return is;
}

ostream& operator<< (ostream& os, Polynomial& p) {

	// 조건 1 첫항 부호 없이
	// 조건 2 계수가 1일때 생략
	// 조건 3 계수가 -1일때 -만 적기
	// 조건 4 지수가 0이면 생략

	for (int i = 0; i < p.terms; i++) {
		if (i != 0) {
			if (p.termArray[i].exp != 0)
			{
				if (p.termArray[i].coef > 1)
					os << "+" << p.termArray[i].coef << "x^" << p.termArray[i].exp;
				else if (p.termArray[i].coef == 1) // 1일때 생략
					os << "+x^" << p.termArray[i].exp;
				else if (p.termArray[i].coef == -1) // -1일때 생략
					os << "-x^" << p.termArray[i].exp;
				else if (p.termArray[i].coef < -1)
					os << p.termArray[i].coef << "x^" << p.termArray[i].exp;
			}
			else
			{
				if (p.termArray[i].coef < 0)
					os << p.termArray[i].coef;
				else
					os << "+" << p.termArray[i].coef;
			}
		}
		else
			if (p.termArray[i].exp != 0)
			{
				if (p.termArray[i].coef > 1)
					os << p.termArray[i].coef << "x^" << p.termArray[i].exp;
				else if (p.termArray[i].coef == 1) // 1일때 생략
					os << "x^" << p.termArray[i].exp;
				else if (p.termArray[i].coef == -1) // -1일때 생략
					os << "-x^" << p.termArray[i].exp;
				else if (p.termArray[i].coef < -1)
					os << p.termArray[i].coef << "x^" << p.termArray[i].exp;
			}
			else
			{
				if (p.termArray[i].coef < 0)
					os << p.termArray[i].coef;
				else
					os << p.termArray[i].coef;
			}
	}
	
	os << endl;
	return os;

}

Polynomial::Polynomial():capacity(1), terms(0)
{
    termArray = new Term[capacity];
}

void Polynomial::NewTerm(const float theCoeff, const int theExp){
    // 다항식 뒤에 새로운 항을 추가하는 함수 교재참고
	if (terms == capacity) {
        capacity *= 2;
        Term * temp = new Term[capacity];
        copy(termArray, termArray+terms, temp);
        delete [] termArray;
        termArray = temp;
    }
    termArray[terms].coef = theCoeff;
    termArray[terms].exp = theExp;
	terms++;
}

Polynomial Polynomial::operator+(Polynomial& b)
{
    Polynomial c;
	int aPos = 0, bPos = 0;
	while ((aPos < terms) && (bPos < b.terms))
	{
		if (termArray[aPos].exp == b.termArray[bPos].exp)
		{
			float t = termArray[aPos].coef + b.termArray[bPos].coef;
			if(t)
				c.NewTerm(t, termArray[aPos].exp);
			aPos++;
			bPos++;
		}
		else if ((termArray[aPos].exp < b.termArray[bPos].exp))
		{
			c.NewTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
			bPos++;
		}
		else
		{
			c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
			aPos++;
		}
	}
	for (; aPos < terms; aPos++)
		c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
	for (; bPos < b.terms; bPos++)
		c.NewTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);

	return c;
}
