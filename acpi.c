#include "inc/common.h"
#include "inc/acpi.h"
#include "inc/descriptors.h"
#include "inc/ioapic.h"
#include "inc/apic.h"

int AcpiParseSDT(struct ACPISDTHeader *p)
{
	unsigned char sum = 0;
	for(int i = 0; i < p->Length; i++)
	{
		sum += ((char *) p)[i];
	}

	if(sum != 0)
	{
		printf("INVALID CHECKSUM");
		return 0;
	} else {
		//print_specific(p->Signature, 4);
		if(cmp(p->Signature, "APIC", 4))
		{

			void* temp_pointer = (void*)p;

//			uint32_t g_localApicAddr = temp_pointer + sizeof(struct ACPISDTHeader);

//			uint32_t compare = cpuGetAPICBase();
//			printf("Local apic: %x\n", (uint32_t)g_localApicAddr);
//			printf("Local apic compare: %x\n", (uint32_t)compare);
//			g_localApicAddr = (void *)compare;
//			cpuSetAPICBase((uint32_t)g_localApicAddr);
			uint8_t lapic_id = getApicId();
			//uint32_t flags = (uint32_t)*(p + sizeof(struct ACPISDTHeader) + 4);
			int offset = sizeof(struct ACPISDTHeader) + 8;
		//	print("\n");
			while((temp_pointer + offset) < (temp_pointer + p->Length))
			{
				//print("Type: ");
				uint8_t type = *((uint8_t *)(temp_pointer + offset));
				//print_dec(type);
				//print("\n");
				uint8_t length = *((uint8_t *)(temp_pointer+offset+1));
				if(type == 0)
				{
					uint8_t processor_id = *((uint8_t *)(temp_pointer+offset+2));
					uint8_t apic_id = *((uint8_t *)(temp_pointer+offset+3));
					printf("LOCAL APIC, pid: %d apicid: %d\n", processor_id, apic_id);
					if(lapic_id != apic_id)
					{
						printf("A cpu! yay!");
					} else {
					}

					// Send startup

//					uint32_t flags = *((uint32_t *)(temp_pointer+offset+4));
				} else if(type == 1)
				{
					printf("IO APIC\n");
					uint8_t ioapicid = *((uint8_t volatile *)(temp_pointer+offset+2));
					uint8_t reserved = *((uint8_t volatile *)(temp_pointer+offset+3));
					uint32_t ioapicaddr = *((uint32_t volatile *)(temp_pointer+offset+4));
					uint32_t gsib = *((uint32_t volatile *)(temp_pointer+offset+8));
					printf("Apic addr: %X, %X, %X, %X\n", ioapicid,reserved,ioapicaddr,gsib);
					g_ioApicAddr = (uint8_t*)*((uint32_t *)(temp_pointer+offset+4));
					IoApicInit();
				} else if(type == 2)
				{
					uint8_t bus = *((uint8_t *)(temp_pointer+offset+2));
					uint8_t source = *((uint8_t *)(temp_pointer+offset+3));
					uint32_t gsi = *((uint32_t *)(temp_pointer+offset+4));
					uint16_t flags = *((uint16_t *)(temp_pointer+offset+8));
					printf("Interrupt override. Bus: %d; Source: %d; GSI: %d; Flags: %d\n", bus, source, gsi, flags);
				} else {
					printf("Unknown, %d\n", type);
				}
				offset += length;
			}


			sleep(2);
		}
	}

	return 0;
}

int AcpiParseRsdp(void *p)
{
	unsigned char *p2 = p;
	unsigned char sum = 0;

	for(unsigned int i = 0; i < 20; ++i)
	{
		sum += p2[i];
	}

	if(sum)
	{
		printf("checksum failed");
		return 0;
	} else {
		printf("verified");
	}

	struct RSDPDescriptor20 *rs = p;

//	print_specific(rs->Rsdt->Signature, 4);

	if(cmp(rs->Rsdt->header.Signature, "RSDT", 4))
	{
		printf("%d", rs->Rsdt->header.Length);
		int entries = (rs->Rsdt->header.Length - sizeof(struct ACPISDTHeader)) / 4;
//		print_dec(entries);
//		print("\n");
		for(int i = 0; i < entries; i++)
		{
			struct ACPISDTHeader *ashp = rs->Rsdt->pointers[i];
			AcpiParseSDT(ashp);
			//print_specific(ashp->Signature, 4);
		}
		printf("Signature is rsdt");
	} else {
		printf("Signature is NOT rsdt");
	}

	return 1;
}


void findrsdp()
{
	unsigned char *p = (unsigned char *)0x000e0000;
	unsigned char *end = (unsigned char *)0x000fffff;

	while (p < end)
	{
		unsigned long long signature = *(unsigned long long *)p;

		if (signature == 0x2052545020445352) // 'RSD PTR '
		{
			printf("Found it! :D");
			if (AcpiParseRsdp(p))
            		{
				break;
			}
		}
		p += 16;
	}
}

void init_acpi()
{
	findrsdp();
}
