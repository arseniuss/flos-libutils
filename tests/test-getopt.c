/*
 * Copyright (C) 2009-2018 Free Software Foundation, Inc.
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

/**
 * Test examples are taken from https://github.com/digitalocean/gnulib/blob/master/tests/test-getopt.h
 */

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <flos/utf8.h>
#include <flos/utils.h>

#include "tap.h"

#define STRINGIZE(x)  STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define LINE_STRING   STRINGIZE(__LINE__)

#define ASSERT(x)     ((x) ? pass("") : fail("assert(" #x ") " __FILE__ ":" LINE_STRING))

#define OPTIND_MIN    1

int optind = 0;

static void getopt_loop(int argc, char **argv, const char *options, int *a_seen, int *b_seen, const char **p_value,
                        const char **q_value, int *non_options_count, const char **non_options, int *unrecognized,
                        bool *message_issued) {
    utf8_char c;
    char *optarg;

    while ((c = utils_getopt(&argc, &argv, &optarg, options))) {
        switch (c) {
        case 'a':
            (*a_seen)++;
            break;
        case 'b':
            (*b_seen)++;
            break;
        case 'p':
            *p_value = optarg;
            break;
        case 'q':
            *q_value = optarg;
            break;
        case '\1':
            /* Must only happen with option '-' at the beginning.  */
            assert(options[0] == '-');
            non_options[(*non_options_count)++] = optarg;
            break;
        case ':':
            /* Must only happen with option ':' at the beginning.  */
            assert(options[0] == ':');
        case '?':
            *unrecognized = *optarg; // TODO: utf8
            break;
        default:
            *unrecognized = c;
            break;
        }
    }

    optind = argc;
}

static void test_getopt(void) {
    int start;

    /* Test processing of boolean options.  */
    for (start = OPTIND_MIN; start <= 1; start++) {
        int a_seen = 0;
        int b_seen = 0;
        const char *p_value = NULL;
        const char *q_value = NULL;
        int non_options_count = 0;
        const char *non_options[10];
        int unrecognized = 0;
        bool output;
        int argc = 0;
        char *argv[10];

        argv[argc++] = strdup("program");
        argv[argc++] = strdup("-a");
        argv[argc++] = strdup("foo");
        argv[argc++] = strdup("bar");
        argv[argc] = NULL;
        optind = start;
        getopt_loop(argc, argv, "ab", &a_seen, &b_seen, &p_value, &q_value, &non_options_count, non_options,
                    &unrecognized, &output);
        ASSERT(a_seen == 1);
        ASSERT(b_seen == 0);
        ASSERT(p_value == NULL);
        ASSERT(q_value == NULL);
        ASSERT(non_options_count == 0);
        ASSERT(unrecognized == 0);
        ASSERT(optind == 2);
        ASSERT(!output);
    }
    for (start = OPTIND_MIN; start <= 1; start++) {
        int a_seen = 0;
        int b_seen = 0;
        const char *p_value = NULL;
        const char *q_value = NULL;
        int non_options_count = 0;
        const char *non_options[10];
        int unrecognized = 0;
        bool output;
        int argc = 0;
        char *argv[10];

        argv[argc++] = strdup("program");
        argv[argc++] = strdup("-b");
        argv[argc++] = strdup("-a");
        argv[argc++] = strdup("foo");
        argv[argc++] = strdup("bar");
        argv[argc] = NULL;
        optind = start;
        getopt_loop(argc, argv, "ab", &a_seen, &b_seen, &p_value, &q_value, &non_options_count, non_options,
                    &unrecognized, &output);
        ASSERT(a_seen == 1);
        ASSERT(b_seen == 1);
        ASSERT(p_value == NULL);
        ASSERT(q_value == NULL);
        ASSERT(non_options_count == 0);
        ASSERT(unrecognized == 0);
        ASSERT(optind == 3);
        ASSERT(!output);
    }
    for (start = OPTIND_MIN; start <= 1; start++) {
        int a_seen = 0;
        int b_seen = 0;
        const char *p_value = NULL;
        const char *q_value = NULL;
        int non_options_count = 0;
        const char *non_options[10];
        int unrecognized = 0;
        bool output;
        int argc = 0;
        char *argv[10];

        argv[argc++] = strdup("program");
        argv[argc++] = strdup("-ba");
        argv[argc++] = strdup("foo");
        argv[argc++] = strdup("bar");
        argv[argc] = NULL;
        optind = start;
        getopt_loop(argc, argv, "ab", &a_seen, &b_seen, &p_value, &q_value, &non_options_count, non_options,
                    &unrecognized, &output);
        ASSERT(a_seen == 1);
        ASSERT(b_seen == 1);
        ASSERT(p_value == NULL);
        ASSERT(q_value == NULL);
        ASSERT(non_options_count == 0);
        ASSERT(unrecognized == 0);
        ASSERT(optind == 2);
        ASSERT(!output);
    }
    for (start = OPTIND_MIN; start <= 1; start++) {
        int a_seen = 0;
        int b_seen = 0;
        const char *p_value = NULL;
        const char *q_value = NULL;
        int non_options_count = 0;
        const char *non_options[10];
        int unrecognized = 0;
        bool output;
        int argc = 0;
        char *argv[10];

        argv[argc++] = strdup("program");
        argv[argc++] = strdup("-ab");
        argv[argc++] = strdup("-a");
        argv[argc++] = strdup("foo");
        argv[argc++] = strdup("bar");
        argv[argc] = NULL;
        optind = start;
        getopt_loop(argc, argv, "ab", &a_seen, &b_seen, &p_value, &q_value, &non_options_count, non_options,
                    &unrecognized, &output);
        ASSERT(a_seen == 2);
        ASSERT(b_seen == 1);
        ASSERT(p_value == NULL);
        ASSERT(q_value == NULL);
        ASSERT(non_options_count == 0);
        ASSERT(unrecognized == 0);
        ASSERT(optind == 3);
        ASSERT(!output);
    }

    /* Test processing of options with arguments.  */
    for (start = OPTIND_MIN; start <= 1; start++) {
        int a_seen = 0;
        int b_seen = 0;
        const char *p_value = NULL;
        const char *q_value = NULL;
        int non_options_count = 0;
        const char *non_options[10];
        int unrecognized = 0;
        bool output;
        int argc = 0;
        char *argv[10];

        argv[argc++] = strdup("program");
        argv[argc++] = strdup("-pfoo");
        argv[argc++] = strdup("bar");
        argv[argc] = NULL;
        optind = start;
        getopt_loop(argc, argv, "p:q:", &a_seen, &b_seen, &p_value, &q_value, &non_options_count, non_options,
                    &unrecognized, &output);
        ASSERT(a_seen == 0);
        ASSERT(b_seen == 0);
        ASSERT(p_value != NULL && strcmp(p_value, "foo") == 0);
        ASSERT(q_value == NULL);
        ASSERT(non_options_count == 0);
        ASSERT(unrecognized == 0);
        ASSERT(optind == 2);
        ASSERT(!output);
    }
    for (start = OPTIND_MIN; start <= 1; start++) {
        int a_seen = 0;
        int b_seen = 0;
        const char *p_value = NULL;
        const char *q_value = NULL;
        int non_options_count = 0;
        const char *non_options[10];
        int unrecognized = 0;
        bool output;
        int argc = 0;
        char *argv[10];

        argv[argc++] = strdup("program");
        argv[argc++] = strdup("-p");
        argv[argc++] = strdup("foo");
        argv[argc++] = strdup("bar");
        argv[argc] = NULL;
        optind = start;
        getopt_loop(argc, argv, "p:q:", &a_seen, &b_seen, &p_value, &q_value, &non_options_count, non_options,
                    &unrecognized, &output);
        ASSERT(a_seen == 0);
        ASSERT(b_seen == 0);
        ASSERT(p_value != NULL && strcmp(p_value, "foo") == 0);
        ASSERT(q_value == NULL);
        ASSERT(non_options_count == 0);
        ASSERT(unrecognized == 0);
        ASSERT(optind == 3);
        ASSERT(!output);
    }
    for (start = OPTIND_MIN; start <= 1; start++) {
        int a_seen = 0;
        int b_seen = 0;
        const char *p_value = NULL;
        const char *q_value = NULL;
        int non_options_count = 0;
        const char *non_options[10];
        int unrecognized = 0;
        bool output;
        int argc = 0;
        char *argv[10];

        argv[argc++] = strdup("program");
        argv[argc++] = strdup("-ab");
        argv[argc++] = strdup("-q");
        argv[argc++] = strdup("baz");
        argv[argc++] = strdup("-pfoo");
        argv[argc++] = strdup("bar");
        argv[argc] = NULL;
        optind = start;
        getopt_loop(argc, argv, "abp:q:", &a_seen, &b_seen, &p_value, &q_value, &non_options_count, non_options,
                    &unrecognized, &output);
        ASSERT(a_seen == 1);
        ASSERT(b_seen == 1);
        ASSERT(p_value != NULL && strcmp(p_value, "foo") == 0);
        ASSERT(q_value != NULL && strcmp(q_value, "baz") == 0);
        ASSERT(non_options_count == 0);
        ASSERT(unrecognized == 0);
        ASSERT(optind == 5);
        ASSERT(!output);
    }

    /* Check that invalid options are recognized; and that both opterr
       and leading ':' can silence output.  */
    for (start = OPTIND_MIN; start <= 1; start++) {
        int a_seen = 0;
        int b_seen = 0;
        const char *p_value = NULL;
        const char *q_value = NULL;
        int non_options_count = 0;
        const char *non_options[10];
        int unrecognized = 0;
        bool output;
        int argc = 0;
        char *argv[10];

        argv[argc++] = strdup("program");
        argv[argc++] = strdup("-p");
        argv[argc++] = strdup("foo");
        argv[argc++] = strdup("-x");
        argv[argc++] = strdup("-a");
        argv[argc++] = strdup("bar");
        argv[argc] = NULL;
        optind = start;
        getopt_loop(argc, argv, "abp:q:", &a_seen, &b_seen, &p_value, &q_value, &non_options_count, non_options,
                    &unrecognized, &output);
        ASSERT(a_seen == 1);
        ASSERT(b_seen == 0);
        ASSERT(p_value != NULL && strcmp(p_value, "foo") == 0);
        ASSERT(q_value == NULL);
        ASSERT(non_options_count == 0);
        ASSERT(unrecognized == 'x');
        ASSERT(optind == 5);
        ASSERT(output);
    }
    for (start = OPTIND_MIN; start <= 1; start++) {
        int a_seen = 0;
        int b_seen = 0;
        const char *p_value = NULL;
        const char *q_value = NULL;
        int non_options_count = 0;
        const char *non_options[10];
        int unrecognized = 0;
        bool output;
        int argc = 0;
        char *argv[10];

        argv[argc++] = strdup("program");
        argv[argc++] = strdup("-p");
        argv[argc++] = strdup("foo");
        argv[argc++] = strdup("-x");
        argv[argc++] = strdup("-a");
        argv[argc++] = strdup("bar");
        argv[argc] = NULL;
        optind = start;
        getopt_loop(argc, argv, "abp:q:", &a_seen, &b_seen, &p_value, &q_value, &non_options_count, non_options,
                    &unrecognized, &output);
        ASSERT(a_seen == 1);
        ASSERT(b_seen == 0);
        ASSERT(p_value != NULL && strcmp(p_value, "foo") == 0);
        ASSERT(q_value == NULL);
        ASSERT(non_options_count == 0);
        ASSERT(unrecognized == 'x');
        ASSERT(optind == 5);
        ASSERT(!output);
    }
    for (start = OPTIND_MIN; start <= 1; start++) {
        int a_seen = 0;
        int b_seen = 0;
        const char *p_value = NULL;
        const char *q_value = NULL;
        int non_options_count = 0;
        const char *non_options[10];
        int unrecognized = 0;
        bool output;
        int argc = 0;
        char *argv[10];

        argv[argc++] = strdup("program");
        argv[argc++] = strdup("-p");
        argv[argc++] = strdup("foo");
        argv[argc++] = strdup("-x");
        argv[argc++] = strdup("-a");
        argv[argc++] = strdup("bar");
        argv[argc] = NULL;
        optind = start;

        getopt_loop(argc, argv, ":abp:q:", &a_seen, &b_seen, &p_value, &q_value, &non_options_count, non_options,
                    &unrecognized, &output);
        ASSERT(a_seen == 1);
        ASSERT(b_seen == 0);
        ASSERT(p_value != NULL && strcmp(p_value, "foo") == 0);
        ASSERT(q_value == NULL);
        ASSERT(non_options_count == 0);
        ASSERT(unrecognized == 'x');
        ASSERT(optind == 5);
        ASSERT(!output);
    }
    for (start = OPTIND_MIN; start <= 1; start++) {
        int a_seen = 0;
        int b_seen = 0;
        const char *p_value = NULL;
        const char *q_value = NULL;
        int non_options_count = 0;
        const char *non_options[10];
        int unrecognized = 0;
        bool output;
        int argc = 0;
        char *argv[10];

        argv[argc++] = strdup("program");
        argv[argc++] = strdup("-p");
        argv[argc++] = strdup("foo");
        argv[argc++] = strdup("-:");
        argv[argc++] = strdup("-a");
        argv[argc++] = strdup("bar");
        argv[argc] = NULL;
        optind = start;
        getopt_loop(argc, argv, "abp:q:", &a_seen, &b_seen, &p_value, &q_value, &non_options_count, non_options,
                    &unrecognized, &output);
        ASSERT(a_seen == 1);
        ASSERT(b_seen == 0);
        ASSERT(p_value != NULL && strcmp(p_value, "foo") == 0);
        ASSERT(q_value == NULL);
        ASSERT(non_options_count == 0);
        ASSERT(unrecognized == ':');
        ASSERT(optind == 5);
        ASSERT(output);
    }
    for (start = OPTIND_MIN; start <= 1; start++) {
        int a_seen = 0;
        int b_seen = 0;
        const char *p_value = NULL;
        const char *q_value = NULL;
        int non_options_count = 0;
        const char *non_options[10];
        int unrecognized = 0;
        bool output;
        int argc = 0;
        char *argv[10];

        argv[argc++] = strdup("program");
        argv[argc++] = strdup("-p");
        argv[argc++] = strdup("foo");
        argv[argc++] = strdup("-:");
        argv[argc++] = strdup("-a");
        argv[argc++] = strdup("bar");
        argv[argc] = NULL;
        optind = start;
        getopt_loop(argc, argv, "abp:q:", &a_seen, &b_seen, &p_value, &q_value, &non_options_count, non_options,
                    &unrecognized, &output);
        ASSERT(a_seen == 1);
        ASSERT(b_seen == 0);
        ASSERT(p_value != NULL && strcmp(p_value, "foo") == 0);
        ASSERT(q_value == NULL);
        ASSERT(non_options_count == 0);
        ASSERT(unrecognized == ':');
        ASSERT(optind == 5);
        ASSERT(!output);
    }
    for (start = OPTIND_MIN; start <= 1; start++) {
        int a_seen = 0;
        int b_seen = 0;
        const char *p_value = NULL;
        const char *q_value = NULL;
        int non_options_count = 0;
        const char *non_options[10];
        int unrecognized = 0;
        bool output;
        int argc = 0;
        char *argv[10];

        argv[argc++] = strdup("program");
        argv[argc++] = strdup("-p");
        argv[argc++] = strdup("foo");
        argv[argc++] = strdup("-:");
        argv[argc++] = strdup("-a");
        argv[argc++] = strdup("bar");
        argv[argc] = NULL;
        optind = start;
        getopt_loop(argc, argv, ":abp:q:", &a_seen, &b_seen, &p_value, &q_value, &non_options_count, non_options,
                    &unrecognized, &output);
        ASSERT(a_seen == 1);
        ASSERT(b_seen == 0);
        ASSERT(p_value != NULL && strcmp(p_value, "foo") == 0);
        ASSERT(q_value == NULL);
        ASSERT(non_options_count == 0);
        ASSERT(unrecognized == ':');
        ASSERT(optind == 5);
        ASSERT(!output);
    }

    /* Check for missing argument behavior.  */
    for (start = OPTIND_MIN; start <= 1; start++) {
        int a_seen = 0;
        int b_seen = 0;
        const char *p_value = NULL;
        const char *q_value = NULL;
        int non_options_count = 0;
        const char *non_options[10];
        int unrecognized = 0;
        bool output;
        int argc = 0;
        char *argv[10];

        argv[argc++] = strdup("program");
        argv[argc++] = strdup("-ap");
        argv[argc] = NULL;
        optind = start;
        getopt_loop(argc, argv, "abp:q:", &a_seen, &b_seen, &p_value, &q_value, &non_options_count, non_options,
                    &unrecognized, &output);
        ASSERT(a_seen == 1);
        ASSERT(b_seen == 0);
        ASSERT(p_value == NULL);
        ASSERT(q_value == NULL);
        ASSERT(non_options_count == 0);
        ASSERT(unrecognized == 'p');
        ASSERT(optind == 2);
        ASSERT(output);
    }
    for (start = OPTIND_MIN; start <= 1; start++) {
        int a_seen = 0;
        int b_seen = 0;
        const char *p_value = NULL;
        const char *q_value = NULL;
        int non_options_count = 0;
        const char *non_options[10];
        int unrecognized = 0;
        bool output;
        int argc = 0;
        char *argv[10];

        argv[argc++] = strdup("program");
        argv[argc++] = strdup("-ap");
        argv[argc] = NULL;
        optind = start;
        getopt_loop(argc, argv, "abp:q:", &a_seen, &b_seen, &p_value, &q_value, &non_options_count, non_options,
                    &unrecognized, &output);
        ASSERT(a_seen == 1);
        ASSERT(b_seen == 0);
        ASSERT(p_value == NULL);
        ASSERT(q_value == NULL);
        ASSERT(non_options_count == 0);
        ASSERT(unrecognized == 'p');
        ASSERT(optind == 2);
        ASSERT(!output);
    }
    for (start = OPTIND_MIN; start <= 1; start++) {
        int a_seen = 0;
        int b_seen = 0;
        const char *p_value = NULL;
        const char *q_value = NULL;
        int non_options_count = 0;
        const char *non_options[10];
        int unrecognized = 0;
        bool output;
        int argc = 0;
        char *argv[10];

        argv[argc++] = strdup("program");
        argv[argc++] = strdup("-ap");
        argv[argc] = NULL;
        optind = start;
        getopt_loop(argc, argv, ":abp:q:", &a_seen, &b_seen, &p_value, &q_value, &non_options_count, non_options,
                    &unrecognized, &output);
        ASSERT(a_seen == 1);
        ASSERT(b_seen == 0);
        ASSERT(p_value == NULL);
        ASSERT(q_value == NULL);
        ASSERT(non_options_count == 0);
        ASSERT(unrecognized == 'p');
        ASSERT(optind == 2);
        ASSERT(!output);
    }

    /* Check that by default, non-options arguments are moved to the end.  */
    for (start = OPTIND_MIN; start <= 1; start++) {
        int a_seen = 0;
        int b_seen = 0;
        const char *p_value = NULL;
        const char *q_value = NULL;
        int non_options_count = 0;
        const char *non_options[10];
        int unrecognized = 0;
        bool output;
        int argc = 0;
        char *argv[10];

        argv[argc++] = strdup("program");
        argv[argc++] = strdup("donald");
        argv[argc++] = strdup("-p");
        argv[argc++] = strdup("billy");
        argv[argc++] = strdup("duck");
        argv[argc++] = strdup("-a");
        argv[argc++] = strdup("bar");
        argv[argc] = NULL;
        optind = start;

        getopt_loop(argc, argv, "abp:q:", &a_seen, &b_seen, &p_value, &q_value, &non_options_count, non_options,
                    &unrecognized, &output);

        ASSERT(strcmp(argv[0], "program") == 0);
        ASSERT(strcmp(argv[1], "donald") == 0);
        ASSERT(strcmp(argv[2], "-p") == 0);
        ASSERT(strcmp(argv[3], "billy") == 0);
        ASSERT(strcmp(argv[4], "duck") == 0);
        ASSERT(strcmp(argv[5], "-a") == 0);
        ASSERT(strcmp(argv[6], "bar") == 0);
        ASSERT(argv[7] == NULL);
        ASSERT(a_seen == 0);
        ASSERT(b_seen == 0);
        ASSERT(p_value == NULL);
        ASSERT(q_value == NULL);
        ASSERT(non_options_count == 0);
        ASSERT(unrecognized == 0);
        ASSERT(optind == 1);
        ASSERT(!output);
    }

    /* Check that '--' ends the argument processing.  */
    for (start = OPTIND_MIN; start <= 1; start++) {
        int a_seen = 0;
        int b_seen = 0;
        const char *p_value = NULL;
        const char *q_value = NULL;
        int non_options_count = 0;
        const char *non_options[10];
        int unrecognized = 0;
        bool output;
        int argc = 0;
        char *argv[20];

        argv[argc++] = strdup("program");
        argv[argc++] = strdup("donald");
        argv[argc++] = strdup("-p");
        argv[argc++] = strdup("billy");
        argv[argc++] = strdup("duck");
        argv[argc++] = strdup("-a");
        argv[argc++] = strdup("--");
        argv[argc++] = strdup("-b");
        argv[argc++] = strdup("foo");
        argv[argc++] = strdup("-q");
        argv[argc++] = strdup("johnny");
        argv[argc++] = strdup("bar");
        argv[argc] = NULL;
        optind = start;

        getopt_loop(argc, argv, "abp:q:", &a_seen, &b_seen, &p_value, &q_value, &non_options_count, non_options,
                    &unrecognized, &output);
        ASSERT(strcmp(argv[0], "program") == 0);
        ASSERT(strcmp(argv[1], "donald") == 0);
        ASSERT(strcmp(argv[2], "-p") == 0);
        ASSERT(strcmp(argv[3], "billy") == 0);
        ASSERT(strcmp(argv[4], "duck") == 0);
        ASSERT(strcmp(argv[5], "-a") == 0);
        ASSERT(strcmp(argv[6], "--") == 0);
        ASSERT(strcmp(argv[7], "-b") == 0);
        ASSERT(strcmp(argv[8], "foo") == 0);
        ASSERT(strcmp(argv[9], "-q") == 0);
        ASSERT(strcmp(argv[10], "johnny") == 0);
        ASSERT(strcmp(argv[11], "bar") == 0);
        ASSERT(argv[12] == NULL);
        ASSERT(a_seen == 0);
        ASSERT(b_seen == 0);
        ASSERT(p_value == NULL);
        ASSERT(q_value == NULL);
        ASSERT(non_options_count == 0);
        ASSERT(unrecognized == 0);
        ASSERT(optind == 1);
        ASSERT(!output);
    }

    /* Check that the '+' flag has to come first.  */
    for (start = OPTIND_MIN; start <= 1; start++) {
        int a_seen = 0;
        int b_seen = 0;
        const char *p_value = NULL;
        const char *q_value = NULL;
        int non_options_count = 0;
        const char *non_options[10];
        int unrecognized = 0;
        bool output;
        int argc = 0;
        char *argv[10];

        argv[argc++] = strdup("program");
        argv[argc++] = strdup("donald");
        argv[argc++] = strdup("-p");
        argv[argc++] = strdup("billy");
        argv[argc++] = strdup("duck");
        argv[argc++] = strdup("-a");
        argv[argc++] = strdup("bar");
        argv[argc] = NULL;
        optind = start;

        getopt_loop(argc, argv, "abp:q:+", &a_seen, &b_seen, &p_value, &q_value, &non_options_count, non_options,
                    &unrecognized, &output);

        ASSERT(strcmp(argv[0], "program") == 0);
        ASSERT(strcmp(argv[1], "donald") == 0);
        ASSERT(strcmp(argv[2], "-p") == 0);
        ASSERT(strcmp(argv[3], "billy") == 0);
        ASSERT(strcmp(argv[4], "duck") == 0);
        ASSERT(strcmp(argv[5], "-a") == 0);
        ASSERT(strcmp(argv[6], "bar") == 0);
        ASSERT(argv[7] == NULL);
        ASSERT(a_seen == 0);
        ASSERT(b_seen == 0);
        ASSERT(p_value == NULL);
        ASSERT(q_value == NULL);
        ASSERT(non_options_count == 0);
        ASSERT(unrecognized == 0);
        ASSERT(optind == 1);
        ASSERT(!output);
    }
}

#define BACKUP_STDERR_FILENO 10
#define TEST_GETOPT_TMP_NAME "test-getopt.tmp"

static FILE *myerr;

int main(void) {
    plan(181);

    if (dup2(STDERR_FILENO, BACKUP_STDERR_FILENO) != BACKUP_STDERR_FILENO ||
        (myerr = fdopen(BACKUP_STDERR_FILENO, "w")) == NULL) {
        return 2;
    }

    assert(freopen(TEST_GETOPT_TMP_NAME, "w", stderr) == stderr);

    test_getopt();

    assert(fclose(stderr) == 0);
    assert(remove(TEST_GETOPT_TMP_NAME) == 0);

    return 0;
}
