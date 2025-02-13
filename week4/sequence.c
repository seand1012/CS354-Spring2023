///////////////////////////////////////////////////////////////////////////////
   // Main File:        sequence
   // This File:        sequence
   // Other Files:      none
   // Semester:         CS 354 Lecture 02 Spring 2023
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
   // Persons:          None
   // Online sources:   None
   //////////////////////////// 80 columns wide ///////////////////////////////
   ////////////////////////////////////////////////////////////////////////////////
   // Copyright 2019 Jim Skrentny
   // Posting or sharing this file is prohibited, including any changes/additions.
   // Used by permission, Spring 2023, Deb Deppeler
////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
	// Check CLAs
	if(argc	!= 5){
		printf("Usage: ./sequence n x0 m b\n");
		exit(1);
	}
	
	// declare local variables
	int n = atoi(*(argv + 1)); // Number of times sequence is run
	int x0 = atoi(*(argv + 2));
	int m = atoi(*(argv + 3));
	int b = atoi(*(argv + 4));
	int total = 0;

	if(n < 1){
		printf("n must be a positive non-zero number");
		exit(1);
	}	

	for(int i = 0; i < n; i++){
		if(i == 0){
			printf("%i", x0);
		}
		else{
			total = ((m * x0) + b);
			x0 = total;
			total = 0;
			printf(",%i", x0);
		}
	}
	printf("\n");
	return 0;
}
