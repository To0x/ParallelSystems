/*
 * bucketSort.h
 *
 *  Created on: 08.06.2015
 *      Author: Master
 */

#ifndef BUCKETSORT_H_
#define BUCKETSORT_H_

float quickSort(struct tweetData *toSort, long len);
tweetData_t *getPivotElements(tweetData_t *toSort, int buckets);
void bucketSort(tweetData_t *toSort, size_t len, size_t tweetSize, int buckets);
unsigned long long int getSmallestUnicode(unsigned char *currentLine, unsigned long long int offset, int *count, int *noMoreResults);
#endif /* BUCKETSORT_H_ */
