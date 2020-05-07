//
// Created by Dylan Brotherston on 16/4/20.
// test runner for Bits ADT
//


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "defs.h"
#include "bits.h"

int main(int argc, char **argv)
{
    assert(argc == 2);
    Bits bits[128] = {NULL};
    FILE *infile = fopen(argv[1], "r"); assert(infile);

    ssize_t length = 0;
    size_t  size   = 0;
    char   *line   = NULL;
    while ((length = getline(&line, &size, infile)) != -1) {
        if (strlen(line) < 1) continue;
        switch (line[0]) {
            case 'N': // N <Bits> <Length>
                bits[line[2]-0] = newBits(atoi(&line[4]));
                break;
            case 'F': // F <Bits>
                freeBits(bits[line[2]-0]);
                bits[line[2]-0] = NULL;
                break;
            case 's': // s <Bits> <Position>
                setBit(bits[line[2]-0], atoi(&line[4]));
                break;
            case 'S': // S <Bits>
                setAllBits(bits[line[2]-0]);
                break;
            case 'u': // u <Bits> <Position>
                unsetBit(bits[line[2]-0], atoi(&line[4]));
                break;
            case 'U': // U <Bits>
                unsetAllBits(bits[line[2]-0]);
                break;
            case 'a': // a <Bits> <Bits>
                andBits(bits[line[2]-0], bits[line[4]-0]);
                break;
            case 'o': // o <Bits> <Bits>
                orBits(bits[line[2]-0], bits[line[4]-0]);
                break;
            case 'O': // O <Bits>
                showBits(bits[line[2]-0]); putchar('\n');
                break;
            case 'i': // i <Bits> <Position>
            {
                Bool t = bitIsSet(bits[line[2]-0], atoi(&line[4]));
                printf("%s\n", t ? "True": "False");
                break;
            }
            case 'I': // I <Bits> <Bits>
            {
                Bool t = isSubset(bits[line[2]-0], bits[line[4]-0]);
                printf("%s\n", t ? "True": "False");
                break;
            }
            case '#': // # <comment>
            default:
                break;
        }
    }
    free(line);
}
