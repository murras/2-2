using namespace std;

struct Node{
    Node(int d=0, Node * l=0):data(d), link(l){ }
    int data;
    Node * link;
};

class IntList {
public:
    IntList() { last = first = 0; }
    bool IsEmpty() { return !first; }
    void Push_Back(int); // 리스트 뒤에 삽입

    int Front(); // 맨 앞의 값을 return
    void Pop_Front(); // 맨 앞 노드 제거
    void Push_Front(int); // 리스트 앞에 삽입
    void Insert(int); // 정렬되어 있는 리스트에 삽입

private:
    Node *first; // 첫노드를 가리킴
    Node *last; // 마지막 노드를 가리킴
    friend ostream& operator<<(ostream&, IntList&);
};

ostream& operator<<(ostream&, IntList&);

