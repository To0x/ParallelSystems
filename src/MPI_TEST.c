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
#include "fileHandler.h"
#include <locale.h>
#include <wchar.h>
#include "dataHolder.h"

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
	fp = fopen("./65536tweets.0", "rb+, ccs=UTF-8");
	if (fp == NULL) {
		printf("file not accessible!\n");
		exit(EXIT_FAILURE);
	}

	int errorCode = 0;
	if (my_rank == 0){
		/* create message */
		sprintf(message, "Hello MPI World from process %d!", my_rank);
		dest = 0;


		//byte *test = readTweets("./");
		//////////


		//int i = 0;
		unsigned char c;
		unsigned char actInt;

		struct tweetData *td;

		while (!feof(fp)) {
			unsigned char *line = readLine(fp);

			/*
			while ((c = *line) != '\0') {
				//actInt = (uint16_t) c;

		        actInt = (unsigned char)c;

		        printf("%c - ", c);
		        printf("%u", actInt);

		        printf("\n");

				line++;
			}*/


			td = parseTweet(line, "test");

			printf("%s - Hashtags: %d, Smileys: %d\n", line, td->hashtags, td->smiles);


			//i++;
			//if (i==5)
			//	break;

			/*
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
			*/
		}

		MPI_Finalize();
		return 0;

		// use strlen+1 so that '\0' get transmitted
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
