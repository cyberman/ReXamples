# ReXamples Style Guide

ReXamples code must teach clearly, run correctly, and remain trustworthy as native reference code.

This guide defines the core style expected from every lesson in the repository.

## 1. Purpose

ReXamples is teaching code first.

Each example must teach one core concept clearly, in a form that is small enough to study, direct enough to trust, and disciplined enough to reuse.

If a lesson is broad, vague, or structurally noisy, it is not ready.

## 2. Code shape

Use plain C89.

Prefer:

- small files
- small single-purpose functions
- explicit state
- readable control flow
- names that teach intent

Do not compress code until the lesson becomes harder to read.

Readable structure is more important than clever brevity.

## 3. Native model

Teach native ReAction, Intuition, BOOPSI, and standard AmigaOS structure directly.

Do not preserve foreign GUI terminology, inherited abstractions, or toolkit-neutral mental models when they obscure the native model.

Names, layout, event flow, and ownership should make native AmigaOS thinking visible.

## 4. Ownership

Every resource has one owner.

Ownership must be visible.
Acquisition must be deliberate.
Release must be explicit.

This applies to libraries, classes, objects, ports, signals, buffers, locks, and any other acquired resources.

If ownership is unclear, the example is not good teaching code.

## 5. Lifecycle

Initialization, failure handling, rollback, shutdown, and cleanup are part of the example.

Do not teach success-path-only code.

A ReXamples lesson must show:

- where initialization begins
- where ownership starts
- how failure is handled
- where shutdown begins
- where cleanup is guaranteed

Cleanup is not boilerplate. It is part of the lesson.

## 6. Event flow

Event-driven structure must remain visible.

A reader should be able to trace an event from arrival to effect without guesswork.

Keep the event loop, dispatch path, and exit conditions understandable.
Do not hide essential control flow behind convenience layers.

If event handling feels magical, the lesson is too opaque.

## 7. Documentation

Document what matters:

- purpose
- ownership
- side effects
- failure behaviour
- lifecycle role

Comments must reduce uncertainty, not restate the obvious.

Structured in-code documentation is preferred when it makes the code easier to review,
easier to maintain, and easier to promote into future reference documentation.

If a function matters enough to teach, it matters enough to document properly.

## 8. Reference quality

Every example must be trustworthy as reference code.

If copied into a real project, it should teach good native habits:

- explicit ownership
- disciplined cleanup
- readable event flow
- clear naming
- honest structure

If something works but teaches bad habits, it does not belong in ReXamples.
