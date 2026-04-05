# 03_Button

## Purpose

This lesson introduces the smallest useful action gadget and its event path.

## What you will learn

- how a button gadget is created
- how an action event is dispatched explicitly
- how a visible UI reaction can stay small and clear

## Native components used

- `window.class`
- `layout.gadget`
- `button.gadget`

## What this lesson deliberately does not cover

- complex workflows
- requesters
- callback frameworks

## Why this lesson matters

This is the first lesson where user action visibly changes application state.

## Build notes

- Target: **AmigaOS 3.2+**
- Language: **plain C89**
- Recommended toolchain: **vbcc**
- Reference SDK: **NDK 3.2R4**

## Key lifecycle points

- give the gadget a stable ID
- decode gadget events explicitly
- route the action through a small dedicated dispatch path

## Typical mistakes

- putting business logic directly into the event loop
- hiding action handling behind abstraction too early
- treating dispatch as incidental

## Next lesson

`04_CheckBox`
