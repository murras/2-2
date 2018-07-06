// B411063 김형석 2학년
#include <fstream>
#include <queue>
#include "mstree.h"
int NNODES; // #nodes in the graph is global

priority_queue<Edge, vector<Edge>, Compare> PQ;

void kruskal() {
	Sets sets(NNODES);
	int nedges = 0; // #edges found up now
	while (nedges < NNODES - 1) {
		if (PQ.empty())
			throw "No Spanning Tree Exists.";
		Edge e = PQ.top();
		PQ.pop();

		int v1root = sets.Find(e.v1);
		int v2root = sets.Find(e.v2);
		if (v1root != v2root) {
			sets.Union(v1root, v2root);
			nedges++;
			cout << e;
		}
	}
}

void ReadEdges4kruskal(istream& is) {
	Edge e;
	while (GetEdge(is, NNODES, e))
		PQ.push(e);
}

int main(int argc, char *argv[]) {
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
	try {
		ReadEdges4kruskal(is);
		kruskal();
	}
	catch (char const *str) {
		cerr << "Exception: " << str << endl;
		return 1;
	}
}

