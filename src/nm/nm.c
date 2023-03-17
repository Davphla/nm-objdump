/*
** EPITECH PROJECT, 2022
** B-PSU-400-LYN-4-1-nmobjdump-david.gozlan
** File description:
** TODO: add description
*/

#include "nm.h"
#include <sys/stat.h>

SINGLETON(DumpElf64_t, {0}, dump)

static size_t get_file_size(const char *filepath)
{
    struct stat st_buf;

    if (lstat(filepath, &st_buf) == -1) {
        not_fatal(ERROR_FILE_FORMAT_NOT_RECOGNIZED, filepath);
        return 0;
    }
    return st_buf.st_size;
}

static void manage_header(const char *filepath, FILE *fd)
{
    size_t size = get_file_size(filepath);
    void *data[size];
    DumpElf64_t dumpelf;

    if (fread(data, size, 1, fd) != 1)
        return not_fatal(ERROR_FILE_FORMAT_NOT_RECOGNIZED, filepath);
    dumpelf.ehdr = (Elf64_Ehdr *)data;
    dumpelf.shdr = (Elf64_Shdr *)((char *)(data) + dumpelf.ehdr->e_shoff);
    dumpelf.sh_tab = (char *)(dumpelf.ehdr) +
    (size_t)((Elf64_Shdr *)(dumpelf.shdr + dumpelf.ehdr->e_shstrndx)
    ->sh_offset);
    *g_dumpelf = dumpelf;
    dump_section(&dumpelf);
}

// General function (For 32 / 64 bits)
static bool manage_ident(const char *filepath, FILE *fd, format_t *format)
{
    unsigned char e_ident[EI_NIDENT];

    if (fread(e_ident, EI_NIDENT, 1, fd) != 1 ||
    strncmp((char *)e_ident, ELFMAG, 4) != 0 ||
    e_ident[EI_CLASS] == ELFCLASSNONE || e_ident[EI_DATA] == ELFCLASSNONE) {
        not_fatal(ERROR_FILE_FORMAT_NOT_RECOGNIZED, filepath);
        return true;
    }
    memcpy(format, e_ident + EI_CLASS, 2);
    fseek(fd, 0, SEEK_SET);
    return false;
}

void dump_file(const char *filepath)
{
    FILE *fd = fopen(filepath, "r");
    format_t format = {0};

    if (fd == NULL)
        return not_fatal("nm: '%s': %s\n", filepath, strerror(errno));
    if (!manage_ident(filepath, fd, &format)) {
        if (format.architecture_format == ELFCLASS64) {
            manage_header(filepath, fd);
        }
    }
    fclose(fd);
}
