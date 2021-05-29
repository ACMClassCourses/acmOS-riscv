/*
 * ELF format according to
 * https://en.wikipedia.org/wiki/Executable_and_Linkable_Format
 */

#include <defs.h>

#define EI_MAG_SIZE 4

#define PT_NULL 0x00000000
#define PT_LOAD 0x00000001
#define PT_DYNAMIC 0x00000002
#define PT_INTERP 0x00000003
#define PT_NOTE 0x00000004
#define PT_SHLIB 0x00000005
#define PT_PHDR 0x00000006
#define PT_LOOS 0x60000000
#define PT_HIOS 0x6fffffff
#define PT_LOPROC 0x70000000
#define PT_HIRPOC 0x7fffffff

#define PF_ALL 0x7
#define PF_X 0x1
#define PF_W 0x2
#define PF_R 0x4

#define EBUSY           -1      /* Busy */
#define EPERM            1      /* Operation not permitted */
#define EAGAIN           2      /* Try again */
#define ENOMEM           3      /* Out of memory */
#define EACCES           4      /* Permission denied */
#define EINVAL           5      /* Invalid argument */
#define EFBIG            6      /* File too large */
#define ENOSPC           7      /* No space left on device */
#define ENOSYS           8      /* Function not implemented */
#define ENODATA          9      /* No data available */
#define ETIME           10      /* Timer expired */
#define ECAPBILITY      11      /* Invalid capability */
#define ESUPPORT        12      /* Not supported */
#define EBADSYSCALL     13      /* Bad syscall number */
#define ENOMAPPING      14      /* Bad memory mapping */
#define ENOENT          15      /* Entry does not exist */
#define EEXIST          16      /* Entry already exists */
#define ENOTEMPTY       17      /* Dir is not empty */
#define ENOTDIR         18      /* Does not refer to a directory */
#define	EFAULT		19	/* Bad address */

#define EMAX            20


#define ERR_PTR(x) ((void *)(sint64)(x))
#define PTR_ERR(x) ((long)(x))
#define IS_ERR(x) ((((sint64)(x)) < 0) && ((sint64)(x)) > -EMAX)



#define le16_to_cpu(x) (x)
#define le32_to_cpu(x) (x)
#define le64_to_cpu(x) (x)

#define be16_to_cpu(x) ((((x) & 0xff) << 8) | (((x) >> 8) & 0xff))
#define be32_to_cpu(x) ((be16_to_cpu((x)) << 16) | (be16_to_cpu((x) >> 16)))
#define be64_to_cpu(x) ((be32_to_cpu((x)) << 32) | (be32_to_cpu((x) >> 32)))


#define be128ptr_to_cpu_hi(x) (be64_to_cpu(*(u64 *)(x)))
#define be128ptr_to_cpu_lo(x) (be64_to_cpu(*((u64 *)(x) + 1)))

#define be96ptr_to_cpu_hi(x) (be32_to_cpu(*(u32 *)(x)))
#define be96ptr_to_cpu_lo(x) (((u64)(be32_to_cpu(*((u32 *)(x) + 1)))) << 32 | \
			      (be32_to_cpu(*((u32 *)(x)) + 2)))

#define beXptr_to_cpu(bits, n) \
	({ \
	 (bits == 32) ? be32_to_cpu(*(u32 *)(n)) : \
	 (bits == 64) ? be64_to_cpu(*(u64 *)(n)) : \
	 ({ BUG("invalid X"); 0; }); \
	 })

#define set_beXptr_to_cpu(bits, n, hi, lo) do {     \
	if (bits > 64) {                            \
		if (bits == 96) {                   \
			lo = be96ptr_to_cpu_lo(n);  \
			hi = be96ptr_to_cpu_hi(n);  \
		} else if (bits == 128) {           \
			lo = be128ptr_to_cpu_lo(n); \
			hi = be128ptr_to_cpu_hi(n); \
		} else {                            \
			BUG("invalid X");           \
		}                                   \
	} else {                                    \
		lo = 0;                             \
		hi = beXptr_to_cpu(bits, n);        \
	}                                           \
} while (0)


/*
 * This part of ELF header is endianness-independent.
 */
struct elf_indent {
    uint8 ei_magic[4];
    uint8 ei_class;
    uint8 ei_data;
    uint8 ei_version;
    uint8 ei_osabi;
    uint8 ei_abiversion;
    uint8 ei_pad[7];
};

/*
 * ELF header format. One should check the `e_indent` to decide the endianness.
 */
struct elf_header {
    struct elf_indent e_indent;
    uint16 e_type;
    uint16 e_machine;
    uint32 e_version;
    uint64 e_entry;
    uint64 e_phoff;
    uint64 e_shoff;
    uint32 e_flags;
    uint16 e_ehsize;
    uint16 e_phentsize; /* The size of a program header table entry */
    uint16 e_phnum;     /* The number of entries in the program header table */
    uint16 e_shentsize; /* The size of a section header table entry */
    uint16 e_shnum;     /* The number of entries in the section header table */
    uint16 e_shstrndx;  /* Index of the section header table entry that
                            contains the section names. */
};

/*
 * 32-Bit of the elf_header. Check the `e_indent` first to decide.
 */
struct elf_header_32 {
    struct elf_indent e_indent;
    uint16 e_type;
    uint16 e_machine;
    uint32 e_version;
    uint32 e_entry;
    uint32 e_phoff;
    uint32 e_shoff;
    uint32 e_flags;
    uint16 e_ehsize;
    uint16 e_phentsize;
    uint16 e_phnum;
    uint16 e_shentsize;
    uint16 e_shnum;
    uint16 e_shstrndx;
};

struct elf_program_header {
    uint32 p_type;
    uint32 p_flags;
    uint64 p_offset;
    uint64 p_vaddr;
    uint64 p_paddr;
    uint64 p_filesz;
    uint64 p_memsz;
    uint64 p_align;
};
struct elf_program_header_32 {
    uint32 p_type;
    uint32 p_offset;
    uint32 p_vaddr;
    uint32 p_paddr;
    uint32 p_filesz;
    uint32 p_memsz;
    uint32 p_flags;
    uint32 p_align;
};

struct elf_section_header {
    uint32 sh_name;
    uint32 sh_type;
    uint64 sh_flags;
    uint64 sh_addr;
    uint64 sh_offset;
    uint64 sh_size;
    uint32 sh_link;
    uint32 sh_info;
    uint64 sh_addralign;
    uint64 sh_entsize;
};
struct elf_section_header_32 {
    uint32 sh_name;
    uint32 sh_type;
    uint32 sh_flags;
    uint32 sh_addr;
    uint32 sh_offset;
    uint32 sh_size;
    uint32 sh_link;
    uint32 sh_info;
    uint32 sh_addralign;
    uint32 sh_entsize;
};

struct elf_file {
    struct elf_header header;
    struct elf_program_header *p_headers;
    struct elf_section_header *s_headers;
};

struct elf_file *elf_parse_file(const char *code);
void elf_free(struct elf_file *elf);