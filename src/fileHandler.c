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

#define BYTE_ALLOCATION 1024;

wchar_t *readLine(FILE *file) {

	//setlocale(LC_ALL, "es_US.UTF-8");
    if (file == NULL) {
        printf("Error: file pointer is null.");
        exit(1);
    }

    int maximumLineLength = BYTE_ALLOCATION;
    wchar_t *lineBuffer = (wchar_t *)malloc(sizeof(wchar_t) * maximumLineLength);

    if (lineBuffer == NULL) {
        printf("Error allocating memory for line buffer.");
        exit(1);
    }

    wchar_t ch = fgetwc(file);
    int count = 0;

    while ((ch != '\n') && (ch != EOF)) {
        if (count == maximumLineLength) {
            maximumLineLength += BYTE_ALLOCATION;
            if ((lineBuffer = realloc(lineBuffer, maximumLineLength)) == NULL) {
                printf("Error reallocating space for line buffer.");
                exit(1);
            }
        }

        lineBuffer[count] = ch;

        /*
        uint8_t intchar;
        intchar = (uint8_t)ch;

        wprintf(L"%lc - ", ch);
        printf("%" PRIu32, intchar);

        int laenge;
        if (intchar <= 127) // 0xxx xxxx --> befindet sich im ASCII bereich
        	laenge = 1;
        else if (intchar <= 224) // 110x xxxx --> UTF8 Encoding beginnt 2Byte
        	laenge = 2;
        else if (intchar <= 240) // 1110 xxxx --> 3 Byte
        	laenge = 3;
        else
        	laenge = 4; // 1111 0xxx --> 4 Byte

        // 10xx xxxx --> all following bytes

        printf("\n");
		*/
        count++;

        ch = fgetwc(file);
    }

    lineBuffer[count] = '\0';
    realloc(lineBuffer, count+1);
    return lineBuffer;
}
