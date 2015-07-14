/*
 * bucketSort.c
 *
 *  Created on: 08.06.2015
 *      Author: Master
 */

#include <stdio.h>
#include "dataHolder.h"
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUMBER_OF_BUCKETS 12
#define PIVOT_SET_SIZE 1000
#define DEBUG 1

int getBiggestKeywordCount(tweetData_t *toSort, size_t len);
int compare(const void *c1, const void *c2);

tweetData_t *getPivotElements(tweetData_t *toSort, int buckets){
    tweetData_t *pivotSet = (tweetData_t *) malloc(PIVOT_SET_SIZE * sizeof(tweetData_t));
    for(int i = 0; i < PIVOT_SET_SIZE; i++){
        pivotSet[i] = toSort[i];
    }
    qsort(pivotSet, PIVOT_SET_SIZE, sizeof(tweetData_t), compare);

    int pivot = PIVOT_SET_SIZE/buckets-1;
    int pivotDelta = pivot;
    tweetData_t* pivotElements = (tweetData_t *) malloc(buckets * sizeof(tweetData_t));
    for (int i = 0; i < buckets-1; i++)
    {
        printf("pivot: %d\n",pivot );
        pivotElements[i] = pivotSet[pivot];
        pivot += pivotDelta;
    }

    return pivotElements;
}

// return 1 if tw1 > tw2
// return -1 if tw2 > tw1
// return 0 if equals

int compareTweet(tweetData_t *tw1, tweetData_t *tw2) {
    if (tw1->keywords > tw2->keywords)
        return 1;
    else if (tw2->keywords > tw1->keywords)
        return -1;
    else {
        if (tw1->smallestUniCode < tw2->smallestUniCode)
            return 1;
        else if (tw2->smallestUniCode < tw1->smallestUniCode)
            return -1;
        else {
            if (tw1->countSmallest > tw2->countSmallest)
                return 1;
            else if (tw2->countSmallest > tw1->countSmallest)
                return -1;
            else
                return 0;
        }
    }
}

bool inRange(tweetData_t *toSort,tweetData_t *bucketRanges,int ownBucket){
    
	//if (DEBUG)
	//	printf("sizeof(bucketRanges): %d, sizeof(tweetData_t): %d, size: %d", sizeof(*bucketRanges), sizeof(tweetData_t), sizeof(*bucketRanges) / sizeof(tweetData_t));

    int bucketLength = sizeof(*bucketRanges) / sizeof(tweetData_t); // TODO: prüfen, sollte bei 8 knoten = 7 sein!
    if (ownBucket == 0) {

        if (compareTweet(toSort, &bucketRanges[0]) == -1) { // toSort < bucketRanges[0]
            return true;
        }  
        return false;

    } else if (ownBucket == bucketLength+1) { // if ownBucket == 8?
        if (compareTweet(toSort, &bucketRanges[bucketLength]) == 1) { // toSort > bucketRanges[7]
            return true;
        }
        return false;
    } else {
        if (compareTweet(toSort, &bucketRanges[ownBucket-1]) == 1 && compareTweet(toSort, &bucketRanges[ownBucket]) == -1){ // größer als Vorgänger, kleiner als der eigenen
            return true;
        }
        return false;
    }
}

void printPivotElements(tweetData_t *elements) {

	if (!DEBUG)
		return;

	for (int i = 0 ; i < 8 ; i++) {
		printf("keyword:%d \t unicode: %llu \t count: %d\n",elements[i].keywords, elements[i].smallestUniCode, elements[i].countSmallest );
	}

}

void bucketSort(tweetData_t *toSort, size_t len, size_t tweetSize, int buckets, int ownBucket) {
    /*
        Bucketsort:

         function bucketSort(array, n) is
         buckets ? new array of n empty lists
         for i = 0 to (length(array)-1) do
             insert array[i] into buckets[msbits(array[i], k)]
         for i = 0 to n - 1 do
             nextSort(buckets[i]);
         return the concatenation of buckets[0], ...., buckets[n-1]

     Here array is the array to be sorted and n is the number of buckets to use. The function msbits(x,k) returns the k most
        significant bits of x (floor(x/2^(size(x)-k))); different functions can be used to translate the range of elements
        in array to n buckets, such as translating the letters A�Z to 0�25 or returning the first character (0�255) for
        sorting strings. The function nextSort is a sorting function; using bucketSort itself as nextSort produces a relative
        of radix sort; in particular, the case n = 2 corresponds to quicksort (although potentially with poor pivot choices).

        Scource: https://en.wikipedia.org/wiki/Bucket_sort
     */    

    tweetData_t *bucketRanges = getPivotElements(toSort, buckets);
   	printPivotElements(bucketRanges);

    tweetData_t *t = (tweetData_t *) malloc(len * sizeof(tweetData_t));

    long currentPosition = 0;
    for (long l = 0; l < len; l++){

    	if(inRange(&toSort[l], bucketRanges, ownBucket)){ // TODO: Check if in range!!
            t[currentPosition++] = toSort[l];  
        } else {
            fprintf(stderr,"No range compare implemented yet\n");
            exit(EXIT_FAILURE);
        } 
    }
    printf("Filled Buckets.\n");


    qsort(t, (size_t) currentPosition-1, sizeof(tweetData_t), compare);
    printf("Buckets sorted.\n");

    // Debug... 
    for (long i1 = (long) len  ; i1 > 65500; i1--) {
        printf("t[%ld] --> %s\n" , i1,t[i1].line);
    }

}

int getBiggestKeywordCount(tweetData_t *toSort, size_t len) {
    int keywordCount = 0;
    for (long i = 0; i < len; i++) {
        if (toSort[i].keywords > keywordCount) {
            keywordCount = toSort[i].keywords;
        }
    }

    return keywordCount;
}

unsigned long long int concatenate(unsigned x, unsigned y) {
    unsigned long long int pow = 10;
    while (y >= pow)
        pow *= 10;
    return x * pow + y;
}

unsigned long long int getSmallestUnicode(unsigned char *currentLine, unsigned long long int offset, int *count,
                                          int *noMoreResults) {
    unsigned long long int currentChar;
    unsigned long long int smallestUni = ULLONG_MAX;

    while ((currentChar = (unsigned long long int) *currentLine) != '\0') {

        if (currentChar < 128) { // ASCII
            if (currentChar < smallestUni && currentChar > offset) {
                smallestUni = currentChar;
                *count = 0;
            }

            if (currentChar == smallestUni)
                (*count)++;
        }
        else if (currentChar < 224) {
            currentLine++;
            currentChar = concatenate(currentChar, *currentLine);

            if (currentChar < smallestUni && currentChar > offset) {
                smallestUni = currentChar;
                *count = 0;
            }

            if (currentChar == smallestUni)
                (*count)++;

        } else if (currentChar < 240) {
            currentLine++;
            currentChar = concatenate(currentChar, *currentLine);
            currentLine++;
            currentChar = concatenate(currentChar, *currentLine);

            if (currentChar < smallestUni && currentChar > offset) {
                smallestUni = currentChar;
                *count = 0;
            }

            if (currentChar == smallestUni)
                (*count)++;

        } else {
            currentLine++;
            currentChar = concatenate(currentChar, *currentLine);
            currentLine++;
            currentChar = concatenate(currentChar, *currentLine);
            currentLine++;
            currentChar = concatenate(currentChar, *currentLine);

            if (currentChar < smallestUni && currentChar > offset) {
                smallestUni = currentChar;
                *count = 0;
            }

            if (currentChar == smallestUni)
                (*count)++;
        }

        currentLine++;
    }

    if (smallestUni == ULLONG_MAX) {
        (*noMoreResults) = 1;
    }


    return smallestUni;
}

int compareHistogram(tweetData_t *tw1, tweetData_t *tw2) {
    unsigned long long int uniCode1 = 0l, uniCode2 = 0l;
    int uniCount1 = 0, uniCount2 = 0, noMoreResults1 = 0, noMoreResults2 = 0;
    int result = QSORT_EQUALS;

    // first round
    uniCode1 = getSmallestUnicode(tw1->line, 0, &uniCount1, &noMoreResults1);
    uniCode2 = getSmallestUnicode(tw2->line, 0, &uniCount2, &noMoreResults2);

    int i = 1;
    int reRoundFlag = 0;

    if (uniCode1 == uniCode2 && uniCount1 == uniCount2) {
        reRoundFlag = 1;


        while (1) {
            uniCode1 = getSmallestUnicode(tw1->line, uniCode1, &uniCount1, &noMoreResults1);
            uniCode2 = getSmallestUnicode(tw2->line, uniCode2, &uniCount2, &noMoreResults2);


            if (uniCode1 != uniCode2 || uniCount1 != uniCount2 || noMoreResults1 == 1 || noMoreResults2 == 1) {
                break;
            }
            i++;
        }
    }

    if (!reRoundFlag) {
        tw1->smallestUniCode = uniCode1;
        tw1->countSmallest = uniCount1;

        tw2->smallestUniCode = uniCode2;
        tw2->countSmallest = uniCount2;
    }

    if (uniCode1 < uniCode2)
        result = P1_ABOVE_P2;

    if (uniCode2 < uniCode1)
        result = P2_ABOVE_P1;

    if (result == QSORT_EQUALS) {
        if (uniCount1 > uniCount2)
            result = P1_ABOVE_P2;

        if (uniCount2 > uniCount1)
            result = P2_ABOVE_P1;
    }
    return result;
}

int compare(const void *c1, const void *c2) {
    int result = QSORT_EQUALS;

    tweetData_t *tw1 = (tweetData_t *) c1;
    tweetData_t *tw2 = (tweetData_t *) c2;

    if (tw1->keywords > tw2->keywords) {
        result = P1_ABOVE_P2;
    }
    if (tw2->keywords > tw1->keywords) {
        result = P2_ABOVE_P1;
    }

    if (tw1->line != NULL && tw2->line != NULL && result == QSORT_EQUALS)
        result = compareHistogram(tw1, tw2);

    return result;
}

float quickSort(struct tweetData *toSort, long len) {

    printf("len in quicksort: %ld", len);
    fflush(stdout);
    //bucketSort(toSort, len, sizeof(tweetData_t));
    qsort(toSort, len, sizeof(tweetData_t), compare);
    return 0.0;
}
