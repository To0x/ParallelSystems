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

#define NUMBER_OF_BUCKETS 12

int getBiggestKeywordCount(tweetData_t *toSort, size_t len);

void bucketSort(tweetData_t *toSort, size_t len, size_t tweetSize, int (*compare)(const void *, const void *)) {
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

    printf("In bucketSort:\n");
    printf("\tlen -> %ld\n", len);
    printf("\ttweetSize -> %zd\n", tweetSize);

    int biggestKeywordCount = getBiggestKeywordCount(toSort, len) + 1;
    printf("\tbiggesKeywordCount -> %d\n", biggestKeywordCount);

    int bucketSize[biggestKeywordCount];
    int bucketCurrentSize[biggestKeywordCount];

    // initialize bucket size and current bucket index
    for (int k = 0; k < biggestKeywordCount; ++k) {
        bucketSize[k] = 0;
        bucketCurrentSize[k] = 0;
    }
    printf("Inialized bucketSize and bucketCurrentSize.\n");

    for (int i = 0; i < len; ++i) {
        bucketSize[toSort[i].keywords] += 1;
    }
    printf("Caluculated bucket sizes.\n");

    // Print bucket sizes
    for (int j = 0; j < biggestKeywordCount; ++j) {
        printf("bucketSize[%d] --> %d\n", j, bucketSize[j]);
    }

    tweetData_t **t = (tweetData_t **) malloc(biggestKeywordCount * sizeof(tweetData_t *));
    for (int k = 0; k < biggestKeywordCount; ++k) {
        t[k] = (tweetData_t *) malloc(bucketSize[k] * sizeof(tweetData_t));
    }
    printf("Created buckets.\n");

    for (int l = 0; l < len; ++l) {
        t[toSort[l].keywords][bucketCurrentSize[toSort[l].keywords]++] = toSort[l];
    }
    printf("Filled Buckets.\n");


    for (int j = 0; j < biggestKeywordCount; ++j) {
        qsort(t[j], (size_t) bucketSize[j], sizeof(tweetData_t), compare);
    }
    printf("Buckets sorted.\n");


    // Print last element of each bucket.
    for (int n = biggestKeywordCount; n >= 0; --n) {
        for (long m = bucketSize[n]; m > 0; --m) {
//            if (m == bucketSize[n] ) {
            if (n == 0 && m > 54400) {
//                printf("t[%d][%ld] keywords %d\n", n, m, t[n][m - 1].keywords);
                printf("t[%d][%ld] --> %s\n", n, m, t[n][m - 1].line);
            }
        }
    }

    int toSortIndex = 0;
    for (int m = biggestKeywordCount-1; m >= 0 ; m--) {
        for (int i = 0; i < bucketCurrentSize[m]; ++i) {
            toSort[toSortIndex++] = t[m][i];
        }
    }
    printf("Result copyed to toSort.\n");

    for (long i1 = (long) len  ; i1 > 65500; i1--) {
        printf("toSort[%ld] --> %s\n" , i1,toSort[i1].line);
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
    bucketSort(toSort, len, sizeof(tweetData_t), compare);
//    qsort(toSort, len, sizeof(tweetData_t), compare);
    return 0.0;
}
