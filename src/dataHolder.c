/*
 * dataHolder.c
 *
 *  Created on: 30.05.2015
 *      Author: Master
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "dataHolder.h"
#include <limits.h>

void resetTweetData(tweetData_t *td) {
	td->keywords = 0;
	td->index = ULLONG_MAX;
	td->smallestUniCode = ULLONG_MAX;
	td->countSmallest = 0;
}

unsigned char next(unsigned char *tweet) {
	unsigned char retVal = (unsigned char) *(++tweet);
	printf("%c %d\n", retVal, retVal);
	return retVal;
}

bool isKeyWord(unsigned char* tweetString, char* keyword) {
	// To avoid to build a for loop for every letter
	if (tweetString[0] != keyword[0]) {
		return false;
	}

	for (int i = 1; i < strlen(keyword); i++) {
		if ((int)tweetString[i] < 128 && tweetString[i] != keyword[i]) {
			return false;
		}
		if((int)tweetString[i] > 128){
			return false;
		}
	}

	return true;
}

tweetData_t* parseTweet(unsigned char* tweetString, char* keyWord) {
	int actChar;

	tweetData_t *thisData;
	thisData = malloc(sizeof(tweetData_t));
	resetTweetData(thisData);
	thisData->line = tweetString;
	// TODO: CAST!
	//thisData->size = sizeof(struct tweetData) + (sizeof(unsigned char) * ((unsigned long)(sizeof(tweetString) / sizeof(unsigned char))) + 1);

	while ((actChar = ((int)*tweetString)) != (int)'\0') {

		if (actChar < 128) { // 0xxx xxxx --> befindet sich im ASCII bereich

			if (isKeyWord(tweetString, keyWord)) {
				thisData->keywords++;
			}
		} else if (actChar < 224) { // 110x xxxx --> UTF8 Encoding beginnt 2Byte
			tweetString++;
		} else if (actChar < 240) { // 1110 xxxx --> 3 Byte
			tweetString += 2;
		} else {// 1111 0xxx --> 4 Byte
			// count Smileys
			if (actChar == 240) {
				actChar = (unsigned char) *(++tweetString); // next(tweetString);
				if (actChar == 159) {
					actChar = (unsigned char) *(++tweetString); // next(tweetString);
					if (actChar == 152) {
						actChar = (unsigned char) *(++tweetString); // next(tweetString);
//						if (actChar >= 129)
//							thisData->smiles++;
					} else if (actChar == 153) {
						actChar = (unsigned char) *(++tweetString); // next(tweetString);
//						if (actChar <= 143)
//							thisData->smiles++;
					} else {
						tweetString += 4 - 3;
					}
				} else {
					tweetString += 4 - 2;
				}
			} else {
				tweetString += 4 - 1;
			}
		}
		tweetString++;
	}
	return thisData;
}

