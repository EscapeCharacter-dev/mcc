#include "../misc/parser.h"
#include <argp.h>
#include <stdio.h>
#include <stdlib.h>

const char *usage = "Usage: mcc86  -[ops] (output file) *(input files)";
const char *ops   = "k                    K&R C\n"
                    "w            More Warnings\n"
                    "W             All Warnings\n"
                    "y              No Warnings\n"
                    "i               Show Infos\n"
                    "I           Show All Infos\n"
                    "t          Total warn/info\n"
                    "l             To be linked\n"
                    "s           Don't assemble\n"
                    "e       Enables Exceptions\n"
                    "q Quiet mode (only fatals)\n"
                    "f             Freestanding\n"
                    "b            Binary output\n"
;

int main(int argc, char **argv)
{

    /*int k_r, wrnlvl = 1, infolvl, tblink, dontassemble, exceptions, quiet, freestanding, binary;

    int opt;

    while ((opt = getopt(argc, argv, "kwWyiIlseqfb")) != -1)
    {
        switch (opt)
        {
        case 'k': //K&R C
            k_r = 1;
        break;
        case 'w': //+warn
            wrnlvl = 2;
        break;
        case 'W': //++warn
            wrnlvl = 3;
        break;
        case 'y': //0warn
            wrnlvl = 0;
        break;
        case 'i': //info
            infolvl = 1;
        break;
        case 'I': //+info
            infolvl = 2;
        break;
        case 't': //all warns & infos
            wrnlvl = 3;
            infolvl = 2;
        break;
        case 'l': //to be linked
            tblink = 1;
        break;
        case 's': //don't assemble
            dontassemble = 1;
        break;
        case 'e': //enable built-in exceptions for debug purposes
            exceptions = 1;
        break;
        case 'q': // disable all warns, infos and errors. If errors occur, will just say 
                  //  "Compilation failed"
            wrnlvl = 0;
            infolvl = 0;
            quiet = 1;
        break;
        case 'f': //freestanding, without standard library or GNU C lib
            freestanding = 1;
        break;
        case 'b': //binary output (doesn't apply with don't assemble)
            binary = 1;
        break;
        default:
            printf("Unknown operator -%c!\n", opt);
            giveup();
        break;
        }
    }*/

    /*if (dontassemble && (binary || freestanding))
    {
        printf("Warning: -s doesn't assure -f or -b, because these are applied at assembler level.\n");
        printf("Still wanna continue [Y/y for yes, other chars afterwards will be ignored.]?\n");
        int c;
        scanf("%c", c);
        if (c != 'y' || c != 'Y') // if uses
        {
            giveup();
        }
    }

    char *outputPath;
    char **inputFiles = malloc(sizeofArgsOPTIND(argv, argc, 1));

    for (int i = optind; i < argc; i++) // looking at the output and input files
    {
        if (i == optind)
        {
            outputPath = argv[i];
            continue;
        }

        inputFiles[i] = argv[i];

    }

    */
    printf("Starting the compiler...\n");

    Token       *tokenbuf   = calloc(1, sizeof(Token));
    ParseTree   *tree       = calloc(1, sizeof(ParseTree));
    if (!tokenbuf || !tree)
    {
        puts("Not enough memory!\n");
        abort();
    }

    Tokenize(argv[0], tokenbuf);
    Parse(tokenbuf, tree);

    free(tokenbuf);
    free(tree);
}

size_t sizeofArgsOPTIND(char **argv, int argc, int offset)
{
    size_t size;
    for (int i = optind + offset; i < argc; i++)
    {
        size += sizeof(argv[i]);
    }
    return size;
}

//checks if an array contains a string.
int containsString(char **argv, int argc, char *str, int minimum)
{

    // thats illegal
    if (minimum >= argc)
        return 0;

    for (int i = minimum; i < argc; i++)
    {
        // does this equal?
        if (argv[i] == str)
            return 1;
    }

    // no, sadly
    return 0;
}

void giveup()
{
    printf("Aborted.\n");
    exit(0);
}