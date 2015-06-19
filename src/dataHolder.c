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

//#define PARSE(__x__, __y__) y=((unsigned char)*(++__x__);)

void resetTweetData(struct tweetData *td) {
	td->hashtags = 0;
	td->keywords = 0;
	td->smiles = 0;
	td->smallestUniCode = 900000;
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
		if (tweetString[i] < 128 && tweetString[i] != keyword[i]) {
			return false;
		}
		if(tweetString[i] > 128){
			return false;
		}
	}

	return true;
}

struct tweetData* parseTweet(unsigned char* tweetString, char* keyWord) {

	unsigned char actChar;
	int actCharByteLenght;

	struct tweetData *thisData;
	thisData = malloc(sizeof(struct tweetData));
	resetTweetData(thisData);
	thisData->line = tweetString;
	// TODO: CAST!
	thisData->size = sizeof(struct tweetData) + (sizeof(unsigned char) * ((unsigned long)strlen(tweetString)) + 1);

	while ((actChar = *tweetString) != '\0') {

		if (actChar < 128) { // 0xxx xxxx --> befindet sich im ASCII bereich
			actCharByteLenght = 1;

			//printf("Act: %c\n", actChar);

			if (isKeyWord(tweetString, keyWord)) {
				thisData->keywords++;
				//printf("KeyWord++\n");
			}

			// A hashtag followed by whitespace is no hashtag in our meaning.
			if (actChar == '#' && (unsigned char) *(1 + tweetString) != ' ')
				thisData->hashtags++;
		} else if (actChar < 224) { // 110x xxxx --> UTF8 Encoding beginnt 2Byte
			actCharByteLenght = 2;
			tweetString++;
		} else if (actChar < 240) { // 1110 xxxx --> 3 Byte
			actCharByteLenght = 3;
			tweetString += 2;
		} else {
			actCharByteLenght = 4; // 1111 0xxx --> 4 Byte

			// count Smileys
			if (actChar == 240) {
				actChar = (unsigned char) *(++tweetString); // next(tweetString);
				if (actChar == 159) {
					actChar = (unsigned char) *(++tweetString); // next(tweetString);
					if (actChar == 152) {
						actChar = (unsigned char) *(++tweetString); // next(tweetString);
						if (actChar >= 129)
							thisData->smiles++;
					} else if (actChar == 153) {
						actChar = (unsigned char) *(++tweetString); // next(tweetString);
						if (actChar <= 143)
							thisData->smiles++;
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

