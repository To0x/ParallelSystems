/*
 * fileHandler.c
 *
 *  Created on: 29.04.2015
 *      Author: Master
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>
#include <inttypes.h>

#define BYTE_ALLOCATION 512;

unsigned char *readLine(FILE *file) {

    if (file == NULL) {
        printf("Error: file pointer is null.");
        exit(1);
    }

    int maximumLineLength = BYTE_ALLOCATION;
    unsigned char *lineBuffer = (unsigned char *)malloc(sizeof(unsigned char) * maximumLineLength);

    if (lineBuffer == NULL) {
        printf("Error allocating memory for line buffer.");
        exit(1);
    }

    int ch = getc(file);
    int count = 0;

    while ((ch != (int)'\n') && (ch != EOF)) {
        if (count == maximumLineLength) {
            maximumLineLength += BYTE_ALLOCATION;
            if ((lineBuffer = realloc(lineBuffer, maximumLineLength)) == NULL) {
                printf("Error reallocating space for line buffer.");
                exit(1);
            }
        }

        lineBuffer[count] = (unsigned char)ch;
        count++;
        ch = getc(file);
    }

    lineBuffer[count] = '\0';
    //realloc(lineBuffer, count+1);
    return lineBuffer;
}
