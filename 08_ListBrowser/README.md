# 08_ListBrowser

## Purpose

This lesson teaches structured list display, entry management, and selection handling.

It moves from editable text to structured collections and shows how visible list data
and user selection fit into disciplined native ReAction code.

## What you will learn

- how a native list browser presents structured entries
- how list entries are managed as real data
- how selection fits into event-driven UI flow

## Native components used

- `window.class`
- `layout.gadget`
- `listbrowser.gadget`

## What this lesson deliberately does not cover

- in-place editing
- sorting frameworks
- large data models

## Why this lesson matters

A list is not just a bigger chooser. It introduces data structure and presentation working together.

## Build notes

- Target: **AmigaOS 3.2+**
- Language: **plain C89**
- Recommended toolchain: **vbcc**
- Reference SDK: **NDK 3.2R4**

## Key lifecycle points

- create and own list data explicitly
- attach it to the gadget in a visible way
- release list-related resources deterministically

## Typical mistakes

- treating a list as only text on screen
- hiding entry ownership
- mixing too much list logic into one lesson

## Next lesson

Continue with **`09_RadioChoice`** for exclusive selection as a distinct UI pattern.
