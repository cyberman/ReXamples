# ReXamples Review Template

Use this template for every new lesson, major revision, or pull request.

A ReXamples lesson is reviewed as teaching code first and implementation code second.

## 1. Lesson focus

- What is the lesson goal in one sentence?
- Is the lesson narrow enough to stay clear?
- Does it avoid mixing unrelated concepts?
- Is the lesson small enough to study in one sitting?

### Review note
If the lesson goal cannot be stated simply, the lesson is still too broad.

## 2. Native model

- Does the example teach native ReAction / Intuition / BOOPSI structure directly?
- Does naming reflect native concepts?
- Does the lesson avoid inherited terminology from other GUI systems where it harms clarity?
- Does the structure feel native rather than toolkit-neutral?

### Review note
ReXamples teaches native AmigaOS GUI practice, not transferable abstraction habits.

## 3. Code clarity

- Is the code plain C89?
- Are functions and state easy to follow?
- Is the main control flow visible?
- Is helper code used only where it improves clarity?

### Review note
If the code works but the lesson is hard to read, it is weak teaching code.

## 4. Ownership and lifetime

- Does every acquired resource have an obvious owner?
- Are acquisition and release both visible?
- Are objects disposed explicitly?
- Are libraries, classes, ports, signals, buffers, and other resources handled clearly?

### Review note
A reviewer should be able to point at every resource and say who releases it.

## 5. Lifecycle and failure handling

- Is initialization structured and readable?
- Are failure paths visible?
- Does the lesson roll back partial initialization correctly?
- Is cleanup complete and deterministic?
- Is shutdown part of the lesson rather than hidden boilerplate?

### Review note
A lesson that ignores failure behaviour teaches the wrong habit.

## 6. Event flow

- Is the event loop visible?
- Are dispatch and exit conditions easy to follow?
- Can a reader trace an event from arrival to effect?
- Is event-driven structure clear without guesswork?

### Review note
If event handling feels magical, the lesson is too opaque.

## 7. Documentation quality

- Does the local README explain the lesson clearly?
- Does the code document purpose, ownership, and failure behaviour where needed?
- Are comments useful rather than repetitive?
- Is the lesson understandable from both code and README together?

### Review note
Documentation is part of the teaching contract.

## 8. Reference quality

- Would you recommend this lesson as a starting point?
- If copied into a real project, would it teach good habits?
- Does it avoid normalizing weak cleanup, vague naming, or hidden side effects?
- Is it trustworthy as native reference code?

### Review note
ReXamples is not demo code. It is reference teaching code.

## 9. Decision

### Approve when
- the lesson is focused
- native structure is clear
- ownership is explicit
- cleanup is complete
- event flow is understandable
- the lesson is trustworthy as reference code

### Request changes when
- the lesson is too broad
- ownership is unclear
- lifecycle is weak
- event flow is hidden
- documentation is insufficient
- the lesson works but teaches bad habits

