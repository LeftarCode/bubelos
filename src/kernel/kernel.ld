ENTRY(kmain)
KERNEL_VMA = 0xffffffff80000000;
SECTIONS
{
    . = KERNEL_VMA;

	.text KERNEL_VMA : AT(KERNEL_VMA) {
		code = .;
		*(.text)
		*(.rodata)
		. = ALIGN(4096);
	}
    
	.data : AT(KERNEL_VMA + (data - code))
	{
		data = .;
		*(.data)
		. = ALIGN(4096);
	}
    
	.bss : AT(KERNEL_VMA + (bss - code))
	{
		bss = .;
		*(.bss)
		. = ALIGN(4096);
	}
    
	end = .;
    
	/DISCARD/ :
	{
		*(.comment)
		*(.eh_frame)
		*(.note.gnu.build-id)
	}
}