#include <iostream>
#include "list.h"
int main(){
    IntList il;
    il.Push_Back(5);
    il.Push_Back(7);
    il.Push_Back(9);
    cout << il;
    // 5 7 9 넣고 출력
    il.Push_Front(4);
    il.Push_Front(3);
    cout << il;
    // 앞에 4 3 넣고 출력
	il.Insert(6);
	cout << il;
    il.Insert(10);
	il.Insert(2);
	il.Insert(5);
	il.Insert(2);
	il.Insert(10);
    while(!il.IsEmpty()){
        cout << il.Front() << " ";
        il.Pop_Front();
        // cout << il 대신 사용
    }
    cout << endl;
}
