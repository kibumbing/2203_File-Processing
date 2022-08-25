#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"


int readRecord(FILE *fp, STUDENT *s, int rrn);
void unpack(const char *recordbuf, STUDENT *s);
int writeRecord(FILE *fp, const STUDENT *s, int rrn);
void pack(char *recordbuf, const STUDENT *s);
int appendRecord(FILE *fp, char *id, char *name, char *dept, char *addr, char *email);
void searchRecord(FILE *fp, enum FIELD f, char *keyval);
void printRecord(const STUDENT *s);
enum FIELD getFieldID(char *fieldname);
int deleteRecord(FILE *fp, enum FIELD f, char *keyval);
int insertRecord(FILE *fp, char *id, char *name, char *dept, char *addr, char *email);

int readRecord(FILE *fp, STUDENT *s, int rrn)
{
	int ret;
	char *recordbuf = (void*)malloc(RECORD_SIZE);
	fseek(fp, HEADER_SIZE+RECORD_SIZE*rrn, SEEK_SET);
	ret = fread((void*)recordbuf, RECORD_SIZE, 1, fp);
	unpack(recordbuf, s);
	free(recordbuf);
	if(ret == 1){
		return 1;
	}
	else{
		return 0;	
	}
}

void unpack(const char *recordbuf, STUDENT *s)
{
	char buf[RECORD_SIZE];
	strcpy(buf, recordbuf);
	char *tmp = strtok(buf,"#");
	strcpy(s->id,tmp);
	tmp = strtok(NULL,"#");
	strcpy(s->name,tmp);
	tmp = strtok(NULL,"#");
	strcpy(s->dept,tmp);
	tmp = strtok(NULL,"#");
	strcpy(s->addr,tmp);
	tmp = strtok(NULL,"#");
	strcpy(s->email,tmp);
}

int writeRecord(FILE *fp, const STUDENT *s, int rrn)
{
	int ret;
	char *recordbuf = (char*)malloc(RECORD_SIZE);
	pack(recordbuf, s);
	fseek(fp, HEADER_SIZE+RECORD_SIZE*rrn, SEEK_SET);
	ret = fwrite(recordbuf, RECORD_SIZE, 1, fp);
	free(recordbuf);
	if(ret == 1){
		return 1;
	}
	else{
		return 0;	
	}
}

void pack(char *recordbuf, const STUDENT *s)
{	
	int buf_count = 0;
	int part_count = 0;
	while(s->id[part_count] != '#'){
		memcpy(recordbuf+buf_count, (s->id)+part_count, 1);
		buf_count++;
		part_count++;
	}
	memcpy(recordbuf+buf_count, "#", 1);
	buf_count++;
	part_count = 0;
	while(s->name[part_count] != '#'){
		memcpy(recordbuf+buf_count, (s->name)+part_count, 1);
		buf_count++;
		part_count++;
	}
	memcpy(recordbuf+buf_count, "#", 1);
	buf_count++;
	part_count = 0;
	while(s->dept[part_count] != '#'){
		memcpy(recordbuf+buf_count, (s->dept)+part_count, 1);
		buf_count++;
		part_count++;
	}
	memcpy(recordbuf+buf_count, "#", 1);
	buf_count++;
	part_count = 0;
	while(s->addr[part_count] != '#'){
		memcpy(recordbuf+buf_count, (s->addr)+part_count, 1);
		buf_count++;
		part_count++;
	}
	memcpy(recordbuf+buf_count, "#", 1);
	buf_count++;
	part_count = 0;
	while(s->email[part_count] != '#'){
		memcpy(recordbuf+buf_count, (s->email)+part_count, 1);
		buf_count++;
		part_count++;
	}
	memcpy(recordbuf+buf_count, "#", 1);
}

int appendRecord(FILE *fp, char *id, char *name, char *dept, char *addr, char *email)
{
	int ret = 0;
	fseek(fp, 0, SEEK_END);
	int size_of_file = ftell(fp);
	char *headerbuf = (void*)malloc(HEADER_SIZE);
	int records = 0;
	if(size_of_file==0){
		records = 1;
		int reserved = -1;
		memcpy(headerbuf, &records,  sizeof(records));
		memcpy(headerbuf+sizeof(records), &reserved, sizeof(reserved));
		fseek(fp, 0, SEEK_SET);
		fwrite(headerbuf, HEADER_SIZE, 1, fp);
	}
	else{
		fseek(fp, 0, SEEK_SET);
		fread(headerbuf, HEADER_SIZE, 1, fp);
		memcpy(&records, headerbuf, sizeof(int));
		records++;
		memcpy(headerbuf, &records, sizeof(int));
		fseek(fp, 0, SEEK_SET);
		fwrite(headerbuf, HEADER_SIZE, 1, fp);
	}
	
	
	STUDENT sdata;
	memcpy(sdata.id, id, strlen(id));
	memcpy((sdata.id)+strlen(id), "#", 1);
	memcpy(sdata.name, name, strlen(name));
	memcpy((sdata.name)+strlen(name), "#", 1);
	memcpy(sdata.dept, dept, strlen(dept));
	memcpy((sdata.dept)+strlen(dept), "#", 1);
	memcpy(sdata.addr, addr, strlen(addr));
	memcpy((sdata.addr)+strlen(addr), "#", 1);
	memcpy(sdata.email, email, strlen(email));
	memcpy((sdata.email)+strlen(email), "#", 1);

	ret = writeRecord(fp, &sdata, records-1);
	
	fclose(fp);

	free(headerbuf);

	return ret;
}

void searchRecord(FILE *fp, enum FIELD f, char *keyval){
	
	char *headerbuf = (void*)malloc(HEADER_SIZE);
	int records;
	fseek(fp, 0, SEEK_SET);
	fread(headerbuf, HEADER_SIZE, 1, fp);
	memcpy(&records, headerbuf, sizeof(int));
	
	for(int i=0; i<records; i++){
		char buf;
		fseek(fp, HEADER_SIZE+RECORD_SIZE*i, SEEK_SET);
		fread(&buf, 1, 1, fp);
		if(buf=='*')
			continue;

		STUDENT sdata;
		readRecord(fp, &sdata, i);

		switch(f){
			case 0:
				if(strcmp(sdata.id, keyval)==0){
					printRecord(&sdata);
				}
				break;
			case 1:
				if(strcmp(sdata.name, keyval)==0){
					printRecord(&sdata);
				}
				break;
			case 2:
				if(strcmp(sdata.dept, keyval)==0){
					printRecord(&sdata);
				}
				break;
			case 3:
				if(strcmp(sdata.addr, keyval)==0){
					printRecord(&sdata);
				}
				break;
			case 4:
				if(strcmp(sdata.email, keyval)==0){
					printRecord(&sdata);
				}
				break;
		}
	}
	fclose(fp);
	free(headerbuf);
}

enum FIELD getFieldID(char *fieldname){
	enum FIELD key;
	if(strcmp(fieldname, "ID")==0){
		key = ID;
	}
	else if(strcmp(fieldname, "NAME")==0){
		key = NAME;
	}
	else if(strcmp(fieldname, "DEPT")==0){
		key = DEPT;
	}
	else if(strcmp(fieldname, "ADDR")==0){
		key = ADDR;
	}
	else if(strcmp(fieldname, "EMAIL")==0){
		key = EMAIL;
	}
	return key;
}

int deleteRecord(FILE *fp, enum FIELD f, char *keyval){
	
	int ret = 0;
	if(f!=0)
		return ret;
	
	int records;
	int del_index;
	fseek(fp, 0, SEEK_SET);
	fread(&records, sizeof(int), 1, fp);
	fread(&del_index, sizeof(int), 1, fp);

	for(int i=0; i<records; i++){
		char buf;
		fseek(fp, HEADER_SIZE+RECORD_SIZE*i, SEEK_SET);
		fread(&buf, 1, 1, fp);
		if(buf=='*')
			continue;

		STUDENT sdata;
		readRecord(fp, &sdata, i);
		if(strcmp(sdata.id, keyval)==0){
				char* record_buf = (void*)calloc(1, 5);
				memcpy(record_buf, "*", 1);
				memcpy(record_buf+1, &del_index, 4);
				fseek(fp, HEADER_SIZE+RECORD_SIZE*i, SEEK_SET);
				fwrite(record_buf, 5, 1, fp);
				free(record_buf);

				del_index = i;
				fseek(fp, 4, SEEK_SET);
				fwrite(&del_index,  sizeof(int), 1, fp);

				ret = 1;
		}
	}
	fclose(fp);

	return ret;
}

int insertRecord(FILE *fp, char *id, char *name, char *dept, char *addr, char *email){
	int ret = 0;
	
	int del_index;

	fseek(fp, sizeof(int), SEEK_SET);
	fread(&del_index, sizeof(int), 1, fp);

	if(del_index != -1){
		int temp = del_index;
		
		fseek(fp, HEADER_SIZE+RECORD_SIZE*del_index+1, SEEK_SET);
		fread(&del_index, sizeof(int), 1, fp);
		fseek(fp, 4, SEEK_SET);
		fwrite(&del_index, sizeof(int), 1, fp);

		STUDENT sdata;
		memcpy(sdata.id, id, strlen(id));
		memcpy((sdata.id)+strlen(id), "#", 1);
		memcpy(sdata.name, name, strlen(name));
		memcpy((sdata.name)+strlen(name), "#", 1);
		memcpy(sdata.dept, dept, strlen(dept));
		memcpy((sdata.dept)+strlen(dept), "#", 1);
		memcpy(sdata.addr, addr, strlen(addr));
		memcpy((sdata.addr)+strlen(addr), "#", 1);
		memcpy(sdata.email, email, strlen(email));
		memcpy((sdata.email)+strlen(email), "#", 1);

		ret = writeRecord(fp, &sdata, temp);
		
		fclose(fp);
	}
	else{
		ret = appendRecord(fp, id, name, dept, addr, email);
	}

	return ret;	
}


void main(int argc, char *argv[])
{
	FILE *fp;
	if(strcmp(argv[1], "-a")==0){
		if(!(fp = fopen(argv[2], "r+b"))){
			fp = fopen(argv[2], "w+b");
		}
		appendRecord(fp, argv[3], argv[4], argv[5], argv[6], argv[7]);  
	}
	else if(strcmp(argv[1], "-s")==0){
		fp = fopen(argv[2], "rb");
		char *key = strtok(argv[3], "=");
		char *value = strtok(NULL, " ");
		searchRecord(fp, getFieldID(key), value);	
	}
	else if(strcmp(argv[1], "-d")==0){
		fp = fopen(argv[2], "r+b");
		char *key = strtok(argv[3], "=");
		char *value = strtok(NULL, " ");
		deleteRecord(fp, getFieldID(key), value);
	}
	else if(strcmp(argv[1], "-i")==0){
		fp = fopen(argv[2], "r+b");
		insertRecord(fp, argv[3], argv[4], argv[5], argv[6], argv[7]);  
	}
	else{
		printf("Wrong Input.");
	}
}

void printRecord(const STUDENT *s)
{
	printf("%s | %s | %s | %s | %s\n", s->id, s->name, s->dept, s->addr, s->email);
}
