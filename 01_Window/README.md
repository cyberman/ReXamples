# 01_Window

## Purpose

This lesson introduces the smallest useful native ReAction window lifecycle.

## What you will learn

- how a native lesson window is created
- how a window is opened and closed explicitly
- how lifecycle and cleanup remain visible from the start

## Native components used

- `window.class`
- `layout.gadget`

## What this lesson deliberately does not cover

- gadget interaction
- menu handling
- application logic

## Why this lesson matters

Every later lesson depends on a trustworthy window lifecycle. This lesson establishes that baseline.

## Build notes

- Target: **AmigaOS 3.2+**
- Language: **plain C89**
- Recommended toolchain: **vbcc**
- Reference SDK: **NDK 3.2R4**

## Key lifecycle points

- open required libraries explicitly
- create the root layout explicitly
- open and close the window through explicit methods
- keep cleanup easy to trace

## Typical mistakes

- hiding lifecycle behind convenience layers
- mixing window creation with later application logic
- treating cleanup as secondary

## Next lesson

`02_Layout`
