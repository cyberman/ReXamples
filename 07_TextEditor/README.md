# 07_TextEditor

## Purpose

This lesson teaches native multiline text editing with `texteditor.gadget`.

It introduces a true editing component that goes beyond simple single-line input
and shows how larger editable text content fits into disciplined native ReAction code.

## What you will learn

- how multiline text editing differs from simple string input
- how `texteditor.gadget` fits into a ReAction object tree
- why editor content, ownership, and event flow must remain explicit

## Native components used

- `window.class`
- `layout.gadget`
- `texteditor.gadget`

## What this lesson deliberately does not cover

- file loading and saving
- syntax highlighting
- editor macros
- document architecture
- ARexx integration

## Why this lesson matters

A native GUI curriculum that teaches only single-line input leaves a major gap.
This lesson introduces real text editing as its own interface concept.

## Build notes

- Target: **AmigaOS 3.2+**
- Language: **plain C89**
- Recommended toolchain: **vbcc**
- Reference SDK: **NDK 3.2R4**

## Key lifecycle points

- create the editor object as part of the normal object tree
- keep editor-related ownership explicit
- make input and event handling readable
- preserve clear shutdown and cleanup paths

## Typical mistakes

- treating multiline editing as just a larger string gadget
- turning the lesson into a mini text processor
- hiding editor-related control flow behind helpers

## Next lesson

Continue with **`08_ListBrowser`** to move from editable text to structured collections.
