# 05_Chooser

## Purpose

This lesson teaches native single-choice selection with a ReAction chooser gadget.

It introduces controlled selection from a fixed set of options.

## What you will learn

- how a chooser presents multiple predefined options
- how the active selection is represented
- how selection changes are handled cleanly

## Native components used

- `window.class`
- `layout.gadget`
- `chooser.gadget`

## What this lesson deliberately does not cover

- editable lists
- dynamic list contents
- list browser semantics

## Why this lesson matters

Selection is a core part of practical GUI work, and it should be learned in native terms.

## Build notes

- Target: **AmigaOS 3.2+**
- Language: **plain C89**
- Recommended toolchain: **vbcc**
- Reference SDK: **NDK 3.2R4**

## Key lifecycle points

- create chooser entries as owned lesson data
- keep selected value handling explicit
- dispatch changes through the main event loop

## Typical mistakes

- preserving foreign “cycle” terminology
- confusing selection with free-form input
- hiding option ownership or selection decoding

## Next lesson

Continue with **`06_String`** to move from choice to text input.

