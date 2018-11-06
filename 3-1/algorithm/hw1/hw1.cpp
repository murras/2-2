#include <fstream>
#include <iostream>
using namespace std;
int tree[1001][2];  // 트리
int level[1001];    // 노드의 레벨
int cnt[1001];      // 각 레벨의 노드 갯수
int main() {
    int n;
    ifstream ifs("input.txt");
    ifs >> n;                              // 테스트 케이스의 개수 입력 받기
    while (n--) {                          // n번 반복
        for (int i = 0; i <= 1001; i++) {  // 0으로 초기화
            tree[i][0] = 0;
            tree[i][1] = 0;
            level[i] = 0;
            cnt[i] = 0;
        }
        level[1] = 1;  // 루트의 레벨은 1
        int node;
        ifs >> node;                      // 노드의 개수 입력 받기
        for (int i = 0; i < node; i++) {  // 노드 개수만큼 반복
            // 부모와 왼쪽, 오른쪽 자식을 입력 받는다.
            int parent, child1, child2;
            ifs >> parent >> child1 >> child2;
            // -1이 아니면 부모의 레벨+1을 자식 레벨에 넣는다.
            if (child1 != -1) {
                tree[parent][0] = child1;
                level[child1] = level[parent] + 1;
            } else {  // -1이면 empty에 1을 추가한다.
                tree[parent][0] = child1;
            }
            // -1이 아니면 부모의 레벨+1을 자식 레벨에 넣는다.
            if (child2 != -1) {
                tree[parent][1] = child2;
                level[child2] = level[parent] + 1;
            } else {  // -1이면 empty에 1을 추가한다.
                tree[parent][1] = child2;
            }
        }
        int max_level = 0;  // 현재 트리가 몇 레벨까지 있는지 저장.
        // 각 레벨별 노드 갯수를 cnt에 저장한다.
        for (int i = 1; i <= node; i++) {
            cnt[level[i]]++;
            if (max_level < level[i]) max_level = level[i];
        }
        int result = 0;
        // 노드가 가장 많은 레벨을 찾는다. 그 때의 레벨을 result에 저장.
        for (int i = 1; i <= max_level; i++) {
            if (result < cnt[i]) {
                result = i;
            }
        }
        // 노드가 가장 많은 레벨을 출력한다.
        cout << result << '\n';
    }
}