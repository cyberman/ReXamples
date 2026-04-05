# Technical Baseline

This document records the currently verified implementation baseline for ReXamples.

It exists to keep the repository's teaching text aligned with the code that actually builds.

## Verified target baseline

ReXamples is currently verified against:

- **AmigaOS 3.2+**
- **NDK 3.2R4**
- **vbcc**
- **aos68k target configuration**
- **plain C89**

## Build model

The current lesson series is built with:

- one lesson per directory
- one small lesson-local makefile per lesson
- shared rules in `mk/lesson.mk`
- a root `Makefile` that builds all lessons

The verified link step currently includes:

- `-lamiga`

## Native implementation style

The verified lesson series currently follows these rules:

- use **A-Forms**
- use **explicit window methods**
- avoid RA convenience layers
- avoid varargs-style construction macros as the primary implementation model
- keep ownership explicit in `struct App`
- use global library bases only as a technical bridge for the current vbcc/proto setup

## Verified architectural line

The current buildable series demonstrates that ReXamples can be implemented as:

- native ReAction/Intuition code
- plain C89
- explicit lifecycle and cleanup
- explicit event dispatch
- no foreign framework layer
- no hidden callback architecture

## Notes from real build verification

The current repository state was verified in a real build environment and includes a few practical findings:

- the effective vbcc target is **aos68k**
- `+amigaos` was not the correct target selector for the verified setup
- some class/proto combinations expect specific global base names
- `texteditor.gadget` required the expected base symbol for the installed header/proto setup
- `page.gadget` was not available in the verified include tree, so `19_RegisterPages` was implemented without it
- Linux host filesystems are case-sensitive, so lesson source filenames must match `main.c` exactly

## Scope of this note

This note is not a design manifesto.

It is a compact record of the implementation baseline that has actually been proven by building the lesson series.

