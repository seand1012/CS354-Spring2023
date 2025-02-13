///////////////////////////////////////////////////////////////////////////////
// Copyright 2020 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission, CS 354 Spring 2022, Deb Deppeler
////////////////////////////////////////////////////////////////////////////////
   
// add your own File Header information here (as provided in p2A or Commenting guide
////////////////////////////////////////////////////////////////////////////////
// Main File:        myMagicSquare.c
// This File:        myMagicSquare.c
// Other Files:      (name of all other files if any)
// Semester:         CS 354 Lecture 002 Spring 2023
// Instructor:       deppeler
// 
// Author:           Sean DeGrazia
// Email:            sdegrazia@wisc.edu
// CS Login:         degrazia
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of 
//                   of any information you find.
//////////////////////////// 80 columns wide ///////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure that represents a magic square
typedef struct {
    int size;           // dimension of the square
    int **magic_square; // pointer to heap allocated magic square
} MagicSquare;

/* TODO:
 * Prompts the user for the magic square's size, reads it,
 * checks if it's an odd number >= 3 (if not display the required
 * error message and exit), and returns the valid number.
 */
int getSize() {
	int s = 0;
	printf("Enter magic square's size (odd integer >=3)\n");
	scanf("%d", &s);
	printf("%d\n", s);
	if(s < 3){
		printf("Magic square size must be >= 3.\n");
		exit(1);
	}

	if(s % 2 != 1){
		printf("Magic square size must be odd.\n");
		exit(1);
	}

		
    return s;   
} 
   
/* TODO:
 * Makes a magic square of size n using the 
 * Siamese magic square algorithm or alternate from assignment 
 * or another valid alorithm that produces a magic square.
 *
 * n is the number of rows and columns
 *
 * returns a pointer to the completed MagicSquare struct.
 */
MagicSquare *generateMagicSquare(int n) {
	MagicSquare* ms = malloc(sizeof(MagicSquare));

	ms->size = n;
	
	// malloc the first dimension of the array
	ms->magic_square = malloc(sizeof(int) * n);
	
	// test if malloc worked properly
	if(ms->magic_square == NULL){
		printf("invalid\n");
		exit(1);
	}
	
	// calloc second dimension of the array
	// use calloc so each value starts at 0
	for(int i = 0; i < n; i++){
		*(ms->magic_square+i) = calloc(n, sizeof(int));
		
		// test if calloc worked properly
		if(*(ms->magic_square+i) == NULL){
			printf("invalid\n");
			exit(1);
		}
	}
	
	// uses alternate algorithm from assignment
	// i == row, j == column
	int curRow = n / 2; // get middle row
	int curCol = n - 1; // get last column
	
	// loop through to add each number to each spot
	for(int i = 1; i <= n*n; i++){
		if(*(*(ms->magic_square + curRow) + curCol) == 0){
			*(*(ms->magic_square + curRow) + curCol) = i;
		}
		// if there's a collision, go back to original spot
		// then go left one more column
		else{
			curRow = curRow - 1;
			// if collision occurs in row 0
			// wrap around to last row
			if(curRow < 0){
				curRow = n - 1;
			}
			curCol = curCol - 2;
			// if collision occurs in column 1 or 2
			// wrap around
			// technically shouldn't happen but might be necessary
			if(curCol < 0){
				curCol = n + curCol;
			}
			*(*(ms->magic_square + curRow) + curCol) = i;
		}
		// increment curRow
		// check if hit the edge of a row
		// if so, wrap around to the beginning		
		if((curRow = curRow + 1) == n){
			curRow = 0;
		}
		// increment curCol
		// check if hit the edge of a col
		// if so, wrap around to the beginning
		if((curCol = curCol + 1) == n){
			curCol = 0;
		}
	}	
    return ms;    
} 

/* TODO:  
 * Opens a new file (or overwrites the existing file)
 * and writes the square in the specified format.
 *
 * magic_square the magic square to write to a file
 * filename the name of the output file
 */
void fileOutputMagicSquare(MagicSquare *magic_square, char *filename) {
	FILE *fp = fopen(filename, "w");
	if(fp == NULL){
		printf("Can't open file for writing.\n");
		exit(1);
	}
	
	//print out size of magic square	
	fprintf(fp, "%d\n", magic_square -> size);
	for(int i = 0; i < (magic_square -> size); i++){
		for(int j = 0; j < (magic_square -> size); j++){
			// print the value in the first column
			if(j == 0)	
				fprintf(fp, "%d", *(*(magic_square->magic_square+i)+j));
			// print the values following the first column
			else
				fprintf(fp, ",%d", *(*(magic_square->magic_square+i)+j));
		}
		// print a new line for each completed row of the magic square
		fprintf(fp, "\n");
	}

	if(fclose(fp) != 0){
		printf("Can't close the file.\n");
		exit(1);
	}
}


/* TODO:
 * Generates a magic square of the user specified size and
 * outputs the square to the output filename.
 * 
 * Must have one CLA for the program and one more for the output file
 */
int main(int argc, char **argv) {
    // TODO: Check input arguments to get output filename
	if(argc != 2){
		printf("Usage: ./myMagicSquare <output_filename>\n");
		exit(1);
	}
    // TODO: Get magic square's size from user
	int s = getSize();
    // TODO: Generate the magic square by correctly interpreting 
    //       the algorithm(s) in the write-up or by writing or your own.  
    //       You must confirm that your program produces a 
    //       Magic Sqare as described in the linked Wikipedia page.
	MagicSquare* completeMagicSquare = generateMagicSquare(s);
    // TODO: Output the magic square
	fileOutputMagicSquare(completeMagicSquare, *(argv + 1));
    return 0;
} 


