/*
 * adder.cpp
 *
 *  Created on: Apr 9, 2015
 *      Author: weenkus
 */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <unordered_set>
#include <map>
#include <cassert>

// Include all the constants
#include "../includes/def.h"

// Include all macros
#include "../includes/macros.h"

// Includes all functions need for easy bit usage, such as printBitVector
#include "../includes/bitUtils.h"

int main(int argc, char **argv) {
	DIR *addPool;
	FILE *referenceGenome;

	// Creating a file to write the encoded multi-genome
	referenceGenome = fopen("../output/referenceGenome", "r+");
	if (referenceGenome == NULL) {
		printf("\nThe OS didn't manage to create/open the referenceGenome!");
		return 1;
	}

	// Create a folder stream for all the genomes fastq files
	if (NULL == (addPool = opendir("../addPool"))) {
		printf("\nThe directory path from pointer folderStream is wrong!");
		return 1;
	}

	// Code table, since it is encoded at the comment line in the ref genome we will have to get it
	std::map<char, int> hashCodeTable;

	// Loop through all the files in the directory and encode them
	struct dirent* fileList;
	int numberOfFilesAdded { 0 };
	while ((fileList = readdir(addPool))) {
		// Skip the current and top folder
		if (!strcmp(fileList->d_name, "."))
			continue;
		if (!strcmp(fileList->d_name, ".."))
			continue;

		// Open the files for reading
		char fileName[MAX_FILENAME_LENGTH + 1] = "../addPool/";
		strcat(fileName, fileList->d_name);

		FILE *currentFile = fopen(fileName, "r");
		if (currentFile == NULL) {
			printf("\nError : Failed to open entry file!\n");
			return 1;
		}

		// Start coding the read to the genome
		char encChar, curChar1, curChar2;

		// Write from the begining of the enconded file every time
		fseek(referenceGenome, 0, SEEK_SET);

		// Since the first line in the multi-genome has code table info, skip it!
		int index { 0 };
		do {
			fscanf(referenceGenome, "%c", &encChar);
			// Get the code table at the comment line
			if (numberOfFilesAdded == 0) {
				if (index > 0 && index < NUCLEOTIDE_NUMBER + 1) { // We add +1 to avoid "#" - comment line char
					hashCodeTable[encChar] = index - 1;
				}
				++index;
			}
		} while (encChar != '\n');

		printf("\nFile: %s\n", fileList->d_name);
		// Encode the current read with the reference genome
		bool secondCharIsEOF = false;

		while ((fscanf(currentFile, "%c", &curChar1)) != EOF) {
			if (fscanf(currentFile, "%c", &curChar2) == EOF)
				secondCharIsEOF = true;
			char bitVector = 0x00;

			// Multi-genome has text we need to combine it with the reads and recode it
			fscanf(referenceGenome, "%c", &encChar);
			fseek(referenceGenome, -1 * sizeof(char), SEEK_CUR); // Ughh xD

			bitVector = setBit(hashCodeTable[curChar1], 1);
			printf("%c ", curChar1);
			if (secondCharIsEOF == false) {
				bitVector = setBit(hashCodeTable[curChar2] + NUCLEOTIDE_NUMBER,
						1);
				printf("%c ", curChar2);
			}
			printBitVector(bitVector);

			bitVector |= encChar;
			fwrite(&bitVector, 1, sizeof(bitVector), referenceGenome);

		}
		// Close the current file/read
		fclose(currentFile);
		++numberOfFilesAdded;
	}
	closedir(addPool);
	fclose(referenceGenome);
	return 0;
}

