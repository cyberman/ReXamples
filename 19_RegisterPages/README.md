# 19_RegisterPages

## Purpose

This lesson teaches multi-page interface organization with clear page-level structure.

It shows how a larger interface can remain readable by being divided into coherent sections.

## What you will learn

- how native page/register structures organize content
- how page selection affects visible interface state
- how larger GUI composition remains manageable

## Native components used

- `window.class`
- `layout.gadget`
- native register or page-management objects

## What this lesson deliberately does not cover

- full preferences applications
- dynamic page generation
- cross-page data persistence policies

## Why this lesson matters

Page-based structure is one of the first places where interface architecture becomes visible as architecture.

## Build notes

- Target: **AmigaOS 3.2+**
- Language: **plain C89**
- Recommended toolchain: **vbcc**
- Reference SDK: **NDK 3.2R4**

## Key lifecycle points

- create each page under visible ownership
- make page structure readable in code
- keep page switching and event flow explicit

## Typical mistakes

- collapsing multiple lessons into one giant multi-page demo
- hiding page semantics behind convenience helpers
- letting navigation logic sprawl

## Next lesson

Continue with **`20_EventDispatch`** to bring the series together with explicit routing of UI events.
