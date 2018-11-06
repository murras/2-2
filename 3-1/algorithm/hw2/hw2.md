## 정렬 알고리즘의 실행시간 비교

- Exchange sort, Merge sort, Quick sort 알고리즘을 구현하기.
- 각각의 정렬 방법에서 key를 비교한 횟수와 실행시간을 측정하여 비교한다. 측정에는 clock 함수를 쓴다.
- Exchange sort와 Quick sort의 비교는 key의 갯수가 다른 3가지 오름차순으로 정렬된 데이터를 생성해여 비교한다.
- Merge sort와 Quick sort의 비교는 key의 갯수가 다른 3가지 경우에 대해 각각 5개의 임의의 데이터를 생성하여 평균값으로 비교한다.
- key의 갯수는 실행시간이 충분한 유효숫자를 가질 수 있도록 각자 정한다.
- 측정된 key의 비교횟수와 실행시간이 정렬 알고리즘들의 이론적 시간 복잡도와 일치하는지 분석한다.

- 입력 형식
  input.txt, 처음은 오름차순 데이터, 그 다음은 임의의 데이터가 입력된다.

- 출력 형식
  
    ![Image](https://i.imgur.com/Xd0eX8g.png)

- Exchange Sort : for문을 중첩하여 선택된 인덱스의 키와 나머지 키를 비교하여 선택된 키보다 작으면 앞으로 가게 한다.(인접한 원소끼리 비교하여 작은 게 앞으로 오게 한다.) 시간 복잡도가 O(N^2)이다.
- Merge Sort : 정렬되지 않은 배열을 반으로 나눈다. 길이가 1이 될 때 까지 나눈 후, 나눈 것을 다시 합친다. 합칠 때 작은 원소 순서대로 오게 한다. 시간 복잡도가 O(NlogN)이다.
- Quick Sort : 한 원소를 pivot으로 잡는다. 배열의 맨 왼쪽 원소를 left, 오른쪽 인덱스를 right라고 하고, left부터 오른쪽으로 옮겨가며 pivot보다 큰 값이 나올때까지, right부터 왼쪽으로 옮겨가며 pivot보다 작은 값이 나올때까지 이동한다. 두 원소를 교환한다. left가 right보다 오른쪽에 있으면 다시 왼쪽 오른쪽으로 나눠진 배열을 다시 퀵 정렬한다. 시간 복잡도가 최악의 경우에 O(N^2), 평균 O(NlogN)이 나온다.

### Exchange Sort와 Quick Sort의 비교 (오름차순으로 정렬된 데이터)
- N값(key의 개수)에 10000, 20000, 40000을 넣어 수행한다.
![Image](https://i.imgur.com/NndS4ss.png)
- Exchange sort의 경우 이론적 시간 복잡도가 O(N^2) 이므로 N이 2배 늘어날때마다 시간이 4배 늘어날 것으로 예상이 된다.
  
    N=10000 일때 0.09375, N=20000일 때 0.359375, N=40000일 때 1.515625가 걸렸다.

    0.09375 : 0.359375 = 1 : 3.83333 이므로 거의 1:4 이고, 0.09375 : 1.515625= 1:16.1666667 이므로 거의 1:16이다. 

    0.359375 : 1.515625= 1 : 4.217391 로 거의 1:4이다. 

    clock의 오차를 감안하면 exchange sort의 시간 복잡도가 O(N^2)인 것을 알 수 있다. 이론적 시간 복잡도와 일치한다.

    key 값의 비교 횟수도 역시 49995000: 199990000 : 799980000 = 1 : 4 : 16의 꼴을 띈다.

- Quick Sort는 정렬된 데이터가 들어갔을 때 최악의 경우로 O(N^2)이 걸린다. 
  
    그러므로 N값이 2배 늘어날때마다 시간이 4배 늘어날 것으로 예상이 된다.

    N=10000일 때 0.078125, N=20000일 때 0.328125, N=40000일 때 1.3125가 걸렸다.

    0.078125 : 0.328125 = 1 : 4.2 로 거의 1:4 이고, 0.078125 : 1.3125 = 1 : 16.8 로 거의 1:16이다. 

    0.328125 : 1.3125 = 1 : 4이다. 

    clock의 오차를 감안하면 Quick Sort는 정렬된 데이터가 들어갔을 때 최악으로 O(N^2)이 걸린다는 것을 알 수 있다.

    key 값의 비교 횟수도 역시 49995000: 199990000 : 799980000 = 1 : 4 : 16의 꼴을 띈다.

    (N)^2: (2N)^2 : (4N)^2 = 1 : 4 : 16

### Merge Sort와 Quick Sort의 비교 (무작위 데이터, 5번 수행 후 평균값)
- N값(key의 개수)이 1000000, 2000000, 4000000을 넣어 수행한다.
![Image](https://i.imgur.com/p80QVWi.png)

- Merge Sort의 경우 이론적 시간 복잡도가 O(NlogN)이다. N값이 2배 늘어날 때 마다 약 2.~배 정도 늘어날 것으로 예상이 된다.
    
    N=1000000일 때 0.23125, N=2000000일 때 0.48125, N=4000000일 때 0.95625가 걸렸다.

    0.23125 : 0.48125 = 1 : 2.081081, 0.23125 : 0.95625 = 1 : 4.135135, 0.48125 : 0.95625 = 1 : 1.987013 으로, clock의 오차를 감안하면 예상된 시간 복잡도와 비슷하다.

    key 값의 비교 횟수는 19951424 : 41902848 : 87805696 = 1 : 2.1 : 4.4 로 2배 보다 약간 더 늘어났다.

- Quick Sort의 경우 평균적으로 O(NlogN)의 시간 복잡도를 가진다. N값이 2배 늘어날때마다 2.~배 늘어날 것으로 예상이 된다.

    N=1000000일 때 0.031250, N=2000000일 때 0.065625, N=4000000일 때 0.131250가 걸렸다.

    0.031250 : 0.065625 = 1 : 2.1, 0.065625 : 0.131250 = 1 : 2, 0.031250 : 0.13125 = 1 : 4.2 이므로 clock의 오차와 평균을 구할 때 5번 밖에 수행하지 않아 오차가 클 수 있다는 점을 감안하면 예상된 시간 복잡도와 일치한다.

    key 값의 비교 횟수는 4088134 : 8475511 : 17790888 = 1 : 2.07319 : 4.351835로 N값이 2배 늘어날 때마다 2배보다 약간 더 늘어났다.

```cpp
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
```