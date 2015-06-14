/*
 * bucketSort.c
 *
 *  Created on: 08.06.2015
 *      Author: Master
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "dataHolder.h"

// last unicode --> 131.071
#define UNICODE_ARRAY_LENGTH 1000

void Bucket_Sort(int array[], int n) {
	int i, j;
	int count[n];
	for (i = 0; i < n; i++)
		count[i] = 0;

	for (i = 0; i < n; i++)
		(count[array[i]])++;

	for (i = 0, j = 0; i < n; i++)
		for (; count[i] > 0; (count[i])--)
			array[j++] = i;
}

long getSmallestUnicode(char* tw1Line) {
	static long long counter = 0;

	long unicode1 = 3000;
	//printf("%s\n",tw1Line);
	long i = 0;
	while (tw1Line[i] != '\0') {
//		if (counter++ % 10000 == 0) {
//			printf("%lli\t%li\n", counter, (long) strlen(tw1Line));
//		}
//		if ((long) tw1Line[i] > 0 && (long) tw1Line[i] < unicode1) {
//			unicode1 = (int) tw1Line[i];
//		}
		i++;
	}
//	printf("smallest unicode: %li\n", unicode1);
	return unicode1;
}

int compareHistogram(struct tweetData *tw1, struct tweetData *tw2) {
	int unicode1 = getSmallestUnicode(tw1->line);
	int unicode2 = getSmallestUnicode(tw2->line);

//	for (int i = 0; i < (int) strlen(tw1->line); i++) {
//		if ((int) tw1->line[i] < unicode1) {
//			unicode1 = (int) tw1->line[i];
//		}
//	}
//
//	for (int i = 0; i < (int) strlen(tw2->line); i++) {
//		if ((int) tw2->line[i] < unicode2) {
//			unicode2 = (int) tw2->line[i];
//		}
//	}

	if (unicode1 > unicode2) {
		return -1;
	} else if (unicode1 == unicode2) {
		return 0;
	} else {
		return 1;
	}
}

int compare(const void *c1, const void *c2) {

	struct tweetData *tw1 = (struct tweetData*) c1;
	struct tweetData *tw2 = (struct tweetData*) c2;

	if (tw1->keywords > tw2->keywords) {
		return -1;
	} else if (tw1->keywords == tw2->keywords) {
		static long i = 0;
		i++;

		if (i % 10000 == 0) {
			//printf("%d\n", compareHistogram(tw1, tw2));
			//printf("%li\n", i);
		}
		if (tw1->line != NULL && tw1->line != NULL) {
			return compareHistogram(tw1, tw2);
		}{
			printf("Some strings are NULL...\n");
		}
		return 0;
	} else {
		return 1;
	}
}

float quickSort(struct tweetData *toSort, long len) {
	time_t t1 = time(NULL);
	//printf("time %lld", (long long)t1);
	qsort(toSort, len, sizeof(struct tweetData), compare);
	time_t t2 = time(NULL);
	//printf("time %lld", (long long)t2);

	//return 0.0;
	return (float) (t2 - t1);
}
