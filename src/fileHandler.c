/*
 * fileHandler.c
 *
 *  Created on: 29.04.2015
 *      Author: Master
 */

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <inttypes.h>

#define BYTE_ALLOCATION 512;

unsigned char *readLine(FILE *file) {

    if (file == NULL) {
        printf("Error: file pointer is null.");
        exit(1);
    }

    int maximumLineLength = BYTE_ALLOCATION;
     char *lineBuffer = ( char *)malloc(sizeof( char) * maximumLineLength);

    if (lineBuffer == NULL) {
        printf("Error allocating memory for line buffer.");
        exit(1);
    }

    int ch = getc(file);
    int count = 0;

    while ((ch != (int)'\n') && (ch != (int)EOF)) {
        if (count == maximumLineLength) {
            maximumLineLength += BYTE_ALLOCATION;
            if ((lineBuffer = realloc(lineBuffer, maximumLineLength)) == NULL) { // TODO: Parameter type mismatch: Using 'size_t' for signed values of type 'int'
                printf("Error reallocating space for line buffer.");
                exit(1);
            }
        }

        lineBuffer[count] = ( char)ch;
        count++;
        ch = getc(file);

        if (ch == EOF)
        {
        	fprintf(stderr,"Reached EOF!\n");
        	lineBuffer[count] = '\0';
        	break;
        }
    }

    lineBuffer[count] = '\0';
    //realloc(lineBuffer, count+1);
    return lineBuffer;  // TODO: Returning 'char*' from a function returning 'unsigned char*': Incompatible pointer types 'unsigned char*' and 'char*'
}
