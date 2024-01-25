/*
 * Copyright (C) 2024 Armands Arseniuss Skolmeisters <arseniuss@arseniuss.id.lv>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>

#include <flos/utils.h>

static int is_short_name(int index) {
    return (index >= 'a' && index <= 'z') || (index >= 'A' && index <= 'Z') || (index >= '0' && index <= '9');
}

// Left-shifts array elements by 1, moving the first element to the end.
static void shift(char *argv[]) {
    char *first_element = argv[0];
    while (argv[1] != NULL) {
        argv[0] = argv[1];
        argv++;
    }
    argv[0] = first_element;
}

utf8_char utils_getopt(int *argc, char **argv[], char **optarg, const char *opts) {
    if (*argc <= 0 || argv == NULL || *argv == NULL || **argv == NULL || optarg == NULL || opts == NULL) {
        goto finished;
    }

    if (*optarg) {
        *optarg = NULL;
    }

    // advance parsing
    if (*argc > 0) {
        (*argv)++;
        (*argc)--;
    }

    if (*argc == 0 || **argv == NULL) {
        goto finished;
    }

    char *argp; // pointer used to probe the command line arguments

start:

    argp = **argv;

    if (*argp == '-') { // option
        argp++;
        if (*argp == '-') { // long option
            argp++;

            // handle "end of options" (--)
            if (*argp == '\0') {
                (*argv)++;
                (*argc)--;

                // shift all remaining operands
                if ((*argv)[*argc] != NULL) {
                    while (*argc) {
                        shift(*argv);
                        (*argc)--;
                    }
                }

                goto finished;
            }

            *optarg = argp + 1;

            return 2;
        } else if (*argp == '\0') { // a single '-'

            // shall return -1 without changing optind

            return -1;
        } else {
            const char *opt = opts;

            int optional = *opt == ':' ? (opt++, 0) : 1;

            while (*opt) {
                if (*opt != ':' && *opt == *argp) {
                    if (*(argp + 1) == '\0') { // no characters are attached
                        if (!optional && *(opt + 1) == ':') {
                            (*argv)++;
                            (*argc)--;

                            if (*argc == 0 || (*optarg = **argv) == NULL) {
                                fprintf(stderr, "Option -%c requires an argument.\n", *argp);
                                *optarg = argp;
                                return ':';
                            }
                        }
                    } else { //
                        if (!optional && *(opt + 1) == ':') {
                            *optarg = argp + 1;
                        } else {
                            if (!is_short_name(*(argp + 1))) {
                                fprintf(stderr, "Option -%c doesn't allow an argument.\n", *argp);
                                return '?';
                            }

                            *argp = '-';
                            **argv = argp; // scan here again next round
                            (*argv)--;
                            (*argc)++;
                        }
                    }
                    return *opt;
                }
                opt++;
            }

            *optarg = argp;

            fprintf(stderr, "Unknown option: -%c\n", *argp); // TODO: utf8

            return '?';
        }
    } else {
        // Move operand to the end of argv[] and hide it for now.
        shift(*argv);
        (*argc)--; // Hide it.

        if (*argc == 0) {
            goto finished;
        }

        goto start;
    }

finished:
    // Unhide previously hidden operands
    while ((*argv)[*argc] != NULL) {
        (*argc)++;
    }

    return 0;
}
