/**
 * Implementace My MALloc
 * Demonstracni priklad pro 1. ukol IPS/2019
 * Ales Smrcka
 */

#include "mmal.h"
#include <sys/mman.h> // mmap
#include <stdbool.h> // bool
#include <assert.h> // assert
#include <string.h> // memcpy

#ifdef NDEBUG
/**
 * The structure header encapsulates data of a single memory block.
 *   ---+------+----------------------------+---
 *      |Header|DDD not_free DDDDD...free...|
 *   ---+------+-----------------+----------+---
 *             |-- Header.asize -|
 *             |-- Header.size -------------|
 */
typedef struct header Header;
struct header {

    /**
     * Pointer to the next header. Cyclic list. If there is no other block,
     * points to itself.
     */
    Header *next;

    /// size of the block
    size_t size;

    /**
     * Size of block in bytes allocated for program. asize=0 means the block 
     * is not used by a program.
     */
    size_t asize;
};

/**
 * The arena structure.
 *   /--- arena metadata
 *   |     /---- header of the first block
 *   v     v
 *   +-----+------+-----------------------------+
 *   |Arena|Header|.............................|
 *   +-----+------+-----------------------------+
 *
 *   |--------------- Arena.size ---------------|
 */
typedef struct arena Arena;
struct arena {

    /**
     * Pointer to the next arena. Single-linked list.
     */
    Arena *next;

    /// Arena size.
    size_t size;
};

#define PAGE_SIZE (128*1024)

#endif // NDEBUG

Arena *first_arena = NULL;

/** 
 * Return size alligned to PAGE_SIZE
 */
static
size_t allign_page(size_t size)
{
    if (size < PAGE_SIZE){
    	return PAGE_SIZE;
    } else {
    	unsigned int c = 1;
    	while (size > PAGE_SIZE){
    		size -= PAGE_SIZE;
    		c++;
    	}
    	return c*PAGE_SIZE;
    }
}

size_t allign_block(size_t size)
{
    return (size % sizeof(void*)) ? ((size/sizeof(void*))+1)*sizeof(void*) : size;
}
/**
 * Allocate a new arena using mmap.
 * @param req_size requested size in bytes. Should be alligned to PAGE_SIZE.
 * @return pointer to a new arena, if successfull. NULL if error.
 * @pre req_size > sizeof(Arena) + sizeof(Header)
 */

/**
 *   +-----+------------------------------------+
 *   |Arena|....................................|
 *   +-----+------------------------------------+
 *
 *   |--------------- Arena.size ---------------|
 */
static
Arena *arena_alloc(size_t req_size)
{
	assert(req_size > (sizeof(struct arena) + sizeof(struct header)));
	size_t arena_size = allign_page(req_size);
    Arena *arena_ptr = mmap(NULL, arena_size, PROT_READ | PROT_WRITE,MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (arena_ptr == MAP_FAILED){
		return NULL;
	} else {
		arena_ptr->size = arena_size;
		arena_ptr->next = NULL;
	    return arena_ptr;
	}
}

/**
 * Appends a new arena to the end of the arena list.
 * @param a     already allocated arena
 */
static
void arena_append(Arena *a)
{
    if (first_arena == NULL){
    	first_arena = a;
    } else {
    	Arena *arena = first_arena;
    	while (arena->next != NULL){
        	arena = arena->next;
        }
        arena->next = a;
    }
}

/**
 * Header structure constructor (alone, not used block).
 * @param hdr       pointer to block metadata.
 * @param size      size of free block
 * @pre size > 0
 */
/**
 *   +-----+------+------------------------+----+
 *   | ... |Header|........................| ...|
 *   +-----+------+------------------------+----+
 *
 *                |-- Header.size ---------|
 */
static
void hdr_ctor(Header *hdr, size_t size)
{
    assert(size > 0);
    hdr->size = size;
    hdr->asize = 0;
    static Header *last_header = NULL;
    if (last_header == NULL){
    	hdr->next = hdr;
    } else {
    	hdr->next = last_header->next;
        last_header->next = hdr;
    }
    last_header = hdr;
}

/**
 * Checks if the given free block should be split in two separate blocks.
 * @param hdr       header of the free block
 * @param size      requested size of data
 * @return true if the block should be split
 * @pre hdr->asize == 0
 * @pre size > 0
 */
static
bool hdr_should_split(Header *hdr, size_t size)
{
    assert(hdr->asize == 0);
    assert(size > 0);
    if (hdr->size >= allign_block(size) + 2*sizeof(Header)){
    	return true;
    } else {
    	return false;
    }
}

/**
 * Splits one block in two.
 * @param hdr       pointer to header of the big block
 * @param req_size  requested size of data in the (left) block.
 * @return pointer to the new (right) block header.
 * @pre   (hdr->size >= req_size + 2*sizeof(Header))
 */
/**
 * Before:        |---- hdr->size ---------|
 *
 *    -----+------+------------------------+----
 *         |Header|........................|
 *    -----+------+------------------------+----
 *            \----hdr->next---------------^
 */
/**
 * After:         |- req_size -|
 *
 *    -----+------+------------+------+----+----
 *     ... |Header|............|Header|....|
 *    -----+------+------------+------+----+----
 *             \---next--------^  \--next--^
 */
static
Header *hdr_split(Header *hdr, size_t req_size)
{
    assert((hdr->size >= req_size + 2 * sizeof(struct header)));
    Header *new = (Header *) ((void *) hdr + sizeof(struct header) + req_size);
    new->asize = 0;
    new->size = hdr->size - req_size - sizeof(struct header);
    new->next = hdr->next;
    hdr->next = new;
    hdr->size = req_size;
    return new;
}

/**
 * Detect if two adjacent blocks could be merged.
 * @param left      left block
 * @param right     right block
 * @return true if two block are free and adjacent in the same arena.
 * @pre left->next == right
 * @pre left != right
 */
static
bool hdr_can_merge(Header *left, Header *right)
{
    assert(left->next == right);
    assert(left != right);
    if (left->asize == 0 && right->asize == 0 &&
    	(Header *)(((void *)left) + left->size + sizeof(struct header)) == right){
    	return true;
    } else {
    	return false;
    }
}

/**
 * Merge two adjacent free blocks.
 * @param left      left block
 * @param right     right block
 * @pre left->next == right
 * @pre left != right
 */
static
void hdr_merge(Header *left, Header *right)
{
    
    assert(left->next == right);
    assert(left != right);
    left->size += right->size + sizeof(struct header);
    left->next = right->next;
    right = NULL;
}

/**
 * Finds the free block that fits best to the requested size.
 * @param size      requested size
 * @return pointer to the header of the block or NULL if no block is available.
 * @pre size > 0
 */
static
Header *best_fit(size_t size)
{
    assert(size > 0);
    Header *header = (void *) first_arena + sizeof(struct arena);
	Header *first = header;
    Header *best = NULL;
    //size_t required_size = ((size + sizeof(struct header)) % sizeof(void*)) ? ((size+sizeof(struct header))/sizeof(void*)+1)*sizeof(void*) : size+sizeof(struct header);
    long lowest_gap = -1;
    do {
    	if (header->size >= size && header->asize == 0){
    		if (lowest_gap == -1){
    			best = header;
    			lowest_gap = header->size - size;
    		} else if (header->size < lowest_gap + size) {
    			lowest_gap = header->size - size;
    			best = header;
    		}
    	}
    	header = header->next;
    } while (header != first);
    return best;
}

/**
 * Search the header which is the predecessor to the hdr. Note that if 
 * @param hdr       successor of the search header
 * @return pointer to predecessor, hdr if there is just one header.
 * @pre first_arena != NULL
 * @post predecessor->next == hdr
 */
static
Header *hdr_get_prev(Header *hdr)
{
    assert(first_arena != NULL);
    if (hdr == NULL || hdr->next == hdr){
    	return hdr;
    }
    Header *prev = hdr;
    while(prev->next != hdr){
        prev = prev->next;
    }
    return prev;
}

/**
 * Allocate memory. Use best-fit search of available block.
 * @param size      requested size for program
 * @return pointer to allocated data or NULL if error or size = 0.
 */
void *mmalloc(size_t size)
{
    if(size == 0){
    	return NULL;
    }
    // ak este nie je vytvorena arena -> vytvorim arenu
    if (first_arena == NULL){
    	Arena *arena = arena_alloc(size + sizeof(Arena) + sizeof(Header));
    	if (arena == NULL){
    		return NULL;
    	}
    	arena_append(arena);
    	Header *header = (void *)arena + sizeof(Arena);
    	hdr_ctor(header, arena->size - sizeof(Header) - sizeof(Arena));
    	header->next = header;
    }
    size_t alligned_size = allign_block(size);
    Header *allocated_block = best_fit(alligned_size);
    // ak sa nenasiel volny blok alokuje sa nova arena
    if (allocated_block == NULL){
    	Arena *new_arena = arena_alloc(size + sizeof(Arena) + sizeof(Header));
    	if (new_arena == NULL){
    		return NULL;
    	}
    	arena_append(new_arena);
    	Header *new_header = (void *)new_arena + sizeof(Arena);
        hdr_ctor(new_header, new_arena->size - sizeof(Header) - sizeof(Arena));
        if(hdr_should_split(new_header, alligned_size)){
            hdr_split(new_header, alligned_size);
            new_header->asize = size;
        	return (void *)(new_header + 1);
        }
    // splitnutie najdeneho bloku -> vytvorenie noveho bloku
    } else {
    	if (hdr_should_split(allocated_block, alligned_size)){
    		hdr_split(allocated_block, alligned_size);
    	}
    	allocated_block->asize = size;
   		return (void *) (allocated_block + 1);
    }
    return NULL;
}

/**
 * Free memory block.
 * @param ptr       pointer to previously allocated data
 * @pre ptr != NULL
 */
void mfree(void *ptr)
{
	//printf("%x\n", ptr);
    assert(ptr != NULL);
    Header *freed_block = ((Header *)ptr - 1);
    freed_block->asize = 0;
    Header *prev = hdr_get_prev(freed_block);
    // zprava
	if(freed_block < freed_block->next && hdr_can_merge(freed_block, freed_block->next)){
        hdr_merge(freed_block,freed_block->next);
    }
    // zlava
    if(prev < freed_block && hdr_can_merge(prev, freed_block)){
        hdr_merge(prev, freed_block);
    }
}

/**
 * Reallocate previously allocated block.
 * @param ptr       pointer to previously allocated data
 * @param size      a new requested size. Size can be greater, equal, or less
 * then size of previously allocated block.
 * @return pointer to reallocated space.
 */
void *mrealloc(void *ptr, size_t size)
{
	void *new;
	Header *old = ((Header *)ptr) - 1;
	if (old->size == size){
		return ptr;
	} else if (old->size > size){
		old->asize = size;
		return ptr;
	} else if (size == 0) {
		mfree(ptr);
		return NULL;
	} else {
		Arena *arena_begining = ((Arena *)ptr) - 1;
		Arena *arena_end = arena_begining + arena_begining->size;
		if (old->next != old && old->next->asize == 0 &&
			old->next > arena_begining && old->next < arena_end &&
			old->next->size >= size - old->size){
			hdr_merge(old, old->next);
			hdr_split(old, allign_block(size));
			old->asize += size - old->asize;
			return ptr;
		} else {
			new = mmalloc(size);
			if (new == NULL){
				return NULL;
			}
	    	memcpy(new, ptr, old->asize);
			mfree(ptr);
			return new;
		}
	}
}