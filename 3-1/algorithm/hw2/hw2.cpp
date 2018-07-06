#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
using namespace std;
char ex[] = "Exchange Sort";  // 13
char qu[] = "Quick Sort";     // 10
char me[] = "Merge Sort";     // 10
char kc[] = "Key Compares";   // 12
char et[] = "Exe.Time";       // 8
char sp[] = "             ";  // 13

unsigned long long key_compare; // 비교횟수 count
unsigned long long result[2][3];  // 1번 key_compare 저장
unsigned long long result2[2][3];  // 2번 key_compare 저장
double ti[2][3];   // 1번 시간 저장
double ti2[2][3];   // 2번 시간 저장

int in[3];         // 1번 N 값 저장
int in2[3];         // 2번 N 값 저장

void init() { key_compare = 0; }
void exchange_sort(unsigned long long * v, int size);
void merge(unsigned long long * v, int low, int mid, int high);
void mergesort(unsigned long long * v, int low, int high);
void quick_sort(unsigned long long *v, int left, int right);

int main() {
	int n;
	ifstream ifs("input.txt");

	// 1번
	// Exchange Sort, Quick Sort 비교, 오름차순 정렬
	for (int i = 0; i < 3; i++) {
		ifs >> n;
		in[i] = n;

		unsigned long long * ev = new unsigned long long[n];
		unsigned long long * qv = new unsigned long long[n];
		for (int j = 0; j < n; j++) {
			ev[j] = qv[j] = j;
		}
		// Exchange Sort
		init(); // key_compare 초기화
		clock_t t = clock();
		exchange_sort(ev, n);
		t = clock() - t; // 시간 저장
		unsigned long long es_key = key_compare;
		double es_time = (double)(t) / CLOCKS_PER_SEC;
		// Quick Sort
		init(); // key compare 초기화
		t = clock();
		quick_sort(qv, 0, n - 1);
		t = clock() - t; // 시간 저장
		unsigned long long qs_key = key_compare;
		double qs_time = (double)(t) / CLOCKS_PER_SEC;

		result[0][i] = es_key;
		result[1][i] = qs_key;
		ti[0][i] = es_time;
		ti[1][i] = qs_time;

		delete ev, qv;
	}

	// 2번
	// Merge Sort, Quick Sort 비교, 랜덤
	srand((unsigned int)time(0));
	for (int i = 0; i < 3; i++) {
		ifs >> n;
		in2[i] = n; // case N 저장

		unsigned long long * mv = new unsigned long long[n+1];
		unsigned long long * qv = new unsigned long long[n+1];

		unsigned long long kc_ms = 0;     // key compares of merge
		unsigned long long kc_qs = 0;     // key compares of quick
		double et_ms = 0;  // ex.time of merge
		double et_qs = 0;  // ex.time of quick

		// Random Number
		for (int k = 0; k < 5; k++) {
			for (int j = 0; j < n; j++) {
				unsigned long long push_num = (unsigned long long)rand() % RAND_MAX;
				mv[j] = qv[j] = push_num;
			}

			// mergesort
			init();  // key_compare 초기화
			clock_t t = clock();
			mergesort(mv, 0, n - 1);  // 실행
			t = clock() - t;
			kc_ms += key_compare;
			et_ms += (double)(t) / CLOCKS_PER_SEC;  // 경과 시간 저장

			// quicksort
			init();  // key_compare 초기화
			t = clock();
			quick_sort(qv, 0, n - 1);  // 실행
			t = clock() - t;
			kc_qs = key_compare;
			et_qs = (double)(t) / CLOCKS_PER_SEC;  // 경과 시간 저장
			
			
		}
		
		kc_ms /= 5;
		kc_qs /= 5;
		et_ms /= 5.0;
		et_qs /= 5.0;

		result2[0][i] = kc_ms;
		result2[1][i] = kc_qs;
		ti2[0][i] = et_ms;
		ti2[1][i] = et_qs;

		delete mv, qv;
	}
	//									N = Case1	N = Case 2	N = Case 3
	printf("%-14s\t %14s\t N=%-10d\t N=%-10d\t N=%-10d\n", sp, sp, in[0], in[1], in[2]);
	// Exchange Sort	Key Compares
	//					Exe.Time
	printf("%-14s\t %-14s\t %-12llu\t %-12llu\t %-12llu\t\t\n", ex, kc, result[0][0], result[0][1], result[0][2]);
	printf("%-14s\t %-14s\t %-12lf\t %-12lf\t %-12lf\t\t\n", sp, et, ti[0][0], ti[0][1], ti[0][2]);
	// Quick Sort	Key Compares
	//				Exe.Time
	printf("%-14s\t %-14s\t %-12llu\t %-12llu\t %-12llu\t\t\n", qu, kc, result[1][0], result[1][1], result[1][2]);
	printf("%-14s\t %-14s\t %-12lf\t %-12lf\t %-12lf\t\t\n", sp, et, ti[1][0], ti[1][1], ti[1][2]);
	printf("\n\n");

	//									N = Case1	N = Case 2	N = Case 3
	printf("%-14s\t %14s\t N=%-10d\t N=%-10d\t N=%-10d\n", sp, sp, in2[0], in2[1], in2[2]);
	// Merge Sort	Key Compares
	//					Exe.Time
	printf("%-14s\t %-14s\t %-12llu\t %-12llu\t %-12llu\t\t\n", me, kc, result2[0][0], result2[0][1], result2[0][2]);
	printf("%-14s\t %-14s\t %-12lf\t %-12lf\t %-12lf\t\t\n", sp, et, ti2[0][0], ti2[0][1], ti2[0][2]);
	// Quick Sort	Key Compares
	//				Exe.Time
	printf("%-14s\t %-14s\t %-12llu\t %-12llu\t %-12llu\t\t\n", qu, kc, result2[1][0],	result2[1][1], result2[1][2]);
	printf("%-14s\t %-14s\t %-12lf\t %-12lf\t %-12lf\t\t\n", sp, et, ti2[1][0],	ti2[1][1], ti2[1][2]);
}

void exchange_sort(unsigned long long * v, int size) {
	for (int i = 0; i < size - 1; i++) {
		for (int j = i + 1; j < size; j++) {
			key_compare++;
			if (v[i] > v[j]) {
				unsigned long long temp = v[i];
				v[i] = v[j];
				v[j] = temp;
			}
		}
	}
}

void merge(unsigned long long * v, int low, int mid, int high) {
	int i, j, k;
	i = low;      // 왼쪽 시작
	j = mid + 1;  // 오른쪽 시작
	k = low;      // 결과 배열 인덱스
	unsigned long long *temp = new unsigned long long[high + 1];
	while (i <= mid && j <= high) {
		if (v[i] < v[j]) {
			key_compare++;
			temp[k] = v[i];
			i++;
		}
		else {
			key_compare++;
			temp[k] = v[j];
			j++;
		}
		k++;
	}
	while (i <= mid) {
		key_compare++;
		temp[k] = v[i];
		i++;
		k++;
	}

	while (j <= high) {
		key_compare++;
		temp[k] = v[j];
		j++;
		k++;
	}
	for (int i = low; i <= high; i++) {
		v[i] = temp[i];
	}
	delete temp;
}
void mergesort(unsigned long long * v, int low, int high) {
	int mid;
	if (low < high) {
		mid = (low + high) / 2;
		mergesort(v, low, mid);
		mergesort(v, mid + 1, high);
		merge(v, low, mid, high);
	}
}

void quick_sort(unsigned long long * v, int left, int right) {
	int i = left, j = right;
	unsigned long long pivot = v[left];
	do {
		while (v[i] < pivot) {
			key_compare++;
			i++;
		}
		while (v[j] > pivot) {
			key_compare++;
			j--;
		}
		if (i <= j) {
			if (v[i] > v[j]) {
				unsigned long long temp = v[i];
				v[i] = v[j];
				v[j] = temp;
			}
			i++;
			j--;
		}
	} while (i <= j);
	if (left < j) {
		quick_sort(v, left, j);
	}
	if (i < right) {
		quick_sort(v, i, right);
	}
}
