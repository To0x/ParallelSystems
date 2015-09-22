/*
 * time.c
 *
 *  Created on: 20.09.2015
 *      Author: Master
 */

#include <stdio.h>
#include <stdlib.h>
#include "time.h"

myTimer *startTimer() {

	myTimer *retVal = malloc(sizeof(myTimer));
	gettimeofday(retVal, NULL);

	return retVal;
}

unsigned long long stopTimer(myTimer *startedTimer) {

	myTimer *finishedAt = malloc(sizeof(myTimer));
	gettimeofday(finishedAt, NULL);

	unsigned long long microsec1 = ((unsigned long long) startedTimer->tv_sec * 1000000) + startedTimer->tv_usec;
	unsigned long long microsec2 = ((unsigned long long) finishedAt->tv_sec * 1000000) + finishedAt->tv_usec;

	free(startedTimer);
	free(finishedAt);

	return ((microsec2 - microsec1) / 1000000);
}
