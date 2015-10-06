#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unordered_set>
#include <map>
#include <malloc.h>
#include <vector>
#include <list>
#include <iostream>
#include <string.h>

// Includes all functions for data structure serialization
#include "../includes/serialization.h"

/**
 * Write the suffix array into a file.
 *
 * The first row is the header marked with the # and after it comes a number.
 * The number tells how many elements are there in the suffix array.
 *
 * The row under has all the suffix array values, each separated with a space.
 */
int serializeSA(int SA[], int decompressedTextLength) {
	FILE *serializeSA;

	// Creating a file to write the suffix array
	serializeSA = fopen("../savedStructures/serializeSA", "w");
	if (serializeSA == NULL) {
		printf("\nThe OS didn't manage to create serializeSA file!");
		return 0;
	}

	// Save the length as a header file
	fprintf(serializeSA, "#%d\n", decompressedTextLength);

	// Save the stracture
	for (int i = 0; i < decompressedTextLength; i++) {
		fprintf(serializeSA, "%d ", SA[i]);
	}

	fclose(serializeSA);
	return 1;
}

/**
 * Write the Burrows-Wheeler transform into a file.
 *
 * The first row is the header marked with the # and after it comes a number.
 * The number tells how many elements are there in the BWT.
 *
 * The row under has all the BWT values.
 */
int serializeAlphabet(char alphabet[], int alphabetLength) {
	FILE *serializeAlphabet;

	// Creating a file to write the suffix array
	serializeAlphabet = fopen("../savedStructures/serializeAlphabet", "w");
	if (serializeAlphabet == NULL) {
		printf("\nThe OS didn't manage to create serializeAlphabet file!");
		return 0;
	}

	// Save the length as a header file
	fprintf(serializeAlphabet, "#%d\n", alphabetLength);

	// Save the structure
	for (int i = 0; i < alphabetLength; i++) {
		fprintf(serializeAlphabet, "%c", alphabet[i]);
	}

	fclose(serializeAlphabet);
	return 1;
}

/**
 * Write the Alphabet into a file.
 * The alphabet is SORTED.
 *
 * The first row is the header marked with the # and after it comes a number.
 * The number tells how many elements are there in the Alphabet.
 *
 * The row under has all the alphabet values.
 */
int serializeBWT(char BWT[], int decompressedTextLength) {
	FILE *serializeBWT;

	// Creating a file to write the suffix array
	serializeBWT = fopen("../savedStructures/serializeBWT", "w");
	if (serializeSA == NULL) {
		printf("\nThe OS didn't manage to create serializeBWT file!");
		return 0;
	}

	// Save the length as a header file
	fprintf(serializeBWT, "#%d\n", decompressedTextLength);

	// Save the structure
	for (int i = 0; i < decompressedTextLength; i++) {
		fprintf(serializeBWT, "%c", BWT[i]);
	}

	fclose(serializeBWT);
	return 1;
}

/**
 * Write the prefix sum table - C into a file.
 *
 * The first row is the header marked with the # and after it comes a number.
 * The number tells how many elements are there in the prefix sum table.
 *
 * The row under has all the prefix sum table values, each separated with a space.
 */
int serializeC(int C[], int alphabetLength) {
	FILE *serializeC;

	// Creating a file to write the suffix array
	serializeC = fopen("../savedStructures/serializeC", "w");
	if (serializeSA == NULL) {
		printf("\nThe OS didn't manage to create serializeC file!");
		return 0;
	}

	// Save the length as a header file
	fprintf(serializeC, "#%d\n", alphabetLength);

	// Save the structure
	for (int i = 0; i < alphabetLength; i++) {
		fprintf(serializeC, "%d ", C[i]);
	}

	fclose(serializeC);
	return 1;
}

/**
 * Recursively (preorder traverals) write the tree to a file VALUE.
 */
void writeWaveletToFile(node *root, FILE *fileToWrite) {
	if (!root) {
		fprintf(fileToWrite, "0\n");
	} else {
		fprintf(fileToWrite, "%d %s\n",strlen(root->val),root->val);
		writeWaveletToFile(root->left, fileToWrite);
		writeWaveletToFile(root->right, fileToWrite);
	}
}

/**
 * Recursively (preorder traverals) write the tree to a file SIZE.
 */
void writeWaveletToFileSize(node *root, FILE *fileToWrite) {
	if (!root) {
		fprintf(fileToWrite, "0 ");
	} else {
		fprintf(fileToWrite, "%d ",strlen(root->val));
		writeWaveletToFileSize(root->left, fileToWrite);
		writeWaveletToFileSize(root->right, fileToWrite);
	}
}

/**
 * Write the wavelet tree into a file.
 *
 * We use the preorder traversal to write the tree to a file.
 *
 * In every row we write the length of the string as well as the string itself.
 * If a node is null we simply write 0.
 * Keep in mind that our tree is actually bit valued (the ilusttration below isn't).
 *
 * So the file could look something like this:
 * It looks something like this:
 *
 * 1 A
 * 4 ACGT
 * 0
 * 0
 * 1 A
 * 0
 * 1 C
 * 0
 * 0
 */
int serializeWavletTree(node *root) {
	FILE *serializeWT;

	// Creating a file to write the waveletTree values
	serializeWT = fopen("../savedStructures/serializeWaveletTree", "w");
	if (serializeWT == NULL) {
		printf("\nThe OS didn't manage to create serializeWaveletTree file!");
		return 0;
	}

	// Write the tree to a file
	writeWaveletToFile(root, serializeWT);

	fclose(serializeWT);
	return 1;
}


//************************************ DESERIALIZATION *********************************************
/**
 * Load the suffix array from a file.
 *
 * The first row is the header marked with the # and after it comes a number.
 * The number tells how many elements are there in the suffix array.
 *
 * The row under has all the suffix array values, each separated with a space.
 */
int deserializeSA(int **SA, int *length) {

	FILE *deserializeSA;

	// Read the SA
	deserializeSA = fopen("../savedStructures/serializeSA", "r");
	if (deserializeSA == NULL) {
		printf("\nThe OS didn't manage to open serializeSA file!");
		return 0;
	}

	// Get the length of the SA
	int suffixArrayLength = 0;
	fscanf(deserializeSA, "#%d\n", &suffixArrayLength);
	*length = suffixArrayLength;

	// Initialise and fill the structure
	*SA = (int *) malloc(sizeof(int) * suffixArrayLength);
	for (int i = 0; i < suffixArrayLength; i++) {
		fscanf(deserializeSA, "%d ", &(*SA)[i]);
	}

	fclose(deserializeSA);
	return 1;
}

/**
 * Load the Burrows-Wheeler transform (BWT) from a file.
 *
 * The first row is the header marked with the # and after it comes a number.
 * The number tells how many elements are there in the BWT.
 *
 * The row under has all the BWT values.
 */
int deserializeBWT(char **BWT) {

	FILE *deserializeBWT;

	// Read the BWT
	deserializeBWT = fopen("../savedStructures/serializeBWT", "r");
	if (deserializeBWT == NULL) {
		printf("\nThe OS didn't manage to open serializeBWT file!");
		return 0;
	}

	// Get the length of the SA
	int BWTlength = 0;
	fscanf(deserializeBWT, "#%d\n", &BWTlength);

	// Initialise and fill the structure
	*BWT = (char *) malloc(sizeof(char) * (BWTlength + 1));
	for (int i = 0; i < BWTlength; i++) {
		fscanf(deserializeBWT, "%c", &(*BWT)[i]);
	}

	fclose(deserializeBWT);
	return 1;
}

/**
 * Load the alphabet from a file.
 *
 * The first row is the header marked with the # and after it comes a number.
 * The number tells how many elements are there in the alphabet.
 *
 * The alphabet is SORTED!
 *
 * The row under has all the alphabet values.
 */
int deserializeAlphabet(char **alphabet, int *length) {

	FILE *deserializeAlphabet;

	// Open a file to read the alphabet
	deserializeAlphabet = fopen("../savedStructures/serializeAlphabet", "r");
	if (deserializeAlphabet == NULL) {
		printf("\nThe OS didn't manage to open serializeAlphabet file!");
		return 0;
	}

	// Get the length of the SA
	int alphabetLength = 0;
	fscanf(deserializeAlphabet, "#%d\n", &alphabetLength);
	*length = alphabetLength;

	// Initialise and fill the structure
	*alphabet = (char *) malloc(sizeof(char) * (alphabetLength + 1));
	for (int i = 0; i < alphabetLength; i++) {
		fscanf(deserializeAlphabet, "%c", &(*alphabet)[i]);
	}

	fclose(deserializeAlphabet);
	return 1;
}

/**
 * Load the prefix sum table from a file.
 *
 * The first row is the header marked with the # and after it comes a number.
 * The number tells how many elements are there in the prefix sum table - C.
 *
 * The row under has all the prefix sum values, separated by a space char.
 */
int deserializeC(int **C) {

	FILE *deserializeC;

	// Open a file for prefix sum table
	deserializeC = fopen("../savedStructures/serializeC", "r");
	if (deserializeC == NULL) {
		printf("\nThe OS didn't manage to open serializeC file!");
		return 0;
	}

	// Get the length of the prefix sum table
	int prefixSumTableLength = 0;
	fscanf(deserializeC, "#%d\n", &prefixSumTableLength);

	// Initialise and fill the structure
	*C = (int *) malloc(sizeof(int) * prefixSumTableLength);
	for (int i = 0; i < prefixSumTableLength; i++) {
		fscanf(deserializeC, "%d ", &(*C)[i]);
	}

	fclose(deserializeC);
	return 1;
}

/**
 * Build a tree and fill it from a file.
 */
void buildTree(node **root, FILE *readFile) {
	int input;
	if (fscanf(readFile, "%d", &input) == EOF)
		return;

	node *newNode = (node *) malloc(sizeof(node));
	if (input != 0) {
		char *value = (char*) malloc(sizeof(char) * (input + 1));
		fscanf(readFile, " %s\n", value);
		newNode->val = value;
	} else {
		newNode->val = NULL;
		fscanf(readFile, "\n");
		return;
	}
	newNode->left = NULL;
	newNode->right = NULL;
	*root = newNode;

	buildTree(&(*root)->left, readFile);
	buildTree(&(*root)->right, readFile);
}

void printTree(node *root) {
	if (root != NULL) {
		puts(root->val);
		printTree(root->left);
		printTree(root->right);
	}
}

/**
 * Load the wavelet tree  - OCC from a file.
 *
 * The tree is saved in a preorder traversal.
 * In a row we save the length of the string and its value,
 * if a node is null we write 0.
 *
 * Keep in mind that our tree is actually bit valued (the ilusttration below isn't)
 *
 * It looks something like this:
 *
 * 1 A
 * 4 ACGT
 * 0
 * 0
 * 1 A
 * 0
 * 1 C
 * 0
 * 0
 *
 */
int deserializeWaveletTree(node **root) {

	FILE *deserializeOcc;

	// Open a file for the wavelet tree size construction
	deserializeOcc = fopen("../savedStructures/serializeWaveletTree", "r");
	if (deserializeOcc == NULL) {
		printf("\nThe OS didn't manage to open serializeWaveletTreeSize file!");
		return 0;
	}

	// Build a tree without values
	buildTree(root, deserializeOcc);

	return 1;
}



