# 12_Space

## Purpose

This lesson teaches that layout structure also includes deliberate empty space.

## What you will learn

- how spacing becomes part of visible composition
- how empty areas can support grouping and clarity
- how layout structure is more than control placement

## Native components used

- `window.class`
- `layout.gadget`
- `space.gadget`

## What this lesson deliberately does not cover

- styling systems
- theme logic
- dynamic spacing policies

## Why this lesson matters

Readable interfaces depend as much on what is not crowded together as on the controls themselves.

## Build notes

- Target: **AmigaOS 3.2+**
- Language: **plain C89**
- Recommended toolchain: **vbcc**
- Reference SDK: **NDK 3.2R4**

## Key lifecycle points

- create spacing objects explicitly
- compose them through normal layout ownership
- treat empty structure as first-class layout information

## Typical mistakes

- treating space as accidental
- replacing structure with hardcoded pixel thinking
- ignoring ownership just because an object is visually empty

## Next lesson

`13_Frame`
