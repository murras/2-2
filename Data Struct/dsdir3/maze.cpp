#include <iostream>
#include <stack>
using namespace std;
const int MAXSIZE = 100; // 100 BY 100 까지 허용
bool maze[MAXSIZE + 2][MAXSIZE + 2] = { 0 };
bool mark[MAXSIZE + 1][MAXSIZE + 1] = { 0 };
static int solcount;
enum directions { N, NE, E, SE, S, SW, W, NW };
struct offsets {
	int a, b;
} _move[8] = { {-1,0}, {-1,1}, {0,1}, {1,1}, {1,0}, {1,-1}, {0,-1}, {-1,-1} };

struct Items {
	Items(int xx = 0, int yy = 0, int dd = 0) : x(xx), y(yy), dir(dd) {}
	int x, y, dir;
};

template<class T>
ostream& operator<< (ostream& os, stack<T>& s) {
	// 스택 내용 역순으로 출력하기 top pop push 사용, 임시스택 사용
	stack<T> temp;
	
	while (!s.empty()) {
		temp.push(s.top());
		s.pop();
	}

	while (!temp.empty()) {
		os << " -> " << temp.top();
		s.push(temp.top());
		temp.pop();
	}
	return os;
}

ostream& operator<< (ostream& os, Items& item) {
	static int count = 0;
	os << "(" << item.x << "," << item.y << ")";
	count++;
	if ((count % 5) == 0)
		cout << endl;

	return os;
}

void Path(const int m, const int p) {
	mark[1][1] = 1;
	stack<Items> stack;
	Items temp(1, 1, E);
	stack.push(temp);

	while (!stack.empty()) {
		temp = stack.top();
		stack.pop();
		int i = temp.x;
		int j = temp.y;
		int d = temp.dir;

		while (d < 8) {
			int g = i + _move[d].a;
			int h = j + _move[d].b;

			if ((g == m) && (h == p)) // 출구에 도착
			{
				cout << stack;
				temp.x = i; temp.y = j;
				cout << " -> " << temp;

				temp.x = m; temp.y = p;
				cout << " -> " << temp << endl;

				cout << "#nodes visited = " << solcount + 2 << " out of " << m*p << endl;
				return;
			}


			if ((!maze[g][h]) && (!mark[g][h])) {
				mark[g][h] = 1;
				temp.x = i;
				temp.y = j;
				temp.dir = d + 1;
				stack.push(temp);
				solcount++;
				i = g;
				j = h;
				d = N;
			}
			else d++;
		}

		
	}

	cout << "no path in maze" << endl;
}

void getdata(istream& is, int& m, int& p) {
	is >> m >> p;
	for (int i = 0; i < m + 2; i++) {
		maze[i][0] = 1;
		maze[i][p + 1] = 1;
	}
	for (int j = 1; j <= p; j++) {
		maze[0][j] = 1;
		maze[m + 1][j] = 1;
	}
	for (int i = 1; i <= m; i++)
		for (int j = 1; j <= p; j++)
			is >> maze[i][j];
}
