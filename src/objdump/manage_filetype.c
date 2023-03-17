/*
** EPITECH PROJECT, 2022
** B-PSU-400-LYN-4-1-nmobjdump-david.gozlan
** File description:
** TODO: add description
*/

#include "objdump.h"
#include <ar.h>

static const char *FILE_FORMAT[] = {NULL, "elf32-i386", "elf64-x86-64"};

bool is_elf(const char *filepath, FILE *fd, format_t *format)
{
    unsigned char e_ident[EI_NIDENT];

    if (fread(e_ident, EI_NIDENT, 1, fd) != 1) {
        not_fatal(ERROR_FILE_FORMAT_NOT_RECOGNIZED, filepath);
        return false;
    }
    fseek(fd, -EI_NIDENT, SEEK_CUR);
    if (strncmp((char *)e_ident, ELFMAG, 4) != 0 ||
    e_ident[EI_CLASS] == ELFCLASSNONE || e_ident[EI_DATA] == ELFCLASSNONE) {
        not_fatal(ERROR_FILE_FORMAT_NOT_RECOGNIZED, filepath);
        return false;
    }
    memcpy(format, e_ident + EI_CLASS, 2);
    printf(
    "\n%s:     file format %s\n", filepath, FILE_FORMAT[e_ident[EI_CLASS]]);
    return true;
}

bool is_archive(FILE *fd)
{
    char ar_name[16];

    if (fread(ar_name, sizeof(ar_name), 1, fd) != 1)
        return false;
    if (strncmp((char *)ar_name, ARMAG, 8) != 0) {
        fseek(fd, 0, SEEK_SET);
        return false;
    }
    fseek(fd, 0, SEEK_SET);
    return true;
}
