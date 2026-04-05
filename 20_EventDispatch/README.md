# 20_EventDispatch

## Purpose

This lesson teaches explicit event routing and controlled UI reactions.

It brings the series together by showing how native event-driven structure remains understandable even when multiple interface elements interact.

## What you will learn

- how events are routed deliberately
- how state changes lead to controlled UI updates
- how explicit dispatch keeps native GUI logic trustworthy

## Native components used

- `window.class`
- `layout.gadget`
- `button.gadget`
- `checkbox.gadget`
- explicit native event handling

## What this lesson deliberately does not cover

- giant callback frameworks
- hidden automation layers
- general-purpose application architectures

## Why this lesson matters

This is the lesson where the whole curriculum proves itself. If event flow is still clear here, the earlier lessons have done their job.

## Build notes

- Target: **AmigaOS 3.2+**
- Language: **plain C89**
- Recommended toolchain: **vbcc**
- Reference SDK: **NDK 3.2R4**

## Verified implementation note

This lesson is part of the verified buildable `01`–`20` core lesson series.

## Key lifecycle points

- keep event arrival, decoding, dispatch, and effect visibly connected
- preserve explicit ownership and cleanup even in a richer example
- make shutdown paths easy to trace

## Typical mistakes

- hiding control flow behind convenience abstractions
- recreating foreign notify-style mental models
- letting event logic become magical instead of readable

## Next lesson

This is the final lesson in the core ReXamples curriculum.
