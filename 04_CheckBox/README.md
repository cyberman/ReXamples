# 04_CheckBox

## Purpose

This lesson introduces a boolean state gadget and explicit state propagation.

## What you will learn

- how a checkbox differs from an action button
- how boolean state is read explicitly
- how state changes lead to visible UI updates

## Native components used

- `window.class`
- `layout.gadget`
- `checkbox.gadget`

## What this lesson deliberately does not cover

- preferences systems
- persistent settings
- hidden notify chains

## Why this lesson matters

Not all user input is an action. Some input changes state, and state must remain visible and controlled.

## Build notes

- Target: **AmigaOS 3.2+**
- Language: **plain C89**
- Recommended toolchain: **vbcc**
- Reference SDK: **NDK 3.2R4**

## Key lifecycle points

- create the state gadget explicitly
- read state through explicit attribute access
- update visible UI state deliberately

## Typical mistakes

- treating state and action as the same thing
- hiding state transitions
- pushing simple state into larger frameworks too early

## Next lesson

`05_Chooser`
