# 14_Menu_Basic

## Purpose

This lesson introduces the smallest useful native menu command path.

## What you will learn

- how a native menu is created and installed
- how menu picks are decoded explicitly
- how command actions remain visible and controlled

## Native components used

- `window.class`
- `layout.gadget`
- `gadtools.library` for menu construction
- Intuition menu message handling

## What this lesson deliberately does not cover

- deep menu trees
- preference persistence
- application command systems beyond the lesson scope

## Why this lesson matters

Menus are not gadgets. This lesson introduces a separate native command path that real applications depend on.

## Build notes

- Target: **AmigaOS 3.2+**
- Language: **plain C89**
- Recommended toolchain: **vbcc**
- Reference SDK: **NDK 3.2R4**

## Key lifecycle points

- create and lay out the menu explicitly
- install and remove the menu strip explicitly
- decode menu picks through the Intuition path

## Typical mistakes

- treating menus like ordinary gadget input
- hiding command routing inside convenience logic
- leaving menu lifetime unclear

## Next lesson

`15_Menu_Advanced`
