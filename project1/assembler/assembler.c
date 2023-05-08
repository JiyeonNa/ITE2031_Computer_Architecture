/* Assembler code fragment for LC-2K */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINELENGTH 1000

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);

int main(int argc, char *argv[]) 
{
	char *inFileString, *outFileString;
	FILE *inFilePtr, *outFilePtr;
	char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH], 
			 arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

	if (argc != 3) {
		printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
				argv[0]);
		exit(1);
	}

	inFileString = argv[1];
	outFileString = argv[2];

	inFilePtr = fopen(inFileString, "r");
	if (inFilePtr == NULL) {
		printf("error in opening %s\n", inFileString);
		exit(1);
	}
	outFilePtr = fopen(outFileString, "w");
	if (outFilePtr == NULL) {
		printf("error in opening %s\n", outFileString);
		exit(1);
	}

	/* TODO: Phase-1 label calculation */
	int lineNum = 0;
	char addressTable[MAXLINELENGTH][MAXLINELENGTH];
	int addressTableIndex = 0;
	while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
		lineNum++;

		/* check if label is not empty */
		if (label[0] != '\0') {
			/* check if there's a duplicate definition of labels */
			for (int i = 0; i < addressTableIndex; i++) {
				if (!strcmp(label, addressTable[i])) {
					printf("error: duplicate label definition at line %d\n", lineNum);
					exit(1);
				}
			}

			/* add label to address table */
			strcpy(addressTable[addressTableIndex], label);
		}
		addressTableIndex++;

		/* check if there's any use of undefined labels */
		if ((strcmp(opcode, "lw") == 0 || strcmp(opcode, "sw") == 0 || strcmp(opcode, "beq") == 0 || strcmp(opcode, "jalr") == 0 || strcmp(opcode, ".fill") == 0) && label[0] != '\0') {
			/* check if label exist in the address table */
			int labelExist = 0;
			for (int i = 0; i < addressTableIndex; i++) {
				if (!strcmp(label, addressTable[i])) {
					labelExist = 1;
					break;
				}
			}

			if (!labelExist) {
				printf("error: undefined label at line %d\n", lineNum);
				exit(1);
			}
		}
	}

	/* this is how to rewind the file ptr so that you start reading from the beginning of the file */
	rewind(inFilePtr);

	/* TODO: Phase-2 generate machine codes to outfile */
	int PC = 0;
	lineNum = 0;
	while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
		lineNum++;

		/* R-type instructions */
		if (strcmp(opcode, "add") == 0 || strcmp(opcode, "nor") == 0) {
			/* check if register argument is an integer */
			if (!isNumber(arg0) || !isNumber(arg1) || !isNumber(arg2)) {
				printf("error: invalid register argument at line %d\n", lineNum);
				exit(1);
			}

			int regA = atoi(arg0);
			int regB = atoi(arg1);
			int destReg = atoi(arg2);

			/* check if register number is inside the range [0,7] */
			if (regA < 0 || regA > 7 || regB < 0 || regB > 7 || destReg < 0 || destReg > 7) {
				printf("error: invalid register number at line %d\n", lineNum);
				exit(1);
			}

			/* generate machine code */
			int machineCode = 0;
			if (!strcmp(opcode, "add")){
				machineCode = machineCode | (0 << 22);
			}
			else if (!strcmp(opcode, "nor")) {
				machineCode = machineCode | (1 << 22);
			}
			machineCode = machineCode | (regA << 19);
			machineCode = machineCode | (regB << 16);
			machineCode = machineCode | (destReg << 0);
			fprintf(outFilePtr, "%d\n", machineCode);
		}

		/* I-type instructions */
		else if (strcmp(opcode, "lw") == 0 || strcmp(opcode, "sw") == 0 || strcmp(opcode, "beq") == 0) {
			/* check if register argument is an integer */
			if (!isNumber(arg0) || !isNumber(arg1)) {
				printf("error: invalid register argument at line %d\n", lineNum);
				exit(1);
			}

			int regA = atoi(arg0);
			int regB = atoi(arg1);
			char* field2 = arg2;
			int offsetField;
			int beqOffset;

			/* check if register number is inside the range [0,7] */
			if (regA < 0 || regA > 7 || regB < 0 || regB > 7) {
				printf("error: invalid register number at line %d\n", lineNum);
				exit(1);
			}

			/* if field2 is a numeric value */
			if (isNumber(field2)) {
				/* check if the offsetField fits in 16 bits */
				if (atoi(field2) < -32768 || atoi(field2) > 32767) {
					printf("error: offsetField out of range at line %d\n", lineNum);
					exit(1);
				}
				/* chop off all but the lowest 16 bits for negatvie values of offsetField */
				if (atoi(field2) < 0) {
					offsetField = atoi(field2) & 0xFFFF;
					beqOffset = atoi(field2) & 0xFFFF;
				}
				else {
					offsetField = atoi(field2);
					beqOffset = atoi(field2);
				}
			}

			/* if offsetField is a symbolic address */
			else {
				/* check if label exist in the address table */
				int labelExist = 0;
				for (int i = 0; i < addressTableIndex; i++) {
					if (!strcmp(field2, addressTable[i])) {
						offsetField = i;
						beqOffset = i - PC - 1;
						if(beqOffset < 0) {
							beqOffset = beqOffset & 0xFFFF;
						}
						labelExist = 1;
						break;
					}
				}

				if (!labelExist) {
					printf("error: undefined label at line %d\n", lineNum);
					exit(1);
				}
			}

			/* generate machine code */
			int machineCode = 0;
			if (!strcmp(opcode, "lw")) {
				machineCode = machineCode | (2 << 22);
				machineCode = machineCode | (regA << 19);
				machineCode = machineCode | (regB << 16);
				machineCode = machineCode | (offsetField << 0);
			}
			else if (!strcmp(opcode, "sw")) {
				machineCode = machineCode | (3 << 22);
				machineCode = machineCode | (regA << 19);
				machineCode = machineCode | (regB << 16);
				machineCode = machineCode | (offsetField << 0);
			}
			else if (!strcmp(opcode, "beq")) {
				machineCode = machineCode | (4 << 22);
				machineCode = machineCode | (regA << 19);
				machineCode = machineCode | (regB << 16);
				machineCode = machineCode | (beqOffset << 0);
			}
			fprintf(outFilePtr, "%d\n", machineCode);
		}

		/* J-type instructions */
		else if (strcmp(opcode, "jalr") == 0) {
			/* check if register argument is an integer */
			if (!isNumber(arg0) || !isNumber(arg1)) {
				printf("error: invalid register argument at line %d\n", lineNum);
				exit(1);
			}

			int regA = atoi(arg0);
			int regB = atoi(arg1);

			/* check if register number is inside the range [0,7] */
			if (regA < 0 || regA > 7 || regB < 0 || regB > 7) {
				printf("error: invalid register number at line %d\n", lineNum);
				exit(1);
			}

			/* generate machine code */
			int machineCode = 0;
			machineCode = machineCode | (5 << 22);
			machineCode = machineCode | (regA << 19);
			machineCode = machineCode | (regB << 16);
			fprintf(outFilePtr, "%d\n", machineCode);
		}

		/* O-type instructions */
		else if (strcmp(opcode, "halt") == 0 || strcmp(opcode, "noop") == 0) {
			int machineCode = 0;
			if (!strcmp(opcode, "halt")) {
				machineCode = machineCode | (6 << 22);
			}
			else if (!strcmp(opcode, "noop")) {
				machineCode = machineCode | (7 << 22);
			}
			fprintf(outFilePtr, "%d\n", machineCode);
		}

		/* .fill */
		else if (strcmp(opcode, ".fill") == 0) {
			char *field0 = arg0;
			int fillValue;

			/* if field0 is a numeric value */
			if (isNumber(field0)) {
				fillValue = atoi(field0);
			}

			/* if field0 is a symbolic address */
			else {
				/* check if label exist in the address table */
				int labelExist = 0;
				for (int i = 0; i < addressTableIndex; i++) {
					if (!strcmp(field0, addressTable[i])) {
						fillValue = i;
						labelExist = 1;
						break;
					}
				}

				if (!labelExist) {
					printf("error: undefined label at line %d\n", lineNum);
					exit(1);
				}
			}

			fprintf(outFilePtr, "%d\n", fillValue);
		}

		else {
			printf("error: invalid opcode at line %d\n", lineNum);
			exit(1);
		}

		PC++;
	}

	if (inFilePtr) {
		fclose(inFilePtr);
	}
	if (outFilePtr) {
		fclose(outFilePtr);
	}
	return(0);
}

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
		char *arg1, char *arg2)
{
	char line[MAXLINELENGTH];
	char *ptr = line;

	/* delete prior values */
	label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

	/* read the line from the assembly-language file */
	if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
		/* reached end of file */
		return(0);
	}

	/* check for line too long (by looking for a \n) */
	if (strchr(line, '\n') == NULL) {
		/* line too long */
		printf("error: line too long\n");
		exit(1);
	}

	/* is there a label? */
	ptr = line;
	if (sscanf(ptr, "%[^\t\n\r ]", label)) {
		/* successfully read label; advance pointer over the label */
		ptr += strlen(label);
	}

	/*
	 * Parse the rest of the line.  Would be nice to have real regular
	 * expressions, but scanf will suffice.
	 */
	sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%"
			"[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]", opcode, arg0, arg1, arg2);
	return(1);
}

int isNumber(char *string)
{
	// /* return 1 if string is a number */
	// int i;
	// return( (sscanf(string, "%d", &i)) == 1);

	/* return 1 if string is a integer */
	char str[20];
    sprintf(str, "%d", atoi(string));
    return(!strcmp(string, str));
}