// 알고리즘 hw1_2.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include <string.h>
#include <cstring>
#include <fstream>
#include <iostream>
using namespace std;

struct Node {
    int leftchild;
    int rightchild;
    int level;
};

class Tree {
   private:
    Node* TreeNode;

   public:
    Tree(int n) {
        TreeNode = new Node[n];

        for (int i = 0; i < n; i++) {
            TreeNode[i].level = 1;
        }
    }

    void setValue(int num1, int num2, int num3) {
        TreeNode[num1].leftchild = num2;   // num1 좌측값
        TreeNode[num1].rightchild = num3;  // num2 우측값

        if (num2 != -1) {
            TreeNode[num2].level = TreeNode[num1].level + 1;
        }
        if (num3 != -1) {
            TreeNode[num3].level = TreeNode[num1].level + 1;
        }
    }

    ~Tree() { delete[] TreeNode; }

    int getLevel(int n) { return TreeNode[n].level; }
};

int main(void) {
    ifstream fin("input.txt");
    if (!fin) {
        cout << "파일 열기 에러" << endl;
        return 1;
    }

    int num;
    fin >> num;

    for (int i = 0; i < num; i++) {
        int num2;
        fin >> num2;

        Tree tr(num2 + 1);  // 트리 생성

        for (int n = 1; n <= num2; n++) {
            int val1, val2, val3;
            fin >> val1 >> val2 >> val3;
            tr.setValue(val1, val2, val3);
        }

        int level[1001];

        memset(level, 0, 1001);
        for (int n = 1; n <= num2; n++) {
            int k = tr.getLevel(n);  // 배열 level에 접근
            level[k] += 1;  // 해당되는 level값을 하나 올려준다.
        }

        int max = level[1];
        for (int n = 1; n < 1001; n++) {
            if (level[n] > max) {
                max = level[n];
            }
        }
        cout << "최대 리프 개수 = " << max << endl;
    }

    return 0;
}