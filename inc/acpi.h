#ifndef ACPI_H
#define ACPI_H

struct ACPISDTHeader {
  char Signature[4];
  uint32_t Length;
  uint8_t Revision;
  uint8_t Checksum;
  char OEMID[6];
  char OEMTableID[8];
  uint32_t OEMRevision;
  uint32_t CreatorID;
  uint32_t CreatorRevision;
} __attribute__ ((__packed__));

struct RSDT {
        struct ACPISDTHeader header;
        struct ACPISDTHeader *pointers[];
} __attribute__ ((__packed__));

struct RSDPDescriptor20 {
 char Signature[8];
 uint8_t Checksum;
 char OEMID[6];
 uint8_t Revision;
 struct RSDT *Rsdt;
 uint32_t Length;
 uint64_t XsdtAddress;
 uint8_t ExtendedChecksum;
 uint8_t reserved[3];
} __attribute__ ((__packed__));

extern void findrsdp();
extern void init_acpi();


#endif
