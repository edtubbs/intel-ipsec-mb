/*****************************************************************************
 Copyright (c) 2018-2020, Intel Corporation

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

     * Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
     * Neither the name of Intel Corporation nor the names of its contributors
       may be used to endorse or promote products derived from this software
       without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "utils.h"

#ifdef _WIN32
#define snprintf _snprintf
#endif

/**
 * @brief Dumps fragment of memory in hex and ASCII into `fp`
 *
 * @param fp file stream to print into
 * @param msg optional extra header string message to print
 * @param p start address of data block to be dumped
 * @param len size of the data block to dump in bytes
 * @param start_ptr can be
 *          - pointer to data being dumped then first column of the dump will
 *            display addresses
 *          - NULL pointer then first column witll display indexes
 */

void
hexdump_ex(FILE *fp,
           const char *msg,
           const void *p,
           size_t len,
           const void *start_ptr)
{
        size_t ofs = 0;
        const unsigned char *data = p;
        const char *start = (const char *) start_ptr;

        if (msg != NULL)
                fprintf(fp, "%s\n", msg);

        while (ofs < len) {
                unsigned int i, out;
                char line[120];

                out = snprintf(line, sizeof(line), "%p:", &start[ofs]);

                for (i = 0; ((ofs + i) < len) && (i < 16); i++)
                        out += snprintf(line + out, sizeof(line) - out,
                                        " %02x", (data[ofs + i] & 0xff));

                for (; i <= 16; i++)
                        out += snprintf(line + out, sizeof(line) - out, " | ");

                for (i = 0; (ofs < len) && (i < 16); i++, ofs++) {
                        unsigned char c = data[ofs];

                        if (!isprint(c))
                                c = '.';
                        out += snprintf(line + out,
                                        sizeof(line) - out, "%c", c);
                }
                fprintf(fp, "%s\n", line);
        }
}

/* Simpler version of hexdump_ex() displaying data indexes only */
void
hexdump(FILE *fp,
        const char *msg,
        const void *p,
        size_t len)
{
        hexdump_ex(fp, msg, p, len, NULL);
}
