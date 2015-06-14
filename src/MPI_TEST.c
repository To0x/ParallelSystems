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
//#include "mpi.h"
#include <locale.h>
#include <wchar.h>
#include "dataHolder.h"
#include "fileHandler.h"
#include "bucketSort.h"
#include <time.h>
#include <sys/time.h>

#define NUMBEROFTWEETS 2500000

int main(int argc, char* argv[]) {
//	int my_rank; /* rank of process */
	//	int p; /* number of processes */
	//int source; /* rank of sender */
	//int dest; /* rank of receiver */
	//int tag = 0; /* tag for messages */
	//char message[100]; /* storage for message */
	//MPI_Status status; /* return status for receive */

	// MPI Initializations
	//MPI_Init(&argc, &argv);
	//MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	//MPI_Comm_size(MPI_COMM_WORLD, &p);

	// Show keyword
	printf("Tweets will be sorted by keyword 'la'.\n");

	// File Initializations
	FILE *fp;
	fp = fopen("./65536tweets.0", "rb+, ccs=UTF-8");
	if (fp == NULL) {
		printf("file not accessible!\n");
		exit (EXIT_FAILURE);
	}

	int errorCode = 0;
	//	if (my_rank == 0) {
	/* create message */
	//sprintf(message, "Hello MPI World from process %d!", my_rank);
	//	dest = 0;
	//byte *test = readTweets("./");
	//////////
	unsigned char c;
	unsigned char actInt;

	struct tweetData *td;
	printf("try to allocate %lu kbytes\n",
			(sizeof(struct tweetData*) * NUMBEROFTWEETS) / 1000);
	struct tweetData *test = (struct tweetData*) malloc(
			sizeof(struct tweetData) * NUMBEROFTWEETS);
	fflush (stdout);

	time_t t1 = time(NULL);
	long numberOfTweets = 0;

	struct timeval time1;
	gettimeofday(&time1, NULL);
	long microsec1 = ((unsigned long long) time1.tv_sec * 1000000)
			+ time1.tv_usec;

	long long allocated = 0;
	while (!feof(fp)) {
		unsigned char *line = readLine(fp);

		if (numberOfTweets == (long) NUMBEROFTWEETS - 1) {
			printf("\n\n");
			printf("Should break now...\n");
			printf("\n\n");
			break;
		}

		td = parseTweet(line, "la");
		allocated += td->size;

		// Usefull for debugging:
//		if (numberOfTweets % 1000 == 0) {
//			printf("%li: %s - Hashtags: %d, Smileys: %d, Keywords %d\n",
//					numberOfTweets, line, td->hashtags, td->smiles,
//					td->keywords);
//			printf("%ld: allocated: %lld Mbytes\n", numberOfTweets,
//					(allocated / 1000000));
//			fflush(stdout);
//		}

		test[numberOfTweets] = *td;
		numberOfTweets++;
	}

	// Calc time
	struct timeval time2;
	gettimeofday(&time2, NULL);
	long microsec2 = ((unsigned long long) time2.tv_sec * 1000000)
			+ time2.tv_usec;
	time_t t2 = time(NULL);
	float timeToSort = quickSort(test, NUMBEROFTWEETS + 1);

	
	FILE *f = fopen("out.txt", "w");
	if (f == NULL) {
		printf("Error opening file!\n");
		exit(1);
	}
	
	for (int j = 0; j < NUMBEROFTWEETS; j += 1) {
		if (test[j].line != NULL) {
//			printf("%d: %s - Hashtags: %d, Smileys: %d, Keywords: %d\n", j,
//					test[j].line, test[j].hashtags, test[j].smiles,
//					test[j].keywords);

			/* print some text */
			fprintf(f, "%s\n", test[j].line);
		}
	}

	printf("einlesen diff: %ld msec\n", (long) (microsec2 - microsec1) / 1000);
	printf("sortieren %8.4f\n", timeToSort);

//	MPI_Finalize();
	return 0;

// use strlen+1 so that '\0' get transmitted
// MPI_Send(message, strlen(message)+1, MPI_CHAR,
//   dest, tag, MPI_COMM_WORLD);

	/*	} else {
	 printf("Hello MPI World From process 0: Num processes: %d\n", p);
	 for (source = 1; source < p; source++) {
	 MPI_Recv(message, 100, MPI_CHAR, source, tag, MPI_COMM_WORLD,
	 &status);
	 printf("%s\n", message);
	 }
	 }
	 /* shut down MPI */
//	MPI_Finalize();
	return 0;
}
