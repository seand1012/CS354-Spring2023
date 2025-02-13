// Copyright 2020-2023 Deb Deppeler based on work by Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission SPRING 2023, CS354-deppeler
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
   // Main File:        p3Heap.c
   // This File:        p3Heap.c
   // Other Files:      p3Heap.h, p3Heap.o
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

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include "p3Heap.h"
 
/*
 * This structure serves as the header for each allocated and free block.
 * It also serves as the footer for each free block but only containing size.
 */
typedef struct blockHeader {           

    int size_status;

    /*
     * Size of the block is always a multiple of 8.
     * Size is stored in all block headers and in free block footers.
     *
     * Status is stored only in headers using the two least significant bits.
     *   Bit0 => least significant bit, last bit
     *   Bit0 == 0 => free block
     *   Bit0 == 1 => allocated block
     *
     *   Bit1 => second last bit 
     *   Bit1 == 0 => previous block is free
     *   Bit1 == 1 => previous block is allocated
     * 
     * Start Heap: 
     *  The blockHeader for the first block of the heap is after skip 4 bytes.
     *  This ensures alignment requirements can be met.
     * 
     * End Mark: 
     *  The end of the available memory is indicated using a size_status of 1.
     * 
     * Examples:
     * 
     * 1. Allocated block of size 24 bytes:
     *    Allocated Block Header:
     *      If the previous block is free      p-bit=0 size_status would be 25
     *      If the previous block is allocated p-bit=1 size_status would be 27
     * 
     * 2. Free block of size 24 bytes:
     *    Free Block Header:
     *      If the previous block is free      p-bit=0 size_status would be 24
     *      If the previous block is allocated p-bit=1 size_status would be 26
     *    Free Block Footer:
     *      size_status should be 24
     */
} blockHeader;         

/* Global variable - DO NOT CHANGE NAME or TYPE. 
 * It must point to the first block in the heap and is set by init_heap()
 * i.e., the block at the lowest address.
 */
blockHeader *heap_start = NULL;     

/* Size of heap allocation padded to round to nearest page size.
 */
int alloc_size;

/*
 * Additional global variables may be added as needed below
 * TODO: add global variables needed by your function
 */

/*
 * Helper to figure out the size of
 * each block
 *
 * Returns the size of the block, plus the header,
 * plus the necessary padding
 * Returns a low negative number if size < 1
 */
int blockSizeHelper(int size){
	if(size < 1){
		printf("size < 1");
		return -10000000;
	}

	int totalBlockSize = 4;

	totalBlockSize += size;
	
	if(totalBlockSize % 8 == 0){
		return totalBlockSize;
	}
	else{
		totalBlockSize += (8 - (totalBlockSize % 8));
		return totalBlockSize;
	}
}

/* 
 * Function for allocating 'size' bytes of heap memory.
 * Argument size: requested size for the payload
 * Returns address of allocated block (payload) on success.
 * Returns NULL on failure.
 *
 * This function must:
 * - Check size - Return NULL if size < 1 
 * - Determine block size rounding up to a multiple of 8 
 *   and possibly adding padding as a result.
 *
 * - Use BEST-FIT PLACEMENT POLICY to chose a free block
 *
 * - If the BEST-FIT block that is found is exact size match
 *   - 1. Update all heap blocks as needed for any affected blocks
 *   - 2. Return the address of the allocated block payload
 *
 * - If the BEST-FIT block that is found is large enough to split 
 *   - 1. SPLIT the free block into two valid heap blocks:
 *         1. an allocated block
 *         2. a free block
 *         NOTE: both blocks must meet heap block requirements 
 *       - Update all heap block header(s) and footer(s) 
 *              as needed for any affected blocks.
 *   - 2. Return the address of the allocated block payload
 *
 *   Return if NULL unable to find and allocate block for required size
 *
 * Note: payload address that is returned is NOT the address of the
 *       block header.  It is the address of the start of the 
 *       available memory for the requesterr.
 *
 * Tips: Be careful with pointer arithmetic and scale factors.
 */
void* balloc(int size) {     
    //TODO: Your code goes in here.
	int padding = 0;	
	if(size < 1){
		return NULL;
	}

	// determine how much padding will need to be added   
	if((size + 4) % 8 != 0){
		padding = (8 - ((size + sizeof(blockHeader)) % 8));
	}

    size = size + sizeof(blockHeader) + padding;
	blockHeader* bHCurr = heap_start;
	blockHeader* bHNext = heap_start;
	blockHeader* bHBest = heap_start;
	//blockHeader* footer = NULL;

	int ss; // shorthand for bH->size_status
	
	// if bH.size_status % 8 == 0 -> previous and current blocks are free
		// bH.size_status is also equal to the free block's exact size
		// bH.size_status is also equal to its footer
	// if bH.size_status % 8 == 1 -> previous block is free and current block is allocated
	// if bH.size_status % 8 == 2 -> previous block is allocated and current block is free
		// bH.size_status - 2 is the footer
	// if bH.size_status % 8 == 3 -> previous and current blocks are allocated

	int aBit = 1; // when size_status anded with 1, if == 1, allocated
	
	
	while(bHCurr->size_status != 1){
		ss = bHCurr->size_status;
		// current block is free
		if((ss & aBit) == 0){

			// case 1: Best Fit finds a free block of the exact size		
			// size is exactly equivalent to free block size
			if((ss & ~3) == size){
				// test to make sure the next block is not the end
				if((bHCurr + (ss & ~3))->size_status != 1 && (bHCurr + (ss & ~3))->size_status <= alloc_size){

					// set next block's p-bit to show this block is allocated
					(bHCurr + (ss & ~3))->size_status += 2;

					// set current block's a-bit to show this block is allocated
					bHCurr->size_status &= 2;
					bHCurr->size_status += size;
			        bHCurr->size_status += 1;
				}

				// best block was found, return
				return ((void*)bHCurr + sizeof(blockHeader));	
			}

			// case 2: free block that's bigger than needed
			// block needs to be split
			else if((ss & ~3) > size){

				// case where bHBest is too small
				// make bHBest bigger
				// will split later
				if((bHBest->size_status & ~3) <= size)
					bHBest = bHCurr;

				// test if current block is smaller than currently held best block
				// assign this better block to best
				// e.g. size = 16, best->size_status = 32, curr->size_status = 24, assign curr to best
				if(bHBest->size_status >= size && (bHCurr->size_status & ~3) <= (bHBest->size_status & ~3)){
					bHBest = bHCurr;
				}
			}
			// case 3:block isn't big enough, advance loop
			else{
				// block is already allocated
				if((ss & 1) == 1){
					;
				}
				// next block is the end
				else if(((blockHeader*)((void*)bHCurr + (ss & ~3)))->size_status != 1){
					;
				}
				// make sure bHBest was updated
				else if(bHBest != heap_start){
					;
				}
				// all else, return NULL
				else{
					bHBest = NULL;
				}
			}
		}

		// move pointer to next block
		bHCurr = (blockHeader*) ((void*)bHCurr + (bHCurr->size_status & ~3));
		
	}

	// split a block that's too big
	if (bHBest) {
		bHNext = (blockHeader*)(((void*) bHBest) + size);

		// split block into 2 blocks
		int x = ((bHBest->size_status & ~3)-size);
		bHNext->size_status = x;		
		// set next block's p-bit to show previous block is allocated
		bHNext->size_status += 2;
       
		// set best block's size to proper size, and a-bit to 1 
		bHBest->size_status &= 2; 
        bHBest->size_status += size;
        bHBest->size_status += 1;
		disp_heap();
        return (void*)bHBest + sizeof(blockHeader);
	}

	// catch-all fail case
    return NULL;
}
 
/* 
 * Function for freeing up a previously allocated block.
 * Argument ptr: address of the block to be freed up.
 * Returns 0 on success.
 * Returns -1 on failure.
 * This function should:
 * - Return -1 if ptr is NULL.
 * - Return -1 if ptr is not a multiple of 8.
 * - Return -1 if ptr is outside of the heap space.
 * - Return -1 if ptr block is already freed.
 * - Update header(s) and footer as needed.
 */                    
int bfree(void *ptr) {    
    //TODO: Your code goes in here.
	blockHeader* bH = ptr;
	disp_heap();
	// FAIL CASES

	// ptr is null
	if(ptr == NULL)
		return -1;

	// ptr is not divisible by 8
	if((unsigned int) ptr % 8 != 0)
		return -1;

	// pointer is out of bounds
	if(bH < heap_start || bH > (blockHeader*)((void*)heap_start + alloc_size))
		return -1;

	// move left the 4 header bytes from the pointer
	bH = (blockHeader*)((void*) bH - 4);
	// block is already free
	if((bH->size_status & 1) == 0){
		return -1;
	}
	else{
		// set a-bit to 0
		bH->size_status  -= 1;

		int ss = bH->size_status;
		// if p-bit is 0, just go to next block
		if(!(bH->size_status & 2)){
			bH = (blockHeader*)((void*)bH + bH->size_status);
		}
		else{
			// set p-bit of next block to 0
			bH = (blockHeader*)((void*)bH + bH->size_status - 2);
			// set a-bit to 0 if necessary
			if(bH->size_status == 1){
				return -1;
			}
			// set next block's p-bit to 0 to show this block is free
			ss -= 2;	
		}
		
		// if p-bit is not 0, set to 0
		if(bH->size_status & 2){
			bH->size_status -= 2;
		}
	
		bH = (blockHeader*)((void*)bH - 4);
		bH->size_status = ss;
		disp_heap();
		return 0;
	}
	return -1;
} 

/*
 * Function for traversing heap block list and coalescing all adjacent 
 * free blocks.
 *
 * This function is used for user-called coalescing.
 * Updated header size_status and footer size_status as needed.
 */
int coalesce() {
    //TODO: Your code goes in here.
	blockHeader* bHCurr = heap_start;
	blockHeader* bHPrev = heap_start;

	int ss = bHCurr->size_status;

	// if allocated, set a-bit to 0
	if(ss & 1)
		ss -= 1;
	// if p-bit is set, set to 0
	if(ss & 2)
		ss -= 2;

	// look at next block
	bHCurr = (blockHeader*)((void*)bHCurr + ss);
	while(bHCurr->size_status != 1){	
		ss = bHCurr->size_status;
		// if this block is set to allocated, set to free
		if(ss & 1)
			ss -= 1;
		// if previous block is set to allocated, set to free
		if(ss & 2)
			ss -= 2;

		// if this block is either set to allocated or says prev block is allocated
		if((bHCurr->size_status & 1) || (bHCurr->size_status & 2)){
			// set previous block to this block
			bHPrev = bHCurr;
			// advance loop by size_status
			bHCurr = (blockHeader*)((void*)bHCurr + ss);

		}
		else{
			
			bHPrev->size_status += ss;
			ss = bHPrev->size_status;

			if(ss & 2)
				ss -= 2;

			
			bHCurr = (blockHeader*)((void*)bHPrev + ss - sizeof(blockHeader));
			bHCurr->size_status = ss;
			// advance current block by previous block and current's size
			bHCurr = (blockHeader*)((void*)bHPrev + ss);	
			
		}
		disp_heap();
	}

	return 0;
}

 
/* 
 * Function used to initialize the memory allocator.
 * Intended to be called ONLY once by a program.
 * Argument sizeOfRegion: the size of the heap space to be allocated.
 * Returns 0 on success.
 * Returns -1 on failure.
 */                    
int init_heap(int sizeOfRegion) {    
 
    static int allocated_once = 0; //prevent multiple myInit calls
 
    int   pagesize; // page size
    int   padsize;  // size of padding when heap size not a multiple of page size
    void* mmap_ptr; // pointer to memory mapped area
    int   fd;

    blockHeader* end_mark;
  
    if (0 != allocated_once) {
        fprintf(stderr, 
        "Error:mem.c: InitHeap has allocated space during a previous call\n");
        return -1;
    }

    if (sizeOfRegion <= 0) {
        fprintf(stderr, "Error:mem.c: Requested block size is not positive\n");
        return -1;
    }

    // Get the pagesize from O.S. 
    pagesize = getpagesize();

    // Calculate padsize as the padding required to round up sizeOfRegion 
    // to a multiple of pagesize
    padsize = sizeOfRegion % pagesize;
    padsize = (pagesize - padsize) % pagesize;

    alloc_size = sizeOfRegion + padsize;

    // Using mmap to allocate memory
    fd = open("/dev/zero", O_RDWR);
    if (-1 == fd) {
        fprintf(stderr, "Error:mem.c: Cannot open /dev/zero\n");
        return -1;
    }
    mmap_ptr = mmap(NULL, alloc_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (MAP_FAILED == mmap_ptr) {
        fprintf(stderr, "Error:mem.c: mmap cannot allocate space\n");
        allocated_once = 0;
        return -1;
    }
  
    allocated_once = 1;

    // for double word alignment and end mark
    alloc_size -= 8;

    // Initially there is only one big free block in the heap.
    // Skip first 4 bytes for double word alignment requirement.
    heap_start = (blockHeader*) mmap_ptr + 1;

    // Set the end mark
    end_mark = (blockHeader*)((void*)heap_start + alloc_size);
    end_mark->size_status = 1;

    // Set size in header
    heap_start->size_status = alloc_size;

    // Set p-bit as allocated in header
    // note a-bit left at 0 for free
    heap_start->size_status += 2;

    // Set the footer
    blockHeader *footer = (blockHeader*) ((void*)heap_start + alloc_size - 4);
    footer->size_status = alloc_size;
  
    return 0;
} 
                  
/* 
 * Function can be used for DEBUGGING to help you visualize your heap structure.
 * Traverses heap blocks and prints info about each block found.
 * 
 * Prints out a list of all the blocks including this information:
 * No.      : serial number of the block 
 * Status   : free/used (allocated)
 * Prev     : status of previous block free/used (allocated)
 * t_Begin  : address of the first byte in the block (where the header starts) 
 * t_End    : address of the last byte in the block 
 * t_Size   : size of the block as stored in the block header
 */                     
void disp_heap() {     
 
    int    counter;
    char   status[6];
    char   p_status[6];
    char * t_begin = NULL;
    char * t_end   = NULL;
    int    t_size;

    blockHeader *current = heap_start;
    counter = 1;

    int used_size =  0;
    int free_size =  0;
    int is_used   = -1;

    fprintf(stdout, 
	"*********************************** HEAP: Block List ****************************\n");
    fprintf(stdout, "No.\tStatus\tPrev\tt_Begin\t\tt_End\t\tt_Size\n");
    fprintf(stdout, 
	"---------------------------------------------------------------------------------\n");
  
    while (current->size_status != 1) {
        t_begin = (char*)current;
        t_size = current->size_status;
    
        if (t_size & 1) {
            // LSB = 1 => used block
            strcpy(status, "alloc");
            is_used = 1;
            t_size = t_size - 1;
        } else {
            strcpy(status, "FREE ");
            is_used = 0;
        }

        if (t_size & 2) {
            strcpy(p_status, "alloc");
            t_size = t_size - 2;
        } else {
            strcpy(p_status, "FREE ");
        }

        if (is_used) 
            used_size += t_size;
        else 
            free_size += t_size;

        t_end = t_begin + t_size - 1;
    
        fprintf(stdout, "%d\t%s\t%s\t0x%08lx\t0x%08lx\t%4i\n", counter, status, 
        p_status, (unsigned long int)t_begin, (unsigned long int)t_end, t_size);
    
        current = (blockHeader*)((char*)current + t_size);
        counter = counter + 1;
    }

    fprintf(stdout, 
	"---------------------------------------------------------------------------------\n");
    fprintf(stdout, 
	"*********************************************************************************\n");
    fprintf(stdout, "Total used size = %4d\n", used_size);
    fprintf(stdout, "Total free size = %4d\n", free_size);
    fprintf(stdout, "Total size      = %4d\n", used_size + free_size);
	fprintf(stdout,
	"*********************************************************************************\n");
    fflush(stdout);

    return;  
} 


// end p3Heap.c (Spring 2023)
