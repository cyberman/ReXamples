# 15_Menu_Advanced

## Purpose

This lesson extends the native menu path with multiple menus and toggle state.

## What you will learn

- how a richer menu structure is laid out
- how checked state is read explicitly
- how menu state can influence visible UI state

## Native components used

- `window.class`
- `layout.gadget`
- `gadtools.library`
- Intuition menu message handling

## What this lesson deliberately does not cover

- full command systems
- persistence
- large application menu frameworks

## Why this lesson matters

Command paths become more realistic once menus can express state, not just one-shot actions.

## Build notes

- Target: **AmigaOS 3.2+**
- Language: **plain C89**
- Recommended toolchain: **vbcc**
- Reference SDK: **NDK 3.2R4**

## Key lifecycle points

- create the menu tree explicitly
- decode checked state directly from selected items
- update visible application state deliberately

## Typical mistakes

- hiding menu state transitions
- confusing command selection with gadget notify thinking
- letting menu handling become magical

## Next lesson

`16_Gauge`
