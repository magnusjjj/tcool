OUTPUT_FORMAT("binary")
ENTRY(start)

phys = 0x00100000;
SECTIONS
{
  kernel_addr_start = .;
  .text phys : AT(phys) {
    code = .;
    *(.text)
    *(.rodata)
    . = ALIGN(4096);
  }
  .data : AT(phys + (data - code))
  {
    data = .;
    *(.data);
	kludge_start = .;
	*(.kludge);
	kludge_end = .;
    . = ALIGN(4096);
  }
  .bss : AT(phys + (bss - code))
  {
    bss = .;
    *(.bss)
    . = ALIGN(4096);
  }
  kernel_addr_end = .;
}
