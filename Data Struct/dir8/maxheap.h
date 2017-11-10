#include <iostream>
using namespace std;

template <class T>
void ChangeSize1D(T* &a, const int oldSize, const int newSize);

template <class T>
class Maxheap{
public:
    Maxheap(int);
    void Push(const T& e);
    void Pop();
    bool IsEmpty(){ return heapSize == 0; }
    T Top();
private:
    int heapSize;
    int capacity;
    T* heap;

    template <class T2>
    friend ostream& operator<<(ostream&, Maxheap<T2>&);
};

template <class T>
T Maxheap<T>::Top(){
    return heap[heapSize];
}
template <class T>
void ChangeSize1D(T*& a, const int oldSize, const int newSize)
{
    if(newSize <0) throw "new length must be >= 0";
    
    T* temp = new T[newSize];
    int number = min(oldSize, newSize);
    copy(a, a + number, temp);
    delete [] a;
    a = temp;
}

template <class T>
ostream& operator<<(ostream& os, Maxheap<T>& H){
    os << "<Heap Contents> ";
    for(int i=1; i<=H.heapSize; i++)
        os << i << ":" << H.heap[i] << " ";
    os << endl;
}

template <class T>
Maxheap<T>::Maxheap(int theCapacity = 10) : heapSize(0) {
    if(theCapacity < 1) throw "Must be >=1";
    capacity = theCapacity;
    heap = new T[capacity + 1];
} // heap[0] 사용 안함

template <class T>
void Maxheap<T>::Push(const T& e){
    // heap[0] 사용 안함
    // ChangeSize1D(heap, capacity + 1, 2*capacity + 1);
    if(heapSize==capacity){
        ChangeSize1D(heap, capacity+1, 2*capacity + 1);
        capacity *= 2;
    }
    int currentNode = ++heapSize;
    while(currentNode != 1 && heap[currentNode/2]<e){ 
        heap[currentNode] = heap[currentNode/2];    // 부모 노드로 이동
        currentNode /= 2;
    }
    heap[currentNode] = e;
}

template <class T>
void Maxheap<T>::Pop(){
    if(IsEmpty()) 
        throw "Heap is empty. Cannot delete.";
    heap[1].~T(); // 최대 원소 삭제
    
    // 히프의 마지막 원소 제거
    T lastE = heap[heapSize--];
    
    // trickle down
    int currentNode = 1; // 루트
    int child = 2; // currentNode의 자식
    while(child <= heapSize){
        // child를 currentNode의 큰 자식으로 설정
        if(child < heapSize && heap[child] < heap[child+1])
            child++;
        // currentNode에 lastE를 삽입할수 있는가?
        if(lastE >= heap[child])
            break; // yes
        
        //no
        heap[currentNode] = heap[child];
        currentNode = child;
        child *= 2;
    }
    heap[currentNode] = lastE;
}

