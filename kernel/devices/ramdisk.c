/* Copyright © 2018-2019 N. Van Bossuyt.                                      */
/* This code is licensed under the MIT License.                               */
/* See: LICENSE.md                                                            */

#include <skift/math.h>
#include <skift/cstring.h>
#include <skift/tar.h>
#include <skift/logger.h>

#include "kernel/filesystem.h"
#include "kernel/multiboot.h"

void ramdiload(multiboot_module_t *module)
{
    // Extract the ramdisk tar archive.

    logger_log(LOG_INFO, "Loading ramdisk at 0x%x...", module->mod_start);

    void *ramdisk = (void *)module->mod_start;

    tar_block_t block;
    for (size_t i = 0; tar_read(ramdisk, &block, i); i++)
    {
        path_t* file_path = path(block.name);

        if (block.name[strlen(block.name) - 1] == '/')
        {
            if (filesystem_mkdir(ROOT, file_path) != 0)
            {
                logger_log(LOG_WARNING, "Failed to create directory %s...", block.name);
            }
        }
        else
        {
            stream_t *s = filesystem_open(ROOT, file_path, IOSTREAM_WRITE | IOSTREAM_CREATE | IOSTREAM_TRUNC);
            
            if (s != NULL)
            {
                filesystem_write(s, block.data, block.size);
                filesystem_close(s);
            }
            else
            {
                logger_log(LOG_WARNING, "Failed to open file %s!", block.name);
            }
        }

        path_delete(file_path);
    }

    logger_log(LOG_FINE, "Loading ramdisk succeeded.");
}