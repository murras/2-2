#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Circle {
	string name;
	int radius;
public:
	Circle(int radius, string name) {
		this->radius = radius;
		this->name = name;
	}
	double getArea() {
		return 3.14*radius*radius;
	}
	string getName() {
		return name;
	}
};

void vect_circle() {
	vector<Circle*> vec;
	int n;
	int r;
	string s;
	vector<Circle*>::iterator iter;
	cout << "원을 삽입하고 삭제하는 프로그램입니다." << endl;
	while (true)
	{	
		
		cout << "삽입:1, 삭제:2, 모두 보기:3, 종료:4 >> ";
		cin >> n;
		if (n == 1) {
			cout << "생성하고자 하는 원의 반지름과 이름은 >> ";
			cin >> r >> s;
			vec.push_back(new Circle(r, s));
		}
		else if (n == 2) {
			cout << "삭제하고자 하는 원의 이름은 >> ";
			cin >> s;
			iter = vec.begin();
			while (iter != vec.end()) {
				Circle *p = *iter;
				if (p->getName() == s)
				{
					iter = vec.erase(iter);
					delete p;
				}
				else iter++;
			}		
		}
		else if (n == 3) {
			iter = vec.begin();
			while (iter != vec.end()) {
				Circle* p = *iter;
				cout << p->getName() << endl;
				iter++;
			}
		}	
		else if (n == 4)
			break;
	}
	

}

int main() {
	vect_circle();
}