/* Burt P. <pburt0@gmail.com>
 *
 *
 * References:
 * https://www.kingston.com/us/memory/memory-part-number-decoder
 * https://www.hyperxgaming.com/us/decoder
 *
 * */

#include <glib.h>
#include <stdio.h>
#define _GNU_SOURCE
#include <string.h>
#include <ctype.h>

typedef struct {
    gchar *part_number; /* copy */

    const gchar *vendor;
    const gchar *product;

    const gchar *drma_vendor;
    const gchar *reg_vendor;

    //...

} mem_part_data;

mem_part_data *mem_part_new(const char *part_number) {
    mem_part_data *p = g_new0(mem_part_data, 1);

    char c[20][4] = {};
    int mc;

    mc = sscanf(part_number,"KSM%2[0-9]%1[ERL]%1[SDQ]%1[48]%1[L]/%[0-9]%1[HM]%1[ABE]%1[IMR]%[0-9]",
            c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7], c[8], c[9] );
    if (mc != 10)
    mc = sscanf(part_number,"KSM%2[0-9]%2[SE]%1[SDQ]%1[48]%1[L]/%[0-9]%1[HM]%1[ABE]%1[IMR]%[0-9]",
            c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7], c[8], c[9] );
    if (mc == 10) {
        p->vendor = "Kingston"; p->product = "Server Premier DDR4";
    }

//KVR%2[0-9]%1[LU]%1[ENRLS]%2[0-9]%1[SDQ]%1[48]%1[LH]K%1[234]/%1[4]
//KVR%2[0-9]%1[ENRLS]%2[0-9]%1[SDQ]%1[48]%1[LH]K%1[234]/%1[4]

    return p;
}

void mem_part_fee(mem_part_data *p) {
    if (p) {
        g_free(p->part_number);
    }
    g_free(p);
}

// gcc -Wall -o mpd mem_part_decode.c -DMPTEST `pkg-config --cflags glib-2.0` -lglib-2.0
#if defined(MPTEST)

const char *test[] = {
    "M471A5244CB0-CTD",
    "CT8G4SFS8266.M8FE",
    "KSM26RD4L/32HAI2",
    "KSM26SED4L/32HAI2",
    "KVR21LR15D8LK2/4HBI",
    "KVR16LR11D8LK2/4HB",
    "KVR1066D3LD8R7SLK2/46HB",
    "KVR400X72RC3AK2/1G",
    "HX429C15PB3AK2/32",
    NULL
};

int main() {
    int i = 0;
    while(test[i]) {
        mem_part_data *p = mem_part_new(test[i]);
        printf("%s: %s %s\n", test[i], p->vendor, p->product);
        mem_part_fee(p);
        i++;
    }
    return 0;
}
#endif
