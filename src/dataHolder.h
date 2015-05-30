/*
 * dataHolder.h
 *
 *  Created on: 30.05.2015
 *      Author: Master
 */

#ifndef DATAHOLDER_H_
#define DATAHOLDER_H_

struct tweetData{

	int keywords;
	int smiles;
	int hashtags;

};

struct tweetData* parseTweet(wchar_t* tweetString, char* keyWord);

#endif /* DATAHOLDER_H_ */
