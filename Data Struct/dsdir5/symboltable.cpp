// B411063 2학년 김형석
#include <iostream>
#include <map>
#include <cstring>
using namespace std;

struct cmp_str {
	bool operator()(char const *a,char const *b) const {
		return strcmp(a, b) < 0;
	}
};

map<char*, float, cmp_str> ST; // Symbol Table


void PrintValue(char *name) {
	map<char*, float, cmp_str>::iterator iter;
	iter = ST.find(name);
	if (iter == ST.end())
		cout << name << " does not exist" << endl;
	else
		cout << iter->first << " " << iter->second << endl;
}

void StoreIntoST(char *name, float val) {
	map<char*, float, cmp_str>::iterator iter;
	iter = ST.find(name);
	if (iter != ST.end())
		ST.erase(name); // 기존의 저장을 제거
	ST[name] = val;
	// ST.insert(pair<char *, float>(name, val)) 다른 저장 방법
}

int main() {
	StoreIntoST((char *)"abro", 23);
	StoreIntoST((char *)"bb", 34.5);
	StoreIntoST((char *)"haha", 99);
	StoreIntoST((char *)"bb", 1234.5);

	PrintValue((char *)"bb");
	PrintValue((char *)"haha");
	PrintValue((char *)"hoho");
}
