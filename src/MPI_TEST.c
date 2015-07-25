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
#include <limits.h>
#include <mpi.h>
#include <time.h>
#include <sys/time.h>

#define NUMBEROFTWEETS 17000000
#define NUMBER_OF_BUCKETS 8
#define NUMBER_OF_NODES 1

char *const OUTPUT_FILE_PATH = "../Debug/out3.txt";
//char* FILE_PATH = "/usr/local/ss15psys/2097152tweets.0";
char FILE_PATH[8][255] = {
		"/usr/local/ss15psys/2097152tweets.0",
		"/usr/local/ss15psys/2097152tweets.1",
		"/usr/local/ss15psys/2097152tweets.2",
		"/usr/local/ss15psys/2097152tweets.3",
		"/usr/local/ss15psys/2097152tweets.4",
		"/usr/local/ss15psys/2097152tweets.5",
		"/usr/local/ss15psys/2097152tweets.6",
		"/usr/local/ss15psys/2097152tweets.7"
	};

//char FILE_PATH[8][255] = {
//		"../Debug/2097152tweets.0",
//		"../Debug/2097152tweets.1",
//		"../Debug/2097152tweets.2",
//		"../Debug/2097152tweets.3",
//		"../Debug/2097152tweets.4",
//		"../Debug/2097152tweets.5",
//		"../Debug/2097152tweets.6",
//		"../Debug/2097152tweets.7"
//	};




char* KEYWORD = "la";

unsigned long getIntervalDelta(unsigned long numberOfTweets, int nodes){
    return numberOfTweets/nodes;
}

int main(int argc, char* argv[]) {
	int my_rank; /* rank of process */
	int world_rank; /* number of processes */
	//int source; /* rank of sender */
	//int dest; /* rank of receiver */
	//int tag = 0; /* tag for messages */
	//char message[100]; /* storage for message */
	MPI_Status status; /* return status for receive */

	// MPI Initializations
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_rank);

    printf("My Rank: %d\n",my_rank);
    printf("World Rank: %d\n",world_rank);

	// Show keyword
	printf("Tweets will be sorted by keyword '%s'.\n",KEYWORD);

	struct timeval time1, time2;
	unsigned long long timeToReadAndInit = 0l, timeToSort = 0l, timeToWrite = 0l, timeForAll = 0l,
			microsec1, microsec2;

	

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
	unsigned long numberOfTweets = 0;
	// File Initializations
	for(int i = 0; i < 1; i++){
		FILE *fp;
		fp = fopen(FILE_PATH[i], "rb, ccs=UTF-8");
		if (fp == NULL) {
	        fprintf(stderr,"file not accessible!\n");
            MPI_Finalize();
	        exit(EXIT_FAILURE);
		}

		gettimeofday(&time1, NULL);
		microsec1 = ((unsigned long long) time1.tv_sec * 1000000) + time1.tv_usec;
	    unsigned long index = 0;
		while (!feof(fp)) {
			unsigned char *line = readLine(fp);

			td = parseTweet(line, KEYWORD);
	        td->index = index++;
			test[numberOfTweets] = *td;
			numberOfTweets++;
		}
		fclose(fp);
	}
	// Calc time
	gettimeofday(&time2, NULL);
	microsec2 = ((unsigned long long) time2.tv_sec * 1000000) + time2.tv_usec;
	timeToReadAndInit = (microsec2 - microsec1) / 1000000;

	gettimeofday(&time1, NULL);
	microsec1 = ((unsigned long long) time1.tv_sec * 1000000) + time1.tv_usec;

	//tweetData_t *pivotSet = (tweetData_t *) malloc(8 * sizeof(tweetData_t));

    unsigned long delta = getIntervalDelta(numberOfTweets, world_rank);
//    unsigned long delta = getIntervalDelta(numberOfTweets, 1);

    //start:(delta * rank) - delta      ende:delta * rank
    int rank = 0;
    unsigned long **indexes = 0;
    indexes = (unsigned long **) bucketSort(test, (size_t) numberOfTweets, NUMBER_OF_BUCKETS,delta * my_rank,delta * my_rank + delta);
//    indexes = (unsigned long **) bucketSort(test, (size_t) numberOfTweets, NUMBER_OF_BUCKETS,delta * 0,delta * 0 + delta);

	gettimeofday(&time2, NULL);
	microsec2 = ((unsigned long long) time2.tv_sec * 1000000) + time2.tv_usec;
	timeToSort = (microsec2 - microsec1) / 1000000;


	gettimeofday(&time1, NULL);
	microsec1 = ((unsigned long long) time1.tv_sec * 1000000) + time1.tv_usec;



	unsigned long sizePerBucket[NUMBER_OF_BUCKETS];
	for (int l = 0; l < NUMBER_OF_BUCKETS; l++) {
		unsigned long j = 0;
		while(indexes[l][j] != ULONG_MAX) {
			j++;
		}
		sizePerBucket[l] = j;
	}

    int bucketsPerNode = NUMBER_OF_BUCKETS / world_rank;

    if (world_rank != 1) {
        printf("Starting communication\n");
        if (my_rank == 0 || my_rank % 2 == 0) {
            for (int i = 0; i < bucketsPerNode; ++i) {
                MPI_Send(&sizePerBucket[i], sizeof(unsigned long), MPI_UNSIGNED_LONG, (my_rank + 1) % world_rank, 0, MPI_COMM_WORLD);
                MPI_Send(&indexes[i], sizePerBucket[i], MPI_UNSIGNED_LONG, (my_rank + 1) % world_rank, 0,
                         MPI_COMM_WORLD);
            }
        } else {
            for (int i = 0; i < bucketsPerNode; ++i) {
                unsigned long increaseBucketSizeBy = 0;
                MPI_Recv(&increaseBucketSizeBy, 1, MPI_UNSIGNED_LONG, (my_rank-1) % world_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                sizePerBucket[i] += increaseBucketSizeBy;
                indexes[i] = realloc(indexes[i],sizePerBucket[i] );
                MPI_Recv(&indexes[i], 1, MPI_UNSIGNED_LONG, (my_rank - 1) % world_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
        printf("Finnished the first part of communication\n");
        if (my_rank == 0 || my_rank % 2 == 1) {
            for (int i = 0; i < bucketsPerNode; ++i) {
                MPI_Send(&sizePerBucket[i], sizeof(unsigned long), MPI_UNSIGNED_LONG, (my_rank + 1) % world_rank, 0, MPI_COMM_WORLD);
                MPI_Send(&indexes[i], sizePerBucket[i], MPI_UNSIGNED_LONG, (my_rank + 1) % world_rank, 0,
                         MPI_COMM_WORLD);
            }
        } else {
            for (int i = 0; i < bucketsPerNode; ++i) {
                unsigned long increaseBucketSizeBy = 0;
                MPI_Recv(&increaseBucketSizeBy, 1, MPI_UNSIGNED_LONG, (my_rank-1) % world_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                sizePerBucket[i] += increaseBucketSizeBy;
                indexes[i] = realloc(indexes[i],sizePerBucket[i] );
                MPI_Recv(&indexes[i], 1, MPI_UNSIGNED_LONG, (my_rank - 1) % world_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
        printf("Finnished communication\n");
//        for (int i = 0; i < world_rank; i++) {
//
//        }
    } else {

    }

	/*
	 * ##################
	 * Exchange data!!
	 * sizePerBucket Update!!
	 * ##################
	 */

    tweetData_t **t = (tweetData_t **) malloc(NUMBER_OF_BUCKETS * sizeof(tweetData_t *));
    for (int k = 0; k < NUMBER_OF_BUCKETS; k++) {
        t[k] = (tweetData_t *) malloc(sizePerBucket[k] * sizeof(tweetData_t));
    }

    for (int i = 0; i < NUMBER_OF_BUCKETS; i++) {
		for (unsigned long k = 0; k < sizePerBucket[i]; k++) {
			t[i][k] = test[indexes[i][k]];
		}
    }

	for (int m = 0; m < NUMBER_OF_BUCKETS; m++) {
		quickSort(t[m],sizePerBucket[m]);
	}

    FILE *f = fopen(OUTPUT_FILE_PATH, "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

	/* print some text */
    for (int i = 0; i < NUMBER_OF_BUCKETS; i++) {
        unsigned long j = 0;
        for (int k = 0; k < sizePerBucket[i] ; k++) {
			fprintf(f, "%s\n", t[i][k].line);
           j++;
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

    MPI_Finalize();
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
