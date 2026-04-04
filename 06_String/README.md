# 06_String

## Purpose

This lesson teaches text input with explicit buffers and clear ownership.

It introduces the first case where application-managed storage becomes central to the lesson.

## What you will learn

- how a string gadget uses caller-owned storage
- why buffer size and ownership must be explicit
- how to handle text input without guesswork

## Native components used

- `window.class`
- `layout.gadget`
- `string.gadget`

## What this lesson deliberately does not cover

- full text validation frameworks
- file I/O
- persistent document editing

## Why this lesson matters

Text input is one of the fastest ways to teach whether ownership discipline is real or only claimed.

## Build notes

- Target: **AmigaOS 3.2+**
- Language: **plain C89**
- Recommended toolchain: **vbcc**
- Reference SDK: **NDK 3.2R4**

## Key lifecycle points

- allocate or define the input buffer under explicit ownership
- bind buffer and gadget clearly
- keep failure and cleanup paths readable

## Typical mistakes

- unclear buffer ownership
- weak maximum-length handling
- treating text input as magic instead of contract

## Next lesson

Continue with **`07_ListBrowser`** to work with structured collections.

