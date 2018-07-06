// B411063 김형석
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
using namespace std;
double dp[101][101];
double p[101];
int order[101][101];

double psum(int, int);  // 확률 합
void preorder(int, int); // 전위순회로 출력하기
int main() {
	ifstream fin("input.txt"); // 파일 입력 받기
	int test;
	fin >> test;  // test case 횟수 입력 받기
	while (test--) { // test case만큼 반복해서 수행한다.
		memset(dp, 0, sizeof(dp));
		memset(p, 0, sizeof(p));
		memset(order, 0, sizeof(order));

		int key;
		fin >> key;  // 데이터 key의 수를 입력받는다
		double itoj = 0;  // 확률을 구하기 위해 전체 합을 저장하기 위한 변수
		for (int i = 1; i <= key; i++) {
			fin >> p[i];
			itoj += p[i];
		}
		for (int i = 1; i <= key; i++) {
			dp[i][i] = p[i] / itoj;  // dp[i][i]에는 i 확률을 넣는다.
			dp[i][i - 1] = 0;             // 0으로 초기화.
			order[i][i] = i;              // 순서 역시 초기화 해준다.
			order[i][i - 1] = 0;		  // 쓰지 않는 항 0으로 초기화하기.
		}
		dp[key + 1][key] = 0; // 쓰지 않는 항 0으로 초기화하기.
		order[key + 1][key] = 0;

		for (int diagonal = 1; diagonal <= key - 1; diagonal++) {
			for (int i = 1; i <= key - diagonal; i++) {
				int j = diagonal + i;
				double mini = dp[i][i - 1] + dp[i + 1][j] + psum(i, j);
				// min 보다 작거나 같으면 i 더해서 저장하고
				// 순서도 저장하고 min 바꿔준다.
				for (int k = i; k <= j; k++) {
					if (dp[i][k - 1] + dp[k + 1][j] + psum(i, j) < mini) {
						order[i][j] = k;
						mini = dp[i][k - 1] + dp[k + 1][j] + psum(i, j);
					}
					if (order[i][j] == 0) {
						order[i][j] = order[i][j - 1];
					}
					dp[i][j] = mini;
				}
			}
		}
		preorder(1, key); // preorder로 출력하기.
		cout << '\n';
	}
}
double psum(int start, int end) { // start ~ end (i ~ j)까지의 확률 합 출력 함수
	double result = 0;
	for (int i = start; i <= end; i++)
		result += dp[i][i];
	return result;
}
void preorder(int start, int end) { // 전위순회로 출력하기.
	int k = order[start][end];
	cout << k << ' '; // 전위순회이므로 먼저 출력하고
	if (start <= k - 1) { // 범위를 벗어나는지 확인.
		preorder(start, k - 1); // 왼쪽 방문.
	}
	if (end >= k + 1) { // 범위를 벗어나는지 확인.
		preorder(k + 1, end); // 오른쪽 방문.
	}
}