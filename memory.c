#include "inc/common.h"
#include "inc/memory.h"

#define MEM_MAP_MAX 256

/*
	Warning, this code makes a couple of DANGEROUS assumptions
	- That the memory map has no overlaps
	- That the memory map does not contain 0 length entries
	- That its always possible to page align addresses
	- That no calculations overflow
	- It only moves the start of a memory area, and cannot 'split' a memory area into pieces.

	It also makes *no* checks on whether it overlaps acpi, apic, the multiboot info
*/


extern void* kernel_addr_start;
extern void* kernel_addr_end;
extern void* kludge_start;
extern void* kludge_end;

typedef struct multiboot_memory_map {
	uint32_t size, base_addr_low,base_addr_high,
	length_low,length_high,	type;
} multiboot_memory_map_t;

typedef struct memory_parts {
	uint32_t start, size;
} memory_parts;

memory_parts memory_map[MEM_MAP_MAX];

static inline void memory_map_clean()
{
	for(uint32_t i = 0; i < MEM_MAP_MAX; i++)
	{
		memory_map[i].start = 0;
		memory_map[i].size = 0;
	}
}

// This is a function to page align a memory pointer
static inline uint32_t page_align_trick(uint32_t the_pointer)
{
	if(the_pointer & 0xFFFFF000)
	{
		return (the_pointer & 0xFFFFF000) + 0x1000;
	} else {
		return the_pointer;
	}
}

uint32_t curr_map_point = 0;
uint32_t* curr_map_point_subpointer = 0;

void push_page(uint32_t p)
{
	memory_parts cm = memory_map[curr_map_point];
	// Note to self, this is where i should resume coding
	*curr_map_point_subpointer = p;
	++curr_map_point_subpointer;
	// If we get out of bounds in our memory region,
	// jump to the next one
	if((uint32_t)curr_map_point_subpointer >= cm.start+cm.size)
	{
		++curr_map_point;
		curr_map_point_subpointer = (uint32_t*)memory_map[curr_map_point].start;
	}
}

uint32_t pop_page()
{
	memory_parts cm = memory_map[curr_map_point];
	// Note to self, this is where i should resume coding
	uint32_t ret = *curr_map_point_subpointer;
	--curr_map_point_subpointer;
	// If we get out of bounds in our memory region,
	// jump to the last one
	if((uint32_t)curr_map_point_subpointer <= cm.start)
	{
		--curr_map_point;
		curr_map_point_subpointer =(uint32_t*) (memory_map[curr_map_point].start +
					memory_map[curr_map_point].size -
					sizeof(uint32_t));
	}
	return ret;
}

void init_memory(mboot_info *mbp)
{
	memory_map_clean();
	printf("Flags: %X\n", mbp->flags);
	printf("Start: %X End: %X Kludge start: %X Kludge end: %X\n",
	&kernel_addr_start, &kernel_addr_end, &kludge_start, &kludge_end);
	uint32_t kludge_length = ((uint32_t)&kludge_end-(uint32_t)&kludge_start);
	/*
		Check bounds for:
		Kernel area
		Kludge area
		Multiboot area - TODO
		ACPI area - TODO
	 */

	if(mbp->flags & 1 << 6)
	{
		multiboot_memory_map_t* mmap = (void *)mbp->mmap_addr;
		uint32_t i = 0;
		while((uint32_t)mmap < mbp->mmap_addr + mbp->mmap_length) {
			printf("Size: %u ba: %X-%X l: %X-%X t: %u\n",
			mmap->size, mmap->base_addr_high, mmap->base_addr_low,
			mmap->length_high, mmap->length_low, mmap->type);
			if(mmap->type == 1)
			{

				// If this memory part overlaps where we want to load our smp kludge
				if(mmap->base_addr_low < (uint32_t)0x8000+kludge_length)
				{
					uint32_t moveto = page_align_trick(0x8000+kludge_length+1);
					uint32_t movelen = (moveto - mmap->base_addr_low);

					if(mmap->length_low > movelen)
					{
						memory_map[i].size = mmap->length_low - movelen;
						memory_map[i].start = moveto;
						i++;
					} else {
					}

				} else if(mmap->base_addr_low >= ((uint32_t)&kernel_addr_start) && mmap->base_addr_low < ((uint32_t)&kernel_addr_end)){
					uint32_t moveto = page_align_trick(((uint32_t)&kernel_addr_end)+1);
					uint32_t movelen = (moveto - mmap->base_addr_low);

					if(mmap->length_low > movelen)
					{
						memory_map[i].size = mmap->length_low - movelen;
						memory_map[i].start = moveto;
						i++;
					} else {
					}
				} else {
					memory_map[i].start = mmap->base_addr_low;
					memory_map[i].size = mmap->length_low;
					i++;
				}
			}
			mmap = (multiboot_memory_map_t*) ( (unsigned int)mmap + mmap->size + sizeof(unsigned int) );
		}

		// Where should we start giving out pages? Loop the map to find out size, divide på 4096 and multiply by four

		uint32_t map_size = 0;

		for(uint32_t i = 0; i < MEM_MAP_MAX; i++)
		{
			if(memory_map[i].start != 0 && memory_map[i].size != 0)
			{
				map_size += memory_map[i].size / 4096;
			}
		}

		map_size = map_size * sizeof(uint32_t);
		curr_map_point_subpointer = (uint32_t*)(memory_map[1].start);


		for(uint32_t i = MEM_MAP_MAX; i != 0; i--)
		{
			if(memory_map[i].start != 0 && memory_map[i].size != 0)
			{
				// Its time to init this mothertrucker. Loop de loop.
				for(uint32_t p = 0; p < memory_map[i].size; p += 0x1000)
				{
					push_page(memory_map[i].start + p);
				}
			}
		}
	}
}
