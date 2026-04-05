# 16_Gauge

## Purpose

This lesson introduces a native display/status gadget whose value is changed by a separate input path.

## What you will learn

- how a display gadget differs from an input gadget
- how UI state can be updated from a separate action source
- how visible status remains explicit and controlled

## Native components used

- `window.class`
- `layout.gadget`
- `button.gadget`
- `fuelgauge.gadget`

## What this lesson deliberately does not cover

- background tasks
- real progress reporting
- threaded update systems

## Why this lesson matters

Some native UI elements show state rather than collecting input. This lesson makes that distinction concrete.

## Build notes

- Target: **AmigaOS 3.2+**
- Language: **plain C89**
- Recommended toolchain: **vbcc**
- Reference SDK: **NDK 3.2R4**

## Key lifecycle points

- create the display object and input object separately
- update gauge state explicitly
- keep visible status changes directly traceable

## Typical mistakes

- treating display objects like input controls
- hiding status updates inside vague helper logic
- simulating complexity the lesson does not need

## Next lesson

`17_TextDisplay`
