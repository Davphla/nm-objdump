/*
** EPITECH PROJECT, 2022
** B-PSU-400-LYN-4-1-nmobjdump-david.gozlan
** File description:
** TODO: add description
*/

#include "objdump.h"

static void print_content(
const void *ehdr, const size_t sh_addr, const size_t nb_element)
{
    char car;
    size_t j;

    printf("  ");
    for (j = 0; j < 16 && j < nb_element; j++) {
        car = *((char *)ehdr + sh_addr - 16 + j);
        printf("%c", car && isprint(car) ? car : '.');
    }
    printf("%*s", (int)(16 - (j % 17)), "");
}

static void print_inside(
const void *data, const void *ehdr, const Elf64_Shdr *shdr, const size_t i)
{
    if (i % 16 == 0) {
        if (i) {
            print_content(ehdr, shdr->sh_offset + i, 16);
            printf("\n");
        }
        printf(" %04lx ", shdr->sh_addr + i);
    } else if (i % 4 == 0) {
        printf(" ");
    }
    printf("%02hhx", *((char *)data + i));
}

static void print_end(const void *ehdr, const Elf64_Shdr *shdr, size_t i)
{
    size_t j = i;

    if (i % 16 != 0) {
        while (i % 16) {
            printf("%*s", (int)(2 + (i % 4 == 0)), "");
            i++;
        }
    }
    print_content(ehdr, shdr->sh_offset + i, j % 16 ?: 16);
}

static void print_section(
const void *data, const Elf64_Shdr *shdr, const void *ehdr)
{
    size_t i = 0;

    while (i < shdr->sh_size) {
        print_inside(data, ehdr, shdr, i);
        i++;
    }
    print_end(ehdr, shdr, i);
}

void dump_section_contents(const DumpElf64_t *dumpelf)
{
    Elf64_Shdr *shdr = dumpelf->shdr + 1;
    char *section_name;

    for (int i = 0; i < dumpelf->ehdr->e_shnum - 1; i++, shdr += 1) {
        section_name = (char *)dumpelf->sh_tab + shdr->sh_name;
        if (shdr->sh_flags == SHN_ABS || shdr->sh_flags == SHN_COMMON ||
        shdr->sh_flags == SHN_UNDEF || shdr->sh_flags == 64 ||
        shdr->sh_type == SHT_NOBITS || shdr->sh_size == 0)
            continue;
        printf("\nContents of section %s:\n", section_name);
        print_section(
        (char *)dumpelf->ehdr + shdr->sh_offset, shdr, dumpelf->ehdr);
    }
}
