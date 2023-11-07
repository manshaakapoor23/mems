/*
All the main functions with respect to the MeMS are inplemented here
read the function discription for more details

NOTE: DO NOT CHANGE THE NAME OR SIGNATURE OF FUNCTIONS ALREADY PROVIDED
you are only allowed to implement the functions 
you can also make additional helper functions a you wish

REFER DOCUMENTATION FOR MORE DETAILS ON FUNSTIONS AND THEIR FUNCTIONALITY
*/
// add other headers as required
#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

/*
Use this macro where ever you need PAGE_SIZE.
As PAGESIZE can differ system to system we should have flexibility to modify this 
macro to make the output of all system same and conduct a fair evaluation. 
*/
#define PAGE_SIZE 4096
typedef struct block{
    size_t size;
    struct block*next;
} block;

// MeMS Heap
typedef struct memHeap{
    void* start;
    size_t mem_size;
    block* freelist;
} memHeap;

memHeap memsheap;

/*
Initializes all the required parameters for the MeMS system. The main parameters to be initialized are:
1. the head of the free list i.e. the pointer that points to the head of the free list
2. the starting MeMS virtual address from which the heap in our MeMS virtual address space will start.
3. any other global variable that you want for the MeMS implementation can be initialized here.
Input Parameter: Nothing
Returns: Nothing
*/
void mems_init(){
    // to request an initial block of memory from the operating system
    memsheap.start = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0); 
    memsheap.mem_size = PAGE_SIZE;
    memsheap.freelist =  (block*)memsheap.start;
    size_t block_size = sizeof(block);
    // allocated memory us added to the free list
    memsheap.freelist->size = PAGE_SIZE - block_size;
    memsheap.freelist->next = NULL;
}


/*
This function will be called at the end of the MeMS system and its main job is to unmap the 
allocated memory using the munmap system call.
Input Parameter: Nothing
Returns: Nothing
*/
void mems_finish(){
    // deallocates all memory allocated to MeMS 
    block* (current_block) = memsheap.freelist;
    while (current_block)  {
        block* next = current_block->next;
        size_t current_size = current_block->size;
        size_t block_size = sizeof(block);
        munmap(current_block, current_size + block_size);
        current_block = next;
    }
}

/*
Allocates memory of the specified size by reusing a segment from the free list if 
a sufficiently large segment is available. 

Else, uses the mmap system call to allocate more memory on the heap and updates 
the free list accordingly.

Note that while mapping using mmap do not forget to reuse the unused space from mapping
by adding it to the free list.
Parameter: The size of the memory the user program wants
Returns: MeMS Virtual address (that is created by MeMS)
*/ 
void* mems_malloc(size_t size){
    
}


/*
this function print the stats of the MeMS system like
1. How many pages are utilised by using the mems_malloc
2. how much memory is unused i.e. the memory that is in freelist and is not used.
3. It also prints details about each node in the main chain and each segment (PROCESS or HOLE) in the sub-chain.
Parameter: Nothing
Returns: Nothing but should print the necessary information on STDOUT
*/
void mems_print_stats(){
    // mapped pages
    int pages = memsheap.mem_size / PAGE_SIZE;
    int memory = 0; // unused memory
    block* current_block = memsheap.freelist;
    //current_block_size = current_block->size;
    //printf("Block size : %zu bytes \n", current_block->size);
    while (current_block) {
        //size_t current_block_size = current_block->size;
        printf("Block size : %zu bytes \n", current_block->size);
        memory = memory + current_block->size;
        current_block = current_block->next;
    }
    printf("Unused memory : %d bytes \n", memory);
}

/*
Returns the MeMS physical address mapped to ptr ( ptr is MeMS virtual address).
Parameter: MeMS Virtual address (that is created by MeMS)
Returns: MeMS physical address mapped to the passed ptr (MeMS virtual address).
*/
void *mems_get(void*v_ptr){
    return v_ptr;
}


/*
this function free up the memory pointed by our virtual_address and add it to the free list
Parameter: MeMS Virtual address (that is created by MeMS) 
Returns: nothing
*/
void mems_free(void *v_ptr) {
    // frees the memory pointed by ptr
    if (v_ptr) {
        // marking the corresponding block as unused for further allocations
        block* current_block = (block*)v_ptr - 1;
        current_block->next = memsheap.freelist; 
        memsheap.freelist = current_block->next;
    }
    else {
        perror("NULL pointer");
        //return -1;
        exit(1);
    }
}
