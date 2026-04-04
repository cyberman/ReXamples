# 01_Window

## Purpose

This lesson teaches the native lifecycle of a minimal ReAction windowed program.

It shows the smallest useful structure of a real ReAction application:
open required classes and libraries, create a minimal object tree, open the window,
wait for events, react to close conditions, and shut everything down cleanly.

## What you will learn

- how a minimal ReAction window is created
- how a basic event loop fits into a GUI application
- why cleanup is part of the design, not an afterthought

## Native components used

- `window.class`
- `layout.gadget`
- Intuition window and event handling

## What this lesson deliberately does not cover

- buttons
- menus
- requesters
- ARexx
- multiple windows

## Why this lesson matters

Before any interesting interface work begins, the reader must understand the
basic lifetime of a native windowed application.

## Build notes

- Target: **AmigaOS 3.2+**
- Language: **plain C89**
- Recommended toolchain: **vbcc**
- Reference SDK: **NDK 3.2R4**

## Key lifecycle points

- initialize application state
- open required classes and libraries
- create the object tree
- open the window
- wait for events
- shut down and release resources in reverse order

## Typical mistakes

- treating cleanup as optional
- mixing setup and event logic too early
- returning from failed initialization without rollback

## Next lesson

Continue with **`02_Layout`** to learn how native ReAction interfaces are arranged in layout groups.

