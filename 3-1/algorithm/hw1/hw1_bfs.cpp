#include <fstream>
#include <iostream>
#include <queue>
using namespace std;
int level[1001];  //해당 노드의 레벨 저장
int tree[1001][2];
int cnt[1001];  //해당 레벨의 노드가 몇 개 인지 저장
void bfs(int n) {
    queue<int> q;
    q.push(1);
    level[1] = 1;
    cnt[1] = 1;
    while (!q.empty()) {
        int p = q.front();
        q.pop();
        int left = tree[p][0];
        int right = tree[p][1];
        if (left != -1) {
            q.push(left);
            level[left] = level[p] + 1;
            cnt[level[left]]++;
        }
        if (right != -1) {
            q.push(right);
            level[right] = level[p] + 1;
            cnt[level[right]]++;
        }
    }
}
int main() {
    int n;
    ifstream in("input.txt");
    in >> n;
    while (n--) {
        int node;
        in >> node;
        for (int i = 1; i <= node; i++) {
            int parent, leftChild, rightChild;
            in >> parent >> leftChild >> rightChild;
            level[parent] = 0;  //초기화
            cnt[parent] = 0;    //초기화
            tree[parent][0] = leftChild;
            tree[parent][1] = rightChild;
        }
        bfs(1);
        int pos = 1;  //가장 큰 자식이 있는 레벨
        for (int i = 2; i <= node; i++) {
            if (cnt[pos] < cnt[i])  //최댓값 탐색
                pos = i;
        }
        cout << pos << '\n';
    }
    return 0;
}