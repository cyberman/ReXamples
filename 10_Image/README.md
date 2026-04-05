# 10_Image

## Purpose

This lesson introduces a minimal native visual object without dragging in media handling or file dependencies.

## What you will learn

- how a visual object can live in the object tree
- how image-like presentation differs from interaction
- how ownership remains explicit even for purely visual elements

## Native components used

- `window.class`
- `layout.gadget`
- `bevel.image`

## What this lesson deliberately does not cover

- datatypes
- file-based image loading
- media pipelines

## Why this lesson matters

Not every visible element is a gadget. This lesson introduces a native visual object as its own structural concept.

## Build notes

- Target: **AmigaOS 3.2+**
- Language: **plain C89**
- Recommended toolchain: **vbcc**
- Reference SDK: **NDK 3.2R4**

## Key lifecycle points

- create the visual object explicitly
- place it in the normal layout tree
- dispose it through the same ownership rules as other objects

## Typical mistakes

- equating visual objects with loaded media
- overengineering the first visual lesson
- hiding image ownership behind larger helper code

## Next lesson

`11_Slider`
