/*
 * common.h
 *
 *  Created on: 20.09.2015
 *      Author: Master
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <limits.h>

typedef unsigned long long ullong;
typedef unsigned long ulong;
typedef unsigned long long int ullint;

typedef struct arguments {
	int numBuckets;
	char *keyword;
} argument_t;

int isnumeric(char *str);

#endif /* COMMON_H_ */
