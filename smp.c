#include "inc/common.h"
#include "inc/smp.h"
#include "inc/apic.h"
#include "inc/descriptors.h"

#define ICR_INIT 0x500
#define ICR_ASSERT 0x4000
#define ICR_STARTUP 0x600

// These come from start.s
extern void* kludge_start;
extern void* kludge_end;

volatile uint32_t proccessors_started = 0;

void init_smp(uint8_t apic_id)
{
	uint32_t len = (uint32_t)&kludge_end - (uint32_t)&kludge_start;
	printf("Copying kludge\n");
	memcpy((void*)0x8000, &kludge_start, len);
	printf("Stopping the copying of the kludge\n");
	// Send init
	apic_write(0x310, apic_id << 24);
	apic_write(0x300, ICR_INIT | ICR_ASSERT);
	apic_wait();
//	printf("After apic1\n");
	// sleep 10 ms
	sleep(50);
	// Send startup
	apic_write(0x310, apic_id << 24);
	apic_write(0x300, 8 | ICR_STARTUP | ICR_ASSERT);
//	printf("After apic2\n");
	// sleep 200us
	sleep(2);
	// Send startup
	apic_write(0x310, apic_id << 24);
	apic_write(0x300, 8 | ICR_STARTUP | ICR_ASSERT);
//	printf("After apic3\n");
	// sleep 200us
	sleep(2);
	__asm__ __volatile__("wbinvd");
	printf("Started CPU's: %u", proccessors_started);
}
