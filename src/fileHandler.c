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

#define BYTE_ALLOCATION 4096;

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
            if ((lineBuffer = realloc(lineBuffer, maximumLineLength)) == NULL) {
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

        	//char actChar;
        	/*while ((actChar = *lineBuffer) != '\0') {
        		printf("%c: %d\n", actChar, (int)actChar);
        		lineBuffer++;
        	}

        	printf("%s", lineBuffer);
        	*/
        	break;
        }
    }

    lineBuffer[count] = '\0';
    //realloc(lineBuffer, count+1);
    return lineBuffer;
}
