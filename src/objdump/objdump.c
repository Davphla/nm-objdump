/*
** EPITECH PROJECT, 2022
** B-PSU-400-LYN-4-1-nmobjdump-david.gozlan
** File description:
** TODO: add description
*/

#include "objdump.h"
#include <sys/stat.h>
#include <ar.h>

static void dump_flags(const DumpElf64_t *dumpelf, const flag_options_t flags)
{
    if (flags.dump_file_header) {
        dump_file_header(dumpelf);
    }
    if (flags.dump_section_contents) {
        dump_section_contents(dumpelf);
    }
}

static size_t get_file_size(const char *filepath)
{
    struct stat st_buf;

    if (lstat(filepath, &st_buf) == -1) {
        not_fatal(ERROR_FILE_FORMAT_NOT_RECOGNIZED, filepath);
        return 0;
    }
    return st_buf.st_size;
}

// Can be 32 or 64 bits
static void manage_header(
const char *filepath, const flag_options_t flags, const size_t size, FILE *fd)
{
    void *data[size];
    DumpElf64_t dumpelf;

    if (fread(data, size, 1, fd) != 1)
        return not_fatal(ERROR_FILE_FORMAT_NOT_RECOGNIZED, filepath);
    dumpelf.ehdr = (Elf64_Ehdr *)data;
    dumpelf.shdr = (Elf64_Shdr *)((char *)(data) + dumpelf.ehdr->e_shoff);
    dumpelf.sh_tab = (char *)(dumpelf.ehdr) + (size_t)((Elf64_Shdr *)
    (dumpelf.shdr + dumpelf.ehdr->e_shstrndx)->sh_offset);
    dump_flags(&dumpelf, flags);
    printf("\n");
}

static void manage_archive(
const char *filepath, const flag_options_t flags, FILE *fd)
{
    format_t format = {0};
    size_t size;
    struct ar_hdr hdr;
    char temp[11];

    printf("In archive %s:\n\n", filepath);
    while (fread(&hdr, sizeof(hdr), 1, fd) == 1) {
        snprintf(temp, 11, "%*s", 10, hdr.ar_size);
        size = strtoul(temp, NULL, 10);
        if (size && is_elf(filepath, fd, &format)) {
            manage_header(hdr.ar_name, flags, size, fd);
        }
    }
}

// TODO Manage 32 / 64 Bits
// TODO Manage LSB / BSB
void dump_file(const char *filepath, const flag_options_t flags)
{
    FILE *fd = fopen(filepath, "r");
    format_t format = {0};

    if (fd == NULL)
        return not_fatal("objdump: '%s': %s\n", filepath, strerror(errno));
    if (is_archive(fd)) {
        manage_archive(filepath, flags, fd);
    } else if (is_elf(filepath, fd, &format)) {
        (format.architecture_format == ELFCLASS64) ?
            manage_header(filepath, flags, get_file_size(filepath), fd)
        : 0;
    }
    fclose(fd);
}
