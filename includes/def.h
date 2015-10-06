/*
 * def.h
 *
 *  Created on: Apr 7, 2015
 *      Author: weenkus
 */

#ifndef DEF_H_
#define DEF_H_

// Upper bound the file name size
const int MAX_FILENAME_LENGTH { 256 };

// Generic buffer
const int BUFFER { 1024 };

// 16 combinations to cover all the possible permutations within the reference genome
const int  IUPAC_SIZE { 16 };

// A permutation of the nucleotide number ACGT
const int  IUPAC_MAX_CODE_WORD_LENGTH { 4 };

// Since we are talking about genomes we will use A, C, T and G
const int NUCLEOTIDE_NUMBER { 4 };

// Number for bits in a byte
const int NUMBER_OF_BITS { 8 };

// Artifical alphabet used to create prefix sum table, suffix array and more
static const char artificialAlphabet[] = "$ABCDEFGHIJKLMNOP";

// IUPAC ALPHABET
static const char iupacAlphabet[] = "#TKGSBYCMHNVRDWA";

// IUPAC ALPHABET with $
static const char iupacAlphabetExtended[] = "$#TKGSBYCMHNVRDWA";

// Bit mask
unsigned char maska[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };


// Subset of IUPAC characters that match A
static const char subSetA[] = "MHNVRDWA";			// Every subset has one more letter in the end since it is needed for FMindex
													// this way, we can get that letter withouth any extra function
// Subset of IUPAC characters that match C			// subSetA doesn't have the extra letter since A is the last one
static const char subSetC[] = "SBYCMHNV";		
	
// Subset of IUPAC characters that match G			// This has been removed (the top comment) because of a different impelmentation
static const char subSetG[] = "KGSBNVRD";			// in the newer version

// Subset of IUPAC characters that match T
static const char subSetT[] = "TKBYHNDW";

// The length of the subSets
const int SUBSET_LENGTH { 8 };

#endif /* DEF_H_ */
