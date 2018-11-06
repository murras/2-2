// Virual Memory Simulator Homework
// Two-level page table system
// Inverted page table with a hashing system 
// Student Name: ¹Î°æÅÂ
// Student Number: B311064
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define PAGESIZEBITS 12			// page size = 4Kbytes
#define VIRTUALADDRBITS 32		// virtual address space size = 4Gbytes

struct pageTableEntry {
	int level;				// page table level (1 or 2)
	char valid;
	struct pageTableEntry *secondLevelPageTable;	// valid if this entry is for the first level page table (level = 1)
	int frameNumber;								// valid if this entry is for the second level page table (level = 2)
};

struct framePage {
	int number;			// frame number
	int pid;			// Process id that owns the frame
	int virtualPageNumber;			// virtual page number using the frame
	struct framePage *lruLeft;	// for LRU circular doubly linked list
	struct framePage *lruRight; // for LRU circular doubly linked list
};

struct invertedPageTableEntry {
	int pid;					// process id
	int virtualPageNumber;		// virtual page number
	int frameNumber;			// frame number allocated
	struct invertedPageTableEntry *next;
};

struct procEntry {
	char *traceName;			// the memory trace name
	int pid;					// process (trace) id
	int ntraces;				// the number of memory traces
	int num2ndLevelPageTable;	// The 2nd level page created(allocated);
	int numIHTConflictAccess; 	// The number of Inverted Hash Table Conflict Accesses
	int numIHTNULLAccess;		// The number of Empty Inverted Hash Table Accesses
	int numIHTNonNULLAcess;		// The number of Non Empty Inverted Hash Table Accesses
	int numPageFault;			// The number of page faults
	int numPageHit;				// The number of page hits
	struct pageTableEntry *firstLevelPageTable;
	FILE *tracefp;
};

struct HT
{
	struct invertedPageTableEntry *head; //head of linked list
};
struct HT * hashtable;

struct framePage *phyMem, *oldestFrame; //physical Memory and the oldest frame pointer

int firstLevelBits, secondLevelBits, phyMemSizeBits, numProcess, nFrame;

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

//oldestFrame->lruLeft = recent
void LRU(int frameNum)
{
	struct framePage * recentFrame = oldestFrame->lruLeft;
	//already recent
	if (recentFrame == &phyMem[frameNum])
		return;
	//update least recent
	else if (oldestFrame == &phyMem[frameNum])
		oldestFrame = oldestFrame->lruRight;
	else
	{
		//connect phyMem[frameNum].lruLeft and phyMem[frameNum].lruRight
		(phyMem[frameNum].lruRight)->lruLeft = phyMem[frameNum].lruLeft;
		(phyMem[frameNum].lruLeft)->lruRight = phyMem[frameNum].lruRight;
		//oldestFrame->lruLeft = phyMem[frameNum] 
		phyMem[frameNum].lruLeft = oldestFrame->lruLeft;
		phyMem[frameNum].lruRight = oldestFrame;
		(oldestFrame->lruLeft)->lruRight = &phyMem[frameNum];
		oldestFrame->lruLeft = &phyMem[frameNum];
	}
}

unsigned int hash(int procId, int vpn)
{
	return (procId + vpn) % nFrame;
}

//math.h is not included in the server
int pow(int num)
{
	int i;
	int result = 1;
	for (i = 0; i < num; i++)
		result *= 2;
	return result;
}

void secondLevelVMSim(struct procEntry *procTable, struct framePage *phyMemFrames) {
	int i, j;
	int numOfByte, procId;
	unsigned int addr, frameNum;
	unsigned char rw;
	int firstIdx, secondIdx, vpn, offset;

	for (;;)
	{
		for (i = 0; i < numProcess; i++)
		{
			if ((numOfByte = fscanf(procTable[i].tracefp, "%x %c", &addr, &rw)) < 0)
				break;
			procTable[i].ntraces++;

			offset = addr & (pow(PAGESIZEBITS) - 1);
			vpn = addr >> PAGESIZEBITS;
			secondIdx = (addr >> PAGESIZEBITS) & (pow(secondLevelBits) - 1);
			firstIdx = (addr >> PAGESIZEBITS) >> secondLevelBits;

			//create secondLevelPageTable
			if (procTable[i].firstLevelPageTable[firstIdx].secondLevelPageTable == NULL)
			{
				procTable[i].firstLevelPageTable[firstIdx].secondLevelPageTable = (struct pageTableEntry *)malloc(sizeof(struct pageTableEntry) * (1 << secondLevelBits));
				for (j = 0; j < (1 << secondLevelBits); j++)
				{
					procTable[i].firstLevelPageTable[firstIdx].secondLevelPageTable[j].valid = 'x';
					procTable[i].firstLevelPageTable[firstIdx].secondLevelPageTable[j].secondLevelPageTable = NULL;
					procTable[i].firstLevelPageTable[firstIdx].secondLevelPageTable[j].frameNumber = -1;
				}
				procTable[i].num2ndLevelPageTable++;
			}
			//page hit
			if (procTable[i].firstLevelPageTable[firstIdx].secondLevelPageTable[secondIdx].valid == 'o')
			{
				frameNum = procTable[i].firstLevelPageTable[firstIdx].secondLevelPageTable[secondIdx].frameNumber;
				LRU(frameNum);
				procTable[i].numPageHit++;
			}
			//page fault
			else
			{
				//previous oldestFrame
				int lruVpn = oldestFrame->virtualPageNumber;
				int lruProcId = oldestFrame->pid;
				frameNum = oldestFrame->number;
				//update recentFrame
				oldestFrame->virtualPageNumber = vpn;
				oldestFrame->pid = i;
				//oldestFrame becomes recentFrame so update oldestFrame to oldestFrame->lruRight
				oldestFrame = oldestFrame->lruRight;
				//update
				procTable[i].firstLevelPageTable[firstIdx].secondLevelPageTable[secondIdx].valid = 'o';
				procTable[i].firstLevelPageTable[firstIdx].secondLevelPageTable[secondIdx].frameNumber = frameNum;
				vpn = lruVpn;
				procId = lruProcId;
				//previous entry unmapped
				if (vpn != -1 && procId != -1)
				{
					secondIdx = vpn & (pow(secondLevelBits) - 1);
					firstIdx = vpn >> secondLevelBits;

					procTable[procId].firstLevelPageTable[firstIdx].secondLevelPageTable[secondIdx].valid = 'x';
				}
				procTable[i].numPageFault++;
			}
			//printf("2Level procID %d traceNumber %d virtual addr %x pysical addr %x\n", i, procTable[i].ntraces, addr, (frameNum << PAGESIZEBITS) | offset);
		}
		//process end
		if (numOfByte == EOF)
			break;
	}
	for (i = 0; i < numProcess; i++) {
		printf("**** %s *****\n", procTable[i].traceName);
		printf("Proc %d Num of traces %d\n", i, procTable[i].ntraces);
		printf("Proc %d Num of second level page tables allocated %d\n", i, procTable[i].num2ndLevelPageTable);
		printf("Proc %d Num of Page Faults %d\n", i, procTable[i].numPageFault);
		printf("Proc %d Num of Page Hit %d\n", i, procTable[i].numPageHit);
		assert(procTable[i].numPageHit + procTable[i].numPageFault == procTable[i].ntraces);
	}
}

void invertedPageVMSim(struct procEntry *procTable, struct framePage *phyMemFrames, int nFrame) {
	int i;
	int numOfByte;
	unsigned int addr, frameNum, idx;
	unsigned char rw;
	int vpn, offset;
	int copyProcId, copyVpn;
	struct invertedPageTableEntry *temp, *cur, *prev;

	for (;;)
	{
		for (i = 0; i < numProcess; i++)
		{
			if ((numOfByte = fscanf(procTable[i].tracefp, "%x %c", &addr, &rw)) < 0)
				break;
			procTable[i].ntraces++;

			offset = addr & (pow(PAGESIZEBITS) - 1);
			vpn = addr >> PAGESIZEBITS;
			idx = hash(procTable[i].pid, vpn);
			copyProcId = procTable[i].pid;
			copyVpn = vpn;

			//page fault
			if (hashtable[idx].head == NULL)
			{
				//previous oldestFrame
				int lruVpn = oldestFrame->virtualPageNumber;
				int lruProcId = oldestFrame->pid;
				frameNum = oldestFrame->number;
				//update recentFrame
				oldestFrame->virtualPageNumber = copyVpn;
				oldestFrame->pid = copyProcId;
				//oldestFrame becomes recentFrame so update oldestFrame to oldestFrame->lruRight
				oldestFrame = oldestFrame->lruRight;
				copyProcId = lruProcId;
				copyVpn = lruVpn;
				//create hashtable
				hashtable[idx].head = (struct invertedPageTableEntry *)malloc(sizeof(struct invertedPageTableEntry));
				hashtable[idx].head->frameNumber = frameNum;
				hashtable[idx].head->pid = procTable[i].pid;
				hashtable[idx].head->virtualPageNumber = vpn;
				hashtable[idx].head->next = NULL;
				if (copyProcId != -1 && copyVpn != -1)
				{
					prev = NULL;
					idx = hash(copyProcId, copyVpn);
					cur = hashtable[idx].head;
					//search
					while (1)
					{
						if (cur->pid == copyProcId && cur->virtualPageNumber == copyVpn)
							break;

						prev = cur;
						cur = cur->next;
					}
					//head
					if (prev == NULL)
					{
						if (cur->next == NULL)
							hashtable[idx].head = NULL;
						else
							hashtable[idx].head = cur->next;
						free(cur);
					}
					else
					{
						if (cur->next == NULL)
							prev->next = NULL;
						else
							prev->next = cur->next;
						free(cur);
					}
				}
				procTable[i].numIHTNULLAccess++;
				procTable[i].numPageFault++;
			}
			else
			{
				int searched = 0; //boolean searched
				int lruVpn, lruProcId;
				cur = hashtable[idx].head;
				procTable[copyProcId].numIHTNonNULLAcess++;

				//search
				while (1)
				{
					if (cur == NULL)
						break;

					procTable[copyProcId].numIHTConflictAccess++;
					//searched?
					if (cur->pid == copyProcId && cur->virtualPageNumber == copyVpn)
					{
						searched++;
						LRU(cur->frameNumber);
						procTable[copyProcId].numPageHit++;
						break;
					}
					cur = cur->next;
				}

				//search successful
				if (searched)
				{
					//printf("IHT procID %d traceNumber %d virtual addr %x pysical addr %x\n", i, procTable[i].ntraces, addr, (it->frameNumber << PAGESIZEBITS) | offset);
					continue;
				}

				//search failed
				temp = hashtable[idx].head;
				//previous oldestFrame
				lruVpn = oldestFrame->virtualPageNumber;
				lruProcId = oldestFrame->pid;
				frameNum = oldestFrame->number;
				//update recentFrame
				oldestFrame->virtualPageNumber = copyVpn;
				oldestFrame->pid = copyProcId;
				//oldestFrame becomes recentFrame so update oldestFrame to oldestFrame->lruRight
				oldestFrame = oldestFrame->lruRight;
				copyProcId = lruProcId;
				copyVpn = lruVpn;
				//create hashtable
				hashtable[idx].head = (struct invertedPageTableEntry *)malloc(sizeof(struct invertedPageTableEntry));
				hashtable[idx].head->frameNumber = frameNum;
				hashtable[idx].head->pid = procTable[i].pid;
				hashtable[idx].head->virtualPageNumber = vpn;
				//insert_front
				hashtable[idx].head->next = temp;
				if (copyProcId != -1 && copyVpn != -1)
				{
					prev = NULL;
					idx = hash(copyProcId, copyVpn);
					cur = hashtable[idx].head;
					//search
					while (1)
					{
						if (cur->pid == copyProcId && cur->virtualPageNumber == copyVpn)
							break;
						prev = cur;
						cur = cur->next;
					}
					//head
					if (prev == NULL)
					{
						if (cur->next == NULL)
							hashtable[idx].head = NULL;
						else
							hashtable[idx].head = cur->next;
						free(cur);
					}
					else
					{
						if (cur->next == NULL)
							prev->next = NULL;
						else
							prev->next = cur->next;
						free(cur);
					}
				}
				procTable[i].numPageFault++;
			}
			//printf("IHT procID %d traceNumber %d virtual addr %x pysical addr %x\n", i, procTable[i].ntraces, addr, (frameNum << PAGESIZEBITS) | offset);
		}
		if (numOfByte == EOF)
			break;
	}

	for (i = 0; i < numProcess; i++) {
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
}

int main(int argc, char *argv[]) {
	int i;
	firstLevelBits = atoi(argv[1]);
	phyMemSizeBits = atoi(argv[2]);
	secondLevelBits = 20 - firstLevelBits; //offset = PAGESIZEBITS
	numProcess = argc - 3;

	if (argc < 4) {
		printf("Usage : %s firstLevelBits PhysicalMemorySizeBits TraceFileNames\n", argv[0]); exit(1);
	}

	if (phyMemSizeBits < PAGESIZEBITS) {
		printf("PhysicalMemorySizeBits %d should be larger than PageSizeBits %d\n", phyMemSizeBits, PAGESIZEBITS); exit(1);
	}

	if (VIRTUALADDRBITS - PAGESIZEBITS - firstLevelBits <= 0) {
		printf("firstLevelBits %d is too Big\n", firstLevelBits); exit(1);
	}

	for (i = 0; i < numProcess; i++) {
		// opening a tracefile for the process
		printf("process %d opening %s\n", i, argv[i + 3]);
	}

	nFrame = (1 << (phyMemSizeBits - PAGESIZEBITS)); assert(nFrame > 0);

	printf("\nNum of Frames %d Physical Memory Size %ld bytes\n", nFrame, (1L << phyMemSizeBits));

	printf("=============================================================\n");
	printf("The 2nd Level Page Table Memory Simulation Starts .....\n");
	printf("=============================================================\n");

	// initialize procTable
	struct procEntry * procTable = (struct procEntry *)malloc(sizeof(struct procEntry)*numProcess);
	for (i = 0; i < numProcess; i++)
	{
		int j;
		procTable[i].traceName = argv[i + 3];
		procTable[i].ntraces = 0;
		procTable[i].num2ndLevelPageTable = 0;
		procTable[i].numPageFault = 0;
		procTable[i].numPageHit = 0;
		procTable[i].firstLevelPageTable = (struct pageTableEntry *)malloc(sizeof(struct pageTableEntry) * (1 << firstLevelBits));
		for (j = 0; j < (1 << firstLevelBits); j++)
			procTable[i].firstLevelPageTable[j].secondLevelPageTable = NULL;
		procTable[i].tracefp = fopen(procTable[i].traceName, "r");
	}
	//initialize physical memory
	phyMem = (struct framePage *)malloc(sizeof(struct framePage) * nFrame);
	initPhyMem(phyMem, nFrame);
	//simulation starts
	secondLevelVMSim(procTable, phyMem);
	//delete procTable
	for (i = 0; i < numProcess; i++)
	{
		int j;
		fclose(procTable[i].tracefp);
		for (j = 0; j < (1 << firstLevelBits); j++)
			if (procTable[i].firstLevelPageTable[j].secondLevelPageTable != NULL)
				free(procTable[i].firstLevelPageTable[j].secondLevelPageTable);
		free(procTable[i].firstLevelPageTable);
	}
	free(procTable);

	/*
	for(i = 0; i < numProcess; i++) {
	// rewind tracefiles
	rewind(procTable[i].tracefp);
	}
	*/

	printf("=============================================================\n");
	printf("The Inverted Page Table Memory Simulation Starts .....\n");
	printf("=============================================================\n");

	//initialize procTable
	procTable = (struct procEntry *)malloc(sizeof(struct procEntry) * numProcess);
	for (i = 0; i < numProcess; i++)
	{
		procTable[i].traceName = argv[i + 3];
		procTable[i].ntraces = 0;
		procTable[i].num2ndLevelPageTable = 0;
		procTable[i].numPageFault = 0;
		procTable[i].numPageHit = 0;
		procTable[i].pid = i;
		procTable[i].numIHTConflictAccess = 0;
		procTable[i].numIHTNonNULLAcess = 0;
		procTable[i].numIHTNULLAccess = 0;
		procTable[i].tracefp = fopen(procTable[i].traceName, "r");
	}
	//initialize hash table
	hashtable = (struct HT *)malloc(sizeof(struct HT) * nFrame);
	for (i = 0; i < nFrame; i++)
		hashtable[i].head = NULL;
	//initialize physical memory
	initPhyMem(phyMem, nFrame);
	//simulation starts
	invertedPageVMSim(procTable, phyMem, nFrame);
	//delete procTable, hashTable, and phyMem
	for (i = 0; i < numProcess; i++)
		fclose(procTable[i].tracefp);
	free(procTable);
	for (i = 0; i < nFrame; i++)
		if (hashtable[i].head)
		{
			struct invertedPageTableEntry *cur, *prev;
			cur = hashtable[i].head;
			while (cur)
			{
				prev = cur;
				cur = cur->next;
				free(prev);
			}
		}
	free(phyMem);
	return(0);
}


