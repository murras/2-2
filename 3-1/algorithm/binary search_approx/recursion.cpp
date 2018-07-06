#include <cstdlib>
#include <iostream>
using namespace std;
int arr[1001];
int bin(int val, int low, int high) {
    int mid = (low + high) / 2;
    if (low > high) {
        return (abs(arr[high] - val) < abs(val - arr[low])) ? arr[high]
                                                            : arr[low];
    } else if (val < arr[mid]) {
        return bin(val, low, mid - 1);
    } else if (val > arr[mid]) {
        return bin(val, mid + 1, high);
    }
}

int main() {
    int n, val;
    cin >> n >> val;
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    int low = 0;
    int high = n - 1;
    cout << bin(val, low, high) << '\n';
}
