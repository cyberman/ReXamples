# Building ReXamples with vbcc

ReXamples expects a native AmigaOS 3.2+ cross-development setup based on vbcc.

## Environment

Before building, load your Amiga development environment:

```sh
source /path/to/env.sh
````

A suitable environment should provide:

- `vc`
    
- `vasmm68k_mot`
    
- `vlink`
    

and should export at least:

- `AMIGA_NDK_HEADERS_INCLUDE`
    
- `VBCC`
    
- `VBCCENV`
    

## Basic build

To build all lessons:

```sh
make
```

To build one lesson:

```sh
make -C 01_Window
```

To clean all lessons:

```sh
make clean
```

## Build philosophy

ReXamples keeps lesson builds deliberately simple:

- each lesson builds in isolation
    
- each lesson has a tiny local makefile
    
- shared rules live in `mk/lesson.mk`
    
- no heavy build framework is required
    

This keeps the build readable and aligned with the teaching goal of the repository.

## Target baseline

ReXamples targets:

- AmigaOS 3.2.3+
    
- NDK 3.2R4
    
- plain C89
    
- vbcc as the primary toolchain
    

## Environment notes

A suitable `env.sh` for ReXamples should define the NDK include root and make  
vbcc available through `PATH`.

In the current environment, the important variables are:

- `AMIGA_NDK_HEADERS_INCLUDE`
    
- `AMIGA_VBCC`
    
- `VBCC`
    
- `VBCCENV`
    
- `PATH`
    

This is sufficient for the simple per-lesson build model used by ReXamples.


---


