/*
 * dataHolder.c
 *
 *  Created on: 30.05.2015
 *      Author: Master
 */

#include <stdio.h>
#include <stdlib.h>
#include "dataHolder.h"

void resetTweetData(struct tweetData *td) {
	td->hashtags = 0;
	td->keywords = 0;
	td->smiles = 0;
}

int isSmile(wchar_t* tweetString) {


	uint16_t actIntChar;
	wchar_t actChar;

	actChar = *tweetString;
	actIntChar = (uint16_t)actChar;




	return 0;
}

struct tweetData* parseTweet(wchar_t* tweetString, char* keyWord) {

	wchar_t actChar;
	uint16_t actIntChar;
	int actCharByteLenght;

	static struct tweetData *thisData;
	thisData = malloc(sizeof(struct tweetData));
	resetTweetData(thisData);

	while ((actChar = *tweetString) != '\0') {

        actIntChar = (uint16_t)actChar;
        if (actIntChar < 128) {// 0xxx xxxx --> befindet sich im ASCII bereich
        	actCharByteLenght = 1;

        	if (actChar == '#')
        		thisData->hashtags++;
        }
        else if (actIntChar < 224) { // 110x xxxx --> UTF8 Encoding beginnt 2Byte
        	actCharByteLenght = 2;
        	tweetString++;
        }
        else if (actIntChar < 240) {// 1110 xxxx --> 3 Byte
        	actCharByteLenght = 3;
        	tweetString+=2;
        }
        else {
        	actCharByteLenght = 4; // 1111 0xxx --> 4 Byte

        	// count Smileys
        	if (actIntChar == 240)
        	{
        		if (actIntChar == 240) {
        			actIntChar = (uint16_t)*(++tweetString);
        			if (actIntChar == 159) {
        				actIntChar = (uint16_t)*(++tweetString);
        				if (actIntChar == 152) {
        					actIntChar = (uint16_t)*(++tweetString);
        					if (actIntChar >= 129)
        						thisData->smiles++;
        				} else if (actIntChar == 153) {
        					actIntChar = (uint16_t)*(++tweetString);
        					if (actIntChar <= 143)
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

        }

		tweetString++; // die Anzahl der Bytes überspringen, solange das Zeichen war.
	}

	return thisData;

}
