# 11_Slider

## Purpose

This lesson introduces a continuous numeric value gadget with explicit readback and visible feedback.

## What you will learn

- how a slider represents a bounded value
- how the current level is read explicitly
- how numeric state changes can remain visible and simple

## Native components used

- `window.class`
- `layout.gadget`
- `slider.gadget`

## What this lesson deliberately does not cover

- preferences storage
- value binding systems
- compound numeric models

## Why this lesson matters

Continuous values are a different interaction category from actions, booleans, and discrete choices.

## Build notes

- Target: **AmigaOS 3.2+**
- Language: **plain C89**
- Recommended toolchain: **vbcc**
- Reference SDK: **NDK 3.2R4**

## Key lifecycle points

- define min, max, and initial level explicitly
- read the active level explicitly
- reflect the value visibly after dispatch

## Typical mistakes

- treating a range control like a button
- hiding numeric state updates
- complicating a small value case with too much abstraction

## Next lesson

`12_Space`
