# 08_ListBrowser

## Purpose

This lesson introduces a small native list model with explicit node ownership and selection handling.

## What you will learn

- how list browser entries are created explicitly
- how list ownership is kept in the application
- how selected rows are handled through explicit dispatch

## Native components used

- `window.class`
- `layout.gadget`
- `listbrowser.gadget`

## What this lesson deliberately does not cover

- virtual list models
- sorting frameworks
- large data abstractions

## Why this lesson matters

This is the first lesson where the UI presents structured data that the application owns and frees deliberately.

## Build notes

- Target: **AmigaOS 3.2+**
- Language: **plain C89**
- Recommended toolchain: **vbcc**
- Reference SDK: **NDK 3.2R4**

## Key lifecycle points

- initialize the list explicitly
- allocate nodes explicitly
- free nodes explicitly in teardown paths

## Typical mistakes

- hiding data ownership inside the widget layer
- assuming list models clean themselves up
- using non-C89 shortcuts in support code

## Next lesson

`09_RadioChoice`
