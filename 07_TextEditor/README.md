# 07_TextEditor

## Purpose

This lesson introduces a native multiline editor object without turning the example into an editor application.

## What you will learn

- how a multiline native editor is placed in a lesson window
- how its lifecycle fits the established series pattern
- how presence and ownership are taught before document logic

## Native components used

- `window.class`
- `layout.gadget`
- `texteditor.gadget`

## What this lesson deliberately does not cover

- file loading and saving
- document structure
- editing commands and menus

## Why this lesson matters

Editing and display are different concerns. This lesson introduces multiline editing only as a native object and lifecycle case.

## Build notes

- Target: **AmigaOS 3.2+**
- Language: **plain C89**
- Recommended toolchain: **vbcc**
- Reference SDK: **NDK 3.2R4**

## Verified implementation note

In the verified build environment, the installed header/proto setup expects the corresponding editor base symbol used by the repository implementation.

## Key lifecycle points

- treat the editor as a native object in the tree
- keep window handling explicit
- stop before document architecture begins

## Typical mistakes

- turning the lesson into a mini editor
- adding file logic too early
- mixing presence, editing, and document concerns

## Next lesson

`08_ListBrowser`
