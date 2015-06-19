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
unsigned long long int concatenate(unsigned x, unsigned y) {
    unsigned long long int pow = 10;
    while(y >= pow)
        pow *= 10;
    return x * pow + y;
}

unsigned long long int getSmallestUnicode(unsigned char *currentLine, unsigned long long int offset, int *count, int *noMoreResults) {

	//unsigned char *currentLine = tweet->line;
	unsigned long long int currentChar;
	unsigned long long int smallestUni = ULLONG_MAX;
	//*count = 0;

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
			currentChar = concatenate(currentChar, *currentLine);
			//currentChar += *currentLine;

			if (currentChar < smallestUni  && currentChar > offset) {
				smallestUni = currentChar;
				*count = 0;
			}

			if (currentChar == smallestUni)
				(*count)++;

		} else if (currentChar < 240) {
			//printf("3 byte!\n");
			currentLine++;
			currentChar = concatenate(currentChar, *currentLine);
			//currentChar += *currentLine;
			currentLine++;
			currentChar = concatenate(currentChar, *currentLine);
			//currentChar += *currentLine;

			if (currentChar < smallestUni  && currentChar > offset) {
				smallestUni = currentChar;
				*count = 0;
			}

			if (currentChar == smallestUni)
				(*count)++;

		} else {
			//printf("4 byte!\n");
			//sleep(5);
			currentLine++;
			//printf("actChar: %llu concat with: %d\n", currentChar, *currentLine);
			currentChar = concatenate(currentChar, *currentLine);
			//printf("new char: %llu\n", currentChar);
			//sleep(5);
			//currentChar += *currentLine;
			currentLine++;
			//printf("actChar: %llu concat with: %d\n", currentChar, *currentLine);
			currentChar = concatenate(currentChar, *currentLine);
			//printf("new char: %llu\n", currentChar);
			//sleep(5);
			//currentChar += *currentLine;
			currentLine++;
			//printf("actChar: %llu concat with: %d\n", currentChar, *currentLine);
			currentChar = concatenate(currentChar, *currentLine);
			//printf("new char: %llu\n", currentChar);
			//sleep(5);
			//currentChar += *currentLine;

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
		smallestUni = offset;
		(*noMoreResults) = 1;
	}


	//printf("Smallest Unicode: %ld[%d] in String: %s\n", smallestUni, *count, currentLine);
	return smallestUni;
}

int equals(unsigned char *tw1, unsigned char *tw2) {

	unsigned char currentChar;

	while (*tw1 != '\0') {
		if (*tw1 != *tw2)
			return 0;
		tw1++; tw2++;
	}
	return 1;
}

int compareHistogram(struct tweetData *tw1, struct tweetData *tw2) {

	if (equals(tw1->line, tw2->line))
		return 0;

	//printf("%s \n vs. \n%s\n", tw1->line, tw2->line);

	/*
	unsigned char *lineBuffer1 = (unsigned char *)malloc(sizeof(unsigned char) * strlen(tw1->line));
	unsigned char *lineBuffer2 = (unsigned char *)malloc(sizeof(unsigned char) * strlen(tw2->line));

	strcpy(lineBuffer1, tw1->line);
	strcpy(lineBuffer2, tw2->line);

	printf("%s", lineBuffer1);
	fflush(stdout);
*/
	unsigned long long int uniCode1 = 0l, uniCode2 = 0l;
	int uniCount1 = 0, uniCount2 = 0, noMoreResults1 = 0, noMoreResults2 = 0;

	int i = 0;
	int sameFlag = 0;
	while (1) {
		//printf("again!");
		//fflush(stdout);
		//sleep(5);
		uniCode1 = getSmallestUnicode(tw1->line, uniCode1, &uniCount1, &noMoreResults1);
		uniCode2 = getSmallestUnicode(tw2->line, uniCode2, &uniCount2, &noMoreResults2);

		/*if (sameFlag) {
			//printf("unicodes are the same\n");
			printf("Durchlauf: %d", i);
			printf("string1: %s\nstring2: %s\n", tw1->line, tw2->line);
			printf("smallest1: %ld, count1: %d\n", uniCode1, uniCount1);
			printf("smallest2: %ld, count2: %d\n", uniCode2, uniCount2);
		}*/

		//break;

		if (uniCode1 != uniCode2 || uniCount1 != uniCount2 || noMoreResults1 == 1 || noMoreResults2 == 1)
			break;

		/*
		printf("unicodes are the same\n");
		printf("string1: %s\nstring2: %s\n", tw1->line, tw2->line);
		printf("smallest1: %ld, count1: %d\n", uniCode1, uniCount1);
		printf("smallest2: %ld, count2: %d\n", uniCode2, uniCount2);
		sameFlag = 1;

		sleep(20);
		*/

		i++;

		if (i == 15){
			//printf("oO!\n");
			//printf("%s\n%s\n", tw1->line, tw2->line);
			break;
			/*printf("keine Übereinstimmung gefunden!\n");
			printf("s1: %s\ns2: %s\n", tw1->line, tw2->line);

			printf("equals %d", equals(tw1->line, tw2->line));
			//sleep(20);
			 */
		}
	}

	//long uniCode1 = getSmallestUnicode(tw1->line, &uniCount1, 0);
	//printf("Smallest Unicode: %ld[%d] in String: %s\n", uniCode1, uniCount1, tw1->line);
	tw1->smallestUniCode = uniCode1;
	tw1->countSmallest = uniCount1;

	//printf("Smallest Unicode: %ld[%d] in String: %s\n", uniCode2, uniCount2, tw2->line);
	tw2->smallestUniCode = uniCode2;
	tw2->countSmallest = uniCount2;

	if (uniCode1 < uniCode2)
	{
		return -1;
	}
	else if (uniCode1 == uniCode2)
	{
		if (uniCount1 > uniCount2)
		{
			return -1;
		}
		else if (uniCount1 < uniCount2)
		{
			return 1;
		}
		else if (uniCount1 == uniCount2)
		{
			if (noMoreResults1 == 1)
				return 1;
			if (noMoreResults2 == 1)
				return -1;

			return 0;
		}
	}
	else
	{
		return 1;
	}
}

int compare(const void *c1, const void *c2) {

	//static unsigned long long int aufrufe = 0;
	//aufrufe++;

	struct tweetData *tw1 = (struct tweetData*) c1;
	struct tweetData *tw2 = (struct tweetData*) c2;

	//printf("%llu\n", aufrufe);
	//fflush(stdout);

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
	qsort(toSort, (int)len, sizeof(struct tweetData), compare);
	//time_t t2 = time(NULL);
	//printf("time %lld", (long long)t2);

	return 0.0;
	//return (float) (t2 - t1);
}
