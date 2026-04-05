# 02_Layout

## Purpose

This lesson introduces visible native layout structure without adding interaction.

## What you will learn

- how layout composition is built explicitly
- how vertical and horizontal grouping differ
- how layout structure can remain readable

## Native components used

- `window.class`
- `layout.gadget`
- `space.gadget`

## What this lesson deliberately does not cover

- gadget events
- application state
- dynamic content changes

## Why this lesson matters

A native GUI is more than a window with controls. Layout structure is the first real architectural layer.

## Build notes

- Target: **AmigaOS 3.2+**
- Language: **plain C89**
- Recommended toolchain: **vbcc**
- Reference SDK: **NDK 3.2R4**

## Key lifecycle points

- create child objects before the parent layout that owns them
- attach child objects explicitly through tag lists
- keep ownership and disposal order readable

## Typical mistakes

- treating layout as decoration
- hiding parent-child relations
- jumping to interaction before structure is clear

## Next lesson

`03_Button`
