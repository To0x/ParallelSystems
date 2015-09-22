/*
 * time.h
 *
 *  Created on: 20.09.2015
 *      Author: Master
 */

#ifndef TIME_H_
#define TIME_H_

#include <time.h>
#include <sys/time.h>

typedef struct timeval myTimer;

myTimer *startTimer();
unsigned long long stopTimer(myTimer *startedTimer);

#endif /* TIME_H_ */
