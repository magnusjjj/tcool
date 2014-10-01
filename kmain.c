#include "inc/printf.h"
#include "inc/common.h"
#include "inc/apic.h"
#include "inc/descriptors.h"
#include "inc/acpi.h"
#include "inc/smp.h"

void kmain()
{
	setup_printf();
	clear_screen();
	init_idt();
	__asm__ __volatile__("sti");
	init_apic();
	init_acpi();
//	__asm__ __volatile__("cli");
	init_smp(3);
//	init_smp(4);
//	init_smp(5);
//	__asm__ __volatile("movl $0x07690748, 0xb8002");
}
