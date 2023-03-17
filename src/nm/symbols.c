/*
** EPITECH PROJECT, 2022
** B-PSU-400-LYN-4-1-nmobjdump-david.gozlan
** File description:
** TODO: add description
*/

#include "nm.h"

static Elf64_Shdr *get_section_symbol(const DumpElf64_t *dumpelf)
{
    for (size_t i = 0; i < dumpelf->ehdr->e_shnum; i++) {
        if (dumpelf->shdr[i].sh_type == SHT_SYMTAB) {
            return (&dumpelf->shdr[i]);
        }
    }
    return NULL;
}

static void dump_symbol(const Elf64_Sym *syms, const size_t size)
{
    DumpElf64_t *dumpelf = g_dumpelf;
    Elf64_Shdr *shdr = get_section_symbol(dumpelf);
    uint8_t *strtab =
    (uint8_t *)dumpelf->ehdr + dumpelf->shdr[shdr->sh_link].sh_offset;
    char *str;
    char c;

    for (size_t i = 0; i < size; i++) {
        str = (char *)&strtab[syms[i].st_name];
        if (!*str || strstr(str, ".o\0") || strstr(str, ".c\0"))
            continue;
        c = get_symbol_type(dumpelf->shdr, &syms[i]);
        if (syms[i].st_value || strcmp(str, "main") == 0) {
            printf("%016lx %c %s\n", syms[i].st_value, c, str);
        } else {
            printf("%*c %c %s\n", 16, ' ', c, str);
        }
    }
}

static char *remove_underscore(const char *string)
{
    while (*string == '_') {
        string += 1;
    }
    return (char *)string;
}

static int sort(const Elf64_Sym *a, const Elf64_Sym *b)
{
    DumpElf64_t dumpelf = *g_dumpelf;
    Elf64_Shdr *shdr = get_section_symbol(&dumpelf);
    uint8_t *strtab =
    (uint8_t *)dumpelf.ehdr + dumpelf.shdr[shdr->sh_link].sh_offset;
    char *first = remove_underscore((char *)&strtab[a->st_name]);
    char *second = remove_underscore((char *)&strtab[b->st_name]);

    if (a->st_value >= b->st_value && strcmp(first, second) == 0) {
        return 1;
    }
    return strcasecmp((first), (second));
}

void dump_section(const DumpElf64_t *dumpelf)
{
    Elf64_Shdr *shdr = get_section_symbol(dumpelf);
    size_t size = shdr->sh_size / shdr->sh_entsize;
    Elf64_Sym syms[size];

    memcpy(syms, (Elf64_Sym *)((char *)dumpelf->ehdr + shdr->sh_offset),
    shdr->sh_size);
    qsort(syms, size, shdr->sh_entsize, (__compar_fn_t)&sort);
    dump_symbol(syms, size);
}
