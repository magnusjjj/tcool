#include "inc/common.h"
#include "inc/ioapic.h"

// Uncermoniously STOLEN from https://github.com/pdoane/osdev/blob/master/intr/ioapic.c
// Then adapted a bit

// ------------------------------------------------------------------------------------------------
static void IoApicOut(uint8_t *base, uint8_t reg, uint32_t val)
{
   *((uint32_t *)(base + IOREGSEL)) = reg;
   *((uint32_t *)(base + IOWIN)) = val;
}

// ------------------------------------------------------------------------------------------------
static uint32_t IoApicIn(uint8_t *base, uint8_t reg)
{
    *((uint32_t *)(base + IOREGSEL)) = reg;
    return *((uint32_t *)(base + IOWIN));
}

// ------------------------------------------------------------------------------------------------
void IoApicSetEntry(uint8_t *base, uint8_t index, uint32_t data, uint32_t data2)
{
    IoApicOut(base, IOREDTBL + index * 2, (uint32_t)data);
    IoApicOut(base, IOREDTBL + index * 2 + 1, (uint32_t)(data2));
}

// ------------------------------------------------------------------------------------------------
void IoApicInit()
{
    // Get number of entries supported by the IO APIC
    uint32_t x = IoApicIn(g_ioApicAddr, IOAPICVER);
    uint32_t count = ((x >> 16) & 0xff) + 1;    // maximum redirection entry

    printf("I/O APIC pins = %d\n", count);

	printf("ioapicaddr: %X\n", g_ioApicAddr);

    // Disable all entries
    for (uint32_t i = 0; i < count; ++i)
    {
        IoApicSetEntry(g_ioApicAddr, i, 1 << 16, 0);
    }
//	IoApicSetEntry(g_ioApicAddr, remap(0), 32, 0);
	IoApicSetEntry(g_ioApicAddr, 2, 32, 0);
}
