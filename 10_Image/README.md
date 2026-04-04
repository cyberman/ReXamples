# 10_Image

## Purpose

This lesson teaches how visual objects are integrated and managed as real interface resources.

It introduces non-text, non-input presentation in native ReAction terms.

## What you will learn

- how image-like interface objects participate in the object tree
- why display resources still need ownership discipline
- how visual elements support interface structure

## Native components used

- `window.class`
- `layout.gadget`
- BOOPSI/ReAction image-related objects

## What this lesson deliberately does not cover

- general-purpose image loading pipelines
- datatype-heavy media handling
- custom drawing frameworks

## Why this lesson matters

Visual elements are easy to treat as decoration. This lesson teaches them as owned interface resources.

## Build notes

- Target: **AmigaOS 3.2+**
- Language: **plain C89**
- Recommended toolchain: **vbcc**
- Reference SDK: **NDK 3.2R4**

## Key lifecycle points

- create the display object explicitly
- keep ownership and disposal visible
- keep the lesson focused on display integration, not media systems

## Typical mistakes

- treating images as side detail with no lifecycle
- introducing too much loading complexity
- letting visuals overshadow structure

## Next lesson

Continue with **`11_Slider`** to learn continuous numeric input.
