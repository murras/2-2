//B411063 김형석 2학년
#include <fstream>
#include <queue>
#include "mstree.h"
using namespace std;
int NNODES;
priority_queue<Edge, vector<Edge>, Compare> PQ;
queue<Edge>* Q;

void MoveIntoPQ_EdgeOfNode(int v) {
	// Move all edges of vertex 'v' into PQ
	while (!PQ.empty())
		PQ.pop();

	for (int i = 0; i < Q->size(); i++) {
		Edge e = Q->front();
		Q->pop();
		if (e.v1 == v || e.v2 == v) {
			PQ.push(e);
		}
		else
			Q->push(e);
	}
}

void prim() {
	Sets sets(NNODES);
	int nedges = 0;
	int n = 0;
	while (nedges < NNODES - 1) {
		if (PQ.empty())
			throw "No Spanning Tree Exists.";
		Edge e = PQ.top();
		PQ.pop();
		int root0 = sets.Find(0);
		// 현재 선택된 노드들의 루트를 구한다
		// 꺼낸 e가 자격이 있으면 출력하고 e의 새로운 vertex에 대해 처리한다
		cout << e;
		if (e.v1 == n) {
			n = e.v2;
		}
		else {
			n = e.v1;
		}
		MoveIntoPQ_EdgeOfNode(n);
		nedges++;
	}
}

void ReadEdges4prim(istream& is) {
	Q = new queue<Edge>[NNODES];
	Edge e;

	while (GetEdge(is, NNODES, e)) {
		//Q[e.v1]에 e를 넣고 Q[e.v2]에도 e를 넣는다.
		Q->push(e);
	}
	MoveIntoPQ_EdgeOfNode(0); // 0의 edge들을 PQ로 이동한다.
}

int main(int argc, char* argv[]) {
	ifstream is;
	if (argc == 1)
		is.open("mstree.dat");
	else
		is.open(argv[1]);

	if (!is) {
		cerr << "No such input file \n";
		return 1;
	}
	is >> NNODES;
	if (NNODES < 2) {
		cerr << "#nodes must be 2.." << endl;
		return 1;
	}
	//kruskal경우와 동일
	try {
		ReadEdges4prim(is);
		prim();
	}
	catch (char const *str) {
		cerr << "Exception: " << str << endl;
		return 1;
	}
}
