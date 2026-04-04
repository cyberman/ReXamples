# 16_Gauge

## Purpose

This lesson teaches status and progress display as a display-only interface concept.

It distinguishes visible feedback from user input.

## What you will learn

- how a gauge-like element communicates status
- why display-only elements deserve the same ownership discipline as controls
- how status updates differ from direct user actions

## Native components used

- `window.class`
- `layout.gadget`
- `fuelgauge.gadget` or equivalent native status object

## What this lesson deliberately does not cover

- threading
- long-running background jobs
- full task progress systems

## Why this lesson matters

Not every visible UI element is an input device. Good lessons teach display semantics separately.

## Build notes

- Target: **AmigaOS 3.2+**
- Language: **plain C89**
- Recommended toolchain: **vbcc**
- Reference SDK: **NDK 3.2R4**

## Key lifecycle points

- create the display object under normal ownership rules
- update it through explicit control flow
- keep the display model distinct from input handling

## Typical mistakes

- treating a gauge like a slider
- introducing fake background complexity
- obscuring the difference between control and feedback

## Next lesson

Continue with **`17_TextDisplay`** to teach display-oriented text presentation.
