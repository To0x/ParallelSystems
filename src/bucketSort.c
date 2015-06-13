/*
 * bucketSort.c
 *
 *  Created on: 08.06.2015
 *      Author: Master
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dataHolder.h"

void Bucket_Sort(int array[], int n)
{
    int i, j;
    int count[n];
    for (i = 0; i < n; i++)
        count[i] = 0;

    for (i = 0; i < n; i++)
        (count[array[i]])++;

    for (i = 0, j = 0; i < n; i++)
        for(; count[i] > 0; (count[i])--)
            array[j++] = i;
}

int compare(const void *c1, const void *c2) {

	struct tweetData *tw1 = (struct tweetData*)c1;
	struct tweetData *tw2 = (struct tweetData*)c2;

	if (tw1->keywords > tw2->keywords)
	{
		return -1;
	}
	else if (tw1->keywords == tw2->keywords)
	{
		//TODO: check for Histogramm!
		return 0;
	}
	else {
		return 1;
	}
}

float quickSort(struct tweetData *toSort, long len) {
	//time_t t1 = time(NULL);
	//printf("time %lld", (long long)t1);
	//qsort(toSort, len, sizeof(struct tweetData), compare);
	//time_t t2 = time(NULL);
	//printf("time %lld", (long long)t2);

	return 0.0;
	//return (float)(t2-t1);
}
