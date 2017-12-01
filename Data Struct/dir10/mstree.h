// B411063 김형석 2학년
#ifndef MSTREE_H
#define MSTREE_H
#include <iostream>
using namespace std;

class Sets {
public:
	Sets(int);
	void Union(int i, int j);
	int Find(int i);
	//void print();
private:
	int *parent;
	int n;
};

struct Edge {
	int v1, v2;
	double weight;
	bool operator==(Edge& e2);
	bool operator!=(Edge& e2);
};

struct Compare {
	bool operator()(Edge e1, Edge e2) {
		return e1.weight > e2.weight;
	}
};

ostream& operator<<(ostream& os, Edge& e);
bool GetEdge(istream& is, const int numnodes, Edge& e);
#endif
