typedef struct {
 uint32_t p_type; (segment type)
 Elf32_Off p_offset; (segment offset)
 Elf32_Addr p_vaddr; (segment virtual address)
 Elf32_Addr p_paddr; (segment physical address)
 uint32_t p_filesz; (size of segment in the file)
 uint32_t p_memsz; (size of segment in memory)
 uint32_t p_flags; (segment flags, I.E execute|read|read)
 uint32_t p_align; (segment alignment in memory)
 } Elf32_Phdr;