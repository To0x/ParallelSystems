/*
 * bucketSort.c
 *
 *  Created on: 08.06.2015
 *      Author: Master
 */

#include <stdio.h>
#include <stdlib.h>
#include "dataHolder.h"
#include <limits.h>
#include <unistd.h>

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

unsigned long long int concatenate(unsigned x, unsigned y) {
    unsigned long long int pow = 10;
    while(y >= pow)
        pow *= 10;
    return x * pow + y;
}

unsigned long long int getSmallestUnicode(unsigned char *currentLine, unsigned long long int offset, int *count, int *noMoreResults) {
	unsigned long long int currentChar;
	unsigned long long int smallestUni = ULLONG_MAX;

	while ((currentChar = (unsigned long long int)*currentLine) != '\0') {

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

			if (currentChar < smallestUni  && currentChar > offset) {
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

			if (currentChar < smallestUni  && currentChar > offset) {
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

			if (currentChar < smallestUni  && currentChar > offset) {
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

int equals(unsigned char *tw1, unsigned char *tw2) {
	while (*tw1 != '\0') {
		if (*tw1 != *tw2) {
			return 0;
		}
		tw1++; tw2++;
	}
	return 1;
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

	if (result == QSORT_EQUALS)
	{
		if (uniCount1 > uniCount2)
			result = P1_ABOVE_P2;

		if (uniCount2 > uniCount1)
			result = P2_ABOVE_P1;
	}
	return result;
}

int compare(const void *c1, const void *c2) {
	int result = QSORT_EQUALS;

	tweetData_t *tw1 = (tweetData_t*) c1;
	tweetData_t *tw2 = (tweetData_t*) c2;

	if (tw1->keywords > tw2->keywords) {
		result = P1_ABOVE_P2;
	}
	if (tw2->keywords > tw1->keywords) {
		result = P2_ABOVE_P1;
	}

	if (tw1->line != NULL && tw2->line != NULL && result == QSORT_EQUALS)
		result = compareHistogram(tw1,tw2);

	return result;
}

float quickSort(struct tweetData *toSort, long len) {
	qsort(toSort, len, sizeof(tweetData_t), compare);
	return 0.0;
}
