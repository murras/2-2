## 이진트리에서 노드의 수가 가장 많은 레벨 찾기

- 조건
    1. 루트의 레벨은 1로 한다
    2. 자식의 레벨은 부모의 레벨 + 1이다.
    3. 이진트리에서 같은 레벨에 있는 노드는 같은 행에 위치한다.

노드의 수가 가장 많은 레벨을 출력하는 프로그램을 작성하시오.

- 입력 형식
  입력 파일의 이름은 input.txt이다. 여러개의 테스트 데이터가 입력될 수 있다. 첫째 줄에는 테스트 데이터의 개수가 입력된다. 둘째 줄에는 첫번째 데이터의 노드의 개수를 나타내는 정수 N(1≤N≤1,000)이 주어진다. 다음 N개의 줄에는 각 줄마다 노드번호와 해당 노드의 왼쪽 자식 노드와 오른쪽 자식 노드의 번호가 순서대로 주어진다. 노드들의 번호는 1부터 N까지로 주어진다. 자식이 없는 경우에는 자식노드의 번호가 -1로 주어진다. 루트 노드의 번호는 1이다. 그 다음 줄부터 나머지 데이터가 같은방식으로 주어진다.

- Test Case
    - 입력
    ```c
    2
    19
    1 2 3
    2 4 5
    3 6 7
    4 8 -1
    5 9 10
    6 11 12
    7 13 -1
    8 -1 -1
    9 14 15
    10 -1 -1
    11 16 -1
    12 -1 -1
    13 17 -1
    14 -1 -1
    15 18 -1
    16 -1 -1
    17 -1 19
    18 -1 -1
    19 -1 -1
    3
    1 2 3
    2 -1 -1
    3 -1 -1
    ```
    - 출력
    ```c
    4
    2
    ```

데이터 노드의 개수가 1과 1000 사이의 정수이기 때문에 전역변수 tree[1001][2]로 잡는다([2]는 왼쪽, 오른쪽 자식). 

트리의 노드 별로 레벨을 저장하기 위해 level[1001]을 잡는다. cnt[1001]은 각 레벨의 노드 개수를 저장한다.

테스트 데이터의 개수를 입력 받는다. 

여러 번의 테스트를 하므로 tree, level, cnt 배열을 0으로 초기화한다. 

노드 1의 레벨을 1로 초기화한다. 노드의 개수를 입력 받는다.

node의 개수만큼 for문을 반복한다. 

for문에서는 parent, child1, child2 (부모, 왼쪽 자식, 오른쪽 자식)을 입력 받고 tree에 넣는다. 

만약 child1, child2가 -1이 아니면 level[child1], level[child2]를 level[parent]+1 값을 넣는다.

현재 트리의 깊이를 저장하기 위한 max_level을 선언하고 0으로 초기화한다.

node 개수만큼 for문을 반복한다. 노드가 i 레벨일때마다 cnt[i]를 1씩 올린다. 

level[i]이 max_level보다 높아지면 max_level에 level[i]를 넣는다.(각 레벨별 노드 개수를 cnt에 저장한다.)

max_level만큼 for문을 반복한다. cnt를 순회하면서 가장 큰 값을 result에 저장한다.

result를 출력하고 다시 반복한다.

이 알고리즘의 시간 복잡도는 한 테스트 케이스당 O(n)이다.(n: 노드의 개수)

```cpp
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
```

**코드는 참고만 해주세요.**