//Jisoo Heo
//B115233
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGESIZEBITS 12
#define VIRTUALADDRBITS 32

#define O_INVERTED 1
#define O_MULTILV 0

enum { VALID,
       INVALID };

struct pageTableEntry {
    char valid;
    unsigned int frameNumber;
    struct pageTableEntry* secondLevelPageTable;
};

struct invertedPageTableEntry {
    int pid;
    int virtualPageNumber;
    unsigned int frameNumber;
    struct invertedPageTableEntry* next;
};

struct HashTable {
    struct invertedPageTableEntry* head;
};

struct pageFrame {
    unsigned int number;
    int pid;
    int virtualPageNumber;
    struct pageFrame* lruLeft;
    struct pageFrame* lruRight;
};

struct procEntry {
    int pid;
    char* traceName;
    int ntraces;
    int num2ndLevelPageTable;
    int numPageFault;
    int numPageHit;
    struct pageTableEntry* firstLevelPageTable;
    FILE* tracefp;

    int numIHTConflictAccess;
    int numIHTNULLAccess;
    int numIHTNonNULLAccess;
};

void InvertedPageTable_sim();
void access_invPageTable(int vpn, int pid);
void delete_invPageTableEntry(int vpn, int pid);
void create_invPageTableEntry(struct invertedPageTableEntry** next, int pid, int vpn, unsigned int frameNumber);
void twoLvPageTable_sim();
void parse_VirtualAddress(unsigned int addr, int* firstIdx, int* secondIdx, int* offset, int* vpn);
void access_pageTable(int i, int firstIdx, int secondIdx, int vpn);
void create_2LvPageTable(struct pageTableEntry** pt);
void LRUPolicy(unsigned int frameNumber);
unsigned int pageFault(int* vpn, int* pid);
void Init_programParam(int argc, char** argv);
void Create_processEntry(char** argv, char mode);
void Delete_processEntry(char mode);
void Create_physicalMemory();
void Delete_physicalMemory();
int found(struct invertedPageTableEntry* iter, int vpn, int pid);

int firstLvBits;
int secondLvBits;
int physicalMemBits;
int nTable;
int nEntry;
int nProcess;
unsigned int nFrame;
struct procEntry* proc;
struct pageFrame* oldestFrame;
struct pageFrame* physicalMem;
struct HashTable* hashTable;

int main(int argc, char** argv) {
    //Step 1
    Init_programParam(argc, argv);

    //Step 2 : 2 level page table simulation
    Create_processEntry(argv, O_MULTILV);

    Create_physicalMemory();

    twoLvPageTable_sim();

    Delete_processEntry(O_MULTILV);

    Delete_physicalMemory();

    //Step 3 : inverted page table simulation
    Create_processEntry(argv, O_INVERTED);

    Create_physicalMemory();

    InvertedPageTable_sim();

    Delete_processEntry(O_INVERTED);

    Delete_physicalMemory();
}

void InvertedPageTable_sim() {
    int i, nbyte;
    int vpn, offset;
    unsigned int addr;
    unsigned char character;

    printf("======================================================\n");
    printf("The Inverted Page Table Memory Simulation Starts .....\n");
    printf("======================================================\n");

    while (1) {
        for (i = 0; i < nProcess; i++) {
            if ((nbyte = fscanf(proc[i].tracefp, "%x %c", &addr, &character)) < 0) break;
            proc[i].ntraces++;

            parse_VirtualAddress(addr, NULL, NULL, &offset, &vpn);

            access_invPageTable(vpn, proc[i].pid);
        }
        if (nbyte == EOF) break;
    }
    for (i = 0; i < nProcess; i++) {
        printf("**** %s *****\n", proc[i].traceName);
        printf("Proc %d Num of traces %d\n", i, proc[i].ntraces);
        printf("Proc %d Num of Inverted Hash Table Access Conflicts %d\n", i, proc[i].numIHTConflictAccess);
        printf("Proc %d Num of Empty Inverted Hash Table Access %d\n", i, proc[i].numIHTNULLAccess);
        printf("Proc %d Num of Non-Empty Inverted Hash Table Access %d\n", i, proc[i].numIHTNonNULLAccess);
        printf("Proc %d Num of Page Faults %d\n", i, proc[i].numPageFault);
        printf("Proc %d Num of Page Hit %d\n", i, proc[i].numPageHit);
        assert(proc[i].numPageHit + proc[i].numPageFault == proc[i].ntraces);
        assert(proc[i].numIHTNULLAccess + proc[i].numIHTNonNULLAccess == proc[i].ntraces);
    }
}

void access_invPageTable(int vpn, int pid) {
    unsigned int idx = (vpn + pid) % nFrame;
    int vpnRetVal = vpn;
    int pidRetVal = pid;
    unsigned int frameNumber;

    struct invertedPageTableEntry *iter, *tempPtr;

    //no entry : 100% page fault
    if (hashTable[idx].head == NULL) {
        frameNumber = pageFault(&vpnRetVal, &pidRetVal);
        create_invPageTableEntry(&(hashTable[idx].head), pid, vpn, frameNumber);
        if (vpnRetVal != -1 && pidRetVal != -1) delete_invPageTableEntry(vpnRetVal, pidRetVal);

        proc[pid].numIHTNULLAccess++;
        proc[pid].numPageFault++;
    }

    //entry more than one : page fault or not
    else {
        iter = hashTable[idx].head;

        proc[pid].numIHTNonNULLAccess++;

        //search
        while (iter != NULL) {
            proc[pid].numIHTConflictAccess++;

            //entry found
            if (found(iter, vpn, pid)) {
                LRUPolicy(iter->frameNumber);
                proc[pid].numPageHit++;
                return;
            }
            iter = iter->next;
        }

        //entry not found
        tempPtr = hashTable[idx].head;
        frameNumber = pageFault(&vpnRetVal, &pidRetVal);
        create_invPageTableEntry(&(hashTable[idx].head), pid, vpn, frameNumber);
        hashTable[idx].head->next = tempPtr;
        if (vpnRetVal != -1 && pidRetVal != -1) delete_invPageTableEntry(vpnRetVal, pidRetVal);
        proc[pid].numPageFault++;
    }
}

int found(struct invertedPageTableEntry* iter, int vpn, int pid) {
    if (iter->virtualPageNumber == vpn && iter->pid == pid)
        return 1;
    else
        return 0;
}

void delete_invPageTableEntry(int vpn, int pid) {
    struct invertedPageTableEntry* iter;
    struct invertedPageTableEntry* before;
    unsigned int idx = (vpn + pid) % nFrame;

    iter = hashTable[idx].head;
    before = NULL;

    //logically, the entry must be exists.
    while (!found(iter, vpn, pid)) {
        before = iter;
        iter = iter->next;
    }

    //head entry
    if (before == NULL) {
        //only one entry
        if (iter->next == NULL) {
            hashTable[idx].head = NULL;
            free(iter);
        }
        //more than one entry
        else {
            hashTable[idx].head = iter->next;
            free(iter);
        }
    }

    //not head entry : at least two entry
    else {
        //rear entry
        if (iter->next == NULL) {
            before->next = NULL;
            free(iter);
        }

        //middle entry
        else {
            before->next = iter->next;
            free(iter);
        }
    }
}

void create_invPageTableEntry(struct invertedPageTableEntry** next, int pid, int vpn, unsigned int frameNumber) {
    *next = (struct invertedPageTableEntry*)malloc(sizeof(struct invertedPageTableEntry));
    (*next)->pid = pid;
    (*next)->virtualPageNumber = vpn;
    (*next)->frameNumber = frameNumber;
    (*next)->next = NULL;
}

void twoLvPageTable_sim() {
    int i, nbyte;
    int firstIdx, secondIdx, offset, vpn;
    unsigned int addr;
    unsigned char character;

    printf("======================================================\n");
    printf("The 2nd level Page Table Memory Simulation Starts ....\n");
    printf("======================================================\n");

    while (1) {
        for (i = 0; i < nProcess; i++) {
            if ((nbyte = fscanf(proc[i].tracefp, "%x %c", &addr, &character)) < 0) break;

            proc[i].ntraces++;

            parse_VirtualAddress(addr, &firstIdx, &secondIdx, &offset, &vpn);

            access_pageTable(i, firstIdx, secondIdx, vpn);
        }
        if (nbyte == EOF) break;
    }

    for (i = 0; i < nProcess; i++) {
        printf("**** %s *****\n", proc[i].traceName);
        printf("Proc %d Num of traces %d\n", i, proc[i].ntraces);
        printf("Proc %d Num of second level page tables aloocated %d\n", i, proc[i].num2ndLevelPageTable);
        printf("Proc %d Num of Page Faults %d\n", i, proc[i].numPageFault);
        printf("Proc %d Num of Page Hits %d\n", i, proc[i].numPageHit);
        assert(proc[i].numPageHit + proc[i].numPageFault == proc[i].ntraces);
    }
}

void parse_VirtualAddress(unsigned int addr, int* firstIdx, int* secondIdx, int* offset, int* vpn) {
    unsigned int i, mask;

    if (offset != NULL && vpn != NULL) {
        mask = 0;
        for (i = 0; i < PAGESIZEBITS; i++) {
            mask <<= 1;
            mask |= 1;
        }

        *offset = addr & mask;
        while (i--) addr = addr >> 1;
        *vpn = addr;
    }

    if (firstIdx != NULL && secondIdx != NULL) {
        mask = 0;
        for (i = 0; i < secondLvBits; i++) {
            mask <<= 1;
            mask |= 1;
        }

        *secondIdx = addr & mask;
        while (i--) addr = addr >> 1;

        *firstIdx = addr;
    }
}

void access_pageTable(int i, int firstIdx, int secondIdx, int vpn) {
    unsigned int frameNumber;
    int pid = i;

    if (proc[i].firstLevelPageTable[firstIdx].secondLevelPageTable == NULL) {
        create_2LvPageTable(&(proc[i].firstLevelPageTable[firstIdx].secondLevelPageTable));
        proc[i].num2ndLevelPageTable++;
    }

    if (proc[i].firstLevelPageTable[firstIdx].secondLevelPageTable[secondIdx].valid == VALID) {
        frameNumber = proc[i].firstLevelPageTable[firstIdx].secondLevelPageTable[secondIdx].frameNumber;
        LRUPolicy(frameNumber);
        proc[i].numPageHit++;
    } else {
        frameNumber = pageFault(&vpn, &pid);
        proc[i].firstLevelPageTable[firstIdx].secondLevelPageTable[secondIdx].valid = VALID;
        proc[i].firstLevelPageTable[firstIdx].secondLevelPageTable[secondIdx].frameNumber = frameNumber;
        if (vpn != -1 && pid != -1) {
            parse_VirtualAddress(vpn, &firstIdx, &secondIdx, NULL, NULL);
            proc[pid].firstLevelPageTable[firstIdx].secondLevelPageTable[secondIdx].valid = INVALID;
        }
        proc[i].numPageFault++;
    }
}

void create_2LvPageTable(struct pageTableEntry** pt) {
    int i;

    *pt = (struct pageTableEntry*)malloc(sizeof(struct pageTableEntry) * nEntry);
    for (i = 0; i < nEntry; i++) {
        (*pt)[i].valid = INVALID;
        (*pt)[i].frameNumber = -1;
        (*pt)[i].secondLevelPageTable = NULL;
    }
}

void LRUPolicy(unsigned int frameNumber) {
    struct pageFrame* youngestFrame = oldestFrame->lruRight;

    if (youngestFrame == &physicalMem[frameNumber])
        return;
    else if (oldestFrame == &physicalMem[frameNumber])
        oldestFrame = oldestFrame->lruLeft;
    else {
        (physicalMem[frameNumber].lruLeft)->lruRight = physicalMem[frameNumber].lruRight;
        (physicalMem[frameNumber].lruRight)->lruLeft = physicalMem[frameNumber].lruLeft;

        physicalMem[frameNumber].lruRight = oldestFrame->lruRight;
        physicalMem[frameNumber].lruLeft = oldestFrame;

        (oldestFrame->lruRight)->lruLeft = &physicalMem[frameNumber];
        oldestFrame->lruRight = &physicalMem[frameNumber];
    }
}

unsigned int pageFault(int* vpn, int* pid) {
    unsigned int frameNumber = oldestFrame->number;
    int retVal1 = oldestFrame->virtualPageNumber;
    int retVal2 = oldestFrame->pid;

    oldestFrame->virtualPageNumber = *vpn;
    oldestFrame->pid = *pid;
    oldestFrame = oldestFrame->lruLeft;

    *vpn = retVal1;
    *pid = retVal2;
    return frameNumber;
}

void Init_programParam(int argc, char** argv) {
    int i, j;

    if (argc < 4) {
        printf("USAGE : %s <First Level Bits> <Physical Memory Bits> <Trace File Name1> <Trace File Name2> ...\n", argv[0]);
        exit(1);
    }

    physicalMemBits = atoi(argv[2]);
    if (physicalMemBits < PAGESIZEBITS) {
        printf("physicalMemBits %d should be larger than PageSizeBits %d\n", physicalMemBits, PAGESIZEBITS);
        exit(1);
    }
    nFrame = (1L << (physicalMemBits - PAGESIZEBITS));
    assert(nFrame > 0);

    firstLvBits = atoi(argv[1]);
    if (VIRTUALADDRBITS <= PAGESIZEBITS + firstLvBits) {
        printf("firstLvBits %d is too big\n", firstLvBits);
        exit(1);
    }
    secondLvBits = VIRTUALADDRBITS - PAGESIZEBITS - firstLvBits;

    nTable = 1 << firstLvBits;
    nEntry = 1 << secondLvBits;

    nProcess = argc - 3;
}

void Create_processEntry(char** argv, char mode) {
    static char ShowMeOnlyOnce = 0;
    unsigned int i, j;

    proc = (struct procEntry*)malloc(sizeof(struct procEntry) * nProcess);
    for (i = 0; i < nProcess; i++) {
        proc[i].traceName = (char*)malloc(sizeof(char) * (strlen(argv[i + 3] + 1)));
        strcpy(proc[i].traceName, argv[i + 3]);

        proc[i].ntraces = 0;
        proc[i].num2ndLevelPageTable = 0;
        proc[i].numPageFault = 0;
        proc[i].numPageHit = 0;

        if (mode == O_MULTILV) {
            proc[i].firstLevelPageTable = (struct pageTableEntry*)malloc(sizeof(struct pageTableEntry) * nTable);
            for (j = 0; j < nTable; j++) proc[i].firstLevelPageTable[j].secondLevelPageTable = NULL;

        } else if (mode == O_INVERTED) {
            proc[i].pid = i;
            proc[i].numIHTConflictAccess = 0;
            proc[i].numIHTNULLAccess = 0;
            proc[i].numIHTNonNULLAccess = 0;
        }

        if ((proc[i].tracefp = fopen(proc[i].traceName, "r")) == NULL) {
            printf("file %s does not exist\n", proc[i].traceName);
            exit(1);
        }
        if (ShowMeOnlyOnce == 0) printf("process %d opening %s\n", i, proc[i].traceName);
    }

    if (mode == O_INVERTED) {
        hashTable = (struct HashTable*)malloc(sizeof(struct HashTable) * nFrame);
        for (i = 0; i < nFrame; i++) {
            hashTable[i].head = NULL;
        }
    }

    if (ShowMeOnlyOnce == 0) printf("\nNum of Frames %d Physical Memory Size %ld bytes\n", nFrame, (1L << physicalMemBits));
    ShowMeOnlyOnce++;
}

void Delete_processEntry(char mode) {
    int i, j;
    struct invertedPageTableEntry* iter;
    struct invertedPageTableEntry* temp;

    for (i = 0; i < nProcess; i++) {
        free(proc[i].traceName);
        fclose(proc[i].tracefp);

        if (mode == O_MULTILV) {
            for (j = 0; j < nTable; j++)
                if (proc[i].firstLevelPageTable[j].secondLevelPageTable != NULL)
                    free(proc[i].firstLevelPageTable[j].secondLevelPageTable);
            free(proc[i].firstLevelPageTable);
        }
    }

    if (mode == O_INVERTED) {
        for (i = 0; i < nFrame; i++)
            if (hashTable[i].head != NULL) {
                iter = hashTable[i].head;
                while (iter != NULL) {
                    temp = iter;
                    iter = iter->next;
                    free(temp);
                }
            }
    }

    free(proc);
}

void Create_physicalMemory() {
    int i;
    physicalMem = (struct pageFrame*)malloc(sizeof(struct pageFrame) * nFrame);

    for (i = 0; i < nFrame; i++) {
        physicalMem[i].number = i;
        physicalMem[i].pid = -1;
        physicalMem[i].virtualPageNumber = -1;
        physicalMem[i].lruLeft = &physicalMem[(i - 1 + nFrame) % nFrame];
        physicalMem[i].lruRight = &physicalMem[(i + 1 + nFrame) % nFrame];
    }
    oldestFrame = &physicalMem[0];
}

void Delete_physicalMemory() {
    free(physicalMem);
}
