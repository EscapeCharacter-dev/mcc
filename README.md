# Magic C Compiler 🧙
The Magic C Compiler, not really magic.
## About
The Magic C Compiler (mcc) is a handwritten C compiler (no bison, etc.)  
It targets the following architectures:  
  - i386 (32-bit, x86)  
  - AMD64 (64-bit, x64)  
  - ARM32 (32-bit for ARM)  
  - ARM64 (64-bit for ARM)  
  - *maybe* i8086 (16-bit)  
## Platform
It is written to support all platforms (Linux, Windows, BSD, Darwin (MacOS), Popcorn, ...)  
None of them is currently supported. 😥
## Why another compiler
Writing compilers is fun.
## Building?
Building the compiler is pretty easy, you need a linker, a C compiler.  
Then type in (depending on the architecture):  
    `86` for i386, will make mcc86  
    `64` for AMD64, will make mcc64  
    `A86` for ARM32, will make mcca86  
    `A64` for ARM64, will make mcca64  
For platforms, only Linux is supported.
### Bootstrapping?
Replace in the Makefile:  
      `CC=cc` by `CC=./mcc86` (for the 32-bit architecture.)
