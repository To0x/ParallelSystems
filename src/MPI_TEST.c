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

#include "time.h"
#include "common.h"

#define NUMBEROFTWEETS 17000000
#define NUMBER_OF_BUCKETS 64
#define NUMBER_OF_NODES 8
#define NUMBER_OF_FILES 1

#define SEND_LENGTH_OF_INDEXES_TAG 0
#define SEND_INDEXES_TAG 1

char *const OUTPUT_FILE_PATH = "../Debug/out";

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
//		"./65536tweets.0",
//		"./2097152tweets.1",
//		"../Debug/2097152tweets.2",
//		"../Debug/2097152tweets.3",
//		"../Debug/2097152tweets.4",
//		"../Debug/2097152tweets.5",
//		"../Debug/2097152tweets.6",
//		"../Debug/2097152tweets.7"
//	};


char* KEYWORD = "la";

ulong getIntervalDelta(ulong numberOfTweets, int nodes){
    return numberOfTweets/nodes;
}

ulong readAndInit(tweetData_t *allTweetData) {
    ulong numberOfTweets = 0;
    tweetData_t *td = NULL;

    for(int i = 0; i < NUMBER_OF_FILES; i++){
        FILE *fp;
        fp = fopen(FILE_PATH[i], "rb, ccs=UTF-8");
        if (fp == NULL) {
            fprintf(stderr,"file not accessible!\n");
            MPI_Finalize();
            exit(EXIT_FAILURE);
        }

	    ulong index = 0;
		while (!feof(fp)) {
			unsigned char *line = readLine(fp);

			td = parseTweet(line, KEYWORD);
	        td->index = index++;
			allTweetData[numberOfTweets] = *td;
			numberOfTweets++;
		}
		fclose(fp);
	}

    return numberOfTweets;
}

Bucket_t* sortInBuckets(ulong numberOfTweets, int world_size, int world_rank, tweetData_t *data) {

    ulong delta = getIntervalDelta(numberOfTweets, world_size);
    //start:(delta * rank) - delta      ende:delta * rank
    int rank = 0;
    ulong **indexes = 0;
    Bucket_t *buckets = (Bucket_t*) malloc(NUMBER_OF_BUCKETS * sizeof(Bucket_t));
    indexes = (ulong **) bucketSort(data, (size_t) numberOfTweets, NUMBER_OF_BUCKETS,delta * world_rank,delta * world_rank + delta);
    printf("Buckets filled\n");
	for (int l = 0; l < NUMBER_OF_BUCKETS; l++) {
		ulong j = 0;
		buckets[l].indizes = indexes[l];
		while(buckets[l].indizes[j] != ULONG_MAX) {
			j++;
		}
		buckets[l].sizeOfBucket = j;
	}

	return buckets;
}

tweetData_t **sortTweets(Bucket_t *buckets, tweetData_t *data, int world_size, int world_rank) {
    tweetData_t **t = (tweetData_t **) malloc(NUMBER_OF_BUCKETS * sizeof(tweetData_t *));
    for (int k = 0; k < NUMBER_OF_BUCKETS; k++) {
    	t[k] = (tweetData_t *) malloc(buckets[k].sizeOfBucket * sizeof(tweetData_t));
    }

    for (int i = 0; i < NUMBER_OF_BUCKETS; i++) {
        for (ulong k = 0; k < buckets[i].sizeOfBucket; k++) {
        	t[i][k] = data[buckets[i].indizes[k]];
        }
    }

    for (int k = (NUMBER_OF_BUCKETS / world_size)*world_rank; k < ((NUMBER_OF_BUCKETS / world_size)*world_rank) + (NUMBER_OF_BUCKETS / world_size); k++) {
        quickSort(t[k],buckets[k].sizeOfBucket);
    }
    return t;
}

void writeFile(int world_rank, int world_size, Bucket_t *buckets, tweetData_t **t) {
    char outPutFileName[255];
    sprintf (outPutFileName, "./out_%d.txt", world_rank);

    FILE *f = fopen(outPutFileName, "w");
    if (f == NULL)
    {
        fprintf(stderr, "Error opening file!\n");
        fflush(stdout);
        exit(1);
    }

	/* print some text */
    for (int i = (NUMBER_OF_BUCKETS / world_size)*world_rank; i < ((NUMBER_OF_BUCKETS / world_size)*world_rank) + (NUMBER_OF_BUCKETS / world_size); i++) {
        ulong j = 0;
        for (int k = 0; k < buckets[i].sizeOfBucket ; k++) {
			fprintf(f, "%s\n", t[i][k].line);
           j++;
        }
    }
	fclose(f);
}

void communicate(int world_size, int world_rank, Bucket_t *buckets) {
    int bucketsPerNode = NUMBER_OF_BUCKETS / world_size;

    if (world_size != 1) {
        printf("Starting communication\n");

        for (int i = 0; i < world_size; i++) {
            if (world_rank == i) {
                for (int j = 0; j < world_size; j++) {
                    if (j != world_rank) {
                        for (int k = (NUMBER_OF_BUCKETS / world_size)*j; k < ((NUMBER_OF_BUCKETS / world_size)*j) + (NUMBER_OF_BUCKETS / world_size); k++) {
                        	MPI_Send(&buckets[k].sizeOfBucket, 1, MPI_UNSIGNED_LONG, j, SEND_LENGTH_OF_INDEXES_TAG,MPI_COMM_WORLD);
                            MPI_Send(&buckets[k].indizes[0], buckets[k].sizeOfBucket, MPI_UNSIGNED_LONG, j, SEND_INDEXES_TAG,MPI_COMM_WORLD);
                        }
                    }
                }
            } else {
                for (int k = (NUMBER_OF_BUCKETS / world_size)*world_rank; k < ((NUMBER_OF_BUCKETS / world_size)*world_rank) + (NUMBER_OF_BUCKETS / world_size); k++) {
                    ulong increaseBucketSizeBy = 0;
                    MPI_Recv(&increaseBucketSizeBy, 1, MPI_UNSIGNED_LONG, i, SEND_LENGTH_OF_INDEXES_TAG, MPI_COMM_WORLD,
                             MPI_STATUS_IGNORE);
                    ulong indexesFromOtherNode[increaseBucketSizeBy];
                    ulong oldBucketSize = buckets[k].sizeOfBucket;
                    MPI_Recv(&indexesFromOtherNode, increaseBucketSizeBy, MPI_UNSIGNED_LONG, i, SEND_INDEXES_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    buckets[k].indizes = realloc(buckets[k].indizes, (buckets[k].sizeOfBucket+increaseBucketSizeBy) * sizeof(ulong));
                    buckets[k].sizeOfBucket += increaseBucketSizeBy;
                    for (ulong j = oldBucketSize; j < buckets[k].sizeOfBucket; j++) {
                    	buckets[k].indizes[j] = indexesFromOtherNode[j-oldBucketSize];
                    }
                    printf("[Rank:%d]: sizePerBucket[%d] is %lu\n", world_rank, k, buckets[k].sizeOfBucket);
                    fflush(stdout);
                }
            }
        }
        printf("[Rank:%d]: Finnished communication\n",world_rank);
    }

}

int main(int argc, char* argv[]) {

	char hostname[256];

	int world_size = -1,
		world_rank = -1;

	myTimer *timer_readAndInit,
			*timer_sortInBuckets,
			*timer_communicate,
			*timer_qsort,
			*timer_write;

	ullong numberOfTweets = 0,
		   time_readAndInit = 0,
		   time_sortInBuckets = 0,
		   time_communicate = 0,
		   time_qsort = 0,
		   time_write = 0,
		   time_all = 0;

	tweetData_t	*td,
				*data;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	// Show keyword
    if (world_rank == 0) {
		printf("Tweets will be sorted by keyword '%s'.\n",KEYWORD);
		printf("try to allocate %lu kbytes for each node\n", (sizeof(tweetData_t*) * NUMBEROFTWEETS) / 1000);
    }
	data = (tweetData_t*) malloc((NUMBEROFTWEETS+1) * sizeof(tweetData_t));

	timer_readAndInit = startTimer();
	numberOfTweets = readAndInit(data);
	time_readAndInit = stopTimer(timer_readAndInit);

	timer_sortInBuckets = startTimer();
	Bucket_t *buckets = sortInBuckets(numberOfTweets, world_size, world_rank, data);
	time_sortInBuckets = stopTimer(timer_sortInBuckets);

	timer_communicate = startTimer();
	communicate(world_size, world_rank, buckets);
    time_communicate = stopTimer(timer_communicate);

    timer_qsort = startTimer();
    tweetData_t **t = sortTweets(buckets, data, world_size, world_rank);
	MPI_Barrier(MPI_COMM_WORLD);
	time_qsort = stopTimer(timer_qsort); // den timer erst nach der Barriere stoppen, damit die maximale sortierzeit erfasst wird!

	timer_write = startTimer();
	writeFile(world_rank, world_size, buckets, t);
	time_write = stopTimer(timer_write);

	MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

	if (world_rank == 0) {
		time_all = time_readAndInit + time_sortInBuckets + time_write + time_communicate + time_qsort;
		printf("Read & Init:\t\t%llu Sec.\n", time_readAndInit);
		printf("Sort in buckets:\t%llu Sec.\n", time_sortInBuckets);
		printf("Communication:\t\t%llu Sec.\n", time_communicate);
		printf("Sort:\t\t\t%llu Sec.\n", time_qsort);
		printf("Write:\t\t\t%llu Sec.\n", time_write);
		printf("All:\t\t\t%llu Sec.\n", time_all);
	}

	return 0;
}
