///////////////////////////////////////////////////////////////////////////////
   // Main File:        sendsig.c
   // This File:        sendsig.c
   // Other Files:      mySigHandler.c, division.c
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
#include <string.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc, char** argv){
	
	if(argc != 3){
		printf("Usage: sendsig <signal type> <pid>\n");
		exit(1);
	}

	// SIGUSR1 signal
	if(strcmp(*(argv + 1), "-u") == 0){
		// runs kill() command with SIGUSR1
		if(kill(atoi(*(argv + 2)), SIGUSR1) != 0){
			printf("Error invoking kill command. Exiting program\n");
			exit(1);
		}		
	}

	// SIGINT signal
	else if(strcmp(*(argv + 1), "-i") == 0){
		// runs kill() command with SIGINT
		if(kill(atoi(*(argv + 2)), SIGINT) != 0){
			printf("Error invoking kill command. Exiting program \n");
			exit(1);
		}
	}
	// invalid input
	else{
		printf("Usage: sendsig <signal type> <pid>\n");
		exit(1);
	}

}
