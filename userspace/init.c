/* Copyright © 2018-2019 N. Van Bossuyt.                                      */
/* This code is licensed under the MIT License.                               */
/* See: LICENSE.md                                                            */

#include <skift/cstring.h>
#include <skift/iostream.h>

#include <skift/logger.h>
#include <skift/process.h>
#include <skift/messaging.h>
#include <skift/filesystem.h>

int init_exec(const char* filename)
{
    logger_log(LOG_INFO, "Starting '%s'", filename);
    int process = process_exec(filename, (const char*[]){filename, NULL});

    if (process < 0)
    {
        logger_log(LOG_WARNING, "Failed to start process: '%s'!", filename);
    }
    else
    {
        logger_log(LOG_FINE, "'%s' started with pid=%d !", filename, process);
    }

    return process;
}

int main(int argc, char **argv)
{
    (void)argc; (void)argv;

    logger_setlevel(LOG_ALL);

    printf("Welcome to \033[1;34mskiftOS\033[0m!\n");
    iostream_flush(out_stream);

    filesystem_mkfifo("/dev/term");
    init_exec("/bin/terminal");
    int shell = init_exec("/bin/sh");
    
    process_wait(shell, NULL);

    printf("\n\e[1;34mGoodbye!");

    return 0;
}