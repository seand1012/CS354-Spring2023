///////////////////////////////////////////////////////////////////////////////
   // Main File:        decode.c
   // This File:        decode.c
   // Other Files:      decode.i
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

int arr2D[128][8];

int main(){
	for(int i = 0; i < 100; i++){
		for(int k = 0; k < 128; k += 64){
			for(int l = 0; l < 8; l++){
				arr2D[k][l] = i + k + l;
			}
		}
	}
}
