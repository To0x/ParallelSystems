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
#define NUMBER_OF_FILES 1

#define SEND_LENGTH_OF_INDEXES_TAG 0
#define SEND_INDEXES_TAG 1

char *const OUTPUT_FILE_PATH = "../Debug/out";
//char* FILE_PATH = "/usr/local/ss15psys/2097152tweets.0";
//char FILE_PATH[8][255] = {
//		"/usr/local/ss15psys/2097152tweets.0",
//		"/usr/local/ss15psys/2097152tweets.1",
//		"/usr/local/ss15psys/2097152tweets.2",
//		"/usr/local/ss15psys/2097152tweets.3",
//		"/usr/local/ss15psys/2097152tweets.4",
//		"/usr/local/ss15psys/2097152tweets.5",
//		"/usr/local/ss15psys/2097152tweets.6",
//		"/usr/local/ss15psys/2097152tweets.7"
//	};

char FILE_PATH[8][255] = {
		"../Debug/65536tweets.0",
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
    MPI_Init(NULL, NULL);
    int world_size; /* number of processes */
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // MPI Initializations
    int world_rank; /* rank of process */
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

//    MPI_Status status; /* return status for receive */
    printf("world_rank: %d\n", world_rank);
    printf("world_size: %d\n", world_size);

	// Show keyword
	printf("Tweets will be sorted by keyword '%s'.\n",KEYWORD);

	struct timeval time1, time2;
	unsigned long long timeToReadAndInit = 0l, timeToSortInBuckets = 0l, timeToSort = 0l,timeToCommunicate = 0l, timeToWrite = 0l, timeForAll = 0l,
			microsec1, microsec2;

	tweetData_t *td;
	printf("try to allocate %lu kbytes\n",
			(sizeof(tweetData_t*) * NUMBEROFTWEETS) / 1000);

    gettimeofday(&time1, NULL);
    tweetData_t *test = (tweetData_t*) malloc(sizeof(tweetData_t) * (NUMBEROFTWEETS+1));
    unsigned long numberOfTweets = 0;
    // File Initializations
    for(int i = 0; i < NUMBER_OF_FILES; i++){
        FILE *fp;
        fp = fopen(FILE_PATH[i], "rb, ccs=UTF-8");
        if (fp == NULL) {
            fprintf(stderr,"file not accessible!\n");
            MPI_Finalize();
            exit(EXIT_FAILURE);
        }

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
    unsigned long delta = getIntervalDelta(numberOfTweets, world_size);
    //start:(delta * rank) - delta      ende:delta * rank
    int rank = 0;
    unsigned long **indexes = 0;
    indexes = (unsigned long **) bucketSort(test, (size_t) numberOfTweets, NUMBER_OF_BUCKETS,delta * world_rank,delta *
                                                                                                                world_rank + delta);
    printf("Buckets filled\n");
	unsigned long sizePerBucket[NUMBER_OF_BUCKETS];
	for (int l = 0; l < NUMBER_OF_BUCKETS; l++) {
		unsigned long j = 0;
		while(indexes[l][j] != ULONG_MAX) {
			j++;
		}
		sizePerBucket[l] = j;
	}
    gettimeofday(&time2, NULL);
    microsec2 = ((unsigned long long) time2.tv_sec * 1000000) + time2.tv_usec;
    timeToSortInBuckets = (microsec2 - microsec1) / 1000000;


    gettimeofday(&time1, NULL);
    microsec1 = ((unsigned long long) time1.tv_sec * 1000000) + time1.tv_usec;

    int bucketsPerNode = NUMBER_OF_BUCKETS / world_size;

    if (world_size != 1) {
        printf("Starting communication\n");

        for (int i = 0; i < world_size; i++) {
            if (world_rank == i) {
                for (int j = 0; j < world_size; j++) {
                    if (j != world_rank) {
                        for (int k = (NUMBER_OF_BUCKETS / world_size)*j; k < ((NUMBER_OF_BUCKETS / world_size)*j) + (NUMBER_OF_BUCKETS / world_size); k++) {
                            MPI_Send(&sizePerBucket[k], 1, MPI_UNSIGNED_LONG, j, SEND_LENGTH_OF_INDEXES_TAG,MPI_COMM_WORLD);
                            MPI_Send(&indexes[k][0], sizePerBucket[k], MPI_UNSIGNED_LONG, j, SEND_INDEXES_TAG,MPI_COMM_WORLD);
                        }
                    }
                }
            } else {
                for (int k = (NUMBER_OF_BUCKETS / world_size)*world_rank; k < ((NUMBER_OF_BUCKETS / world_size)*world_rank) + (NUMBER_OF_BUCKETS / world_size); k++) {
                    unsigned long increaseBucketSizeBy = 0;
                    MPI_Recv(&increaseBucketSizeBy, 1, MPI_UNSIGNED_LONG, i, SEND_LENGTH_OF_INDEXES_TAG, MPI_COMM_WORLD,
                             MPI_STATUS_IGNORE);
                    unsigned long indexesFromOtherNode[increaseBucketSizeBy];
                    unsigned long oldBucketSize = sizePerBucket[k];
                    MPI_Recv(&indexesFromOtherNode, increaseBucketSizeBy, MPI_UNSIGNED_LONG, i, SEND_INDEXES_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    indexes[k] = realloc(indexes[k], (sizePerBucket[k]+increaseBucketSizeBy) * sizeof(unsigned long));
                    sizePerBucket[k] += increaseBucketSizeBy;
                    for (unsigned long j = oldBucketSize; j < sizePerBucket[k]; j++) {
                        indexes[k][j] = indexesFromOtherNode[j-oldBucketSize];
//                        printf("[Rank:%d]:  indexes[%d][%lu] --> %lu\n", world_rank, k, j, indexes[k][j]);
                    }
                    printf("[Rank:%d]: sizePerBucket[%d] is %lu\n", world_rank, k, sizePerBucket[k]);
                    fflush(stdout);
                }
            }
        }
        printf("[Rank:%d]: Finnished communication\n",world_rank);
    }
    gettimeofday(&time2, NULL);
    microsec2 = ((unsigned long long) time2.tv_sec * 1000000) + time2.tv_usec;
    timeToCommunicate = (microsec2 - microsec1) / 1000000;

    gettimeofday(&time1, NULL);
    microsec1 = ((unsigned long long) time1.tv_sec * 1000000) + time1.tv_usec;

    fflush(stdout);
    tweetData_t **t = (tweetData_t **) malloc(NUMBER_OF_BUCKETS * sizeof(tweetData_t *));
    for (int k = 0; k < NUMBER_OF_BUCKETS; k++) {
        t[k] = (tweetData_t *) malloc(sizePerBucket[k] * sizeof(tweetData_t));
    }

    fflush(stdout);
    for (int i = 0; i < NUMBER_OF_BUCKETS; i++) {
        for (unsigned long k = 0; k < sizePerBucket[i]; k++) {
            t[i][k] = test[indexes[i][k]];
        }
    }

    fflush(stdout);
    for (int k = (NUMBER_OF_BUCKETS / world_size)*world_rank; k < ((NUMBER_OF_BUCKETS / world_size)*world_rank) + (NUMBER_OF_BUCKETS / world_size); k++) {
        quickSort(t[k],sizePerBucket[k]);
    }

    gettimeofday(&time2, NULL);
    microsec2 = ((unsigned long long) time2.tv_sec * 1000000) + time2.tv_usec;
    timeToSort = (microsec2 - microsec1) / 1000000;

    gettimeofday(&time1, NULL);
    microsec1 = ((unsigned long long) time1.tv_sec * 1000000) + time1.tv_usec;

    fflush(stdout);

    char outPutFileName[50];
    sprintf (outPutFileName, "../Debug/out%d.txt", world_rank);

    FILE *f = fopen(outPutFileName, "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

	/* print some text */
    for (int i = (NUMBER_OF_BUCKETS / world_size)*world_rank; i < ((NUMBER_OF_BUCKETS / world_size)*world_rank) + (NUMBER_OF_BUCKETS / world_size); i++) {
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

    MPI_Finalize();
    timeForAll = timeToReadAndInit + timeToSort + timeToWrite + timeToCommunicate + timeToSortInBuckets;
    printf("Read & Init:\t\t%llu Sec.\n", timeToReadAndInit);
    printf("Sort in buckets:\t%llu Sec.\n", timeToSortInBuckets);
    printf("Communication:\t\t%llu Sec.\n", timeToCommunicate);
    printf("Sort:\t\t\t%llu Sec.\n", timeToSort);
    printf("Write:\t\t\t%llu Sec.\n", timeToWrite);
    printf("All:\t\t\t%llu Sec.\n", timeForAll);

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
