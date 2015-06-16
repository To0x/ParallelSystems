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

#define UNICODE_ARRAY_LENGTH 15000

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

int compareHistogram(struct tweetData *tw1, struct tweetData *tw2) {
	static long y = 0;
	printf("Index -> %d\n",y++);
	
	int unicode[2][UNICODE_ARRAY_LENGTH];
	for (int i = 0; i < UNICODE_ARRAY_LENGTH; i++) {
		unicode[0][i] = 0;
		unicode[1][i] = 0;
	}

	for (int i = 0; i < strlen(tw1->line); i++) {
		int actChar = (int) tw1->line[i];
		if (actChar < 128) { // 0xxx xxxx --> befindet sich im ASCII bereich
			unicode[0][tw1->line[i]] += 1;
		} else if (actChar < 224) { // 110x xxxx --> UTF8 Encoding beginnt 2Byte
			unicode[0][tw1->line[i] + tw1->line[i++]] += 1;
		} else if (actChar < 240) { // 1110 xxxx --> 3 Byte
			unicode[0][tw1->line[i] + tw1->line[i++] + tw1->line[i++]] += 1;
		} else if (actChar < 248) { // 1111 0xxx --> 4 Byte{
			unicode[0][tw1->line[i] + tw1->line[i++] + tw1->line[i++]
					+ tw1->line[i++]] += 1;
		//	printf("4 Byte\n");
		} else if (actChar < 252) { // 1111 10xx --> 5 Byte{
			unicode[0][tw1->line[i] + tw1->line[i++] + tw1->line[i++]
					+ tw1->line[i++] + tw1->line[i++]] += 1;
			printf("5 Byte\n");
		} else if (actChar < 254) { // 1111 110x --> 6 Byte{
			unicode[0][tw1->line[i] + tw1->line[i++] + tw1->line[i++]
					+ tw1->line[i++] + tw1->line[i++] + tw1->line[i++]] += 1;
			printf("6 Byte\n");
		}
	}
	for (int i = 0; i < strlen(tw2->line); i++) {
		int actChar = (int) tw2->line[i];
		if (actChar < 128) { // 0xxx xxxx --> befindet sich im ASCII bereich
			unicode[1][tw2->line[i]] += 1;
		} else if (actChar < 224) { // 110x xxxx --> UTF8 Encoding beginnt 2Byte
			unicode[1][tw2->line[i] + tw2->line[i++]] += 1;
		} else if (actChar < 240) { // 1110 xxxx --> 3 Byte
			unicode[1][tw2->line[i] + tw2->line[i++] + tw2->line[i++]] += 1;
		} else if (actChar < 248) { // 1111 xxxx --> 4 Byte{
			unicode[1][tw2->line[i] + tw2->line[i++] + tw2->line[i++]
					+ tw2->line[i++]] += 1;
			//printf("4 Byte\n");
		} else if (actChar < 252) { // 1111 1xxx --> 5 Byte{
			unicode[1][tw2->line[i] + tw2->line[i++] + tw2->line[i++]
					+ tw2->line[i++] + tw2->line[i++]] += 1;
			printf("5 Byte\n");
		} else if (actChar < 254) { // 1111 1xxx --> 6 Byte{
			unicode[1][tw2->line[i] + tw2->line[i++] + tw2->line[i++]
					+ tw2->line[i++] + tw2->line[i++] + tw2->line[i++]] += 1;
			printf("6 Byte\n");
		}
	}
	for (int i = 0; i < UNICODE_ARRAY_LENGTH; i++) {
		if (unicode[0][i] > unicode[1][i]) {
			if (tw1->keywords == 0) {
				printf("%s --> %d -> %d --> %c\n", tw1->line, i, unicode[0][i],(char)i);
				printf("%s --> %d -> %d --> %c\n", tw2->line, i, unicode[1][i],(char)i);
				printf(
						"-------------------------------------------------------------------------------\n");
			}
			return -1;
		} else if (unicode[0][i] < unicode[1][i]) {
			if (tw2->keywords == 0) {
				printf("%s --> %d -> %d --> %c\n", tw1->line, i, unicode[0][i],(char)i);
				printf("%s --> %d -> %d --> %c\n", tw2->line, i, unicode[1][i],(char)i);
				printf(
						"-------------------------------------------------------------------------------\n");
			}
			return 1;
		}
	}
	return 0;
}

int compare(const void *c1, const void *c2) {

	struct tweetData *tw1 = (struct tweetData*) c1;
	struct tweetData *tw2 = (struct tweetData*) c2;

	if (tw1->keywords > tw2->keywords) {
		return -1;
	} else if (tw1->keywords == tw2->keywords) {
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
