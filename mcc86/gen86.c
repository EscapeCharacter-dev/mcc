#include "gen86.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

FILE *fHandle;
char **symbolTable;
int symbolCount;
int childIndex;

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

void x86wpf(int indentc, const char *format, ...)
{
    va_list vlist;
    va_start(vlist, format);
    char *fend = "\n";
    char *indent = "";
    for (int i = 0; i <= indentc; i++) concat(indent, "\t");
    char *past = concat(indent, format);
    char *format_ = concat(past, fend);
    vfprintf(fHandle, format_, vlist);
    va_end(vlist);
}

void x86wpfnln(int indentc, const char *format, ...)
{
    va_list vlist;
    va_start(vlist, format);
    char *indent = "";
    for (int i = 0; i <= indentc; i++) concat(indent, "\t");
    char *past = concat(indent, format);
    vfprintf(fHandle, past, vlist);
    va_end(vlist);
}

void x86SetupFile(const char *file)
{
    fHandle = fopen(file, "w");
    if (!fHandle) return 0;
    symbolTable = calloc(4096, sizeof(char*)); // the public symbol table. Excludes the static functions.
}

static void x86weax(int val) { x86wpf(1, "movl %%eax, %i", val); }
static void x86webx(int val) { x86wpf(1, "movl %%ebx, %i", val); }
static void x86wecx(int val) { x86wpf(1, "movl %%ecx, %i", val); }
static void x86weaxp(int val) { x86wpf(1, "movl %%eax, $%i", val); }
static void x86webxp(int val) { x86wpf(1, "movl %%ebx, $%i", val); }
static void x86wecxp(int val) { x86wpf(1, "movl %%ecx, $%i", val); }
static void x86wreg(int leftreg, int rightreg) { x86weax(leftreg); x86webx(rightreg); }
static void x86waddlnr() { x86wpf(1, "addl eax, ebx"); }
static void x86wsublnr() { x86wpf(1, "subl eax, ebx"); }
static void x86wmullnr() { x86wpf(1, "mull eax, ebx"); }
static void x86wdivlnr() { x86wpf(1, "divl eax, ebx"); }
static void x86wmodlnr() { x86wpf(1, "modl eax, ebx"); }
static void x86wcalln(const char *symbol) { x86wpf(1, "call %s", symbol); }
static void x86wjumpn(const char *symbol) { x86wpf(1, "jmp %s", symbol); }
static void x86wnop() { x86wpf(1, "nop"); }
static void x86wsect() { x86wpf(0, "section .text"); }
static void x86wsecd() { x86wpf(0, "section .data"); }
static void x86wsecr() { x86wpf(0, "section .rodata"); }
static void x86wsym(char *symbolName) { x86wpf(0, "%s:", symbolName); }
static void x86wfp(int saveStackSize) { x86wpf(1, "push ebp"); x86wpf(1, "mov ebp, esp");
                                        x86wpf(1, "sub esp, %i", saveStackSize); }
static void x86wfe() { x86wpf(1, "mov esp, ebp"); x86wpf(1, "pop ebx"); x86wpf(1, "ret"); }
static void x86wfl(char *label) { x86wpf(0, "global %s", label); x86wpf(0, "%s:", label); }
void x86waddl(int leftreg, int rightreg) { x86wreg(leftreg, rightreg); x86waddlnr(); }
void x86wsubl(int leftreg, int rightreg) { x86wreg(leftreg, rightreg); x86wsublnr(); }
void x86wmull(int leftreg, int rightreg) { x86wreg(leftreg, rightreg); x86wmullnr(); }
void x86wdivl(int leftreg, int rightreg) { x86wreg(leftreg, rightreg); x86wdivlnr(); }
void x86wmodl(int leftreg, int rightreg) { x86wreg(leftreg, rightreg); x86wmodlnr(); }
void x86wchildrenprototypeH() { x86wpf(0, ".C%i:", index); }
void x86wchildrenprototypeL() { x86wpf(1, "ret"); }
void x86wfuncl(const char *symbolName, int saveStackSize) { x86wfl(symbolName); x86wfp(saveStackSize); }