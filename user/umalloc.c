#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

// Mem alignment and page size
#define ALIGNMENT 16     // mem allocations are 16-byte 
#define PAGE_SIZE 4096  // mem is allocated in page sized chunks
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1)) //aligns mem to the nearest 16 byte 


// Memory allocator by Kernighan and Ritchie,
// The C programming Language, 2nd ed.  Section 8.7.


//Global pointers
static struct mem_block *free_list = NULL; //tracks free mem blocks
static struct mem_block *block_list = NULL; //tracks all allocated mem blocks

// Mem block double linked list
struct mem_block {
    char name[8];
    uint size; //block size
    struct mem_block *next_block; //pointer to next block on list
    struct mem_block *prev_block; // pointer to previous block on list
};

// Function declarations
void *malloc(uint size);
void free(void *ptr);
void *calloc(uint nmemb, uint size);
void malloc_print(void);

// Finds free block of at least requested size using first fit
static struct mem_block *find_free_block(uint size) {
    struct mem_block *curr = free_list;
    
    while (curr) {
        if ((curr->size & 1) && curr->size >= size) {  // Check if block is free    
            curr->size &= ~1;  // mark block as used and clear free flag

			//remove the block from free list if it is the head
            if (free_list == curr) {
                free_list = curr->next_block;
            }

            return curr;
        }
        curr = curr->next_block;  //move to  next free block
    }
    return NULL; //no usable block found
}

// Splits a block into allocated free parts if it has exceess space
struct mem_block *split(struct mem_block *block, uint size) {
    if (!block || block->size < size + sizeof(struct mem_block) + ALIGNMENT) {
        return block; //not enough space to split
    }

    struct mem_block *new_block = (struct mem_block *)((char *)block + sizeof(struct mem_block) + size);
    new_block->size = block->size - size - sizeof(struct mem_block);
    new_block->size |= 1;  //mark new block as free
    new_block->next_block = block->next_block;
    new_block->prev_block = block;  //ensure new block points back to old block

    block->size = size; //set allocated block size
    block->size &= ~1;  // Mark block as used
    block->next_block = new_block; //link new free block

	// ensure that next block is properly linked
    if (new_block->next_block) {
        new_block->next_block->prev_block = new_block;
    }
    return block;
}



 // Request more memory using sbrk
 static struct mem_block *request_memory(uint size) {
    uint total_size = size + sizeof(struct mem_block);
    total_size = (total_size + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1); // round up to page size

    struct mem_block *block = (struct mem_block *)sbrk(total_size);
    if (block == (void *)-1) {
        return NULL; // sbrk failed
    }

    block->size = total_size - sizeof(struct mem_block);
    block->size &= ~1;  // mark as used
    block->next_block = block_list;

    //ensure proper doubly linked list struct
    block->prev_block = NULL;  // this is now the head
    if (block_list) {
        block_list->prev_block = block;
    }

    block_list = block;  // update head of the list
    return block;
}


// Allocates memory
void*
malloc(uint nbytes) {
    if (nbytes == 0) {
        return NULL;
    }
    nbytes = ALIGN(nbytes);  //align size to 16 bytes

    struct mem_block *block = find_free_block(nbytes);
    if (block) {
        return split(block, nbytes) + 1; //return user memory
    }

    //or  request new memory
    block = request_memory(nbytes);
    if (!block) {
        return NULL;
    }
    return block + 1; // return ppointer to user mem 	
}

// free memory and merge free blocks
void free(void *ptr) {
    if (!ptr) {
        return;
    }

    struct mem_block *block = ((struct mem_block *)ptr) - 1;
    block->size |= 1;  // mark as free and set lowest bit

    //try to merge with next block
    if (block->next_block && (block->next_block->size & 1)) {
        block->size += sizeof(struct mem_block) + block->next_block->size;
        block->next_block = block->next_block->next_block;
        
        //ensure prev block of next block is updated correctly
        if (block->next_block) {
            block->next_block->prev_block = block;
        }
    }

    //try to merge with previous block
    if (block->prev_block && (block->prev_block->size & 1)) {
        block->prev_block->size += sizeof(struct mem_block) + block->size;
   
        if (block->next_block) {
            block->next_block->prev_block = block;
        }
        
    
        if (block->next_block) {
            block->next_block->prev_block = block->prev_block;
        }
        block = block->prev_block;
    }

    //insert block into free list ascending order
    struct mem_block **curr = &free_list;
    while (*curr && *curr < block) {
        curr = &((*curr)->next_block);
    }
    block->next_block = *curr;
    *curr = block;
}


// Prints state of mem allocation
void malloc_print(void) {
    struct mem_block *curr = block_list;

    if (!block_list) {
        return; //nothing to print if block_list is empty
    }

    printf("\n-- Memory Blocks --\n");
    int count = 0;

    while (curr && count < 20) {
        printf("[BLOCK] Address: %p\n", curr);
        printf("Size: %d\n", curr->size & ~1);  //mask out free flag for size
        if (curr->size & 1) {
            printf("Status: [FREE]\n");
        } else {
            printf("Status: [USED]\n");
        }
        curr = curr->next_block;
        count++;
    }

    printf("\n-- Free List --\n");
    curr = free_list;
    count = 0;

    while (curr && count < 20) {
        printf("[FREE BLOCK] Address: %p\n", curr);
        printf("Size: %d\n", curr->size & ~1);
        curr = curr->next_block;
        count++;
    }
}


// allocate memory and zero it
void *
calloc(uint nmeb, uint size)
{
    uint total_size = nmeb * size;
    void *memory = malloc(total_size);
    if (!memory) return NULL;

    memset(memory, 0, total_size);
    return memory;
}

