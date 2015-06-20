/*
 * dataHolder.h
 *
 *  Created on: 30.05.2015
 *      Author: Master
 */

#ifndef DATAHOLDER_H_
#define DATAHOLDER_H_

typedef struct tweetData {

	//int number;
	unsigned char *line;
	unsigned char keywords;
	unsigned char smiles;
	unsigned char hashtags;
	unsigned long long int smallestUniCode;
	int countSmallest;
	//unsigned long size;
	//unsigned long long int reRoundSmallest;
	//int reRoundCountSmallest;

	//struct tweetData *next;
} tweetData_t;

enum QSORT_ANSWERS {
	P1_AFTER_P2 = 1,
	P1_ABOVE_P2 = -1,
	P2_AFTER_P1 = -1,
	P2_ABOVE_P1 = 1,
	QSORT_EQUALS = 0,
};

tweetData_t* parseTweet(unsigned char* tweetString, char* keyWord);

#endif /* DATAHOLDER_H_ */
