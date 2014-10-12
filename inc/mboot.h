#ifndef MBOOT_H
#define MBOOT_H
	struct mboot_info {
		uint32_t flags;
		uint32_t mem_lower;
		uint32_t mem_upper;
		uint32_t boot_device;
		uint32_t cmdline;
		uint32_t mods_count;
		uint32_t mods_addr;
		uint32_t dont_use[4];
		uint32_t mmap_length;
		uint32_t mmap_addr;
	} __attribute__ ((__packed__));

	typedef struct mboot_info mboot_info;
#endif
