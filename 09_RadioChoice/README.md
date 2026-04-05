# 09_RadioChoice

## Purpose

This lesson introduces exclusive selection as its own native interaction pattern.

## What you will learn

- how radio-style exclusive selection differs from chooser-based selection
- how active choice state is read explicitly
- how the chosen mode is reflected visibly

## Native components used

- `window.class`
- `layout.gadget`
- `radiobutton.gadget`

## What this lesson deliberately does not cover

- dynamic mode systems
- nested preference models
- higher-level mode frameworks

## Why this lesson matters

Exclusive choice is common and should remain understandable without hidden logic or foreign UI models.

## Build notes

- Target: **AmigaOS 3.2+**
- Language: **plain C89**
- Recommended toolchain: **vbcc**
- Reference SDK: **NDK 3.2R4**

## Key lifecycle points

- define the native label set explicitly
- read the active selection explicitly
- update visible state after dispatch

## Typical mistakes

- confusing radio groups with general menu selection
- burying state updates inside convenience abstractions
- treating exclusive choice as a special case of everything else

## Next lesson

`10_Image`
