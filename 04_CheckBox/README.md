# 04_CheckBox

## Purpose

This lesson teaches how to work with simple boolean interface state in a native way.

It introduces the difference between an action gadget and a state gadget.

## What you will learn

- how a checkbox represents on/off state
- how state is read and interpreted
- why UI state should remain explicit

## Native components used

- `window.class`
- `layout.gadget`
- `checkbox.gadget`

## What this lesson deliberately does not cover

- complex dependencies between many gadgets
- persistent settings
- menu-controlled state

## Why this lesson matters

A native GUI is not only about actions. It also needs disciplined handling of visible state.

## Build notes

- Target: **AmigaOS 3.2+**
- Language: **plain C89**
- Recommended toolchain: **vbcc**
- Reference SDK: **NDK 3.2R4**

## Key lifecycle points

- create and own the checkbox as part of the window object tree
- read state through explicit event handling
- keep UI state separate from broader application logic

## Typical mistakes

- treating checkbox state as hidden global state
- using the gadget as the whole model
- mixing too much control logic into one lesson

## Next lesson

Continue with **`05_Chooser`** to learn native single-choice selection.
