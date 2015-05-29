/*
 ============================================================================
 Name        : MPI_TEST.c
 Author      : TomSchubert
 Version     :
 Copyright   : 
 Description : Hello MPI World in C 
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"
#include "jsmn.h"
#include "fileHandler.h"
#include <locale.h>
#include <wchar.h>

int main(int argc, char* argv[]){
	int  my_rank; /* rank of process */
	int  p;       /* number of processes */
	int source;   /* rank of sender */
	int dest;     /* rank of receiver */
	int tag=0;    /* tag for messages */
	char message[100];        /* storage for message */
	MPI_Status status ;   /* return status for receive */
	
	// MPI Initializations
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	
	// File Initializations
	FILE *fp;
	fp = _wfopen("./twitter-klein.json", "rb+, ccs=UTF-8");
	if (fp == NULL) {
		printf("file not accessible!\n");
		exit(EXIT_FAILURE);
	}

	int errorCode = 0;
	if (my_rank == 0){
		/* create message */
		sprintf(message, "Hello MPI World from process %d!", my_rank);
		dest = 0;

		int i = 0;
		wchar_t c;
		while (!feof(fp)) {
			wchar_t *line = readLine(fp);
			wprintf(L"%ls\n", line);


			c = *line;
			while (c != 0) {
				wprintf(L"%lc-", c);
				c = *line++;
			}

			i++;
			printf("\n");
			//printf("%c", line);

			if (i == 5)
				break;

		}

		MPI_Finalize();
		return 0;

		/* use strlen+1 so that '\0' get transmitted */
		//MPI_Send(message, strlen(message)+1, MPI_CHAR,
		//   dest, tag, MPI_COMM_WORLD);
	}
	else{
		printf("Hello MPI World From process 0: Num processes: %d\n",p);
		for (source = 1; source < p; source++) {
			MPI_Recv(message, 100, MPI_CHAR, source, tag,
			      MPI_COMM_WORLD, &status);
			printf("%s\n",message);
		}
	}
	/* shut down MPI */
	MPI_Finalize(); 
	
	
	return 0;
}
