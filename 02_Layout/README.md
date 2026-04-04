# 02_Layout

## Purpose

This lesson teaches layout groups as the structural foundation of native ReAction interfaces.

It shows how a window stops being an empty shell and becomes an interface with
deliberate spatial structure.

## What you will learn

- how ReAction layout groups organize content
- why layout is a structural concept, not decoration
- how grouping improves interface clarity

## Native components used

- `window.class`
- `layout.gadget`

## What this lesson deliberately does not cover

- button actions
- menus
- complex gadget interaction

## Why this lesson matters

A reader who does not understand layout will build interfaces as loose collections
of objects instead of coherent native structures.

## Build notes

- Target: **AmigaOS 3.2+**
- Language: **plain C89**
- Recommended toolchain: **vbcc**
- Reference SDK: **NDK 3.2R4**

## Key lifecycle points

- build a structured object tree
- keep layout ownership visible
- preserve the same clear window lifecycle as in `01_Window`

## Typical mistakes

- treating layout as a cosmetic wrapper
- adding interaction before structural clarity is established
- hiding group structure behind unnecessary helpers

## Next lesson

Continue with **`03_Button`** to connect layout structure with a first real action gadget.

