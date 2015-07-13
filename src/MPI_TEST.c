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
#include <locale.h>
#include "dataHolder.h"
#include "fileHandler.h"
#include "bucketSort.h"
#include <time.h>
#include <sys/time.h>

#define NUMBEROFTWEETS 2100000

char *const OUTPUT_FILE_PATH = "out4.txt";
char* FILE_PATH = "./65536tweets.0";
char* KEYWORD = "la";

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
	printf("Tweets will be sorted by keyword '%s'.\n",KEYWORD);

	struct timeval time1, time2;
	unsigned long long timeToReadAndInit = 0l, timeToSort = 0l, timeToWrite = 0l, timeForAll = 0l,
			microsec1, microsec2;

	// File Initializations
	FILE *fp;
	fp = fopen(FILE_PATH, "rb+, ccs=UTF-8");
	if (fp == NULL) {
        fprintf(stderr,"file not accessible!\n");
        exit(EXIT_FAILURE);
	}

	//	if (my_rank == 0) {
	/* create message */
	//sprintf(message, "Hello MPI World from process %d!", my_rank);
	//	dest = 0;
	//byte *test = readTweets("./");
	//////////

	tweetData_t *td;
	printf("try to allocate %lu kbytes\n",
			(sizeof(tweetData_t*) * NUMBEROFTWEETS) / 1000);

	tweetData_t *test = (tweetData_t*) malloc(sizeof(tweetData_t) * (NUMBEROFTWEETS+1));
	fflush (stdout);

	long numberOfTweets = 0;

	gettimeofday(&time1, NULL);
	microsec1 = ((unsigned long long) time1.tv_sec * 1000000) + time1.tv_usec;

	while (!feof(fp)) {
		unsigned char *line = readLine(fp);

		td = parseTweet(line, KEYWORD);

		test[numberOfTweets] = *td;
		numberOfTweets++;
	}
	
	fclose(fp);

	// Calc time
	gettimeofday(&time2, NULL);
	microsec2 = ((unsigned long long) time2.tv_sec * 1000000) + time2.tv_usec;
	timeToReadAndInit = (microsec2 - microsec1) / 1000000;

	gettimeofday(&time1, NULL);
	microsec1 = ((unsigned long long) time1.tv_sec * 1000000) + time1.tv_usec;

	//tweetData_t *pivotSet = (tweetData_t *) malloc(8 * sizeof(tweetData_t));
	tweetData_t* tom = getPivotElements(test, 8);
	for (int i = 0; i < 8; i++)
	{	
		printf("keyword:%d \t unicode: %llu \t count: %d\n",tom[i].keywords, tom[i].smallestUniCode, tom[i].countSmallest );
	}


	bucketSort(test, numberOfTweets, sizeof(tweetData_t));
	gettimeofday(&time2, NULL);
	microsec2 = ((unsigned long long) time2.tv_sec * 1000000) + time2.tv_usec;
	timeToSort = (microsec2 - microsec1) / 1000000;


	gettimeofday(&time1, NULL);
	microsec1 = ((unsigned long long) time1.tv_sec * 1000000) + time1.tv_usec;
	
	FILE *f = fopen(OUTPUT_FILE_PATH, "w");
	if (f == NULL)
	{
	    printf("Error opening file!\n");
	    exit(1);
	}

	/* print some text */
	for (int j = 0; j <= numberOfTweets; j++) {
		if (test[j].line != NULL) {
			fprintf(f, "%s\n", test[j].line);
		}
	}
	fclose(f);

	gettimeofday(&time2, NULL);
	microsec2 = ((unsigned long long) time2.tv_sec * 1000000) + time2.tv_usec;
	timeToWrite = (microsec2 - microsec1) / 1000000;

	timeForAll = timeToReadAndInit + timeToSort + timeToWrite;
	printf("Read & Init:\t%llu Sec.\n", timeToReadAndInit);
	printf("Sort:\t\t%llu Sec.\n", timeToSort);
	printf("Write:\t\t%llu Sec.\n", timeToWrite);
	printf("All:\t\t%llu Sec.\n", timeForAll);

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
