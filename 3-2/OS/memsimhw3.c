//
// Virual Memory Simulator Homework
// Two-level page table system
// Inverted page table with a hashing system
// Student Name: Kim Hyeong Seok 김형석
// Student Number: B411063
//
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define PAGESIZEBITS 12     // page size = 4Kbytes
#define VIRTUALADDRBITS 32  // virtual address space size = 4Gbytes

struct pageTableEntry {
    int level;  // page table level (1 or 2)
    char valid;
    struct pageTableEntry *secondLevelPageTable;
    // valid if this entry is for the first level page table (level = 1)
    int frameNumber;
    // valid if this entry is for the second level page table (level = 2)
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
    int numIHTConflictAccess;  // The number of Inverted Hash Table Conflict
                               // Accesses
    int numIHTNULLAccess;    // The number of Empty Inverted Hash Table Accesses
    int numIHTNonNULLAcess;  // The number of Non Empty Inverted Hash Table
                             // Accesses
    int numPageFault;        // The number of page faults
    int numPageHit;          // The number of page hits
    struct pageTableEntry *firstLevelPageTable;
    FILE *tracefp;
};

struct framePage *oldestFrame;  // the oldest frame pointer

int firstLevelBits, secondLevelBits, phyMemSizeBits, numProcess;

void initPhyMem(struct framePage *phyMem, int nFrame) {
    int i;
    for (i = 0; i < nFrame; i++) {
        phyMem[i].number = i;
        phyMem[i].pid = -1;
        phyMem[i].virtualPageNumber = -1;
        phyMem[i].lruLeft = &phyMem[(i - 1 + nFrame) % nFrame];
        phyMem[i].lruRight = &phyMem[(i + 1 + nFrame) % nFrame];
    }
    oldestFrame = &phyMem[0];
}

void secondLevelVMSim(struct procEntry *procTable,
                      struct framePage *phyMemFrames) {
    for (int i = 0; i < numProcess; i++) {
        printf("**** %s *****\n", procTable[i].traceName);
        printf("Proc %d Num of traces %d\n", i, procTable[i].ntraces);
        printf("Proc %d Num of second level page tables allocated %d\n", i,
               procTable[i].num2ndLevelPageTable);
        printf("Proc %d Num of Page Faults %d\n", i, procTable[i].numPageFault);
        printf("Proc %d Num of Page Hit %d\n", i, procTable[i].numPageHit);
        assert(procTable[i].numPageHit + procTable[i].numPageFault ==
               procTable[i].ntraces);
    }
}

void invertedPageVMSim(struct procEntry *procTable,
                       struct framePage *phyMemFrames, int nFrame) {
    for (int i = 0; i < numProcess; i++) {
        printf("**** %s *****\n", procTable[i].traceName);
        printf("Proc %d Num of traces %d\n", i, procTable[i].ntraces);
        printf("Proc %d Num of Inverted Hash Table Access Conflicts %d\n", i,
               procTable[i].numIHTConflictAccess);
        printf("Proc %d Num of Empty Inverted Hash Table Access %d\n", i,
               procTable[i].numIHTNULLAccess);
        printf("Proc %d Num of Non-Empty Inverted Hash Table Access %d\n", i,
               procTable[i].numIHTNonNULLAcess);
        printf("Proc %d Num of Page Faults %d\n", i, procTable[i].numPageFault);
        printf("Proc %d Num of Page Hit %d\n", i, procTable[i].numPageHit);
        assert(procTable[i].numPageHit + procTable[i].numPageFault ==
               procTable[i].ntraces);
        assert(procTable[i].numIHTNULLAccess +
                   procTable[i].numIHTNonNULLAcess ==
               procTable[i].ntraces);
    }
}

int main(int argc, char *argv[]) {
    int i;
    // 인자 에러시 메시지
    if (argc < 4) {
        printf(
            "Usage : %s firstLevelBits PhysicalMemorySizeBits TraceFileNames\n",
            argv[0]);
        exit(1);
    }
    firstLevelBits = atoi(argv[1]);
    phyMemSizeBits = atoi(argv[2]);
    secondLevelBits = VIRTUALADDRBITS - PAGESIZEBITS - firstLevelBits;
    // physical Memory Size 크기 에러 체크
    if (phyMemSizeBits < PAGESIZEBITS) {
        printf(
            "PhysicalMemorySizeBits %d should be larger than PageSizeBits %d\n",
            phyMemSizeBits, PAGESIZEBITS);
        exit(1);
    }
    // secondLevelBits가 양수가 아닐때 에러 체크
    if (VIRTUALADDRBITS - PAGESIZEBITS - firstLevelBits <= 0) {
        printf("firstLevelBits %d is too Big\n", firstLevelBits);
        exit(1);
    }
    // initialize procTable for two-level page table
    numProcess = argc - 3;
    for (i = 0; i < numProcess; i++) {
        // opening a tracefile for the process
        printf("process %d opening %s\n", i, argv[i + 3]);
    }

    int nFrame = (1 << (phyMemSizeBits - PAGESIZEBITS));
    assert(nFrame > 0);
//
//
//
//
    struct framePage *phyMem;
    phyMem = (struct framePage *)malloc((1 << phyMemSizeBits) *
                                        sizeof(struct framePage));
    initPhyMem(phyMem, nFrame);

    struct pageTableEntry *firstPageTableEntry;
	firstPageTableEntry=(struct pageTableEntry)

    printf("\nNum of Frames %d Physical Memory Size %ld bytes\n", nFrame,
           (1L << phyMemSizeBits));
    // Second Level Page Table
    printf("=============================================================\n");
    printf("The 2nd Level Page Table Memory Simulation Starts .....\n");
    printf("=============================================================\n");

    // secondLevelVMSim
//
//
//
//
//
//
//
    // initialize procTable for the inverted Page Table
    for (i = 0; i < numProcess; i++) {
        // rewind tracefiles
        rewind(procTable[i].tracefp);
    }
    // Inverted Page Table
    printf("=============================================================\n");
    printf("The Inverted Page Table Memory Simulation Starts .....\n");
    printf("=============================================================\n");
    // invertedPageVMSim
    return (0);
}
