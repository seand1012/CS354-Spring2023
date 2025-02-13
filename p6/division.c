///////////////////////////////////////////////////////////////////////////////
   // Main File:        division.c
   // This File:        division.c
   // Other Files:      sendsig.c, mySigHandler.c
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

// Include statements
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

// Global var
int count = 0;

/*
 * Handles FPE exception
 * Prints error message and the # of valid calculations
 * then exits program
 */
void fpeHandler(){
	
	printf("Error: a division by 0 operation was attempted.\n");
	printf("Total number of operations completed successfully: %i\n", count);
	exit(0);

}

/*
 * Handles SIGINT exception
 * Prints the # of valid calculations
 * then exits program
 */
void intHandler(){
	printf("Total number of operations completed successfully: %i\n", count);
	printf("The program will be terminated.\n");
	exit(0);
}

/*
 * Main function
 * Runs infinite loop for dividing inputted numbers
 */
int main(){
	
	// declare and initialize local variables
	int first = 0;
	int second = 0;
	int quotient = 0;
	int remainder = 0;
	char* ptr;
	char buf[100];
	struct sigaction fpe;
	struct sigaction sigint;
	memset(&fpe, 0, sizeof(fpe));
	memset(&sigint, 0, sizeof(sigint));

	// assign sig action handlers
	sigint.sa_handler = intHandler;
	fpe.sa_handler = fpeHandler;

	// sigaction for SIGFPE
	if(sigaction(SIGFPE, &fpe, NULL) != 0){
		printf("Error binding SIGFPE handler\n");
		exit(1);
	}

	// sigaction for SIGINT
	if(sigaction(SIGINT, &sigint, NULL) != 0){
		printf("Error binding SIGINT handler\n");
		exit(1);
	}

	// infinite loop
	while(1){

		// Get first value from user
		printf("Enter first integer: ");
		if(fgets(buf, 100, stdin) != NULL)
			first = strtol(buf, &ptr, 0);
		else{
			printf("Error getting value from user. Exiting now.\n");
			exit(1);
		}

		// Get second value from user
		printf("Enter second integer: ");
		if(fgets(buf, 100, stdin) != NULL)
			second = strtol(buf, &ptr, 0);
		else{
			printf("Error getting value from user. Exiting now.\n");
			exit(1);
		}

		// Get quotient and remainder from the inputted values
		quotient = first / second;
		remainder = first % second;

		// print out final values
		printf("%i / %i is %i with a remainder of %i\n", first, second, quotient, remainder);
		
		// increment counter variable
		count++;

	}

	return 0;
}
