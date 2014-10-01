#ifndef IOAPIC_H
#define IOAPIC_H
// ------------------------------------------------------------------------------------------------
// Globals
uint8_t *g_ioApicAddr;

extern void IoApicInit();

// ------------------------------------------------------------------------------------------------
// Memory mapped registers for IO APIC register access
#define IOREGSEL                        0x00
#define IOWIN                           0x10

// ------------------------------------------------------------------------------------------------
// IO APIC Registers
#define IOAPICID                        0x00
#define IOAPICVER                       0x01
#define IOAPICARB                       0x02
#define IOREDTBL                        0x10	
#endif
