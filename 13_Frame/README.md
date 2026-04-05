# 13_Frame

## Purpose

This lesson introduces framing as a native grouping aid rather than decoration for its own sake.

## What you will learn

- how a frame-like visual object supports structure
- how grouping can be made visible without adding logic
- how visual separation remains part of the object tree

## Native components used

- `window.class`
- `layout.gadget`
- `bevel.image`

## What this lesson deliberately does not cover

- decorative styling systems
- preference panels
- nested application group logic

## Why this lesson matters

A frame can clarify structure, but only when it serves meaning instead of visual noise.

## Build notes

- Target: **AmigaOS 3.2+**
- Language: **plain C89**
- Recommended toolchain: **vbcc**
- Reference SDK: **NDK 3.2R4**

## Key lifecycle points

- create the frame object explicitly
- keep it inside the normal ownership chain
- use it as structure, not spectacle

## Typical mistakes

- using frames as mere decoration
- confusing grouping with visual clutter
- hiding simple structure inside larger abstractions

## Next lesson

`14_Menu_Basic`
