/*
 * dataHolder.h
 *
 *  Created on: 30.05.2015
 *      Author: Master
 */

#ifndef DATAHOLDER_H_
#define DATAHOLDER_H_

struct tweetData {

	unsigned char *line;
	unsigned char keywords;
	unsigned char smiles;
	unsigned char hashtags;
	unsigned long size;
	unsigned long smallestUniCode;
	unsigned char countSmallest;

	//struct tweetData *next;
};

struct tweetData* parseTweet(unsigned char* tweetString, char* keyWord);

#endif /* DATAHOLDER_H_ */
