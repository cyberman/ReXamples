# 06_String

## Purpose

This lesson introduces native string input with explicit application-owned buffer handling.

## What you will learn

- how a string gadget uses an application buffer
- how buffer ownership stays explicit
- how entered text is read and reflected deliberately

## Native components used

- `window.class`
- `layout.gadget`
- `string.gadget`

## What this lesson deliberately does not cover

- validation frameworks
- file I/O
- document editing

## Why this lesson matters

This is the first lesson where application-owned text storage becomes part of the UI contract.

## Build notes

- Target: **AmigaOS 3.2+**
- Language: **plain C89**
- Recommended toolchain: **vbcc**
- Reference SDK: **NDK 3.2R4**

## Key lifecycle points

- initialize the application buffer before opening the window
- give the gadget controlled access to the buffer
- keep readback and UI update explicit

## Typical mistakes

- hiding buffer ownership
- assuming text input owns its own application model
- introducing document logic too early

## Next lesson

`07_TextEditor`
