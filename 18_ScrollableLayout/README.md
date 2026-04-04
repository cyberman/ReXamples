# 18_ScrollableLayout

## Purpose

This lesson teaches how larger-than-visible content is structured in a native scrollable interface.

It introduces composition where visible space and content size no longer match.

## What you will learn

- how native scrolling relates to layout and content structure
- how scrollable composition remains readable in code
- how to keep interface size and content size conceptually separate

## Native components used

- `window.class`
- `layout.gadget`
- native scrollable layout mechanisms

## What this lesson deliberately does not cover

- document viewers
- custom virtual widget systems
- arbitrary large data models

## Why this lesson matters

Scrollability is where many interfaces stop being simple. This lesson teaches complexity without surrendering structure.

## Build notes

- Target: **AmigaOS 3.2+**
- Language: **plain C89**
- Recommended toolchain: **vbcc**
- Reference SDK: **NDK 3.2R4**

## Key lifecycle points

- keep content ownership explicit
- make scrollable structure readable in the object tree
- preserve clear event handling around navigation and exit

## Typical mistakes

- teaching a foreign virtual-group mental model
- hiding the layout relationship between content and viewport
- adding too much content logic at once

## Next lesson

Continue with **`19_RegisterPages`** to organize a larger interface into distinct pages.
