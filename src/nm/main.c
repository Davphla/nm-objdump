/*
** EPITECH PROJECT, 2022
** B-PSU-400-LYN-4-1-nmobjdump-david.gozlan
** File description:
** Main file
*/

#include "nm.h"

SINGLETON(int, 0, global)

static void usage(const int status)
{
    puts(USAGE);
    exit(status);
}

static void get_flags(const int ac, char *const *av)
{
    int option;

    while ((option = getopt(ac, av, "h")) != EOF) {
        switch (option) {
        case 'h': usage(EXIT_ERROR); break;
        default:
            not_fatal("Unrecognized option: %c\n", option);
            usage(EXIT_ERROR);
        }
    }
}

static void main_objdump(const int ac, char *const *av)
{
    get_flags(ac, av);
    if (optind == ac) {
        dump_file(DEFAULT_FILE);
    } else {
        for (int i = optind; i < ac; i++) {
            (ac > 2) ? printf("\n%s:\n", av[i]) : 0;
            dump_file(av[i]);
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
    if (ac == 1)
        usage(EXIT_SUCCESS);
    main_objdump(ac, av);
    if (*g_exit_status)
        return 84;
    return 0;
}
