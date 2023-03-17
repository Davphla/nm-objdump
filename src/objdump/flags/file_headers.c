/*
** EPITECH PROJECT, 2022
** B-PSU-400-LYN-4-1-nmobjdump-david.gozlan
** File description:
** TODO: add description
*/

#include "objdump.h"

static void fill_flags(const size_t e_flags, const char **flags)
{
    for (size_t i = 0; i < sizeof(FLAGS) / sizeof(FLAGS[0]); i++) {
        if (FLAGS[i].value & e_flags) {
            *flags++ = FLAGS[i].key;
        }
    }
}

static void dump_flags(const flagword flag_value)
{
    const char *flags_key[sizeof(FLAGS) / sizeof(FLAGS[0])] = {NULL};

    fill_flags(flag_value, flags_key);
    for (size_t i = 0; flags_key[i]; i++) {
        if (i)
            printf(", ");
        printf("%s", flags_key[i]);
    }
    if (!flags_key[0]) {
        printf("%s", FLAGS[0].key);
    }
    printf("\n");
}

static flagword get_flag_value(const DumpElf64_t *dumpelf)
{
    flagword flag_value = 0;
    Elf64_Shdr *shdr = dumpelf->shdr;
    Elf64_Ehdr *ehdr = dumpelf->ehdr;

    flag_value += (ehdr->e_type == ET_REL) * HAS_RELOC;
    flag_value += (ehdr->e_type == ET_EXEC) * EXEC_P;
    flag_value += (ehdr->e_type == ET_DYN) * DYNAMIC;
    flag_value += (ehdr->e_phnum != 0) * D_PAGED;
    for (size_t i = 0; i < dumpelf->ehdr->e_shnum; i++) {
        if (!(flag_value & HAS_DEBUG) &&
        strcmp(dumpelf->sh_tab + shdr->sh_name, ".debug") == 0)
            flag_value += HAS_DEBUG;
        if (!(flag_value & HAS_SYMS) && shdr->sh_type == SHT_SYMTAB)
            flag_value += HAS_SYMS;
        if (!(flag_value & HAS_LINENO) &&
        strcmp(dumpelf->sh_tab + shdr->sh_name, ".line") == 0)
            flag_value += HAS_LINENO;
        shdr += 1;
    }
    return flag_value;
}

void dump_file_header(const DumpElf64_t *dumpelf)
{
    flagword flag_value = get_flag_value(dumpelf);

    printf("architecture: i386:x86-64, flags %#010x:\n", flag_value);
    dump_flags(flag_value);
    printf("start address 0x%016lx\n", dumpelf->ehdr->e_entry);
}
