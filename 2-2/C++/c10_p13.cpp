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
	cout << "���� �����ϰ� �����ϴ� ���α׷��Դϴ�." << endl;
	while (true)
	{	
		
		cout << "����:1, ����:2, ��� ����:3, ����:4 >> ";
		cin >> n;
		if (n == 1) {
			cout << "�����ϰ��� �ϴ� ���� �������� �̸��� >> ";
			cin >> r >> s;
			vec.push_back(new Circle(r, s));
		}
		else if (n == 2) {
			cout << "�����ϰ��� �ϴ� ���� �̸��� >> ";
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