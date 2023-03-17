/*
** EPITECH PROJECT, 2022
** B-PSU-400-LYN-4-1-nmobjdump-david.gozlan
** File description:
** TODO: add description
*/

#ifndef B_PSU_400_LYN_4_1_NMOBJDUMP_DAVID_GOZLAN_NM_H
    #define B_PSU_400_LYN_4_1_NMOBJDUMP_DAVID_GOZLAN_NM_H

    #include <ctype.h>
    #include <elf.h>
    #include <errno.h>
    #include <malloc.h>
    #include <stdarg.h>
    #include <stdbool.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>

// Structures
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

    #define LEN(a) (sizeof(a) / sizeof(a)[0])

void not_fatal(const char *error, ...);
void dump_file(const char *filepath);
void dump_section(const DumpElf64_t *dumpelf);
char get_symbol_type(const Elf64_Shdr *shdr, const Elf64_Sym *sym);

// Singleton
    DumpElf64_t *s_dump(void);
    #define g_dumpelf s_dump()

    int *s_global(void);
    #define g_exit_status s_global()

    #define DEFAULT_FILE "a.out"

    #define EXIT_ERROR 84

    #define ERROR_FILE_FORMAT_NOT_RECOGNIZED                                  \
    "nm: %s: file format not recognized\n"

    #define USAGE                                                             \
    "Usage: nm [file(s)]\n"                                                   \
    "List symbols in [file(s)] (a.out by default).\n"                         \
    "No options are available."

static const struct {
    char c;
    int value;
} SIMPLE_TYPE[] = {{'U', SHN_UNDEF}, {'A', SHN_ABS}, {'C', SHN_COMMON}};

static const struct COMPLEX_TYPE {
    char c;
    size_t type;
    size_t flags;
} COMPLEX_TYPE[] = {{'B', SHT_NOBITS, (SHF_ALLOC | SHF_WRITE)},
    {'D', SHT_PROGBITS, (SHF_ALLOC | SHF_WRITE)},
    {'D', SHT_DYNAMIC, (SHF_ALLOC | SHF_WRITE)},
    {'D', SHT_INIT_ARRAY, (SHF_ALLOC | SHF_WRITE)},
    {'D', SHT_FINI_ARRAY, (SHF_ALLOC | SHF_WRITE)},
    {'R', SHT_PROGBITS, SHF_ALLOC},
    {'R', SHT_NOTE, SHF_ALLOC},
    {'T', SHT_PROGBITS, (SHF_ALLOC | SHF_EXECINSTR)}};


#endif // B_PSU_400_LYN_4_1_NMOBJDUMP_DAVID_GOZLAN_NM_H
