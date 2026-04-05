# 19_RegisterPages

## Purpose

This lesson teaches page-oriented structure and register navigation in a form that matches the verified native build baseline.

## What you will learn

- how register navigation can be modeled explicitly
- how active page state can remain application-owned
- how displayed content can be updated deliberately on tab changes

## Native components used

- `window.class`
- `layout.gadget`
- `clicktab.gadget`
- `button.gadget` used as a conservative read-only content carrier

## What this lesson deliberately does not cover

- full preferences applications
- hidden page-management frameworks
- unavailable gadget assumptions

## Why this lesson matters

Tabbed structure is common, but the important lesson is not the visual tab itself. The important lesson is explicit page state and controlled content switching.

## Build notes

- Target: **AmigaOS 3.2+**
- Language: **plain C89**
- Recommended toolchain: **vbcc**
- Reference SDK: **NDK 3.2R4**

## Verified implementation note

In the verified build environment, this lesson is implemented **without** `page.gadget`.

The active page is tracked explicitly, and the visible content is updated deliberately when the selected register changes.

## Key lifecycle points

- own the active page state in the application
- treat register selection as explicit input
- update visible content deliberately

## Typical mistakes

- assuming unavailable classes are part of the verified baseline
- hiding page state inside convenience logic
- turning a structural lesson into a full application

## Next lesson

`20_EventDispatch`
