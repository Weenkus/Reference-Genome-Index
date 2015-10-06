/*
 * serialization.h
 *
 *  Created on: 19 5 2015
 *      Author: weenkus
 */

#ifndef SERIALIZATION_H_
#define SERIALIZATION_H_

/**
 * Write the suffix array into a file.
 *
 * The first row is the header marked with the # and after it comes a number.
 * The number tells how many elements are there in the suffix array.
 *
 * The row under has all the suffix array values, each separated with a space.
 */
int serializeSA(int SA[], int decompressedTextLength);

/**
 * Write the Burrows-Wheeler transform into a file.
 *
 * The first row is the header marked with the # and after it comes a number.
 * The number tells how many elements are there in the BWT.
 *
 * The row under has all the BWT values.
 */
int serializeAlphabet(char alphabet[], int alphabetLength);

/**
 * Write the Alphabet into a file.
 * The alphabet is SORTED.
 *
 * The first row is the header marked with the # and after it comes a number.
 * The number tells how many elements are there in the Alphabet.
 *
 * The row under has all the alphabet values.
 */
int serializeBWT(char BWT[], int decompressedTextLength);

/**
 * Write the prefix sum table - C into a file.
 *
 * The first row is the header marked with the # and after it comes a number.
 * The number tells how many elements are there in the prefix sum table.
 *
 * The row under has all the prefix sum table values, each separated with a space.
 */
int serializeC(int C[], int alphabetLength);

/**
 * Write the wavelet tree into a file.
 *
 * We use the preorder traversal, writing a special sentinel on every null node.
 * The special sentinel is #.
 *
 * For more information check:
 * http://articles.leetcode.com/2010/09/serializationdeserialization-of-binary.html
 */
int serializeWavletTree(node *root);

/**
 * Load the suffix array from a file.
 *
 * The first row is the header marked with the # and after it comes a number.
 * The number tells how many elements are there in the suffix array.
 *
 * The row under has all the suffix array values, each separated with a space.
 */
int deserializeSA(int **SA, int *length);

/**
 * Load the Burrows-Wheeler transform (BWT) from a file.
 *
 * The first row is the header marked with the # and after it comes a number.
 * The number tells how many elements are there in the BWT.
 *
 * The row under has all the BWT values.
 */
int deserializeBWT(char **BWT);

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
int deserializeAlphabet(char **alphabet, int *length);

/**
 * Load the prefix sum table from a file.
 *
 * The first row is the header marked with the # and after it comes a number.
 * The number tells how many elements are there in the prefix sum table - C.
 *
 * The row under has all the prefix sum values, separated by a space char.
 */
int deserializeC(int **C);

/**
 * Print a tree in a preorder traversal.
 */
void printTree(node *root);

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
int deserializeWaveletTree(node **root);

#endif /* SERIALIZATION_H_*/
