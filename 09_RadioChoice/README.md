# 09_RadioChoice

## Purpose

This lesson teaches exclusive choice between multiple options as a distinct interface pattern.

It clarifies the difference between boolean state, chooser selection, and grouped exclusive selection.

## What you will learn

- how exclusive choice differs from a checkbox
- how grouped options represent one decision
- how the selected value is handled cleanly

## Native components used

- `window.class`
- `layout.gadget`
- native exclusive-choice controls

## What this lesson deliberately does not cover

- multi-selection
- nested option groups
- persistent preferences

## Why this lesson matters

Good GUI teaching depends on distinguishing similar-looking controls by behaviour, not by appearance alone.

## Build notes

- Target: **AmigaOS 3.2+**
- Language: **plain C89**
- Recommended toolchain: **vbcc**
- Reference SDK: **NDK 3.2R4**

## Key lifecycle points

- create the choice group as one logical decision path
- keep group state explicit
- dispatch changes clearly in the event loop

## Typical mistakes

- using checkboxes to simulate exclusive choice
- hiding group semantics
- making the example about too many options at once

## Next lesson

Continue with **`10_Image`** to introduce display objects with their own resource character.
