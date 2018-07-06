#include <iostream>
using namespace std;

template <class T>
void PrintArray(T *a, const int n){
    for(int i = 1; i <= n; i++)
        cout << a[i] << " ";
    cout << endl;
}

template <class T>
void BubbleSort1(T *a, int n){
        for(int i = n-1; i >= 1; i--)
            for(int j = 1; j <= i; j++)
            if(a[j] > a[j+1])
                swap(a[j], a[j+1]);
}

template <class T>
void BubbleSort2(T *a, int n) {
    int newn;
    while(n > 1) {
        newn = 1;
        for(int i = 1; i < n; i++)
            if(a[i] > a[i+1]){
                swap(a[i], a[i+1]);
                newn = i+1;
            }
        n = newn;
    }
}

int main(){
    int a[] = {0, 26, 5, 77, 1, 61, 11, 59, 15, 48, 19};
    int b[] = {0, 26, 5, 77, 1, 61, 11, 59, 15, 48, 19};

    int n = sizeof(a) / sizeof(int) - 1;
    cout << "Before Sorting : ";
    PrintArray(a, n);

    BubbleSort1(a, n);

    cout << "After Sorting : ";
    PrintArray(a, n);

    cout << "Before Sorting : ";
    PrintArray(b, n);

    BubbleSort2(b, n);

    cout << "After Sorting : ";
    PrintArray(b, n);
}
        
