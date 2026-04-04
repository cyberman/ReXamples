# 03_Button

## Purpose

This lesson teaches how a basic action gadget is created, identified, and handled in the event loop.

It introduces the first direct user action in the series.

## What you will learn

- how to create a native button gadget
- how gadget IDs fit into event dispatch
- how a button action should be handled cleanly

## Native components used

- `window.class`
- `layout.gadget`
- `button.gadget`

## What this lesson deliberately does not cover

- multiple competing actions
- requesters
- menus
- application state beyond a minimal action path

## Why this lesson matters

Buttons are simple, but they are the first place where the reader must connect
object construction, event handling, and program behaviour.

## Build notes

- Target: **AmigaOS 3.2+**
- Language: **plain C89**
- Recommended toolchain: **vbcc**
- Reference SDK: **NDK 3.2R4**

## Key lifecycle points

- create the button as part of the object tree
- assign a visible and meaningful gadget ID
- dispatch the event clearly in the main loop

## Typical mistakes

- mixing button action logic into setup code
- hiding gadget IDs in unrelated places
- making the lesson broader than one action path

## Next lesson

Continue with **`04_CheckBox`** to handle simple interface state.

