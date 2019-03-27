//
// Virual Memory Simulator Homework
// Two-level page table system
// Inverted page table with a hashing system
// Student Name: 
// Student Number: 
//
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define PAGESIZEBITS 12     // page size = 4Kbytes
#define VIRTUALADDRBITS 32  // virtual address space size = 4Gbytes

#define DEBUG 0  // 디버깅용 로그 출력 여부

#define PRINTWANT 1        // 제출할때 1로
#define MULTILEVEL_EXEC 1  // 제출할때 1로
#define INVERTED_EXEC 1    // 제출할때 1로

#define SECONDSYSTEM 0    // 주소 계산용 스위치
#define INVERTEDSYSTEM 1  // 주소 계산용 스위치

struct pageTableEntry {
    int level;                                    // page table level (1 or 2)
    char valid;                                   // 1일때 valid, 0일때 invalid
    struct pageTableEntry *secondLevelPageTable;  // valid if this entry is for the first level page table (level = 1)
    int frameNumber;                              // valid if this entry is for the second level page table (level = 2)
};

struct framePage {
    int number;                  // frame number
    int pid;                     // Process id that owns the frame
    int virtualPageNumber;       // virtual page number using the frame
    struct framePage *lruLeft;   // for LRU circular doubly linked list
    struct framePage *lruRight;  // for LRU circular doubly linked list
};

struct invertedPageTableEntry {
    int pid;                // process id
    int virtualPageNumber;  // virtual page number
    int frameNumber;        // frame number allocated
    struct invertedPageTableEntry *next;
};

struct procEntry {
    char *traceName;           // the memory trace name
    int pid;                   // process (trace) id
    int ntraces;               // the number of memory traces
    int num2ndLevelPageTable;  // The 2nd level page created(allocated);
    int numIHTConflictAccess;  // The number of Inverted Hash Table Conflict Accesses
    int numIHTNULLAccess;      // The number of Empty Inverted Hash Table Accesses
    int numIHTNonNULLAcess;    // The number of Non Empty Inverted Hash Table Accesses
    int numPageFault;          // The number of page faults
    int numPageHit;            // The number of page hits
    struct pageTableEntry *firstLevelPageTable;
    FILE *tracefp;
};

int global_argc;                    // argc 복사
char **global_argv;                 // argv 복사
unsigned int phyMemSizeBits;        // physical Memory Size Bits
unsigned int nProcess;              // Process의 수 (trace 파일 input 갯수)
unsigned int nFrame;                // Frame의 수(비트 연산으로 구해줘야한다)
unsigned int addr;                  // virtualAddress 변수 (trace로부터 입력)
char rw;                            // read or write (사실 이과제에선 쓸모없다)
unsigned int phyAddr;               // physical Address값
unsigned int offset;                // offset
bool *eofTrace;                     // 탈출을 위한 동적 할당해야함.
int findFrameNum;                   // FrameNum
unsigned int nowProcessID;          // 현재 돌아가는 Process ID,
unsigned int oldProcessID;          // pageFault시 저장되는 oldPID
unsigned int oldVirtualPageNumber;  // pageFault시 저장되는 oldVPN

// Two Level Page System
unsigned int firstLevelBits, secondLevelBits;    // First, Second Level Bit 수 입력받고
unsigned int nFirstTable, nSecondTable;          // first, secondTable의 전체 Entry 수
unsigned int firstTableIndex, secondTableIndex;  // first Table과 second Table의 index
// Inverted Page System
unsigned int hashTableIndex;     // HashTable Index
int virtualPN;                   // Virtual Page Number 변수
unsigned int oldHashTableIndex;  // 지워질 hashTable Index

struct framePage *oldestFrame;     // the oldest frame pointer
struct framePage *physicalMemory;  // physicalMemory
struct procEntry *procTable;       // process Table

struct invertedPageTableEntry *hashTable;              // hashTable next 이외에 다 -1로 초기화해서 쓰자.
struct invertedPageTableEntry *nNext;                  // 탐색용 포인터
struct invertedPageTableEntry *toDelete, *saveBefore;  // 지울 애들
struct invertedPageTableEntry **makeNext;              // 링크드 리스트 만들기 위해

void CheckSetPrint();         // Error 체크, 변수 설정, 기본 print
void PhysicalMemoryMalloc();  // Physical Memory Setting
void PhysicalMemoryFree();    // Physical Memory Free
void LRU();                   // LRU Setting
bool PageFault();             // PageFault 처리 함수, table에서 삭제해줘야하면 true를 반환한다.
void GetIndex(int);           // index 정보 얻기 (second system, inverted system 따로 구현)
void GetPhyAddr();            // physical address 얻기
void eofMalloc();             // 탈출 조건을 만들기 위한 bool 동적 배열 할당.
bool eofEscape();             // 탈출 조건 확인.

void SecondLvPageTableSystem();                        // SecondLvMemSim 실행
void SecondLv_ProcessMalloc();                         // SecondLv Process Malloc
void SecondLv_ProcessFree();                           // SecondLv Process 메모리 반환
void SecondLv_Simulation();                            // SecondLV의 main
void SecondLv_MallocSecond(struct pageTableEntry **);  // Second Table 할당
void SecondLv_GoSecondPT();                            // Second Table 접근
void SecondLv_PrintResult();                           // SecondLv 결과 Print

void InvertedPageTableSystem();   // InvertedPTSystem 실행
void Inverted_ProcessMalloc();    // Inverted Page Process Malloc
void Inverted_ProcessFree();      // Inverted Page Process 메모리 반환
void Inverted_HashTableMalloc();  // HashTable Malloc
void Inverted_HashTableFree();    // HashTable 메모리 반환
void Inverted_Simulation();       // Inverted PT System의 main
void Inverted_GoHashTable();      // hashTable 탐색
void Inverted_MappingDelete();    // hashTable 삭제
void Inverted_PrintResult();      // Invereted Page 결과 Print

int main(int argc, char *argv[]) {
    global_argc = argc;
    global_argv = argv;
    CheckSetPrint();
    if (MULTILEVEL_EXEC) SecondLvPageTableSystem();
    if (INVERTED_EXEC) InvertedPageTableSystem();
    return 0;
}

void SecondLvPageTableSystem() {
    printf("=============================================================\n");
    printf("The 2nd Level Page Table Memory Simulation Starts .....\n");
    printf("=============================================================\n");
    PhysicalMemoryMalloc();
    SecondLv_ProcessMalloc();
    SecondLv_Simulation();
    SecondLv_PrintResult();
    PhysicalMemoryFree();
    SecondLv_ProcessFree();
}  // complete

void SecondLv_ProcessMalloc() {
    procTable = (struct procEntry *)malloc(nProcess * sizeof(struct procEntry));
    int i;
    for (i = 0; i < nProcess; i++) {
        procTable[i].traceName = global_argv[i + 3];
        procTable[i].pid = i;
        procTable[i].ntraces = 0;
        procTable[i].num2ndLevelPageTable = 0;
        procTable[i].numPageFault = 0;
        procTable[i].numPageHit = 0;
        procTable[i].firstLevelPageTable = (struct pageTableEntry *)malloc(nFirstTable * sizeof(struct pageTableEntry));
        int j;
        for (j = 0; j < nFirstTable; ++j) {
            procTable[i].firstLevelPageTable[j].level = 1;
            procTable[i].firstLevelPageTable[j].valid = 1;
            procTable[i].firstLevelPageTable[j].secondLevelPageTable = NULL;
            procTable[i].firstLevelPageTable[j].frameNumber = -1;
        }
        procTable[i].tracefp = fopen(procTable[i].traceName, "r");
        if (procTable[i].tracefp == NULL) {
            printf("trace file input(name: %s) is wrong.\n", i + 1, procTable[i].traceName);
            exit(1);
        }
    }
}  // complete

void SecondLv_ProcessFree() {
    int i;
    for (i = 0; i < nProcess; ++i) {
        int j;
        for (j = 0; j < nFirstTable; ++j) {
            if (procTable[i].firstLevelPageTable[j].secondLevelPageTable == NULL) {
                continue;
            }
            free(procTable[i].firstLevelPageTable[j].secondLevelPageTable);
        }
        free(procTable[i].firstLevelPageTable);
        fclose(procTable[i].tracefp);
    }
    free(procTable);
}  // complete

void SecondLv_Simulation() {
    eofMalloc();
    // process마다 한 trace 읽기.
    while (true) {
        int i;
        for (i = 0; i < nProcess; ++i) {
            nowProcessID = i;
            // addr rw 값을 받는데, fscanf는 EOF시 -1 반환한다.
            // EOF면 이 프로세스는 더이상 살펴볼 필요가 없으니까
            // eofTrace를 true로 바꿔주고 다음부턴 살피지 않는다.
            // eof 이후에도 또 fscanf를 하면 아까우니까
            // eofTrace가 true이면 fscanf 하지않고 바로 넘어갈 수 있게끔 조건식을 정한다.
            if (!eofTrace[nowProcessID] && (fscanf(procTable[nowProcessID].tracefp, "%x %c", &addr, &rw) == -1)) {
                eofTrace[nowProcessID] = true;
            }
            // i번째 프로세스가 읽을 trace 파일이 이미 끝났으면 다음 프로세스를 살피자.
            if (eofTrace[nowProcessID]) {
                continue;
            }
            // trace++
            procTable[i].ntraces++;
            // Index 정보 얻어내기
            GetIndex(SECONDSYSTEM);

            // secondLvPT를 찾아가자.
            SecondLv_GoSecondPT();
            if (PRINTWANT) {
                printf("2Level procID %d traceNumber %d virtual addr %x pysical addr %x\n", nowProcessID, procTable[i].ntraces, addr, phyAddr);
            }
            if (DEBUG) {
                printf("\n");
            }
        }
        // 모두 eof를 만났으면일때 while문 탈출
        bool escape = eofEscape();
        if (escape) {
            break;
        }
    }
    free(eofTrace);
}  // complete

void eofMalloc() {
    // 탈출 조건을 만들기 위한 bool 동적 배열.
    eofTrace = (bool *)malloc(nProcess * sizeof(bool));
    int i;
    for (i = 0; i < nProcess; ++i) {
        eofTrace[i] = false;
    }
}  // complete

bool eofEscape() {
    int i;
    for (i = 0; i < nProcess; ++i) {
        if (eofTrace[i] == false) {
            return false;
        }
    }
    return true;
}  // complete

void SecondLv_MallocSecond(struct pageTableEntry **pTE) {
    *pTE = (struct pageTableEntry *)malloc(nSecondTable * sizeof(struct pageTableEntry));
    int i;
    for (i = 0; i < nSecondTable; ++i) {
        (*pTE)[i].level = 2;
        (*pTE)[i].valid = 0;
        (*pTE)[i].secondLevelPageTable = NULL;
        (*pTE)[i].frameNumber = -1;
    }
}  // complete

void SecondLv_GoSecondPT() {
    // 1) NULL이면 만들어주자.
    if (procTable[nowProcessID].firstLevelPageTable[firstTableIndex].secondLevelPageTable == NULL) {
        SecondLv_MallocSecond(&(procTable[nowProcessID].firstLevelPageTable[firstTableIndex].secondLevelPageTable));
        procTable[nowProcessID].num2ndLevelPageTable++;
    }
    // 2) valid일때
    if (procTable[nowProcessID].firstLevelPageTable[firstTableIndex].secondLevelPageTable[secondTableIndex].valid == 1) {
        if (DEBUG) {
            printf("valid\n");
        }
        // LRU 갱신.
        // Hit++
        findFrameNum = procTable[nowProcessID].firstLevelPageTable[firstTableIndex].secondLevelPageTable[secondTableIndex].frameNumber;
        LRU();
        procTable[nowProcessID].numPageHit++;
        GetPhyAddr();
    }
    // 3) invalid일때
    else {
        if (DEBUG) {
            printf("invalid\n");
        }
        // PageFault.
        // valid로 설정.
        // Fault++
        bool shouldDelete = false;
        shouldDelete = PageFault();
        procTable[nowProcessID].firstLevelPageTable[firstTableIndex].secondLevelPageTable[secondTableIndex].valid = 1;
        procTable[nowProcessID].firstLevelPageTable[firstTableIndex].secondLevelPageTable[secondTableIndex].frameNumber = findFrameNum;
        procTable[nowProcessID].numPageFault++;
        GetPhyAddr();

        if (shouldDelete) {
            unsigned int deleteFirstTableIndex, deleteSecondTableIndex;
            deleteFirstTableIndex = (oldVirtualPageNumber >> secondLevelBits);
            int j, mask = 1;
            for (j = 0; j < secondLevelBits - 1; ++j) {
                mask <<= 1;
                mask += 1;
            }
            deleteSecondTableIndex = (oldVirtualPageNumber & mask);
            if (DEBUG) {
                printf("deleteFirst : %d \n deleteSecond : %d\n", deleteFirstTableIndex, deleteSecondTableIndex);
            }
            if (procTable[oldProcessID].firstLevelPageTable[deleteFirstTableIndex].secondLevelPageTable == NULL) {
                if (DEBUG) {
                    printf("here error\n");
                }
            }
            procTable[oldProcessID].firstLevelPageTable[deleteFirstTableIndex].secondLevelPageTable[deleteSecondTableIndex].valid = 0;
        }
    }
}  // complete

void InvertedPageTableSystem() {
    printf("=============================================================\n");
    printf("The Inverted Page Table Memory Simulation Starts .....\n");
    printf("=============================================================\n");
    PhysicalMemoryMalloc();
    Inverted_ProcessMalloc();
    Inverted_HashTableMalloc();
    Inverted_Simulation();
    Inverted_PrintResult();
    PhysicalMemoryFree();
    // Inverted_ProcessFree();
    // Inverted_HashTableFree();
}  // complete

void Inverted_ProcessMalloc() {
    procTable = (struct procEntry *)malloc(nProcess * sizeof(struct procEntry));
    int i;
    for (i = 0; i < nProcess; ++i) {
        procTable[i].traceName = global_argv[i + 3];
        procTable[i].pid = i;
        procTable[i].ntraces = 0;
        procTable[i].num2ndLevelPageTable = 0;
        procTable[i].numIHTConflictAccess = 0;
        procTable[i].numIHTNULLAccess = 0;
        procTable[i].numIHTNonNULLAcess = 0;
        procTable[i].numPageFault = 0;
        procTable[i].numPageHit = 0;
        procTable[i].firstLevelPageTable = NULL;
        procTable[i].tracefp = fopen(procTable[i].traceName, "r");
        if (procTable[i].tracefp == NULL) {
            printf("trace file input(name: %s) is wrong.\n", i + 1, procTable[i].traceName);
            exit(1);
        }
    }
}  // complete

void Inverted_ProcessFree() {
    int i;
    for (i = 0; i < nProcess; ++i) {
        free(procTable[i].traceName);
        fclose(procTable[i].tracefp);
    }
    free(procTable);
}  // complete

void Inverted_HashTableMalloc() {
    int i;
    hashTable = (struct invertedPageTableEntry *)malloc(nFrame * sizeof(struct invertedPageTableEntry));
    for (i = 0; i < nFrame; ++i) {
        hashTable[i].pid = -1;
        hashTable[i].virtualPageNumber = -1;
        hashTable[i].frameNumber = -1;
        hashTable[i].next = NULL;
    }
}  // complete

void Inverted_HashTableFree() {
    int i;
    struct invertedPageTableEntry *freePT1, *freePT2;
    for (i = 0; i < nFrame; ++i) {
        if (hashTable[i].next != NULL) {
            freePT1 = freePT2 = hashTable[i].next;
            while (freePT2 != NULL) {
                freePT1 = freePT2;
                freePT2 = freePT2->next;
                free(freePT1);
            }
            free(freePT2);
        }
    }
    free(hashTable);
}  // complete

void Inverted_Simulation() {
    eofMalloc();
    // process마다 한 trace 읽기.
    while (true) {
        int i;
        for (i = 0; i < nProcess; ++i) {
            nowProcessID = i;
            if (!eofTrace[nowProcessID] && fscanf(procTable[nowProcessID].tracefp, "%x %c", &addr, &rw) == -1) {
                eofTrace[nowProcessID] = true;
            }
            // i번째 프로세스가 읽을 trace 파일이 이미 끝났으면 다음 프로세스를 살피자.
            if (eofTrace[nowProcessID]) {
                continue;
            }
            // trace++
            procTable[nowProcessID].ntraces++;
            GetIndex(INVERTEDSYSTEM);
            if (DEBUG) {
                printf("virtualPN : %x hashTableIndex : %x offset : %x \n", virtualPN, hashTableIndex, offset);
            }
            Inverted_GoHashTable();
            if (PRINTWANT) {
                printf("IHT procID %d traceNumber %d virtual addr %x pysical addr %x\n", i, procTable[i].ntraces, addr, phyAddr);
            }
            // hashTable 다 까보기.
            if (DEBUG) {
                int j;
                for (j = 0; j < nFrame; ++j) {
                    if (hashTable[j].next == NULL) {
                        continue;
                    }
                    struct invertedPageTableEntry *printTemp = hashTable[j].next;
                    while (printTemp != NULL) {
                        printf("hashtable index : %d process ID: %d vPN: %x frameNumber :%d\n", j, printTemp->pid, printTemp->virtualPageNumber, printTemp->frameNumber);
                        printTemp = printTemp->next;
                    }
                }
                printf("\n\n");
            }
        }
        // 모두 eof를 만났으면일때 while문 탈출
        bool escape = eofEscape();
        if (escape) {
            break;
        }
    }
    free(eofTrace);
}  // complete

void Inverted_GoHashTable() {
    // 해쉬테이블 링크에 아무것도 없을 때
    if (hashTable[hashTableIndex].next == NULL) {
        // NULL Access와 Fault 1씩 증가.
        procTable[nowProcessID].numIHTNULLAccess++;
        procTable[nowProcessID].numPageFault++;
        if (DEBUG) {
            printf("fault count : %d\n", procTable[nowProcessID].numPageFault);
        }
        // findFrameNum이 setting된다.
        if (DEBUG) {
            printf("hashTable[index].next is null\n");
        }
        bool shouldDelete = false;
        shouldDelete = PageFault();
        GetPhyAddr();
        // 새로 만들어서
        makeNext = &(hashTable[hashTableIndex].next);
        *makeNext = (struct invertedPageTableEntry *)malloc(1 * sizeof(struct invertedPageTableEntry));
        (*makeNext)->pid = nowProcessID;
        (*makeNext)->virtualPageNumber = virtualPN;
        (*makeNext)->frameNumber = findFrameNum;
        (*makeNext)->next = NULL;
        // mapping된 상태였으면
        if (shouldDelete) {
            Inverted_MappingDelete();
        }
        if (DEBUG) {
            printf("new.pid : %d, new.vPN: %X, new.frameNumber : %d\n", nowProcessID, virtualPN, findFrameNum);
            printf("oldProcessID : %d oldVPN : %x\n", oldProcessID, oldVirtualPageNumber);
        }
    }
    // next가 있을때 원하는 값이 있는지 확인해야한다.
    else {
        if (DEBUG) {
            printf("hashTable[index].next is not null\n");
        }
        procTable[nowProcessID].numIHTNonNULLAcess++;
        nNext = hashTable[hashTableIndex].next;
        bool find = false;

        while (nNext != NULL) {
            // next 따라가면서 찾아본다
            // NULL이면 탈출
            procTable[nowProcessID].numIHTConflictAccess++;
            // hit인 경우
            if ((nNext->pid == nowProcessID) && (nNext->virtualPageNumber == virtualPN)) {
                // hit++, frameNum을 얻어내서 offset과 OR로
                // phyAddr을 얻어내고 탈출한다.
                procTable[nowProcessID].numPageHit++;
                if (DEBUG) {
                    printf("hit count : %d\n", procTable[nowProcessID].numPageHit);
                }
                findFrameNum = nNext->frameNumber;
                LRU();
                GetPhyAddr();
                find = true;
                break;
            }
            nNext = nNext->next;
        }
        // 마지막까지 갔는데도 못 찾았으면
        if (!find) {
            nNext = hashTable[hashTableIndex].next;
            // page fault 내준다.
            bool shouldDelete = false;
            shouldDelete = PageFault();
            GetPhyAddr();

            // 최신 접근을 해쉬 테이블에 가까이 배치한다.
            makeNext = &(hashTable[hashTableIndex].next);
            *makeNext = (struct invertedPageTableEntry *)malloc(1 * sizeof(struct invertedPageTableEntry));
            (*makeNext)->pid = nowProcessID;
            (*makeNext)->virtualPageNumber = virtualPN;
            (*makeNext)->frameNumber = findFrameNum;
            (*makeNext)->next = NULL;
            hashTable[hashTableIndex].next->next = nNext;

            // mapping 정보를 삭제해줘야해
            if (shouldDelete) {
                Inverted_MappingDelete();
            }
            procTable[nowProcessID].numPageFault++;
            if (DEBUG) {
                printf("fault count : %d\n", procTable[nowProcessID].numPageFault);
            }
        }
    }
}

void Inverted_MappingDelete() {
    if (DEBUG) {
        printf("deleting oldProcessID: %d, oldVPN: %x \n", oldProcessID, oldVirtualPageNumber);
    }
    oldHashTableIndex = (oldProcessID + oldVirtualPageNumber) % nFrame;
    toDelete = hashTable[oldHashTableIndex].next;
    bool first = true;

    while (!(toDelete->pid == oldProcessID && toDelete->virtualPageNumber == oldVirtualPageNumber)) {
        first = false;
        if (DEBUG) {
            printf("link trace\n");
        }
        saveBefore = toDelete;
        toDelete = toDelete->next;
    }
    // 첫번째 링크이고
    if (first) {
        // 마지막 링크이면
        if (toDelete->next == NULL) {
            if (DEBUG) {
                printf("delete case 1\n");
            }
            hashTable[oldHashTableIndex].next = NULL;
        }
        // 뒤에 더있으면
        else {
            if (DEBUG) {
                printf("delete case 2\n");
            }
            hashTable[oldHashTableIndex].next = toDelete->next;
        }
    }
    // 첫번째 링크가 아니고
    else {
        // 마지막 링크이면{
        if (toDelete->next == NULL) {
            if (DEBUG) {
                printf("delete case 3\n");
            }
            saveBefore->next = NULL;
        }

        // 중간 링크이면
        else {
            if (DEBUG) {
                printf("delete case 4\n");
            }
            saveBefore->next = toDelete->next;
        }
    }
}  // complete

void SecondLv_PrintResult() {
    int i;
    for (i = 0; i < nProcess; i++) {
        printf("**** %s *****\n", procTable[i].traceName);
        printf("Proc %d Num of traces %d\n", i, procTable[i].ntraces);
        printf("Proc %d Num of second level page tables allocated %d\n", i, procTable[i].num2ndLevelPageTable);
        printf("Proc %d Num of Page Faults %d\n", i, procTable[i].numPageFault);
        printf("Proc %d Num of Page Hit %d\n", i, procTable[i].numPageHit);
        assert(procTable[i].numPageHit + procTable[i].numPageFault == procTable[i].ntraces);
    }
}  // copmplete

void Inverted_PrintResult() {
    int i;
    for (i = 0; i < nProcess; i++) {
        printf("**** %s *****\n", procTable[i].traceName);
        printf("Proc %d Num of traces %d\n", i, procTable[i].ntraces);
        printf("Proc %d Num of Inverted Hash Table Access Conflicts %d\n", i, procTable[i].numIHTConflictAccess);
        printf("Proc %d Num of Empty Inverted Hash Table Access %d\n", i, procTable[i].numIHTNULLAccess);
        printf("Proc %d Num of Non-Empty Inverted Hash Table Access %d\n", i, procTable[i].numIHTNonNULLAcess);
        printf("Proc %d Num of Page Faults %d\n", i, procTable[i].numPageFault);
        printf("Proc %d Num of Page Hit %d\n", i, procTable[i].numPageHit);
        assert(procTable[i].numPageHit + procTable[i].numPageFault == procTable[i].ntraces);
        assert(procTable[i].numIHTNULLAccess + procTable[i].numIHTNonNULLAcess == procTable[i].ntraces);
    }
}  // complete

void CheckSetPrint() {
    // input 수가 4개 미만일때 Error
    if (global_argc < 4) {
        printf("Usage : %s firstLevelBits PhysicalMemorySizeBits TraceFileNames\n", global_argv[0]);
        exit(1);
    }
    firstLevelBits = atoi(global_argv[1]);
    secondLevelBits = VIRTUALADDRBITS - PAGESIZEBITS - firstLevelBits;
    phyMemSizeBits = atoi(global_argv[2]);

    // 페이지보다 작게 들어왔을때 Error
    if (phyMemSizeBits < PAGESIZEBITS) {
        printf("PhysicalMemorySizeBits %d should be larger than PageSizeBits %d\n", phyMemSizeBits, PAGESIZEBITS);
        exit(1);
    }
    nProcess = global_argc - 3;
    nFrame = (1 << (phyMemSizeBits - PAGESIZEBITS));
    nFirstTable = (1 << firstLevelBits);
    nSecondTable = (1 << secondLevelBits);
    if (DEBUG) {
        printf("nProcess : %d nFrame: %d, firstLevelBits: %d, secondLevelBits : %d\n", nProcess, nFrame, firstLevelBits, secondLevelBits);
    }
    // secondLevelBits가 0보다 작게 나와버릴때 Error
    if (secondLevelBits <= 0) {
        printf("firstLevelBits %d is too Big\n", firstLevelBits);
        exit(1);
    }
    int i;
    for (i = 0; i < nProcess; ++i) {
        printf("process %d opening %s\n", i, global_argv[i + 3]);
        ;
    }
    assert(nFrame > 0);
    unsigned long long phyMemSize = (unsigned long long)(1ULL << phyMemSizeBits);
    printf("\nNum of Frames %d Physical Memory Size %lld bytes\n", nFrame, phyMemSize);
}  // complete

void PhysicalMemoryMalloc() {
    physicalMemory = (struct framePage *)malloc(nFrame * sizeof(struct framePage));
    int i;
    for (i = 0; i < nFrame; ++i) {
        physicalMemory[i].number = i;
        physicalMemory[i].pid = -1;
        physicalMemory[i].virtualPageNumber = -1;
        physicalMemory[i].lruLeft = &physicalMemory[(i - 1 + nFrame) % nFrame];
        physicalMemory[i].lruRight = &physicalMemory[(i + 1 + nFrame) % nFrame];
    }
    oldestFrame = &physicalMemory[0];
}  // complete

void PhysicalMemoryFree() {
    free(physicalMemory);
}  // complete

void LRU() {
    // oldest의 lruLeft를 최신 Frame을 연결시켜서 cycle을 이루게 한다.

    // 최신 접근시 그냥 지나가자.
    //if (oldestFrame->lruLeft == &physicalMemory[findFrameNum]) {
    //    return;
    //}
    // oldest가 최신이 되면 oldest의 우측(다음 오래된것)이 oldest가 된다.
    if (&physicalMemory[findFrameNum] == oldestFrame) {
        oldestFrame = oldestFrame->lruRight;
        return;
    }
    /* 그림 좌우 화살표 그려서 이해하고 코드 짜자. */
    // 4<--> | 0<-->1<-->2<-->3<-->4 (0:oldest, 4:최신)
    // 위 상황에서 2가 최신이 되면
    // 2<--> | 0<-->1<-->3<-->4<-->2 (0:oldest, 2:최신)
    if (oldestFrame->lruLeft != &physicalMemory[findFrameNum]) {
        // oldest가 바뀌지 않게 실행 순서에 유의.
        struct framePage *temp = &physicalMemory[findFrameNum];
        // 2를 떼냈으니 1과 3의 우측 좌측을 서로 연결한다.
        // 2의 left(1)의 right를 2의 right(3)와 연결
        (physicalMemory[findFrameNum].lruLeft)->lruRight = temp->lruRight;
        // 2의 right(3)의 left를 2의 left(1)과 연결
        (physicalMemory[findFrameNum].lruRight)->lruLeft = temp->lruLeft;

        // 최신의 왼쪽을 구 최신과 연결
        physicalMemory[findFrameNum].lruLeft = oldestFrame->lruLeft;
        // 최신의 오른쪽을 oldest로 연결
        physicalMemory[findFrameNum].lruRight = oldestFrame;

        // oldest의 왼쪽(구 최신)의 오른쪽을 최신으로
        (oldestFrame->lruLeft)->lruRight = &physicalMemory[findFrameNum];
        // oldest의 왼쪽을 최신으로
        oldestFrame->lruLeft = &physicalMemory[findFrameNum];
    }
}  // complete

bool PageFault() {
    // 프레임 번호 및 저장되어 있던 pID와 vPN 저장
    bool shouldDelete = false;
    findFrameNum = oldestFrame->number;
    oldProcessID = oldestFrame->pid;
    oldVirtualPageNumber = oldestFrame->virtualPageNumber;

    oldestFrame->pid = nowProcessID;
    oldestFrame->virtualPageNumber = (addr >> PAGESIZEBITS);
    oldestFrame = oldestFrame->lruRight;

    if (oldProcessID != -1) {  // && oldVirtualPageNumber != -1) {
        shouldDelete = true;
    }
    return shouldDelete;
}  // complete

void GetPhyAddr() {
    phyAddr = ((findFrameNum << PAGESIZEBITS) | offset);
}  // complete

void GetIndex(int system) {
    // Second Page Table System
    if (system == SECONDSYSTEM) {
        // 비트를 움직여서 first Table index Number 정보를 얻어낸다.
        // second index, offset은 masking을 통해 구해낸다.
        firstTableIndex = (addr >> (VIRTUALADDRBITS - firstLevelBits));
        int j, mask = 1;
        for (j = 0; j < secondLevelBits - 1; ++j) {
            mask <<= 1;
            mask += 1;
        }
        secondTableIndex = ((addr >> PAGESIZEBITS) & mask);
        mask = 1;
        for (j = 0; j < PAGESIZEBITS - 1; ++j) {
            mask <<= 1;
            mask += 1;
        }
        offset = addr & mask;
        if (DEBUG) {
            printf("firstTableIndex: %x, secondTableIndex: %x offset : %x\n", firstTableIndex, secondTableIndex, offset);
        }
    }
    // Inverted Page System
    else if (system == INVERTEDSYSTEM) {
        // virtualPageNumber와 HTindex를 구한다.
        // hash table index = (virtual page number + pid ) % nFrame
        virtualPN = (addr >> PAGESIZEBITS);
        hashTableIndex = virtualPN + nowProcessID;
        hashTableIndex %= nFrame;
        // offset 계산
        offset = (addr << (VIRTUALADDRBITS - PAGESIZEBITS)) >> (VIRTUALADDRBITS - PAGESIZEBITS);
    }
}  // complete
