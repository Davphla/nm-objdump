/*
** EPITECH PROJECT, 2022
** B-PSU-400-LYN-4-1-nmobjdump-david.gozlan
** File description:
** TODO: add description
*/

#include "nm.h"

static char get_global_symbol_type(const Elf64_Shdr *shdr,
const Elf64_Sym *sym)
{
    for (size_t i = 0; i < LEN(SIMPLE_TYPE); i++) {
        if (SIMPLE_TYPE[i].value == sym->st_shndx)
            return SIMPLE_TYPE[i].c;
    }
    for (size_t i = 0; i < LEN(COMPLEX_TYPE); i++) {
        if (COMPLEX_TYPE[i].type == shdr[sym->st_shndx].sh_type &&
        COMPLEX_TYPE[i].flags == shdr[sym->st_shndx].sh_flags)
            return COMPLEX_TYPE[i].c;
    }
    return '?';
}

char get_symbol_type(const Elf64_Shdr *shdr, const Elf64_Sym *sym)
{
    char c = get_global_symbol_type(shdr, sym);
    size_t bind = ELF64_ST_BIND(sym->st_info);

    if (bind == STB_GNU_UNIQUE)
        return 'u';
    if (bind == STB_WEAK) {
        if (ELF64_ST_TYPE(sym->st_info) == STT_OBJECT)
            return (c != 'U') ? 'V' : 'v';
        return (c != 'U') ? 'W' : 'w';
    }
    if (c != '?' && bind == STB_LOCAL)
        c += 32;
    return c;
}
