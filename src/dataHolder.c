/*
 * dataHolder.c
 *
 *  Created on: 30.05.2015
 *      Author: Master
 */

#include <stdio.h>
#include <stdlib.h>
#include "dataHolder.h"

//#define PARSE(__x__, __y__) y=((uint8_t)*(++__x__);)

void resetTweetData(struct tweetData *td) {
	td->hashtags = 0;
	td->keywords = 0;
	td->smiles = 0;
}

uint8_t next(unsigned char *tweet) {
	uint8_t retVal = (uint8_t)*(++tweet);
	printf("%c %d\n", retVal, retVal);
	return retVal;
	//return (uint8_t)*(++tweet);
}

struct tweetData* parseTweet(unsigned char* tweetString, char* keyWord) {

	uint8_t actChar;
	int actCharByteLenght;

	static struct tweetData *thisData;
	thisData = malloc(sizeof(struct tweetData));
	resetTweetData(thisData);

	while ((actChar = *tweetString) != '\0') {

        if (actChar < 128) {// 0xxx xxxx --> befindet sich im ASCII bereich
        	actCharByteLenght = 1;

        	if (actChar == '#')
        		thisData->hashtags++;
        }
        else if (actChar < 224) { // 110x xxxx --> UTF8 Encoding beginnt 2Byte
        	actCharByteLenght = 2;
        	tweetString++;
        }
        else if (actChar < 240) {// 1110 xxxx --> 3 Byte
        	actCharByteLenght = 3;
        	tweetString+=2;
        }
        else {
        	actCharByteLenght = 4; // 1111 0xxx --> 4 Byte

        	// count Smileys
			if (actChar == 240) {
				actChar = (uint8_t)*(++tweetString); // next(tweetString);
				if (actChar == 159) {
					actChar = (uint8_t)*(++tweetString); // next(tweetString);
					if (actChar == 152) {
						actChar = (uint8_t)*(++tweetString); // next(tweetString);
						if (actChar >= 129)
							thisData->smiles++;
					} else if (actChar == 153) {
						actChar = (uint8_t)*(++tweetString); // next(tweetString);
						if (actChar <= 143)
							thisData->smiles++;
					} else {
						tweetString+=4-3;
					}
				} else
				{
					tweetString+=4-2;
				}
			} else
			{
				tweetString+=4-1;
			}
        }

		tweetString++;
	}

	return thisData;

}
