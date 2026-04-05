# 05_Chooser

## Purpose

This lesson introduces native single-choice selection from a fixed set of entries.

## What you will learn

- how a chooser represents selection state
- how the selected index is read explicitly
- how selection can be reflected visibly in the UI

## Native components used

- `window.class`
- `layout.gadget`
- `chooser.gadget`

## What this lesson deliberately does not cover

- dynamic entry population
- persistence
- data binding systems

## Why this lesson matters

Selection is a different kind of interface contract than action or boolean state. This lesson makes that distinction explicit.

## Build notes

- Target: **AmigaOS 3.2+**
- Language: **plain C89**
- Recommended toolchain: **vbcc**
- Reference SDK: **NDK 3.2R4**

## Key lifecycle points

- provide a fixed native label set
- keep selected state application-visible
- update the visible result explicitly after dispatch

## Typical mistakes

- hiding selected state inside convenience logic
- mixing display text and selection logic carelessly
- overcomplicating a simple single-choice case

## Next lesson

`06_String`
