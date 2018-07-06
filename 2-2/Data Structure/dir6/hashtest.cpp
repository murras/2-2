#include <iostream>
#include <list>
#include <cstring>
using namespace std;
#define HTSIZE 137 // 소수 사용 권장

struct Node {
    Node(char *n, float v) : name(n), val(v) {}
    char * name;
    float val;
};

list<Node> HT[HTSIZE];

int hashfcn(char *name) {
    return name[0] * name[strlen(name)-1] % HTSIZE;
}

bool WhereInHT(char * name, list<Node>::iterator& iter) {
    int hashval = hashfcn(name);
    for(iter=HT[hashval].begin(); iter!=HT[hashval].end(); iter++)
        if(!strcmp((*iter).name, name))
            return true;
    return false;
}

bool GetFromHT(char *name, float& val) {
    list<Node>::iterator iter;
    if(WhereInHT(name, iter)) {
        val = (*iter).val;
        return true;
    }
    return false;
}

void StoreIntoHT(char *name, float val) {
    // Hash Table에 name을 키로 val을 값으로 하는 노드를 저장하라
    list<Node>::iterator iter;
    if(WhereInHT(name, iter))
        (*iter).val = val;
    else
        HT[hashfcn(name)].push_front(Node(name,val));
}

void PrintValue(char *name){
    float val;
    if(!GetFromHT(name, val))
        cout << name << " does not exist" << endl;
    else
        cout << name << " " << val << endl;
}

int main() {
    StoreIntoHT((char*)"abro", 23);
    StoreIntoHT((char*)"bb", 34.5);
    StoreIntoHT((char*)"haha", 99);
    StoreIntoHT((char*)"bb", 1234.5); // bb 수정

    PrintValue((char*)"bb");
    PrintValue((char*)"haha");
    PrintValue((char*)"hoho");
}