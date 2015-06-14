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
#include <stdbool.h>

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

bool isvalueinarray(long val, int *arr, int size) {
	int i;
	for (i = 0; i < size; i++) {
		if (arr[i] == val)
			return true;
	}
	return false;
}

int countUnicode(char* tw1Line, int unicode1) {
	int count = 0;
	int i = 0;
	while (tw1Line[i] != '\0') {
		if ((long) tw1Line[i] == unicode1) {
			count++;
		}
		i++;
	}
	return count;
}

long getSmallestUnicode(char* tw1Line, int* toIgnore) {
	long unicode1 = 3000;

	int i = 0;
	while (tw1Line[i] != '\0') {
		// TODO Why are some unicodes < 0 ?
		if ((long) tw1Line[i] > 0 && (long) tw1Line[i] < unicode1) {
			if (toIgnore != NULL
					&& !(isvalueinarray((long) tw1Line[i], toIgnore,
							sizeof(toIgnore)))) {
				unicode1 = (int) tw1Line[i];
			}
		}
		i++;
	}
	return unicode1;
}

int compareHistogram(struct tweetData *tw1, struct tweetData *tw2) {
	int unicode1 = getSmallestUnicode(tw1->line, NULL);
	int unicode2 = getSmallestUnicode(tw2->line, NULL);

	if (unicode1 > unicode2) {
		return -1;
	} else if (unicode1 == unicode2) { // Same unicode. Have to check for count.
		int c1 = countUnicode(tw1->line, unicode1);
		int c2 = countUnicode(tw2->line, unicode2);
		if (c1 > c2) {
			return -1;
		} else if (c1 == c2) { // Same unicode and count. Get next Unicode
			return 0;
		} else {
			return 1;
		}
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

		if (tw1->line != NULL && tw2->line != NULL) {
			return compareHistogram(tw1, tw2);
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
