/*
** EPITECH PROJECT, 2022
** B-PSU-400-LYN-4-1-nmobjdump-david.gozlan
** File description:
** TODO: add description
*/

#ifndef B_PSU_400_LYN_4_1_NMOBJDUMP_DAVID_GOZLAN_OBJDUMP_H
    #define B_PSU_400_LYN_4_1_NMOBJDUMP_DAVID_GOZLAN_OBJDUMP_H

    #include <elf.h>
    #include "flags.h"

    #include <ctype.h>
    #include <errno.h>
    #include <malloc.h>
    #include <stdbool.h>
    #include <stdio.h>
    #include <string.h>
    #include <unistd.h>
    #include <stdlib.h>

// Debug
    #define EXIT_ERROR 84

    #define ERROR_FILE_FORMAT_NOT_RECOGNIZED "objdump: %s: "                \
        "file format not recognized\n"

// Structures
typedef struct flag_options_s {
    bool dump_file_header;
    bool dump_section_contents;
} flag_options_t;

typedef struct format_s {
    char architecture_format;
    char endian_format;
} format_t;

typedef struct DumpElf64_s {
    Elf64_Ehdr *ehdr;
    Elf64_Shdr *shdr;
    const char *sh_tab;
} DumpElf64_t;

    #define SINGLETON(t, init, name)                                          \
        t *s_##name()                                                         \
        {                                                                     \
            static t inst = init;                                             \
            return &inst;                                                     \
        }
    #define g_exit_status s_global()

// Prototype
void not_fatal(const char *error, ...);

bool is_elf(const char *filepath, FILE *fd, format_t *format);
bool is_archive(FILE *fd);

void dump_file(const char *filepath, flag_options_t flags);
void dump_file_header(const DumpElf64_t *);
void dump_section_contents(const DumpElf64_t *);

// Macro
    #define USAGE                                                             \
    "Usage: %s <option(s)> <file(s)>\n"                                       \
    "Display information from object <file(s)>.\n"                            \
    "   -f       Display the contents of the overall file header\n"           \
    "   -s       Display the full contents of all sections requested"

    #define DEFAULT_FILE "a.out"

static const struct flag_s {
    size_t value;
    char key[32];
} FLAGS[] = {{BFD_NO_FLAGS, "BFD_NO_FLAGS"}, {HAS_RELOC, "HAS_RELOC"},
    {EXEC_P, "EXEC_P"}, {HAS_LINENO, "HAS_LINENO"}, {HAS_DEBUG, "HAS_DEBUG"},
    {HAS_SYMS, "HAS_SYMS"}, {HAS_LOCALS, "HAS_LOCALS"}, {DYNAMIC, "DYNAMIC"},
    {WP_TEXT, "WP_TEXT"}, {D_PAGED, "D_PAGED"}};

#endif // B_PSU_400_LYN_4_1_NMOBJDUMP_DAVID_GOZLAN_OBJDUMP_H
