/*
 * typedef.h
 *
 *  Created on: Apr 13, 2015
 *      Author: weenkus
 */

#ifndef TYPEDEF_H_
#define TYPEDEF_H_

typedef char char8;

typedef unsigned char uchar8;

// Tree node
struct element {
	char *val;
	struct element *right, *left;
};

// Tree node renamed for easier usage
typedef struct element node;

// Multi index struct for Rp and Lp
struct touple {
	int Lp;
	int Rp;
};

typedef struct touple suffixInterval;




#endif /* TYPEDEF_H_*/
