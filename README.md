# ReXamples

**Independent native ReAction teaching examples for AmigaOS 3.2+**  
**Plain C89 lessons for disciplined system-native GUI development**

ReXamples is a focused teaching project for **ReAction** on **AmigaOS 3.2+**.

It exists to show how graphical applications should be structured when built in a
**native AmigaOS style** using:

- ReAction
- Intuition
- BOOPSI
- standard AmigaOS event and lifecycle patterns

All examples are written in **plain C89** and are intentionally designed to be:

- small
- readable
- self-contained
- resource-safe
- deterministic in cleanup
- event-driven by design

## Project goals

ReXamples teaches:

- how to build native ReAction windows and layouts
- how to use common AmigaOS GUI components correctly
- how to handle gadget and menu events cleanly
- how to structure small GUI programs with explicit ownership and predictable cleanup
- how to think in native AmigaOS terms instead of importing habits from other GUI systems

## Non-goals

ReXamples does not aim to:

- imitate other GUI systems
- preserve foreign naming, structure, or mental models
- hide ReAction behind heavy abstraction layers
- teach framework-agnostic GUI design
- optimize architecture around backward-compatibility concerns
- trade clarity and correctness for clever shortcuts

## Design principles

Each example follows the same core rules:

- **one example, one lesson**
- **plain C89**
- **native system components**
- **explicit ownership**
- **cleanup-first error handling**
- **event-driven structure**
- **clarity over cleverness**

## Target platform

- **Primary target:** AmigaOS 3.2.3+
- **Reference SDK:** NDK 3.2R4
- **Language:** C89
- **Recommended toolchain:** vbcc

## Repository structure

Each lesson is self-contained and focuses on one core concept.

Suggested learning order:

1. `01_Window`
2. `02_Layout`
3. `03_Button`
4. `04_CheckBox`
5. `05_Chooser`
6. `06_String`
7. `07_ListBrowser`
8. `08_RadioChoice`
9. `09_Image`
10. `10_Slider`
11. `11_Space`
12. `12_Frame`
13. `13_Menu_Basic`
14. `14_Menu_Advanced`
15. `15_Gauge`
16. `16_TextDisplay`
17. `17_ScrollableLayout`
18. `18_RegisterPages`
19. `19_EventDispatch`

See `docs/curriculum.md` for the full learning path.

## What defines ReXamples

ReXamples is built to show not only what works, but what good native style looks like on AmigaOS:

- libraries and classes are opened deliberately
- objects are disposed explicitly
- partial initialization failures are rolled back correctly
- event handling remains visible and understandable
- each lesson stays focused on a single teaching goal

## Documentation

ReXamples treats documentation as part of the teaching contract.

The project documentation is organized as follows:

- `docs/manifesto.md` — project identity and intent
- `docs/rules.md` — core project rules
- `docs/style-guide.md` — code style baseline
- `docs/documentation-style.md` — structured in-code documentation rules
- `docs/curriculum.md` — lesson sequence and teaching progression
- `docs/review-template.md` — review checklist
- `docs/review-blockers.md` — hard rejection criteria

## Audience

ReXamples is for:

- developers learning ReAction
- Amiga programmers who want a native GUI reference
- C programmers looking for small and disciplined AmigaOS examples
- developers who value correctness, maintainability, and system integration

## Identity

ReXamples is an independent native ReAction teaching repository.

Its purpose is not comparison, emulation, or adaptation.  
Its purpose is to teach **ReAction on its own terms**, in a form that is clear,
disciplined, and genuinely useful for AmigaOS development.

## Contributing

Contributions are welcome, but ReXamples accepts teaching-quality native examples only.

Please read:

- `CONTRIBUTING.md`
- `docs/manifesto.md`
- `docs/rules.md`
- `docs/style-guide.md`

before submitting changes.

## License

See repository license information.
