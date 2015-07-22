#include "barcodeParser.h"

const char NewLineBreakTag[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const char StartStopTag[] = { 0x00, 0x53, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const char SkipTag[] =  { 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

const barcodeMapping CharacterMap[] = {
	{ 'A', 1, {0} },
	{ 'B', 1, {0} },
	{ 'C', 1, {0} },
	{ 'D', 1, {0} },
	{ 'E', 1, {0} },
	{ 'F', 1, {0} },
	{ 'G', 1, {0} },
	{ 'H', 1, {0} },
	{ 'I', 1, {0} },
	{ 'J', 1, {0} },
	{ 'K', 1, {0} },
	{ 'L', 1, {0} },
	{ 'M', 1, {0} },
	{ 'N', 1, {0} },
	{ 'O', 1, {0} },
	{ 'P', 1, {0} },
	{ 'Q', 1, {0} },
	{ 'R', 1, {0} },
	{ 'S', 1, {0} },
	{ 'T', 1, {0} },
	{ 'U', 1, {0} },
	{ 'V', 1, {0} },
	{ 'W', 1, {0} },
	{ 'X', 1, {0} },
	{ 'Y', 1, {0} },
	{ 'Z', 1, {0} },
	{ '0', 1, {0} },
	{ '1', 1, {0} },
	{ '2', 1, {0} },
	{ '3', 1, {0} },
	{ '4', 1, {0} },
	{ '5', 1, {0} },
	{ '6', 1, {0} },
	{ '7', 1, {0} },
	{ '8', 1, {0} },
	{ '9', 1, {0} }
};

barcodeContext initializeBarcodeContext(){
	barcodeContext context;
	bzero(&context, sizeof(context));
	return context;
}

int addInputLineToContext(barcodeContext* context, barcodeInput input){
	if (context == NULL){
		return FAILURE;
	}
	
	if (context->noInputLines < MAX_BARCODE_INPUTS){
		context->inputLines[context->noInputLines] = input;
		context->noInputLines++;
		return SUCCESS;
	}
	
	return FAILURE;
}

int validString(char* buffer, size_t size){
	if (buffer != NULL && size == INPUT_LINE_LENGTH){
		return SUCCESS;
	}
	
	return FAILURE;
}

int parseBarcodeContext(barcodeContext context, barcodeOutput* output){
	unsigned int 
		iter = 0, 
		newLine = 0, 
		startStop = 0, 
		bufferSize = 0,
		characterCounter = 0;
	char buffer[MAX_BARCODE_OUTPUT] = {0};
	
	if (output == NULL){
		return FAILURE;
	}
	
	bzero(output, sizeof(barcodeOutput));
	
	for (iter = 0 ; iter < context.noInputLines && startStop < 2; iter++){
        if (newLine == 1){
			char ch = 0x00;
			if (output->size < MAX_BARCODE_OUTPUT && mapInputToCharacter(buffer, &ch, bufferSize) == SUCCESS){
				output->line[output->size] = ch;
				output->size += 1;
			}
            newLine = 0;
            bufferSize = 0;
            bzero(buffer, sizeof(buffer));
        }

		if (isNewBlockSeparator(context.inputLines[iter]) == SUCCESS){
			newLine = 1;
		}else if (isBufferStartEndTag(context.inputLines[iter]) == SUCCESS){
			startStop++;
		}else if (isCharacterTag(context.inputLines[iter]) == SUCCESS){
			if (isSkipTag(context.inputLines[iter]) == FAILURE){
				memcpy(buffer + bufferSize, context.inputLines[iter].line, INPUT_LINE_LENGTH);
				bufferSize += INPUT_LINE_LENGTH;
			}
		}
	}
	
	if (startStop == 2){
		return SUCCESS;
	}
	
	return FAILURE;
}

int isNewBlockSeparator(barcodeInput input){
	if (memcmp((void *)input.line, (void *)NewLineBreakTag, INPUT_LINE_LENGTH) == 0){
		return SUCCESS;
	}
	return FAILURE;
}

int isBufferStartEndTag(barcodeInput input){
	if (memcmp((void *)input.line, (void *)StartStopTag, INPUT_LINE_LENGTH) == 0){
		return SUCCESS;
	}
	return FAILURE;
}

int isSkipTag(barcodeInput input){
	if (memcmp((void *)input.line, (void *)SkipTag, INPUT_LINE_LENGTH) == 0){
		return SUCCESS;
	}
	return FAILURE;
}

int isCharacterTag(barcodeInput input){
	if (input.line[0] == 0x04){
		return SUCCESS;
	}
	return FAILURE;
}

int mapInputToCharacter(char* input, char* output, size_t size){
	int found = FAILURE;
	unsigned int iter = 0;

	for (iter = 0 ; iter < sizeof(CharacterMap) / sizeof(barcodeMapping) && found == FAILURE; iter++){
		if (size == CharacterMap[iter].size){
			if (memcmp(CharacterMap[iter].pattern, input, size) == 0){
				found = SUCCESS;
				*output = CharacterMap[iter].character;
			}
		}
	}
	
	if (found == SUCCESS){
		return SUCCESS;
	}
	
	return FAILURE;
}

barcodeInput convertStringToLineInput(char* buffer, size_t size){
	barcodeInput barcodeLine = {{0}};
	
	if (buffer != NULL && size == INPUT_LINE_LENGTH){
		memcpy((void *)&barcodeLine.line, (void *)buffer, INPUT_LINE_LENGTH);
	}
	
	return barcodeLine;
}




