# 15_Menu_Advanced

## Purpose

This lesson builds on the basic menu lesson with richer menu structure and controlled state.

It introduces the next level of command organization without losing clarity.

## What you will learn

- how more capable menu structures are organized
- how menu state can be controlled deliberately
- how command paths remain understandable as menus grow

## Native components used

- Intuition menu system
- `window.class`
- explicit menu command dispatch

## What this lesson deliberately does not cover

- full application command frameworks
- scripting
- document model complexity

## Why this lesson matters

A serious native application usually needs more than one or two menu items. This lesson teaches how to scale without turning the code into noise.

## Build notes

- Target: **AmigaOS 3.2+**
- Language: **plain C89**
- Recommended toolchain: **vbcc**
- Reference SDK: **NDK 3.2R4**

## Key lifecycle points

- keep menu creation and teardown explicit
- preserve readable command mapping
- keep menu state changes traceable

## Typical mistakes

- making menu growth collapse readability
- entangling menu logic with unrelated application state
- hiding command identity behind abstraction

## Next lesson

Continue with **`16_Gauge`** to separate display-only status from user-controlled input.
