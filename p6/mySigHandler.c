///////////////////////////////////////////////////////////////////////////////
   // Main File:        mySignalHandler.c
   // This File:        mySignalHandler.c
   // Other Files:      sendsig.c, division.c
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

// Included libraries
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>

// Global Vars
int seconds = 4;
int count = 0;

/**
 * Handles the ALRM signal
 * Prints PID and current time every 4 seconds
 */
void alrmHandler(){
	time_t curTime;
	if(time(&curTime) == 0){
		printf("Error getting the time. Exiting the program.\n");
		exit(1);
	}

	alarm(seconds);

	if(ctime(&curTime) != NULL){
		printf("PID: %d CURRENT TIME: %s", getpid(), ctime(&curTime));		
	}
	else{
		printf("Error printing the time. Exiting the program.\n");
		exit(1);
	}
}


/*
 * Handles USR1 signal
 * Increments counter of #
 * of times USR1 signal was sent
 */
void user1Handler(){
	count++;
	printf("SIGUSR1 handled and counted!\n");
	
}

/**
 * Handles SIGINT signal
 * Prints Number of times SIGUSR1 was handled
 */
void sigintHandler(){

	printf("SIGINT handled.\n");
	printf("SIGUSR1 was handled %i times. Exiting now.\n", count);
	exit(0);
}

int main(){
	// Create 3 sigaction structs
	struct sigaction alrm;
	struct sigaction usr1;
	struct sigaction sigint;
	
	// Zero out all sigactions
	memset(&alrm, 0, sizeof(alrm));
	memset(&usr1, 0, sizeof(usr1));
	memset(&sigint, 0, sizeof(sigint));

	// Print starting messages to console
	printf("PID and time print every 4 seconds.\n");
	printf("Type Ctrl-C to end the program.\n");	

	// Start 4 second alarm and set alarm handler
	alarm(seconds);
	alrm.sa_handler = alrmHandler;

	// Calls handler for alarm
	if(sigaction(SIGALRM, &alrm, NULL) != 0){
		printf("Error binding SIGALARM handler\n");
		exit(1);
	}

	usr1.sa_handler = user1Handler;

	// Calls handler for USR1
	if(sigaction(SIGUSR1, &usr1, NULL) != 0){
		printf("Error binding SIGUSR1 handler\n");
        exit(1);		
	}

	sigint.sa_handler = sigintHandler;

	// Calls handler for SIGINT
	if(sigaction(SIGINT, &sigint, NULL) != 0){
		printf("Error binding SIGINT handler \n");
		exit(1);
	}

	// Infinite loop
	while(1){

	}


	return 0;
}
