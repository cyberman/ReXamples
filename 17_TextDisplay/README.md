# 17_TextDisplay

## Purpose

This lesson teaches simple native text presentation without turning the example into an editor or document system.

## What you will learn

- how static text can support structure
- how text display differs from text editing
- how presentation can remain explicit and small

## Native components used

- `window.class`
- `layout.gadget`
- `button.gadget` used as a conservative read-only text carrier

## What this lesson deliberately does not cover

- text editing
- document management
- file I/O

## Why this lesson matters

A GUI must not treat all text as editable. This lesson keeps display and editing conceptually separate.

## Build notes

- Target: **AmigaOS 3.2+**
- Language: **plain C89**
- Recommended toolchain: **vbcc**
- Reference SDK: **NDK 3.2R4**

## Key lifecycle points

- create display objects explicitly
- place them in a normal layout
- keep the event loop minimal and visible

## Typical mistakes

- using an editor where display is enough
- hiding presentation behind a larger document abstraction
- mixing display and editing concerns too early

## Next lesson

`18_ScrollableLayout`
