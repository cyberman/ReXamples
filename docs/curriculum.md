# ReXamples Curriculum

ReXamples is a progressive native ReAction learning path for AmigaOS 3.2+.

The goal of the series is not to present disconnected GUI samples.
The goal is to teach native ReAction practice step by step, with clear lessons,
visible lifecycle, explicit ownership, and disciplined event-driven structure.

Each lesson should introduce one core concept and prepare the reader for the next one.

## 1. Learning model

The series is built on four principles:

- start with structure before complexity
- teach one concept at a time
- keep ownership and lifecycle visible
- treat event flow as part of every serious GUI lesson

A lesson is successful when the reader can answer:

- what this example teaches
- what native components it uses
- what it deliberately does not cover
- how it fits into the larger ReAction model

## 2. Lesson order

### 01_Window
Teaches the native lifecycle of a minimal ReAction windowed program.

### 02_Layout
Introduces layout groups as the structural foundation of ReAction interfaces.

### 03_Button
Shows how button gadgets are created, identified, and handled in the event loop.

### 04_CheckBox
Teaches how to work with simple on/off interface state in a native way.

### 05_Chooser
Introduces single-choice selection with a native ReAction selector gadget.

### 06_String
Shows how text input is handled with explicit buffers and clear ownership.

### 07_ListBrowser
Introduces structured list display, entry management, and selection handling.

### 08_RadioChoice
Teaches exclusive choice between multiple options as a distinct UI pattern.

### 09_Image
Shows how visual objects are integrated and managed as real interface resources.

### 10_Slider
Introduces continuous numeric input and value-change handling.

### 11_Space
Shows how layout spacing and visual breathing room are part of interface design.

### 12_Frame
Teaches visual grouping and structural separation without decorative excess.

### 13_Menu_Basic
Introduces native AmigaOS menus as a normal part of the event-driven UI model.

### 14_Menu_Advanced
Builds on the basic menu lesson with richer menu structure and controlled state.

### 15_Gauge
Shows how read-only progress or status display differs from user-controlled input gadgets.

### 16_TextDisplay
Introduces static or structured text presentation beyond simple labels.

### 17_ScrollableLayout
Teaches how larger-than-visible content is structured in a native scrollable interface.

### 18_RegisterPages
Shows how multi-page interfaces are organized with clear page-level structure.

### 19_EventDispatch
Brings the series together by teaching explicit event routing and controlled UI reactions.

## 3. Teaching progression

The curriculum moves through five stages.

### Stage 1: Foundation
Lessons:
- Window
- Layout
- Button
- CheckBox

Purpose:
Teach the basic structure of a native ReAction program:
window lifetime, object hierarchy, simple gadgets, and event-driven interaction.

### Stage 2: Input and selection
Lessons:
- Chooser
- String
- ListBrowser
- RadioChoice
- Slider

Purpose:
Teach how the interface accepts user input, how values are represented,
and how state changes are handled cleanly.

### Stage 3: Visual structure
Lessons:
- Image
- Space
- Frame
- TextDisplay
- Gauge

Purpose:
Teach how interfaces communicate visually through structure, grouping,
feedback, and display-only elements.

### Stage 4: Application-level interaction
Lessons:
- Menu_Basic
- Menu_Advanced
- RegisterPages

Purpose:
Teach larger interface organization and native command paths beyond simple gadgets.

### Stage 5: Composition and control flow
Lessons:
- ScrollableLayout
- EventDispatch

Purpose:
Teach the reader how more complex interfaces are assembled and how explicit
event routing keeps native GUI code understandable and disciplined.

## 4. Scope discipline

Each lesson must remain narrow.

A lesson should:

- teach one core concept
- stay readable in one sitting
- avoid feature creep
- avoid becoming a miniature framework
- make its lifecycle and ownership visible

A lesson becomes weaker when it tries to demonstrate too much at once.

## 5. README role in the curriculum

Each example should include a local `README.md`.

That README should explain:

- the purpose of the lesson
- what the reader will learn
- which native components are used
- what the lesson deliberately does not cover
- key lifecycle points
- typical mistakes
- which lesson comes next

The README and the source must reinforce the same lesson.

## 6. Naming policy

Lesson names should teach native concepts directly.

Prefer names that describe the ReAction or AmigaOS concept being taught.

Do not preserve inherited terminology from other GUI systems when it weakens
clarity or teaches the wrong mental model.

The curriculum exists to teach native ReAction on its own terms.

## 7. Graduation rule

A lesson belongs in ReXamples only if it satisfies all of the following:

- it teaches a clear concept
- it uses native structure
- it keeps ownership visible
- it shows disciplined cleanup
- it keeps event flow understandable
- it is trustworthy as reference code

If an example works but teaches the wrong habits, it does not belong in the curriculum.

## 8. Final standard

ReXamples is not a collection of random examples.

It is a structured native learning path.

Each lesson should make the next lesson easier to understand.
Each lesson should strengthen correct habits.
Each lesson should help define a visible baseline for disciplined ReAction development on AmigaOS 3.2+.

