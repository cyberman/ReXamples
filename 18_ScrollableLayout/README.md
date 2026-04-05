# 18_ScrollableLayout

## Purpose

This lesson introduces the idea that visible space and content size are not always the same.

## What you will learn

- how to represent a larger content region
- how scrolling relates to structure rather than document complexity
- how native layout and scroller elements can be kept explicit

## Native components used

- `window.class`
- `layout.gadget`
- `button.gadget`
- `scroller.gadget`

## What this lesson deliberately does not cover

- document viewers
- text engines
- automatic content virtualization

## Why this lesson matters

Interfaces often need to present more structure than a single visible region can comfortably show. This lesson introduces that case in a small native form.

## Build notes

- Target: **AmigaOS 3.2+**
- Language: **plain C89**
- Recommended toolchain: **vbcc**
- Reference SDK: **NDK 3.2R4**

## Key lifecycle points

- keep content objects explicit
- keep the scroller explicit
- do not hide layout structure behind abstraction layers

## Typical mistakes

- treating scrolling as a document-only concept
- introducing unnecessary complexity too early
- hiding content ownership and layout structure

## Next lesson

`19_RegisterPages`
