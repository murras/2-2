#include <stdio.h>
#include <stdlib.h>
int main(void) {
	int n;
	float **t;
	float a, b, c;

	int i, j, k;
	printf("Matrix Size : ");
	scanf("%d", &n); // n input
	// n * n+1 matrix create
	t = (float **)malloc(sizeof(float)*n);
	for (i = 0; i < n; i++)
		t[i] = (float*)malloc(sizeof(float)*(n + 1));

	printf("input elements : "); // 
	for (i = 0; i < n; i++)
		for (j = 0; j < n + 1; j++)
			scanf(" %f", &t[i][j]);

	// t[k][k] 항을 1로 변환시키고 k열 밑에 0으로 만들기
	// 맨 밑행은 안돼서 따로해줘야한다.
	for (k = 0; k < n-1; k++) { // n-1
		c = t[k][k];
		for (j = 0; j < n+1; j++) { // n+1
			if (t[k][j] != 0)  // -0.00000으로 표기 방지
				t[k][j] /= c;
		}
		for (i = k; i < n-1; i++) { // n-1
			a = t[i + 1][k];
			b = t[k][k];
			for (j = k; j < n+1; j++) { // n+1
				t[i + 1][j] = t[i + 1][j] - ((t[k][j] * a) / b);
			}
		}

	}
	// 마지막행 1 만들기 위해서 나누기
	t[n-1][n] /= t[n-1][n-1];
	t[n-1][n-1] /= t[n-1][n-1];
	
	// 위에도 0으로 마저 만들기
	for (k = n-2; k >= 0; k--) {
		for (i = k; i >= 0; i--) {
			for (j = n; j > k; j--) {
				t[i][j] = t[i][j] - t[k + 1][j] * t[i][k + 1];
			}
		}
	}

	printf("output result : \n");
	for (i = 0; i < n; i++) { // 3-> n으로 수정
		for (j = 0; j < n + 1; j++)
			printf("%f ", t[i][j]);
		printf("\n");
	}

	//system("pause");
	for (int i = 0; i < n; i++) // 동적할당 받은거 반환하기
		free(t[i]);
	free(t);
}
