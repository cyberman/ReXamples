# 11_Slider

## Purpose

This lesson teaches continuous numeric input and value-change handling.

It introduces a control whose state is more than simple on/off or one-of-many.

## What you will learn

- how a slider represents a numeric range
- how range values are constrained
- how value changes are handled in the event loop

## Native components used

- `window.class`
- `layout.gadget`
- `slider.gadget`

## What this lesson deliberately does not cover

- complex linked controls
- persistent numeric preferences
- background processing

## Why this lesson matters

A slider is a clean way to teach range-based input and visible state changes.

## Build notes

- Target: **AmigaOS 3.2+**
- Language: **plain C89**
- Recommended toolchain: **vbcc**
- Reference SDK: **NDK 3.2R4**

## Key lifecycle points

- define min, max, and current value explicitly
- update state through visible event handling
- avoid hiding numeric meaning behind arbitrary constants

## Typical mistakes

- weak range definition
- unclear value ownership
- coupling slider changes to too much secondary logic

## Next lesson

Continue with **`12_Space`** to learn that layout also teaches through what it leaves empty.
