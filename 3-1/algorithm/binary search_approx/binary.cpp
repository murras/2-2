#include <cstdlib>
#include <iostream>
using namespace std;
int arr[1001];
int bin(int val, int low, int high) {
    int mid;
    while (low <= high) {
        mid = (low + high) / 2;
        if (arr[mid] == val) {
            return mid;
        } else if (val < arr[mid]) {
            high = mid - 1;
        } else if (val > arr[mid]) {
            low = mid + 1;
        }
    }
    if (abs(arr[low] - val) > abs(arr[high] - val))
        return high;
    else
        return low;
}

int main() {
    int n;
    cin >> n;
    int val;
    cin >> val;
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
        cout << arr[i] << ' ';
    }
    cout << '\n';
    int low = 0;
    int high = n - 1;
    int result = bin(val, low, high);
    cout << "index: " << result << '\n';
    cout << "arr[index]: " << arr[result] << '\n';
}
