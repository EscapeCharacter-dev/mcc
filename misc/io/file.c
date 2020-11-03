#include "file.h"

#include <stdio.h>
#include <stdlib.h>

// macro that replaces fclose by a more sophisticated one
#define fclose(fp)  ((fp) ? fclose(fp) : 0, (fp) = 0)

char *readFileBuffer(const char *path)
{
    if (!path)
    {
        // oopsi
        fprintf(stderr, "[{INTERNAL ERROR}::{misc/io/file/readFileBuffer}] : Path pointer is zero (NULL)\n");
        abort();    // I'm a thug of programming
    }

    FILE *file = fopen(path, "r");

    if (!file)
    {
        fprintf(stderr, "Couldn't open path! File is probably not existing or already used.\n");
        exit(-1);
    }

    // going to the end of the file.
    fseek(file, 0, SEEK_END);

    // where are we relative to the start of the file?
    size_t len = ftell(file);

    // returns to the beginning
    fseek(file, 0, SEEK_SET);

    char *buffer = (char*) calloc(len, sizeof(char)); // just to be preventive, we do sizeof(char).

    if (!buffer)
    {
        fprintf(stderr, "Out of memory!\n");
        abort();
    }

    if (    // if fread fails
    !fread(buffer, sizeof(char) /*sry for that cmnt, preventive still*/,
          len, file
          )
    )
    {
        fprintf(stderr, "Couldn't read from file!\n");
        exit(-1);
    }

    // closing the file
    fclose(file);

    return buffer;
}

int writeFileBuffer(const char *path, const char *buffer)
{
    if ((!path) && (!buffer))
    {
        // oopsi
        fprintf(stderr, "[{INTERNAL ERROR}::{misc/io/file/readFileBuffer}] : Path pointer is zero (NULL)\n");
        abort();    // I'm a thug of programming
    }

    FILE *file = fopen(path, "w");

    if (!file)
    {
        fprintf(stderr, "Couldn't open path! File is probably not existing or already used.\n");
        exit(-1);
    }

    if (!fputs(buffer, file))
    {
        // ummm wait that's illegal
        fprintf(stderr, "Couldn't write to file!\n");
        exit(-1);
    }

    fclose(file);

    return 1;
}