//B411063 김형석 2학년
#include <fstream>
#include <queue>
#include <algorithm>
#include "mstree.h"
using namespace std;
int NNODES;

priority_queue<Edge, vector<Edge>, Compare> * PQ;

void sollin()
{
	// 각 단계에서 여려개의 간선을 선택한다.
	// 한 단계의 시작점에서 선택된 간선들은 n개의 모든 그래프 정점들을 포함한 신장 포리스트가 된다.
	// 각 단계에서는 포리스트에 있는 각 트리에 대해 하나의 간선을 선택한다.
	// 이 간선은 오직 하나의 정점만 그 트리에 속한 최소비용 간선이다.
	Sets sets(NNODES);
	int nedges = 0;


	int i = 0;
	while (i < 7)  // 각 단계에서 여려개의 간선을 선택해 신장 포리스트를 만든다.
	{
		if (!PQ[i].empty()) // 우선순위 큐의 원소가 있으면
		{
			Edge e = PQ[i].top();
			PQ[i].pop();

			int v1root = sets.Find(e.v1);
			int v2root = sets.Find(e.v2);

			if (v1root != v2root)
			{
				sets.Union(v1root, v2root);
				sets.Union(v1root, v2root);
				nedges++;
				cout << e;

			}  // if (v1root != v2root)의 끝
		}  // if(!PQ[i].empty())의 끝
		i++;
	}


	priority_queue<Edge, vector<Edge>, Compare> tmp;  // 나머지 간선들을 정렬하기 위한 우선순위 큐

	while (nedges < NNODES - 1)  // 나머지 간선을 정렬하기
	{
		int i = 0;

		while (i < 7)
		{
			if (!PQ[i].empty()) // 우선순위 큐의 원소가 있으면
			{
				Edge e = PQ[i].top();
				PQ[i].pop();
				tmp.push(e);
			}
			i++;
		}


		for (int j = 0; j < tmp.size(); j++)  // 신장 포리스트 트리를 만들기
		{
			Edge e = tmp.top();
			tmp.pop();
			int v1root = sets.Find(e.v1);
			int v2root = sets.Find(e.v2);
			if (v1root != v2root)
			{
				sets.Union(v1root, v2root);
				sets.Union(v1root, v2root);
				nedges++;
				cout << e;

			}  // if (v1root != v2root)의 끝
		}  // if(!PQ[i].empty())의 끝
	}
}


void ReadEdges4sollin(istream& is)
{
	PQ = new priority_queue<Edge, vector<Edge>, Compare >[NNODES];
	Edge e;

	while (GetEdge(is, NNODES, e))	{
		PQ[e.v1].push(e);
		PQ[e.v2].push(e);
	}
}

//void sollin() {
//	Sets sets(NNODES);
//	Edge e;
//	for (int i = 0; i < NNODES; i++) {
//		e = PQ[i].top();
//		PQ[i].pop();
//		int root1 = sets.Find(e.v1);
//		int root2 = sets.Find(e.v2);
//		if (root1 != root2) {
//			sets.Union(root1, root2);
//			cout << e;
//		}
//	}
//	
//	for (int i = 0; i < NNODES; i++) {
//		int check = sets.Find(i);
//		while (!PQ[check].empty()) {
//			e = PQ[check].top();
//			PQ[check].pop();
//			PQ[i].push(e);
//		}
//		
//	}
//
//	for (int i = 0; i < NNODES; i++) {
//		if (!PQ[i].empty()) {
//			e = PQ[i].top();
//			PQ[i].pop();
//			int root1 = sets.Find(e.v1);
//			int root2 = sets.Find(e.v2);
//			if (root1 != root2) {
//				sets.Union(root1, root2);
//				cout << e;
//			}
//		}
//	}
//	
//}
//void ReadEdges4sollin(istream& is) {
//	PQ = new priority_queue<Edge, vector<Edge>, Compare>[NNODES];
//	Edge e;
//
//	while (GetEdge(is, NNODES, e)) {
//		PQ[e.v1].push(e);
//		PQ[e.v2].push(e);
//	}
//}

int main(int argc, char * argv[]) {
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
		ReadEdges4sollin(is);
		sollin();
	}
	catch (char const *str) {
		cerr << "Exception: " << str << endl;
		return 1;
	}
}
