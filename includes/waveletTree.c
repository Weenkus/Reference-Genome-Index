/*
 * waveletTree.c
 *
 *  Created on: Dec 15, 2014
 *      Author: kodly
 */
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <string.h>


// Mostly macros and def (not used in newver version)
#include "../includes/oldDef.h"

// Sadrzi sve strukture podataka koje koristimo i statice varijable (treba ih izbaciti)
#include "../includes/typedef.h"

// Sadrzi sve funkcije za stvaranje, pretrazivanje i dealokaciju stabla valica
#include "../includes/waveletTree.h"

// Includes all functions need for easy bit usage, such as printBitVector
#include "../includes/bitUtils.h"



// Bit mask
unsigned char maskaOld[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };


/* Wavelet tree */
void insert(node ** tree, int duljinaNiz, int duljinaAbecede, char *abeceda,
		char *niz) {

	//printf("%s   %d\n", niz, duljinaAbecede);

	int imaNula = 0;
	int i = 0;
	int j = 0;
	int znakPrveAbecede = 0;
	int brojac = 0;
	int brojacNula = 0;
	char pomocni[2];
	char *bitovi = (char *) malloc(duljinaNiz + 1);
	//char *i+1] = '\0'pointerNaNePointovano = bitovi;
	//char bitovi[duljinaniz+1];

	char *sljedeciPodniz1 = NULL, *sljedeciPodniz2 = NULL;

	char *prvaAbeceda = (char *) malloc(duljinaAbecede / 2);
	char *drugaAbeceda = (char *) malloc(duljinaAbecede - duljinaAbecede / 2);

	// Inicijaliziraj
	//memset(bitovi, '\0', sizeof(bitovi) +1);
	memset(prvaAbeceda, '\0', sizeof(char) * ((duljinaAbecede / 2) - 1));
	memset(drugaAbeceda, '\0',
			sizeof(char) * ((duljinaAbecede - duljinaAbecede / 2) - 1));

	/* Izlucivanje dvije abecede */
	// Napravi prvu abecedu
	for (i = 0; i < duljinaAbecede / 2; i++) {
		prvaAbeceda[i] = abeceda[i];
		brojac++;
	}

	// Napravi drugu abecedu
	for (i = 0; i < duljinaAbecede - duljinaAbecede / 2; i++) {
		drugaAbeceda[i] = abeceda[i + duljinaAbecede / 2];
	}

	/* Petlja za stvaranje podatka unutar cvora i znakova sljedeceg cvora s pripadnim abecedama */
	for (i = 0; i < duljinaNiz; i++) {
		for (j = 0; j < brojac; j++) {
			if (*(niz + i) == prvaAbeceda[j]) {
				imaNula++;
			}
		}
	}

	sljedeciPodniz1 = (char *) malloc(imaNula + 1);
	memset(sljedeciPodniz1, '\0', sizeof(char) * (imaNula));

	sljedeciPodniz2 = (char *) malloc(duljinaNiz - imaNula + 1);
	memset(sljedeciPodniz2, '\0', sizeof(char) * (duljinaNiz - imaNula));

	/* Petlja za stvaranje podatka unutar cvora i znakova sljedeceg cvora s pripadnim abecedama */
	for (i = 0; i < duljinaNiz; i++) {
		for (j = 0; j < brojac; j++) {
			if (*(niz + i) == prvaAbeceda[j]) {
				znakPrveAbecede = 1;
			}
		}
		pomocni[0] = *(niz + i);
		// Znak pripada prvoj abecedi
		if (znakPrveAbecede == 1) {
			//strncat(bitovi, "0", 1);
			bitovi[i] = '0';
			//bitovi[i+1] = '\0';

			strncat(sljedeciPodniz1, pomocni, 1);

			brojacNula++; /* brojac nula mi je duljina sljedeceg podniza */
			// Znak prpada drugoj abecedi
		} else {
			//strncat(bitovi, "1", 1);
			bitovi[i] = '1';
			//bitovi[i+1] = '\0';

			strncat(sljedeciPodniz2, pomocni, 1);
		}
		znakPrveAbecede = 0;
	}

	// Stvori novi cvor, pazi da to nije dijete
	if (*tree == NULL) {
		//puts(bitovi);
		*tree = NoviCvor(bitovi);
	}
	// Pozovi rekurziju
	if (duljinaAbecede > 2) {
		if (duljinaAbecede - duljinaAbecede / 2 > 1) {
			insert(&((*tree)->right), duljinaNiz - brojacNula,
					duljinaAbecede - duljinaAbecede / 2, drugaAbeceda,
					sljedeciPodniz2);
		}
		if (duljinaAbecede / 2 > 1) {
			insert(&((*tree)->left), brojacNula, duljinaAbecede / 2,
					prvaAbeceda, sljedeciPodniz1);
		}
	}

	// Oslobodi svu zauzetu memoriju
	free(prvaAbeceda);
	free(drugaAbeceda);
	free(sljedeciPodniz1);
	free(sljedeciPodniz2);
}

// Rekurzivno obidi stablo i obrisi ga
void deallocateTree(node ** tree) {

	// Ako dodes do kraja vrati se razinu gore
	if (*tree == NULL) {
		return;
	}
	// Stablo je binarno, tako ga i obidi
	deallocateTree(&((*tree)->left));
	deallocateTree(&((*tree)->right));
	// Izbrisi sve podatke
	free((*tree)->val);
	free(*tree);
}

// Stvori novi cvor
node *NoviCvor(char bitovi[]) {
	node *novi = (node *) malloc(sizeof(node));
	novi->val = bitovi;
	novi->left = NULL;
	novi->right = NULL;
	return novi;
}


// Pokusaj rekurzivnog ranka
int rank(node * cvor, char znak, int indeks, char abeceda[], int duljinaAbecede) {

	if( strlen(cvor->val) < indeks)
		return 0;

	int znakPripradaLijevoAbecedi = 0, znakPripadaDesnojAbecedi = 0;
	char *trenutnaAbeceda = (char *) malloc(duljinaAbecede + 1);
	char *sljedecaAbceda = (char *)malloc(duljinaAbecede + 1);
	int i = 0, noviIndeks = 0;
	// Prepisi abecedu
	for (i = 0; i < duljinaAbecede; i++) {
		trenutnaAbeceda[i] = abeceda[i];
		sljedecaAbceda[i] = abeceda[i];
	}
	trenutnaAbeceda[i] = '\0';
	// Saznaj o kojem dijelu abecede se radi (lijevom ili desnom)
	// Ovo vrijedi samo za korjen
	indeks = indeks + 1;
	do {
		//puts(trenutnaAbeceda);
		//printf("%d\n",indeks);
		znakPripradaLijevoAbecedi = 0;
		znakPripadaDesnojAbecedi = 0;
		// Provjeri za lijevu stranu
		for (i = 0; i < duljinaAbecede / 2; i++) {
			if (znak == trenutnaAbeceda[i]) {
				znakPripradaLijevoAbecedi = 1;
				break;
			}
		}
		// Provjeri za desnu stranu
		for (i = duljinaAbecede / 2; i < duljinaAbecede ; i++) {
			if (znak == trenutnaAbeceda[i]) {
				znakPripadaDesnojAbecedi = 1;
				break;
			}
		}
		// Prebroji 0 ili 1 ovisno o abecedi i znaku
		// Ocito je da se radi o nekoj nebulozi
		if (znakPripradaLijevoAbecedi == 0 && znakPripadaDesnojAbecedi == 0)
			return 0;
		// Radi se o znaku koji pripada lijevoj strani abecede
		else if (znakPripradaLijevoAbecedi == 1
				&& znakPripadaDesnojAbecedi == 0) {
			for (i = 0; i < indeks; i++) {
				// izracunaj novi indeks za sljedeci cvor
				if (cvor->val[i] == '0')
					noviIndeks++;
			}
			// Izracunaj novu lijevu abecedu za novi poziv funkcije
			trenutnaAbeceda = (char *)realloc(trenutnaAbeceda, duljinaAbecede/2 + 1);
			//memset(sljedecaAbceda, '\0', duljinaAbecede / 2 + 1);
			int duzinaNoveAbeede = 0;
			for (i = 0; i < duljinaAbecede / 2; i++) {
				trenutnaAbeceda[i] = sljedecaAbceda[i];
				duzinaNoveAbeede++;
			}
			trenutnaAbeceda[i] = '\0';
			duljinaAbecede = duzinaNoveAbeede;
			// Pozovi sljedeci rank, tj spusti se za jednu razinu u stablu
			cvor = cvor->left;
		}
		// Radi se o znaku koji pripada desnoj strani abecede
		else if (znakPripadaDesnojAbecedi == 1
				&& znakPripradaLijevoAbecedi == 0) {
			for (i = 0; i < indeks; i++) {
				// Izracunaj indeks za novi cvor
				if (cvor->val[i] == '1')
					noviIndeks++;
			}
			// Izracunaj novu lijevu abecedu za novi poziv funkcijebr
			trenutnaAbeceda = (char *)realloc(trenutnaAbeceda, duljinaAbecede - duljinaAbecede/2 + 1);
			//memset(sljedecaAbceda, '\0', duljinaAbecede - duljinaAbecede/2 + 1);
			int duzinaNoveAbeede = 0;
			for (i = 0; i < duljinaAbecede - (duljinaAbecede / 2); i++) {
				trenutnaAbeceda[i] = sljedecaAbceda[i + (duljinaAbecede / 2)];
				duzinaNoveAbeede++;
			}
			trenutnaAbeceda[i] = '\0';
			duljinaAbecede = duzinaNoveAbeede;
			// Pozovi sljedeci rank, tj spusti se za jednu razinu u stablu
			cvor = cvor->right;
		}
		// Namjesti da u sljedecm cvor ide se do onog znaka do kojeg se upravo doslo
		indeks = noviIndeks;
		noviIndeks = 0;
		// Namjesti abecede za sljedece izvodenje
		for( i = 0; i < duljinaAbecede; i++) {
			sljedecaAbceda[i] = trenutnaAbeceda[i];
		}
		sljedecaAbceda[i] = '\0';
	} while (cvor != NULL || duljinaAbecede > 2);
	free(sljedecaAbceda);
	free(trenutnaAbeceda);
	return indeks;
}

// Stvara stablo valica s bitovima
void insertBit(node ** tree, int duljinaNiz, int duljinaAbecede, char *abeceda,
		char *niz) {

	int imaNula = 0;
	int i = 0;
	int j = 0;
	int znakPrveAbecede = 0;
	int brojac = 0;
	int brojacNula = 0;
	char pomocni[2];
	char *bitovi =
			duljinaNiz % 8 ?
					(char *) malloc(duljinaNiz / 8 + 1) :
					(char *) malloc(duljinaNiz / 8);
	char *sljedeciPodniz1 = NULL, *sljedeciPodniz2 = NULL;
	char *prvaAbeceda = (char *) malloc(duljinaAbecede / 2);
	char *drugaAbeceda = (char *) malloc(duljinaAbecede - duljinaAbecede / 2);

	// Inicijaliziraj
	memset(prvaAbeceda, '\0', sizeof(char) * ((duljinaAbecede / 2) - 1));
	memset(drugaAbeceda, '\0',
			sizeof(char) * ((duljinaAbecede - duljinaAbecede / 2) - 1));

	// Napravi prvu abecedu
	for (i = 0; i < duljinaAbecede / 2; i++) {
		prvaAbeceda[i] = abeceda[i];
		brojac++;
	}

	// Napravi drugu abecedu
	for (i = 0; i < duljinaAbecede - duljinaAbecede / 2; i++) {
		drugaAbeceda[i] = abeceda[i + duljinaAbecede / 2];
	}

	for (i = 0; i < duljinaNiz; i++) {
		for (j = 0; j < brojac; j++) {
			if (*(niz + i) == prvaAbeceda[j]) {
				imaNula++;
			}
		}
	}

	// Napravi dva podniza i oba inicijaliziraj
	sljedeciPodniz1 = (char *) malloc(imaNula + 1);
	memset(sljedeciPodniz1, '\0', sizeof(char) * (imaNula));

	sljedeciPodniz2 = (char *) malloc(duljinaNiz - imaNula + 1);
	memset(sljedeciPodniz2, '\0', sizeof(char) * (duljinaNiz - imaNula));

	// Napravi bitove za cvor
	for (i = 0; i < duljinaNiz; i++) {
		for (j = 0; j < brojac; j++) {
			if (*(niz + i) == prvaAbeceda[j]) {
				znakPrveAbecede = 1;
			}
		}
		pomocni[0] = *(niz + i);
		// Znak pripada prvoj abecedi
		if (znakPrveAbecede == 1) {
			*(bitovi + i / 8) = setBitOld(i, 0);
			strncat(sljedeciPodniz1, pomocni, 1);
			brojacNula++;
		}
		// Znak pripada drugoj abecedi
		else {
			*(bitovi + i / 8) = setBitOld(i, 1);
			strncat(sljedeciPodniz2, pomocni, 1);
		}
		znakPrveAbecede = 0;
	}
	// Napravi novi cvor
	if (*tree == NULL) {
		*tree = NoviCvor(bitovi);
	}
	// Pozovi rekurziju
	if (duljinaAbecede > 2) {
		if (duljinaAbecede - duljinaAbecede / 2 > 1) {
			insertBit(&((*tree)->right), duljinaNiz - brojacNula,
					duljinaAbecede - duljinaAbecede / 2, drugaAbeceda,
					sljedeciPodniz2);
		}
		if (duljinaAbecede / 2 > 1) {
			insertBit(&((*tree)->left), brojacNula, duljinaAbecede / 2,
					prvaAbeceda, sljedeciPodniz1);
		}
	}
	// Oslobodi memoriju
	free(prvaAbeceda);
	free(drugaAbeceda);
	free(sljedeciPodniz1);
	free(sljedeciPodniz2);
}

// Pokusaj rekurzivnog ranka
int rankBit(node * cvor, char znak, int indeks, char abeceda[], int duljinaAbecede, int duljinaNiz) {

 	//Pazi da indeks ne preleti niz
	if (duljinaNiz < indeks || duljinaNiz == 0)
		return 0;

	int znakPripradaLijevoAbecedi = 0, znakPripadaDesnojAbecedi = 0;
	char *trenutnaAbeceda = (char *) malloc(duljinaAbecede + 1);
	char *sljedecaAbceda = (char *)malloc(duljinaAbecede + 1);
	char bitovi[1];
	int i = 0, noviIndeks = 0;
	// Prepisi abecedu
	for (i = 0; i < duljinaAbecede; i++) {
		trenutnaAbeceda[i] = abeceda[i];
		sljedecaAbceda[i] = abeceda[i];
	}
	trenutnaAbeceda[i] = '\0';
	// Saznaj o kojem dijelu abecede se radi (lijevom ili desnom)
	// Ovo vrijedi samo za korjen
	indeks = indeks + 1;
	do {
		//puts(trenutnaAbeceda);
		//printf("%d\n",indeks);
		znakPripradaLijevoAbecedi = 0;
		znakPripadaDesnojAbecedi = 0;
		// Provjeri za lijevu stranu
		for (i = 0; i < duljinaAbecede / 2; i++) {
			if (znak == trenutnaAbeceda[i]) {
				znakPripradaLijevoAbecedi = 1;
				break;
			}
		}
		// Provjeri za desnu stranu
		for (i = duljinaAbecede / 2; i < duljinaAbecede ; i++) {
			if (znak == trenutnaAbeceda[i]) {
				znakPripadaDesnojAbecedi = 1;
				break;
			}
		}
		// Prebroji 0 ili 1 ovisno o abecedi i znaku
		// Ocito je da se radi o nekoj nebulozi
		if (znakPripradaLijevoAbecedi == 0 && znakPripadaDesnojAbecedi == 0)
			return 0;
		// Radi se o znaku koji pripada lijevoj strani abecede
		else if (znakPripradaLijevoAbecedi == 1
				&& znakPripadaDesnojAbecedi == 0) {
			for (i = 0; i < indeks; i++) {
				bitovi[0] = (cvor)->val[i / 8];
				// izracunaj novi indeks za sljedeci cvor
				if (getRankBitOld(i) == 0)
					noviIndeks++;
			}
			// Izracunaj novu lijevu abecedu za novi poziv funkcije
			trenutnaAbeceda = (char *)realloc(trenutnaAbeceda, duljinaAbecede/2 + 1);
			//memset(sljedecaAbceda, '\0', duljinaAbecede / 2 + 1);
			int duzinaNoveAbeede = 0;
			for (i = 0; i < duljinaAbecede / 2; i++) {
				trenutnaAbeceda[i] = sljedecaAbceda[i];
				duzinaNoveAbeede++;
			}
			trenutnaAbeceda[i] = '\0';
			duljinaAbecede = duzinaNoveAbeede;
			// Pozovi sljedeci rank, tj spusti se za jednu razinu u stablu
			cvor = cvor->left;
		}
		// Radi se o znaku koji pripada desnoj strani abecede
		else if (znakPripadaDesnojAbecedi == 1
				&& znakPripradaLijevoAbecedi == 0) {
			for (i = 0; i < indeks; i++) {
				bitovi[0] = (cvor)->val[i / 8];
				// Izracunaj indeks za novi cvor
				if (getRankBitOld(i) == 1)
					noviIndeks++;
			}
			// Izracunaj novu lijevu abecedu za novi poziv funkcijebr
			trenutnaAbeceda = (char *)realloc(trenutnaAbeceda, duljinaAbecede - duljinaAbecede/2 + 1);
			//memset(sljedecaAbceda, '\0', duljinaAbecede - duljinaAbecede/2 + 1);
			int duzinaNoveAbeede = 0;
			for (i = 0; i < duljinaAbecede - (duljinaAbecede / 2); i++) {
				trenutnaAbeceda[i] = sljedecaAbceda[i + (duljinaAbecede / 2)];
				duzinaNoveAbeede++;
			}
			trenutnaAbeceda[i] = '\0';
			duljinaAbecede = duzinaNoveAbeede;
			// Pozovi sljedeci rank, tj spusti se za jednu razinu u stablu
			cvor = cvor->right;
		}
		// Namjesti da u sljedecm cvor ide se do onog znaka do kojeg se upravo doslo
		indeks = noviIndeks;
		noviIndeks = 0;
		// Namjesti abecede za sljedece izvodenje
		for( i = 0; i < duljinaAbecede; i++) {
			sljedecaAbceda[i] = trenutnaAbeceda[i];
		}
		sljedecaAbceda[i] = '\0';
	} while (cvor != NULL || duljinaAbecede > 2);
	free(sljedecaAbceda);
	free(trenutnaAbeceda);
	return indeks;
}
