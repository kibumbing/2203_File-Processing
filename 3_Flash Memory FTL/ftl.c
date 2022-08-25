#define PRINT_FOR_DEBUG

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <time.h>
#include "blkmap.h"

AddrMapTbl addrmaptbl;
extern FILE *devicefp;
int reserved_empty_blk = DATABLKS_PER_DEVICE;

/****************  prototypes ****************/
void ftl_open();
void ftl_write(int lsn, char *sectorbuf);
void ftl_read(int lsn, char *sectorbuf);
void print_block(int pbn);
void print_addrmaptbl();


void ftl_open()
{
	int i;

	// initialize the address mapping table
	for(i = 0; i < DATABLKS_PER_DEVICE; i++)
	{
		addrmaptbl.pbn[i] = -1;
	}


	return;
}

void ftl_write(int lsn, char *sectorbuf)
{
#ifdef PRINT_FOR_DEBUG
	print_addrmaptbl();
#endif
	int lbn = lsn / PAGES_PER_BLOCK;
	int offset = lsn % PAGES_PER_BLOCK;

	char *pagebuf = (char*)malloc(PAGE_SIZE);
	memcpy(pagebuf, sectorbuf, SECTOR_SIZE);
	

	SpareData *sdata = (SpareData*)malloc(SPARE_SIZE);
	sdata->lsn = lsn;
	memcpy(pagebuf+SECTOR_SIZE, sdata, SPARE_SIZE);

	char *readbuf = (char*)malloc(PAGE_SIZE);

	if(addrmaptbl.pbn[lbn] == -1){
		int count = 0;
		while(count != PAGES_PER_BLOCK){
			count = 0;
			addrmaptbl.pbn[lbn] = rand()%(DATABLKS_PER_DEVICE-1);
			for(int i=0; i<PAGES_PER_BLOCK; i++){
				dd_read(addrmaptbl.pbn[lbn]*PAGES_PER_BLOCK+i, readbuf);
				memcpy(sdata, readbuf+SECTOR_SIZE, SPARE_SIZE);
				if(sdata->lsn == -1)
					count++;
			}
		}
	}

	dd_read(addrmaptbl.pbn[lbn]*PAGES_PER_BLOCK+offset, readbuf);
	memcpy(sdata, readbuf+SECTOR_SIZE, SPARE_SIZE);
	if(sdata->lsn != -1){
		for(int i=0; i<PAGES_PER_BLOCK; i++){
			dd_read(addrmaptbl.pbn[lbn]*PAGES_PER_BLOCK+i, readbuf);
			dd_write(reserved_empty_blk*PAGES_PER_BLOCK+i, readbuf);		
		}
		dd_erase(addrmaptbl.pbn[lbn]);
		dd_write(addrmaptbl.pbn[lbn]*PAGES_PER_BLOCK+offset, pagebuf);
		for(int i=0; i<PAGES_PER_BLOCK; i++){
			if(i != offset){
				dd_read(reserved_empty_blk*PAGES_PER_BLOCK+i, readbuf);
				dd_write(addrmaptbl.pbn[lbn]*PAGES_PER_BLOCK+i, readbuf);
			}		
		}
		int count = 0;
		while(count != PAGES_PER_BLOCK){
			count = 0;
			reserved_empty_blk = rand()%(DATABLKS_PER_DEVICE-1);
			for(int i=0; i<PAGES_PER_BLOCK; i++){
				dd_read(reserved_empty_blk*PAGES_PER_BLOCK+i, readbuf);
				memcpy(sdata, readbuf+SECTOR_SIZE, SPARE_SIZE);
				if(sdata->lsn == -1)
					count++;
			}
		}
	}
	else{
		dd_write(addrmaptbl.pbn[lbn]*PAGES_PER_BLOCK+offset, pagebuf);
	}


	free(pagebuf);
	free(sdata);
	free(readbuf);

	return;
}


void ftl_read(int lsn, char *sectorbuf)
{
#ifdef PRINT_FOR_DEBUG	
	print_addrmaptbl();
#endif
	int lbn = lsn / PAGES_PER_BLOCK;
	int offset = lsn % PAGES_PER_BLOCK;

	char *readbuf = (char*)malloc(PAGE_SIZE);

	dd_read(addrmaptbl.pbn[lbn]*PAGES_PER_BLOCK+offset, readbuf);
	memcpy(sectorbuf, readbuf, SECTOR_SIZE);

	free(readbuf);
	
	return;
}

//
// for debugging
//
void print_block(int pbn)
{
	char *pagebuf;
	SpareData *sdata;
	int i;
	
	pagebuf = (char *)malloc(PAGE_SIZE);
	sdata = (SpareData *)malloc(SPARE_SIZE);

	printf("Physical Block Number: %d\n", pbn);

	for(i = pbn*PAGES_PER_BLOCK; i < (pbn+1)*PAGES_PER_BLOCK; i++)
	{
		dd_read(i, pagebuf);
		memcpy(sdata, pagebuf+SECTOR_SIZE, SPARE_SIZE);
		printf("\t   %5d-[%7d]\n", i, sdata->lsn);
	}

	free(pagebuf);
	free(sdata);

	return;
}

//
// for debugging
//
void print_addrmaptbl()
{
	int i;

	printf("Address Mapping Table: \n");
	for(i = 0; i < DATABLKS_PER_DEVICE; i++)
	{
		if(addrmaptbl.pbn[i] >= 0)
		{
			printf("[%d %d]\n", i, addrmaptbl.pbn[i]);
		}
	}
}
