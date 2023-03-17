/*
** EPITECH PROJECT, 2022
** B-PSU-400-LYN-4-1-nmobjdump-david.gozlan
** File description:
** Main file
*/

#include "objdump.h"
#include <stdarg.h>
#include <stdlib.h>

SINGLETON(int, 0, global)

static void usage(const int status)
{
    puts(USAGE);
    exit(status);
}

static void get_flags(const int ac, char *const *av, flag_options_t *flags)
{
    int option;

    flags->dump_section_contents = true;
    flags->dump_file_header = true;
    while ((option = getopt(ac, av, "h")) != EOF) {
        switch (option) {
        case 'h': usage(EXIT_SUCCESS); break;
        default: usage(EXIT_ERROR); break;
        }
    }
}

static void main_objdump(const int ac, char *const *av)
{
    flag_options_t flags = {0};

    get_flags(ac, av, &flags);
    if (optind == ac) {
        dump_file(DEFAULT_FILE, flags);
    } else {
        for (int i = optind; i < ac; i++) {
            dump_file(av[i], flags);
        }
    }
}

void not_fatal(const char *error, ...)
{
    va_list va_args;

    va_start(va_args, error);
    *g_exit_status = 1;
    vfprintf(stderr, ERROR_FILE_FORMAT_NOT_RECOGNIZED, va_args);
}

int main(const int ac, char *const *av)
{
    main_objdump(ac, av);
    if (*g_exit_status)
        return 84;
    return 0;
}
