/* Link.ld -- Linker script for the kernel - ensure everything goes in the */
/*            Correct place.  */
/*            Original file taken from Bran's Kernel Development */
/*            tutorials: http://www.osdever.net/bkerndev/index.php. */

ENTRY(start)
OUTPUT_FORMAT(binary)
SECTIONS
{
    .text : AT(0x100000)
    {
	mboot_pointer = .;
	*(.mboot);
        code = .;
        *(.text);
        *(.rodata);
	code_end = .;
    }

    .data :
    {
        data = .;
        *(.data);
	kludge_start = .;
	*(.kludge);
	kludge_end = .;
	data_end = .;
    }

    .bss :
    {
        bss = .;
        *(.bss);
	*(.bootstrap_stack);
    }
    end = .;

}
