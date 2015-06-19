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
#include <limits.h>

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

/*getSmallestUnicode(unsigned char *tweetString, int offset) {

}

int compareHistogram(struct tweetData *tw1, struct tweetData *tw2) {
	static long y = 0;
	//printf("Index -> %d\n",y++);
	
	int unicode[2][UNICODE_ARRAY_LENGTH];
	for (int i = 0; i < UNICODE_ARRAY_LENGTH; i++) {
		unicode[0][i] = 0;
		unicode[1][i] = 0;
	}

	for (int i = 0; i < strlen(tw1->line); i++) {
		int actChar = (int) tw1->line[i];
		if (actChar < 128) { // 0xxx xxxx --> befindet sich im ASCII bereich
			unicode[0][(int)tw1->line[i]] += 1;
			//printf("Unicode[0][%d]++\n", tw1->line[i]);
		} else if (actChar < 224) { // 110x xxxx --> UTF8 Encoding beginnt 2Byte
			unicode[0][(int)tw1->line[i] + (int)tw1->line[i+1]] += 1;
			//printf("Unicode[0][%d]++\n", tw1->line[i] + tw1->line[i+1]);
		} else if (actChar < 240) { // 1110 xxxx --> 3 Byte
			unicode[0][(int)tw1->line[i] + (int)tw1->line[i+1] + (int)tw1->line[i+2]] += 1;
			//printf("Unicode[0][%d]++\n", tw1->line[i] + tw1->line[i+1] + tw1->line[i+2]);
		} else if (actChar < 248) { // 1111 0xxx --> 4 Byte{
			unicode[0][(int)tw1->line[i] + (int)tw1->line[i+1] + (int)tw1->line[i+2]
					+ (int)tw1->line[i+3]] += 1;
			//printf("Unicode[0][%d]++\n",tw1->line[i] + tw1->line[i+1] + tw1->line[i+2] + tw1->line[i+3]);
		//	printf("4 Byte\n");
		} else if (actChar < 252) { // 1111 10xx --> 5 Byte{
			unicode[0][(int)tw1->line[i] + (int)tw1->line[i+1] + (int)tw1->line[i+2]
					+ (int)tw1->line[i+3] + (int)tw1->line[i+4]] += 1;
			//printf("Unicode[0][%d]++\n",tw1->line[i] + tw1->line[i+1] + tw1->line[i+2] + tw1->line[i+3] + tw1->line[i+4]);
			printf("5 Byte\n");
		} else if (actChar < 254) { // 1111 110x --> 6 Byte{
			unicode[0][(int)tw1->line[i] + (int)tw1->line[i+1] + (int)tw1->line[i+2]
					+ (int)tw1->line[i+3] + (int)tw1->line[i+4] + (int)tw1->line[i+5]] += 1;
			//printf("Unicode[0][%d]++\n", tw1->line[i] + tw1->line[i+1] + tw1->line[i+2] + tw1->line[i+3] + tw1->line[i+4] + tw1->line[i+5]);
			printf("6 Byte\n");
		}
	}
	for (int i = 0; i < strlen(tw2->line); i++) {
		int actChar = (int) tw2->line[i];
		if (actChar < 128) { // 0xxx xxxx --> befindet sich im ASCII bereich
			unicode[1][(int)tw2->line[i]] += 1;
		} else if (actChar < 224) { // 110x xxxx --> UTF8 Encoding beginnt 2Byte
			unicode[1][tw2->line[i] + tw2->line[i+1]] += 1;
		} else if (actChar < 240) { // 1110 xxxx --> 3 Byte
			unicode[1][tw2->line[i] + tw2->line[i+1] + tw2->line[i+2]] += 1;
		} else if (actChar < 248) { // 1111 xxxx --> 4 Byte{
			unicode[1][tw2->line[i] + tw2->line[i+1] + tw2->line[i+2]
					+ tw2->line[i+3]] += 1;
			//printf("4 Byte\n");
		} else if (actChar < 252) { // 1111 1xxx --> 5 Byte{
			unicode[1][tw2->line[i] + tw2->line[i+1] + tw2->line[i+2]
					+ tw2->line[i+3] + tw2->line[i+4]] += 1;
			printf("5 Byte\n");
		} else if (actChar < 254) { // 1111 1xxx --> 6 Byte{
			unicode[1][tw2->line[i] + tw2->line[i+1] + tw2->line[i+2]
					+ tw2->line[i+3] + tw2->line[i+4] + tw2->line[i+5]] += 1;
			printf("6 Byte\n");
		}
	}
	for (int i = 0; i < UNICODE_ARRAY_LENGTH; i++) {


		if (unicode[0][i] > unicode[1][i]) {
			tw1->smallestUniCode = i;
			tw1->countSmallest = unicode[0][i];
			if (tw1->keywords == 0) {
				//printf("%s --> %d -> %d --> %c\n", tw1->line, i, unicode[0][i],(char)i);
				//printf("%s --> %d -> %d --> %c\n", tw2->line, i, unicode[1][i],(char)i);
				//printf(
				//		"-------------------------------------------------------------------------------\n");
			}
			return -1;
		} else if (unicode[0][i] < unicode[1][i]) {
			tw2->smallestUniCode = i;
			tw2->countSmallest = unicode[1][i];
			if (tw2->keywords == 0) {
				//printf("%s --> %d -> %d --> %c\n", tw1->line, i, unicode[0][i],(char)i);
				//printf("%s --> %d -> %d --> %c\n", tw2->line, i, unicode[1][i],(char)i);
				//printf(
				//		"-------------------------------------------------------------------------------\n");
			}
			return 1;
		}
	}
	return 0;
}
*/
long getSmallestUnicode(unsigned char *currentLine, long offset, int *count) {

	//unsigned char *currentLine = tweet->line;
	int currentChar;

	long smallestUni = 1500000;

	while ((currentChar = (int)*currentLine) != '\0') {

		if (currentChar < 128) { // ASCII
			if (currentChar < smallestUni && currentChar > offset) {
				smallestUni = currentChar;
				*count = 0;
			}

			if (currentChar == smallestUni)
				(*count)++;
		}
		else if (currentChar < 224) {
			//printf("2 byte!\n");
			currentLine++;
			currentChar += *currentLine;

			if (currentChar < smallestUni  && currentChar > offset) {
				smallestUni = currentChar;
				*count = 0;
			}

			if (currentChar == smallestUni)
				(*count)++;

		} else if (currentChar < 240) {
			//printf("3 byte!\n");
			currentLine++;
			currentChar += *currentLine;
			currentLine++;
			currentChar += *currentLine;

			if (currentChar < smallestUni  && currentChar > offset) {
				smallestUni = currentChar;
				*count = 0;
			}

			if (currentChar == smallestUni)
				(*count)++;

		} else {
			//printf("4 byte!\n");
			currentLine++;
			currentChar += *currentLine;
			currentLine++;
			currentChar += *currentLine;
			currentLine++;
			currentChar += *currentLine;

			if (currentChar < smallestUni  && currentChar > offset) {
				smallestUni = currentChar;
				*count = 0;
			}

			if (currentChar == smallestUni)
				(*count)++;
		}

		currentLine++;
	}
	//printf("Smallest Unicode: %ld[%d] in String: %s\n", smallestUni, *count, currentLine);
	return smallestUni;
}

int compareHistogram(struct tweetData *tw1, struct tweetData *tw2) {


	//printf("%s \n vs. \n%s\n", tw1->line, tw2->line);

	/*
	unsigned char *lineBuffer1 = (unsigned char *)malloc(sizeof(unsigned char) * strlen(tw1->line));
	unsigned char *lineBuffer2 = (unsigned char *)malloc(sizeof(unsigned char) * strlen(tw2->line));

	strcpy(lineBuffer1, tw1->line);
	strcpy(lineBuffer2, tw2->line);

	printf("%s", lineBuffer1);
	fflush(stdout);
*/
	long uniCode1 = 0l, uniCode2 = 0l;
	int uniCount1 = 0, uniCount2 = 0;

	int i = 0;
	while (i != 5) {
		uniCode1 = getSmallestUnicode(tw1->line, (uniCode1), &uniCount1);
		uniCode2 = getSmallestUnicode(tw2->line, (uniCode2), &uniCount2);


		if (uniCode1 != uniCode2 || uniCount1 != uniCount2)
			break;

		i++;
	}

	//long uniCode1 = getSmallestUnicode(tw1->line, &uniCount1, 0);
	//printf("Smallest Unicode: %ld[%d] in String: %s\n", uniCode1, uniCount1, tw1->line);
	tw1->smallestUniCode = uniCode1;
	tw1->countSmallest = uniCount1;

	//printf("Smallest Unicode: %ld[%d] in String: %s\n", uniCode2, uniCount2, tw2->line);
	tw2->smallestUniCode = uniCode2;
	tw2->countSmallest = uniCount2;

	if (uniCode1 < uniCode2)
		return -1;
	else if (uniCode1 == uniCode2) {
		if (uniCount1 > uniCount2 )
			return -1;
		else if (uniCount1 < uniCount2)
			return 1;
		else
			return 0;
	} else
		return 1;
}

int compare(const void *c1, const void *c2) {

	struct tweetData *tw1 = (struct tweetData*) c1;
	struct tweetData *tw2 = (struct tweetData*) c2;

	//printf("compare\n");
	//printf("Keywords1: %d %s\n", tw1->keywords, tw1->line);
	//printf("keywords2: %d %s\n", tw2->keywords, tw2->line);

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
	//time_t t1 = time(NULL);
	//printf("time %lld", (long long)t1);
	qsort(toSort, len, sizeof(struct tweetData), compare);
	//time_t t2 = time(NULL);
	//printf("time %lld", (long long)t2);

	return 0.0;
	//return (float) (t2 - t1);
}
