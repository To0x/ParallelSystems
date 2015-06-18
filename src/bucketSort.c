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
#include <math.h>

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

void getBinOfDec(int dec, char bin[9]) {
	for (int i = 7; i >= 0; i--) {
		if (dec % 2 == 1) {
			bin[i] = '1';
			dec--;
		} else {
			bin[i] = '0';
		}
		dec = (int)dec/2;
	}
	bin[8] = '\0';
}

long binToDec(char bin[50]) /* Function to convert binary to decimal.*/
{
	int decimal = 0;
	int len = strlen(bin);
	for (int i = len; i >= 0; i--) {
		if (bin[i] == '1') {
			decimal += (int) pow(2, 7 - i);
		}
	}
	return decimal;
}
int compareHistogram(struct tweetData *tw1, struct tweetData *tw2) {
	static long y = 0;
	y++;
//	char test[9]= "\0";
//	getBinOfDec(12,test);
//	printf("12 ---> %s\n",test);
	//printf("00001100 --> %d\n", binToDec("00001100\0"));
	
	int unicode[2][UNICODE_ARRAY_LENGTH];
	for (int i = 0; i < UNICODE_ARRAY_LENGTH; i++) {
		unicode[0][i] = 0;
		unicode[1][i] = 0;
	}

	for (int i = 0; i < strlen(tw1->line); i++) {

		int actChar = (int) tw1->line[i];
		if (actChar < 128) { // 0xxx xxxx --> befindet sich im ASCII bereich
			unicode[0][(int) tw1->line[i]] += 1;
		} else if (actChar < 224) { // 110x xxxx --> UTF8 Encoding beginnt 2Byte
			char binString[17] = "\0";
			char byte[9] = "\0";
			getBinOfDec(tw1->line[i], byte);
//			byte[0] = '0';
//			byte[1] = '0';
			for (int i = 0; i < 8; i++) {
				binString[i] = byte[i];
			}

			getBinOfDec(tw1->line[i + 1], byte);
			for (int i = 8; i < 16; i++) {
				binString[i] = byte[8 - i];
			}
			binString[16] = '\0';
			unicode[0][binToDec(binString)] += 1;
			i++;
		} else if (actChar < 240) { // 1110 xxxx --> 3 Byte
			char binString[25] = "\0";
			char byte[9] = "\0";

			getBinOfDec(tw1->line[i], byte);
//			byte[0] = '0';
//			byte[1] = '0';
//			byte[2] = '0';
			for (int i = 0; i < 8; i++) {
				binString[i] = byte[i];
			}

			getBinOfDec(tw1->line[i + 1], byte);
			for (int i = 8; i < 16; i++) {
				binString[i] = byte[8 - i];
			}

			getBinOfDec(tw1->line[i + 1], byte);
			for (int i = 16; i < 24; i++) {
				binString[i] = byte[16 - i];
			}
			binString[24] = '\0';
			unicode[0][binToDec(binString)] += 1;
			i++;
			i++;
		} else if (actChar < 248) { // 1111 0xxx --> 4 Byte{
			char binString[33] = "\0";
			char byte[9] = "\0";

			getBinOfDec(tw1->line[i], byte);
//			byte[0] = '0';
//			byte[1] = '0';
//			byte[2] = '0';
//			byte[3] = '0';
			for (int i = 0; i < 8; i++) {
				binString[i] = byte[i];
			}

			getBinOfDec(tw1->line[i + 1], byte);
			for (int i = 8; i < 16; i++) {
				binString[i] = byte[8 - i];
			}

			getBinOfDec(tw1->line[i + 1], byte);
			for (int i = 16; i < 24; i++) {
				binString[i] = byte[16 - i];
			}

			getBinOfDec(tw1->line[i + 1], byte);
			for (int i = 24; i < 32; i++) {
				binString[i] = byte[24 - i];
			}
			binString[32] = '\0';
			unicode[0][binToDec(binString)] += 1;
			i++;
			i++;
			i++;
			//printf("Unicode[0][%d]++\n",tw1->line[i] + tw1->line[i+1] + tw1->line[i+2] + tw1->line[i+3]);
			//	printf("4 Byte\n");
		} else {
			printf("test.......\n");
		}
	}
	for (int i = 0; i < strlen(tw2->line); i++) {
		int actChar = (int) tw2->line[i];
		if (actChar < 128) { // 0xxx xxxx --> befindet sich im ASCII bereich
			unicode[1][(int) tw2->line[i]] += 1;
		} else if (actChar < 224) { // 110x xxxx --> UTF8 Encoding beginnt 2Byte
			char binString[17] = "\0";
			char byte[9] = "\0";
			getBinOfDec(tw1->line[i], byte);
//			byte[0] = '0';
//			byte[1] = '0';
			for (int i = 0; i < 8; i++) {
				binString[i] = byte[i];
			}

			getBinOfDec(tw1->line[i + 1], byte);
			for (int i = 8; i < 16; i++) {
				binString[i] = byte[8 - i];
			}
			binString[16] = '\0';
			unicode[1][binToDec(binString)] += 1;
			i++;
		} else if (actChar < 240) { // 1110 xxxx --> 3 Byte
			char binString[25] = "\0";
			char byte[9] = "\0";

			getBinOfDec(tw1->line[i], byte);
//			byte[0] = '0';
//			byte[1] = '0';
//			byte[2] = '0';
			for (int i = 0; i < 8; i++) {
				binString[i] = byte[i];
			}

			getBinOfDec(tw1->line[i + 1], byte);
			for (int i = 8; i < 16; i++) {
				binString[i] = byte[8 - i];
			}

			getBinOfDec(tw1->line[i + 1], byte);
			for (int i = 16; i < 24; i++) {
				binString[i] = byte[16 - i];
			}
			binString[24] = '\0';
			unicode[1][binToDec(binString)] += 1;
			i++;
			i++;
		} else if (actChar < 248) { // 1111 xxxx --> 4 Byte{
			char binString[33] = "\0";
			char byte[9] = "\0";

			getBinOfDec(tw1->line[i], byte);
//			byte[0] = '0';
//			byte[1] = '0';
//			byte[2] = '0';
//			byte[3] = '0';
			for (int i = 0; i < 8; i++) {
				binString[i] = byte[i];
			}

			getBinOfDec(tw1->line[i + 1], byte);
			for (int i = 8; i < 16; i++) {
				binString[i] = byte[8 - i];
			}

			getBinOfDec(tw1->line[i + 1], byte);
			for (int i = 16; i < 24; i++) {
				binString[i] = byte[16 - i];
			}

			getBinOfDec(tw1->line[i + 1], byte);
			for (int i = 24; i < 32; i++) {
				binString[i] = byte[24 - i];
			}
			binString[32] = '\0';
			unicode[1][binToDec(binString)] += 1;
			i++;
			i++;
			i++;
			//printf("4 Byte\n");
		}
		else {
					printf("test.......\n");
				}
	}

	for (int i = 0; i < UNICODE_ARRAY_LENGTH; i++) {
		if (unicode[0][i] != unicode[1][i]) {
			if (unicode[0][i] > unicode[1][i]) {
				tw1->smallestUniCode = i;
				tw1->countSmallest = unicode[0][i];
//				if (tw1->keywords == 0 && i > 100) {
//					printf("%s --> %d -> %d --> %c\n", tw1->line, i,
//							unicode[0][i], (char) i);
//					printf("%s --> %d -> %d --> %c\n", tw2->line, i,
//							unicode[1][i], (char) i);
//					printf("Return -1\n");
//					printf(
//							"-------------------------------------------------------------------------------\n");
//				}
				return -1;
			} else if (unicode[0][i] < unicode[1][i]) {
				tw2->smallestUniCode = i;
				tw2->countSmallest = unicode[1][i];
//				if (tw2->keywords == 0 && i > 100) {
//					printf("%s --> %d -> %d --> %c\n", tw1->line, i,
//							unicode[0][i], (char) i);
//					printf("%s --> %d -> %d --> %c\n", tw2->line, i,
//							unicode[1][i], (char) i);
//					printf("Return 1\n");
//					printf(
//							"-------------------------------------------------------------------------------\n");
//				}
				return 1;
			}
		}
	}
	return 0;
}

int compare(const void *c1, const void *c2) {

	struct tweetData *tw1 = (struct tweetData*) c1;
	struct tweetData *tw2 = (struct tweetData*) c2;

	//printf("compare");

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
