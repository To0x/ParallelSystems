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

int compareHistogram(struct tweetData *tw1, struct tweetData *tw2) {
	static long l = 0;
	l++;
	long debug = 19000000;
	
	if (l % 1 == 0 && l > debug) {
		//printf("%li\n",l);
	}

//	printf("%s - Hashtags: %d, Smileys: %d, Keywords: %d\n", tw1->line,
//			tw1->hashtags, tw1->smiles, tw1->keywords);
//	printf("%s - Hashtags: %d, Smileys: %d, Keywords: %d\n", tw2->line,
//			tw2->hashtags, tw2->smiles, tw2->keywords);
//	printf(
//			"-----------------------------------------------------------------------\n");

	int *unicode1 = calloc(UNICODE_ARRAY_LENGTH + 2, sizeof(*unicode1));
	int *unicode2 = calloc(UNICODE_ARRAY_LENGTH + 2, sizeof(*unicode2));

//	static long unicode1[UNICODE_ARRAY_LENGTH+1];
//	static long unicode2[UNICODE_ARRAY_LENGTH+1];

	for (long i = 0; i < UNICODE_ARRAY_LENGTH; i++) {
		unicode1[i] = 0;
		unicode2[i] = 0;
	}

	for (int i = 0; i < (int) strlen(tw1->line); i++) {
		int actChar = (int) tw1->line[i];
		if (actChar < 128) { // 0xxx xxxx --> befindet sich im ASCII bereich
			unicode1[(int) (tw1->line[i])]++;
			if (unicode1[(int) (tw1->line[i])] > UNICODE_ARRAY_LENGTH) {
//				printf("unicode1[%d] += 1 --> %d\n", (int) tw1->line[i],
//						unicode1[(int) tw1->line[i]]);
				//printf("%d \t %s\n",(int) strlen(tw1->line), tw1->line);
				//printf("%d\n",actChar);
			}
		}

		//else if (actChar < 224) { // 110x xxxx --> UTF8 Encoding beginnt 2Byte
		//		unicode1[(int)tw1->line[i] + (int)tw1->line[i + 1]] += 1;
		//	i += 1; }
//		} else if (actChar < 240) { // 1110 xxxx --> 3 Byte
//			unicode1[tw1->line[i] + tw1->line[i + 1] + tw1->line[i + 2]] += 1;
//			i += 2;
//		} else {
//			unicode1[tw1->line[i] + tw1->line[i + 1] + tw1->line[i + 2]
//					+ tw1->line[i + 3]] += 1;
//			//printf("index: %d\n",tw1->line[i] + tw1->line[i + 1] + tw1->line[i + 2]
//			//                                             					+ tw1->line[i + 3]);
//			i += 3;
//		}
	}

//	for (int i = 0; i < strlen(tw2->line); i++) {
//		int actChar = (int) tw2->line[i];
//		if (actChar < 128) { // 0xxx xxxx --> befindet sich im ASCII bereich
//			unicode1[tw2->line[i]] += 1;
//			//printf("i: %d ---> %c ---> %d \n", i, (char)tw1->line[i], tw1->line[i]);
//		} else if (actChar < 224) { // 110x xxxx --> UTF8 Encoding beginnt 2Byte
//			unicode1[tw2->line[i] + tw2->line[i + 1]] += 1;
//			i += 1;
//		} else if (actChar < 240) { // 1110 xxxx --> 3 Byte
//			unicode1[tw2->line[i] + tw2->line[i + 1] + tw2->line[i + 2]] += 1;
//			i += 2;
//		} else {
//			unicode1[tw2->line[i] + tw2->line[i + 1] + tw2->line[i + 2]
//					+ tw2->line[i + 3]] += 1;
//			i += 3;
//		}
//	}

	for (long i = 0; i < UNICODE_ARRAY_LENGTH; i++) {
//		if (l > 1490000  && i > 990) {
//			printf("Filled array[%li]: %li ", i, unicode1[i]);
//			printf("vs %li\n", unicode2[i]);
//		}

		if (unicode1[i] > unicode2[i]) {
			free(unicode1);
			free(unicode2);
			return -1;
		} else if (unicode1[i] < unicode2[i]) {
			free(unicode1);
			free(unicode2);
			return 1;
		}
	}

	free(unicode1);
	free(unicode2);
	return 0;
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
			//printf("%li\n", i);
		}
		return compareHistogram(tw1, tw2);
		//return 0;
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
