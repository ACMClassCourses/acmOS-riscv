/*
 * Copyright (c) 2020 Institute of Parallel And Distributed Systems (IPADS), Shanghai Jiao Tong
 * University (SJTU) OS-Lab-2020 (i.e., ChCore) is licensed under the Mulan PSL v1. You can use this
 * software according to the terms and conditions of the Mulan PSL v1. You may obtain a copy of
 * Mulan PSL v1 at: http://license.coscl.org.cn/MulanPSL THIS SOFTWARE IS PROVIDED ON AN "AS IS"
 * BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
 * NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE. See the Mulan PSL v1 for more
 * details.
 */

#include "elf.h"
#include "mm.h"

static bool is_elf_magic(struct elf_indent *indent) {
    return (indent->ei_magic[0] == 0x7F && indent->ei_magic[1] == 'E' &&
            indent->ei_magic[2] == 'L' && indent->ei_magic[3] == 'F');
}

#define ELF_ENDIAN_LE(indent) ((indent).ei_data == 1)
#define ELF_ENDIAN_BE(indent) ((indent).ei_data == 2)

#define ELF_BITS_32(indent) ((indent).ei_class == 1)
#define ELF_BITS_64(indent) ((indent).ei_class == 2)

/**
 * Parse an ELF file header. We use the 64-bit structure `struct elf_header` as
 * the output structure.
 *
 * On error, the negative error code is returned.
 * On success, 0 is returned, and the header is written in the given parameter.
 */
static int parse_elf_header(const char *code, struct elf_header *header) {
    struct elf_header *header_64 = (struct elf_header *)code;
    struct elf_header_32 *header_32 = (struct elf_header_32 *)code;

    if (!is_elf_magic(&header_64->e_indent)) {
        DEBUG("Invalid elf magic number: %x %x %x %x\n", header_64->e_indent.ei_magic[0],
               header_64->e_indent.ei_magic[1], header_64->e_indent.ei_magic[2],
               header_64->e_indent.ei_magic[3]);
        return -EINVAL;
    }

    header->e_indent = *(struct elf_indent *)code;

    if (ELF_ENDIAN_LE(header->e_indent)) {
        /*
         * For the first few bytes, both 32-bit and 64-bit ELF headers
         * have the same field width. So, we simply use header_64 at
         * first.
         */
        header->e_type = le16_to_cpu(header_64->e_type);
        header->e_machine = le16_to_cpu(header_64->e_machine);
        header->e_version = le32_to_cpu(header_32->e_version);
        if (ELF_BITS_32(header->e_indent)) {
            header->e_entry = le32_to_cpu(header_32->e_entry);
            header->e_phoff = le32_to_cpu(header_32->e_phoff);
            header->e_shoff = le32_to_cpu(header_32->e_shoff);
            header->e_flags = le32_to_cpu(header_32->e_flags);
            header->e_ehsize = le16_to_cpu(header_32->e_ehsize);
            header->e_phentsize = le16_to_cpu(header_32->e_phentsize);
            header->e_phnum = le16_to_cpu(header_32->e_phnum);
            header->e_shentsize = le16_to_cpu(header_32->e_shentsize);
            header->e_shnum = le16_to_cpu(header_32->e_shnum);
            header->e_shstrndx = le16_to_cpu(header_32->e_shstrndx);
        } else if (ELF_BITS_64(header->e_indent)) {
            header->e_entry = le64_to_cpu(header_64->e_entry);
            header->e_phoff = le64_to_cpu(header_64->e_phoff);
            header->e_shoff = le64_to_cpu(header_64->e_shoff);
            header->e_flags = le32_to_cpu(header_64->e_flags);
            header->e_ehsize = le16_to_cpu(header_64->e_ehsize);
            header->e_phentsize = le16_to_cpu(header_64->e_phentsize);
            header->e_phnum = le16_to_cpu(header_64->e_phnum);
            header->e_shentsize = le16_to_cpu(header_64->e_shentsize);
            header->e_shnum = le16_to_cpu(header_64->e_shnum);
            header->e_shstrndx = le16_to_cpu(header_64->e_shstrndx);
        } else {
            return -EINVAL;
        }
    } else if (ELF_ENDIAN_BE(header->e_indent)) {
        /*
         * We use header_64 for the same reason as above.
         */
        header->e_type = be16_to_cpu(header_64->e_type);
        header->e_machine = be16_to_cpu(header_64->e_machine);
        header->e_version = be32_to_cpu(header_32->e_version);
        if (ELF_BITS_32(header->e_indent)) {
            header->e_entry = be32_to_cpu(header_32->e_entry);
            header->e_phoff = be32_to_cpu(header_32->e_phoff);
            header->e_shoff = be32_to_cpu(header_32->e_shoff);
            header->e_flags = be32_to_cpu(header_32->e_flags);
            header->e_ehsize = be16_to_cpu(header_32->e_ehsize);
            header->e_phentsize = be16_to_cpu(header_32->e_phentsize);
            header->e_phnum = be16_to_cpu(header_32->e_phnum);
            header->e_shentsize = be16_to_cpu(header_32->e_shentsize);
            header->e_shnum = be16_to_cpu(header_32->e_shnum);
            header->e_shstrndx = be16_to_cpu(header_32->e_shstrndx);
        } else if (ELF_BITS_64(header->e_indent)) {
            header->e_entry = be64_to_cpu(header_64->e_entry);
            header->e_phoff = be64_to_cpu(header_64->e_phoff);
            header->e_shoff = be64_to_cpu(header_64->e_shoff);
            header->e_flags = be32_to_cpu(header_64->e_flags);
            header->e_ehsize = be16_to_cpu(header_64->e_ehsize);
            header->e_phentsize = be16_to_cpu(header_64->e_phentsize);
            header->e_phnum = be16_to_cpu(header_64->e_phnum);
            header->e_shentsize = be16_to_cpu(header_64->e_shentsize);
            header->e_shnum = be16_to_cpu(header_64->e_shnum);
            header->e_shstrndx = be16_to_cpu(header_64->e_shstrndx);
        } else {
            return -EINVAL;
        }
    } else {
        return -EINVAL;
    }
    return 0;
}

/**
 * Parse an ELF program header. We use the 64-bit structure
 * `struct elf_program_header` as the output structure.
 *
 * On error, the negative error code is returned.
 * On success, 0 is returned, and the header is written in the given parameter.
 */
static int parse_elf_program_header(const char *code, const struct elf_header *elf,
                                    struct elf_program_header *header) {
    struct elf_program_header *header_64;
    struct elf_program_header_32 *header_32;

    if (ELF_ENDIAN_LE(elf->e_indent)) {
        if (ELF_BITS_32(elf->e_indent)) {
            header_32 = (struct elf_program_header_32 *)code;
            header->p_type = le32_to_cpu(header_32->p_type);
            header->p_flags = le32_to_cpu(header_32->p_flags);
            header->p_offset = le32_to_cpu(header_32->p_offset);
            header->p_vaddr = le32_to_cpu(header_32->p_vaddr);
            header->p_paddr = le32_to_cpu(header_32->p_paddr);
            header->p_filesz = le32_to_cpu(header_32->p_filesz);
            header->p_memsz = le32_to_cpu(header_32->p_memsz);
            header->p_align = le32_to_cpu(header_32->p_align);
        } else if (ELF_BITS_64(elf->e_indent)) {
            header_64 = (struct elf_program_header *)code;
            header->p_type = le32_to_cpu(header_64->p_type);
            header->p_flags = le32_to_cpu(header_64->p_flags);
            header->p_offset = le64_to_cpu(header_64->p_offset);
            header->p_vaddr = le64_to_cpu(header_64->p_vaddr);
            header->p_paddr = le64_to_cpu(header_64->p_paddr);
            header->p_filesz = le64_to_cpu(header_64->p_filesz);
            header->p_memsz = le64_to_cpu(header_64->p_memsz);
            header->p_align = le64_to_cpu(header_64->p_align);
        } else {
            return -EINVAL;
        }
    } else if (ELF_ENDIAN_BE(elf->e_indent)) {
        if (ELF_BITS_32(elf->e_indent)) {
            header_32 = (struct elf_program_header_32 *)code;
            header->p_type = be32_to_cpu(header_32->p_type);
            header->p_flags = be32_to_cpu(header_32->p_flags);
            header->p_offset = be32_to_cpu(header_32->p_offset);
            header->p_vaddr = be32_to_cpu(header_32->p_vaddr);
            header->p_paddr = be32_to_cpu(header_32->p_paddr);
            header->p_filesz = be32_to_cpu(header_32->p_filesz);
            header->p_memsz = be32_to_cpu(header_32->p_memsz);
            header->p_align = be32_to_cpu(header_32->p_align);
        } else if (ELF_BITS_64(elf->e_indent)) {
            header_64 = (struct elf_program_header *)code;
            header->p_type = be32_to_cpu(header_64->p_type);
            header->p_flags = be32_to_cpu(header_64->p_flags);
            header->p_offset = be64_to_cpu(header_64->p_offset);
            header->p_vaddr = be64_to_cpu(header_64->p_vaddr);
            header->p_paddr = be64_to_cpu(header_64->p_paddr);
            header->p_filesz = be64_to_cpu(header_64->p_filesz);
            header->p_memsz = be64_to_cpu(header_64->p_memsz);
            header->p_align = be64_to_cpu(header_64->p_align);
        } else {
            return -EINVAL;
        }
    } else {
        return -EINVAL;
    }
    return 0;
}

/**
 * Parse an ELF section header. We use the 64-bit structure
 * `struct elf_section_header` as the output structure.
 *
 * On error, the negative error code is returned.
 * On success, 0 is returned, and the header is written in the given parameter.
 */
static int parse_elf_section_header(const char *code, const struct elf_header *elf,
                                    struct elf_section_header *header) {
    struct elf_section_header *header_64;
    struct elf_section_header_32 *header_32;

    if (ELF_ENDIAN_LE(elf->e_indent)) {
        if (ELF_BITS_32(elf->e_indent)) {
            header_32 = (struct elf_section_header_32 *)code;
            header->sh_name = le32_to_cpu(header_32->sh_name);
            header->sh_type = le32_to_cpu(header_32->sh_type);
            header->sh_flags = le32_to_cpu(header_32->sh_flags);
            header->sh_addr = le32_to_cpu(header_32->sh_addr);
            header->sh_offset = le32_to_cpu(header_32->sh_offset);
            header->sh_size = le32_to_cpu(header_32->sh_size);
            header->sh_link = le32_to_cpu(header_32->sh_link);
            header->sh_info = le32_to_cpu(header_32->sh_info);
            header->sh_addralign = le32_to_cpu(header_32->sh_addralign);
            header->sh_entsize = le32_to_cpu(header_32->sh_entsize);
        } else if (ELF_BITS_64(elf->e_indent)) {
            header_64 = (struct elf_section_header *)code;
            header->sh_name = le32_to_cpu(header_64->sh_name);
            header->sh_type = le32_to_cpu(header_64->sh_type);
            header->sh_flags = le64_to_cpu(header_64->sh_flags);
            header->sh_addr = le64_to_cpu(header_64->sh_addr);
            header->sh_offset = le64_to_cpu(header_64->sh_offset);
            header->sh_size = le64_to_cpu(header_64->sh_size);
            header->sh_link = le32_to_cpu(header_64->sh_link);
            header->sh_info = le32_to_cpu(header_64->sh_info);
            header->sh_addralign = le64_to_cpu(header_64->sh_addralign);
            header->sh_entsize = le64_to_cpu(header_64->sh_entsize);
        } else {
            return -EINVAL;
        }
    } else if (ELF_ENDIAN_BE(elf->e_indent)) {
        if (ELF_BITS_32(elf->e_indent)) {
            header_32 = (struct elf_section_header_32 *)code;
            header->sh_name = be32_to_cpu(header_32->sh_name);
            header->sh_type = be32_to_cpu(header_32->sh_type);
            header->sh_flags = be32_to_cpu(header_32->sh_flags);
            header->sh_addr = be32_to_cpu(header_32->sh_addr);
            header->sh_offset = be32_to_cpu(header_32->sh_offset);
            header->sh_size = be32_to_cpu(header_32->sh_size);
            header->sh_link = be32_to_cpu(header_32->sh_link);
            header->sh_info = be32_to_cpu(header_32->sh_info);
            header->sh_addralign = be32_to_cpu(header_32->sh_addralign);
            header->sh_entsize = be32_to_cpu(header_32->sh_entsize);
        } else if (ELF_BITS_64(elf->e_indent)) {
            header_64 = (struct elf_section_header *)code;
            header->sh_name = be32_to_cpu(header_64->sh_name);
            header->sh_type = be32_to_cpu(header_64->sh_type);
            header->sh_flags = be64_to_cpu(header_64->sh_flags);
            header->sh_addr = be64_to_cpu(header_64->sh_addr);
            header->sh_offset = be64_to_cpu(header_64->sh_offset);
            header->sh_size = be64_to_cpu(header_64->sh_size);
            header->sh_link = be32_to_cpu(header_64->sh_link);
            header->sh_info = be32_to_cpu(header_64->sh_info);
            header->sh_addralign = be64_to_cpu(header_64->sh_addralign);
            header->sh_entsize = be64_to_cpu(header_64->sh_entsize);
        } else {
            return -EINVAL;
        }
    } else {
        return -EINVAL;
    }
    return 0;
}

void elf_free(struct elf_file *elf) {
    if (elf->s_headers) mm_kfree(elf->s_headers);
    if (elf->p_headers) mm_kfree(elf->p_headers);
    mm_kfree(elf);
}

struct elf_file *elf_parse_file(const char *code) {
    struct elf_file *elf;
    int err;
    int i;

    elf = mm_kalloc(sizeof(*elf));
    if (!elf) return ERR_PTR(-ENOMEM);

    err = parse_elf_header(code, &elf->header);
    if (err) goto out_free_elf;

    /* Allocate memory for program headers and section headers */
    err = -ENOMEM;
    elf->p_headers = mm_kalloc(elf->header.e_phentsize * elf->header.e_phnum);
    if (!elf->p_headers) goto out_free_elf;
    elf->s_headers = mm_kalloc(elf->header.e_shentsize * elf->header.e_shnum);
    if (!elf->s_headers) goto out_free_elf_p;

    /* Parse program headers and section headers */
    for (i = 0; i < elf->header.e_phnum; ++i) {
        err = parse_elf_program_header(code + elf->header.e_phoff + elf->header.e_phentsize * i,
                                       &elf->header, &elf->p_headers[i]);
        if (err) goto out_free_all;
    }
    for (i = 0; i < elf->header.e_shnum; ++i) {
        err = parse_elf_section_header(code + elf->header.e_shoff + elf->header.e_shentsize * i,
                                       &elf->header, &elf->s_headers[i]);
        if (err) goto out_free_all;
    }
    return elf;

out_free_all:
    mm_kfree(elf->s_headers);
out_free_elf_p:
    mm_kfree(elf->p_headers);
out_free_elf:
    mm_kfree(elf);
    return ERR_PTR((long)err);
}

static void kprint_elf_pheader(const char *prefix, struct elf_program_header *pheader) {
    DEBUG("%s.p_type   = 0x%x\n", prefix, pheader->p_type);
    DEBUG("%s.p_flags  = 0x%x\n", prefix, pheader->p_flags);
    DEBUG("%s.p_offset = 0x%llx\n", prefix, pheader->p_offset);
    DEBUG("%s.p_vaddr  = 0x%llx\n", prefix, pheader->p_vaddr);
    DEBUG("%s.p_paddr  = 0x%llx\n", prefix, pheader->p_paddr);
    DEBUG("%s.p_filesz = 0x%llx\n", prefix, pheader->p_filesz);
    DEBUG("%s.p_memsz  = 0x%llx\n", prefix, pheader->p_memsz);
    DEBUG("%s.p_align  = 0x%llx\n", prefix, pheader->p_align);
}

static void kprint_elf_sheader(const char *prefix, struct elf_section_header *sheader) {
    DEBUG("%s.sh_name      = 0x%x\n", prefix, sheader->sh_name);
    DEBUG("%s.sh_type      = 0x%x\n", prefix, sheader->sh_type);
    DEBUG("%s.sh_flags     = 0x%llx\n", prefix, sheader->sh_flags);
    DEBUG("%s.sh_addr      = 0x%llx\n", prefix, sheader->sh_addr);
    DEBUG("%s.sh_offset    = 0x%llx\n", prefix, sheader->sh_offset);
    DEBUG("%s.sh_size      = 0x%llx\n", prefix, sheader->sh_size);
    DEBUG("%s.sh_link      = 0x%x\n", prefix, sheader->sh_link);
    DEBUG("%s.sh_info      = 0x%x\n", prefix, sheader->sh_info);
    DEBUG("%s.sh_addralign = 0x%llx\n", prefix, sheader->sh_addralign);
    DEBUG("%s.sh_entsize   = 0x%llx\n", prefix, sheader->sh_entsize);
}

__attribute__((unused)) static void kprint_elf(struct elf_file *elf) {
    int i;

    DEBUG("elf -> %p\n", elf);
    DEBUG(" .header\n");
    DEBUG("  .e_indent\n");
    DEBUG("   .ei_magic      = 0x%x 0x%x 0x%x 0x%x\n", elf->header.e_indent.ei_magic[0],
          elf->header.e_indent.ei_magic[1], elf->header.e_indent.ei_magic[2],
          elf->header.e_indent.ei_magic[3]);
    DEBUG("   .ei_class      = 0x%x\n", elf->header.e_indent.ei_class);
    DEBUG("   .ei_data       = 0x%x\n", elf->header.e_indent.ei_data);
    DEBUG("   .ei_version    = 0x%x\n", elf->header.e_indent.ei_version);
    DEBUG("   .ei_osabi      = 0x%x\n", elf->header.e_indent.ei_osabi);
    DEBUG("   .ei_abiversion = 0x%x\n", elf->header.e_indent.ei_abiversion);
    DEBUG("   .ei_pad        = 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n",
          elf->header.e_indent.ei_pad[0], elf->header.e_indent.ei_pad[1],
          elf->header.e_indent.ei_pad[2], elf->header.e_indent.ei_pad[3],
          elf->header.e_indent.ei_pad[4], elf->header.e_indent.ei_pad[5],
          elf->header.e_indent.ei_pad[6]);

    DEBUG("  .e_type      = 0x%x\n", elf->header.e_type);
    DEBUG("  .e_machine   = 0x%x\n", elf->header.e_machine);
    DEBUG("  .e_version   = 0x%x\n", elf->header.e_version);
    DEBUG("  .e_entry     = 0x%llx\n", elf->header.e_entry);
    DEBUG("  .e_phoff     = 0x%llx\n", elf->header.e_phoff);
    DEBUG("  .e_shoff     = 0x%llx\n", elf->header.e_shoff);
    DEBUG("  .e_flags     = 0x%x\n", elf->header.e_flags);
    DEBUG("  .e_ehsize    = 0x%x\n", elf->header.e_ehsize);
    DEBUG("  .e_phentsize = 0x%x\n", elf->header.e_phentsize);
    DEBUG("  .e_phnum     = 0x%x\n", elf->header.e_phnum);
    DEBUG("  .e_shentsize = 0x%x\n", elf->header.e_shentsize);
    DEBUG("  .e_shnum     = 0x%x\n", elf->header.e_shnum);
    DEBUG("  .e_shstrndx  = 0x%x\n", elf->header.e_shstrndx);

    for (i = 0; i < elf->header.e_phnum; ++i) {
        DEBUG(" .pheader[%d]\n", i);
        kprint_elf_pheader("  ", &elf->p_headers[i]);
    }

    for (i = 0; i < elf->header.e_shnum; ++i) {
        DEBUG(" .sheader[%d]\n", i);
        kprint_elf_sheader("  ", &elf->s_headers[i]);
    }
}
