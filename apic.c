#include "inc/common.h"
#include "inc/apic.h"

#define APIC_ADDR 0xFEE00000


void init_apic()
{
	printf("Initilizing APIC\n");
	cpuSetMSR(0x1B, APIC_ADDR | 0x800, 0); // Enable the apic!
	printf("APIC enabled\n");

	uint32_t version = apic_read(0x30);

	uint8_t the_version = version & 0xFF;
	uint8_t max_lvt = (version & 0x7F0000) >> 16;
	uint8_t can_suppress_eoi = (version & 0x800000) >> 23;
	printf("Version: %d Max LVT: %d Can suppress EOI: %d\n", the_version, max_lvt, can_suppress_eoi);
	if(max_lvt == 5 && the_version == 17)
	{

		// Since we want all the mode settings that are '0', we can just
		// Plonk in the interrupt numbers we want to use, and we get
		// One shot, not masked, edge triggered
/*		apic_write(0x2F0, 33);
*/
		apic_write(0x320, 34);

// 0x330 thermal
//		apic_write(0x330, 35);

		apic_write(0x350, 36);
		apic_write(0x360, 37);
		apic_write(0x370, 38);


		// We set the Logical Destination Register to use
		// 1 as a logical apic id:

		// And then we set the Task Priority Register to 0
		// To enable all interrupts
		apic_write(0x80, 0);

		// These i dont quite understand, but.
		// We set the Destination Format Register to flat mode
		apic_write(0xE0, 0xFFFFFFFF);


		apic_write(0xD0, 0x01000000);

		// Oh, and its time to enable the apic.
		// We give it a spurius interrupt number, 32
		apic_write(0xF0, 0x100 | 0xFF); //| 32);


	} else {
		printf("Unknown APIC type\n");
	}
}

void apic_eoi()
{
	apic_write(0xB0, 0xFFFFFFFF);
}

uint8_t getApicId()
{
	uint32_t apic_id = apic_read(0x20);
	return apic_id >> 24;
}
