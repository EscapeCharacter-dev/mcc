#ifndef __FILE_H
#define __FILE_H

//reads the file and returns it in the form of a buffer.
char *readFileBuffer(const char *path);

//writes a file. Returns 1 if successful, returns 0 if failed.
int writeFileBuffer(const char *path, const char *buffer);

#endif