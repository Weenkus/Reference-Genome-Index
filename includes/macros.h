/*
 * macros.h
 *
 *  Created on: Apr 7, 2015
 *      Author: weenkus
 */

#ifndef MACROS_H_
#define MACROS_H_


// Set a bit from a char
#define setBit(position, bit) ((bit)== 1 ? bitVector|maska[position] : bitVector&(~(maska[position])))

// Set a bit to one from a char
#define setBitOne(position)  (bitVector|maska[position])

// Get bit from a char 
#define getBit(position) (bitVector&maska[position] ? 1 : 0)

// Get bit from a char array
#define getBitArray(position) (bitVector[position/8]&maska[position%8] ? 1 : 0)

// Get the rank of a certain bit
#define getRankBit(position) (bitVector[0]&maska[(position)%8] ? 1 : 0)



#endif /* MACROS_H_ */
