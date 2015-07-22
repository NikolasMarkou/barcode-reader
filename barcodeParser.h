#ifndef BARCODE_PARSER_H
#define BARCODE_PARSER_H

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <string.h>

#define SUCCESS 0
#define FAILURE -1
#define INPUT_LINE_LENGTH  8
#define MAX_BARCODE_PATTERN 512
#define MAX_BARCODE_INPUTS 1024
#define MAX_BARCODE_OUTPUT 1024

typedef struct barcodeMapping{
	char character;
	unsigned int size;
	char pattern[MAX_BARCODE_PATTERN];
} barcodeMapping;

typedef struct barcodeInput {
	char line[INPUT_LINE_LENGTH];
} barcodeInput;

typedef struct barcodeContext {
	unsigned int noInputLines;
	barcodeInput inputLines[MAX_BARCODE_INPUTS];
} barcodeContext;

typedef struct barcodeOutput {
	unsigned int size;
	char line[MAX_BARCODE_OUTPUT];
} barcodeOutput;

int isSkipTag(barcodeInput input);
int isCharacterTag(barcodeInput input);
int isNewBlockSeparator(barcodeInput input);

barcodeContext initializeBarcodeContext();
int addInputLineToContext(barcodeContext*, barcodeInput);
barcodeInput convertStringToLineInput(char* buffer, size_t size);
int parseBarcodeContext(barcodeContext context, barcodeOutput* output);

#endif
