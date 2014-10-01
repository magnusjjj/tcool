#ifndef APIC_H
#define APIC_H
	#define APIC_ADDR 0xFEE00000
	extern void init_apic();
	extern void apic_eoi();
	extern uint8_t getApicId();


	static inline uint32_t apic_read(uint32_t the_register)
	{
	        return *((volatile uint32_t volatile *)(APIC_ADDR + the_register));
	}

	static inline void apic_write(uint32_t the_register, uint32_t value)
	{
	        *((volatile uint32_t volatile *)(APIC_ADDR + the_register)) = value;
	}

	static inline void apic_wait()
	{
		while(apic_read(0x310) & 0x1000);
	}

#endif
