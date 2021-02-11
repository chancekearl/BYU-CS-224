#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define BAD_NUMBER_ARGS 1
int BYTES_HEX = 16;
int BYTES_BINARY = 6;

FILE *parseCommandLine(int argc, char **argv, int *bits) {
    if (argc > 2) {
        printf("Usage: %s [-b|-bits]\n", argv[0]);
        exit(BAD_NUMBER_ARGS);
    }

    if (argc == 2 &&
        (strcmp(argv[1], "-b") == 0 || strcmp(argv[1], "-bits") == 0)) {
        *bits = TRUE;
    } else {
        *bits = FALSE;
    }

    return stdin;
}

void printDataAsHex(unsigned char *data, size_t size) {
    for (int i = 0; i < BYTES_HEX; i++) {
        if (i < BYTES_HEX - 1 && !(i % 2)) {
            printf(" ");
        }
        if (i < (int)size) {
            printf("%02x", *(data + i));
        } else {
            printf("%*c", 2, ' ');
        }
    }
}

void printDataAsChars(unsigned char *data, size_t size) {
    for (int i = 0; i < (int) size; i++) {
        unsigned char currentChar = *(data + i);
        if (currentChar > 126 || currentChar < 32) {
            currentChar = '.';
        }
        printf("%c", currentChar);
    }
}

void printDataAsBinary(unsigned char *data, size_t size) {
    for (int i = 0; i < BYTES_BINARY; i++) {
        printf(" ");

        if (i < (int)size) {
            char byteStr[8];
            unsigned char currentChar = *(data + i);

            for (int i = 7; i >= 0; i--) {
                byteStr[i] = currentChar % 2 == 1 ? '1' : '0';
                currentChar /= 2;
            }

            printf("%s", byteStr);
        } else {
            // 8 spaces where a byte would go
            printf("%*c", 8, ' ');
        }
    }
}

void readAndPrintInputAsHex(FILE *input) {
    unsigned char data[16];
    int numBytesRead = fread(data, 1, 16, input);
    unsigned int offset = 0;
    while (numBytesRead != 0) {
        printf("%08x:", offset);
        offset += numBytesRead;
        printDataAsHex(data, numBytesRead);
        printf("  ");
        printDataAsChars(data, numBytesRead);
        printf("\n");
        numBytesRead = fread(data, 1, 16, input);
    }
}

void readAndPrintInputAsBits(FILE *input) {
    unsigned char data[BYTES_BINARY];
    int numBytesRead = fread(data, 1, BYTES_BINARY, input);
    unsigned int offset = 0;
    while (numBytesRead != 0) {
        printf("%08x:", offset);
        offset += numBytesRead;
        printDataAsBinary(data, numBytesRead);
        printf("  ");
        printDataAsChars(data, numBytesRead);
        printf("\n");
        numBytesRead = fread(data, 1, BYTES_BINARY, input);
    }
}

int main(int argc, char **argv) {
    int bits = FALSE;
    FILE *input = parseCommandLine(argc, argv, &bits);

    if (bits == FALSE) {
        readAndPrintInputAsHex(input);
    } else {
        readAndPrintInputAsBits(input);
    }
    return 0;
}
