# 14_Menu_Basic

## Purpose

This lesson teaches native AmigaOS menus as a normal part of the event-driven UI model.

It introduces menu commands without making them look like a separate world.

## What you will learn

- how a simple native menu is defined
- how menu commands enter the event path
- how command dispatch differs from gadget activation

## Native components used

- Intuition menu system
- `window.class`
- normal event handling and dispatch

## What this lesson deliberately does not cover

- large menu trees
- dynamic menu generation
- context menus

## Why this lesson matters

Menus are one of the classic AmigaOS command paths and should be taught as first-class native structure.

## Build notes

- Target: **AmigaOS 3.2+**
- Language: **plain C89**
- Recommended toolchain: **vbcc**
- Reference SDK: **NDK 3.2R4**

## Key lifecycle points

- create menu definitions under clear ownership
- install and remove them deterministically
- dispatch menu events visibly in the main loop

## Typical mistakes

- treating menu logic as special-case magic
- burying IDs or command mapping
- overloading a basic lesson with advanced state handling

## Next lesson

Continue with **`15_Menu_Advanced`** to extend menu structure in a controlled way.
