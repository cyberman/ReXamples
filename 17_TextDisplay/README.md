# 17_TextDisplay

## Purpose

This lesson teaches static or structured text presentation beyond simple labels.

It focuses on readable presentation rather than editable text.

## What you will learn

- how display text differs from input text
- how text can support structure and explanation in an interface
- how text presentation fits into a native object tree

## Native components used

- `window.class`
- `layout.gadget`
- native text display elements

## What this lesson deliberately does not cover

- text editing
- document viewers
- general-purpose text formatting engines

## Why this lesson matters

A GUI teaches through text as well as through controls. Text display deserves its own clear lesson.

## Build notes

- Target: **AmigaOS 3.2+**
- Language: **plain C89**
- Recommended toolchain: **vbcc**
- Reference SDK: **NDK 3.2R4**

## Key lifecycle points

- treat display text as part of structural interface composition
- keep ownership visible for any backing data
- keep the lesson focused on presentation, not editing

## Typical mistakes

- blurring the line between input and display
- overengineering formatting
- turning a text lesson into a document system

## Next lesson

Continue with **`18_ScrollableLayout`** to handle content that exceeds the visible area.
