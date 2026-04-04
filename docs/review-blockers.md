# ReXamples Review Blockers

A lesson must not be approved if any of the following is true.

## 1. Unclear lesson

- the lesson goal is vague
- multiple major concepts are mixed without need
- the example cannot be explained in one sentence

## 2. Non-native teaching model

- naming teaches foreign GUI concepts instead of native ones
- structure preserves non-native mental models where they obscure ReAction
- the lesson presents GUI design as toolkit-neutral when the native model matters

## 3. Weak ownership

- ownership of resources is unclear
- acquired resources do not have obvious release paths
- cleanup responsibility is implicit or guess-based

## 4. Weak lifecycle

- initialization is difficult to follow
- failure rollback is missing or unreliable
- shutdown is incomplete
- cleanup is partial, hidden, or accidental

## 5. Hidden event flow

- control flow depends on guesswork
- event handling is buried behind unnecessary abstraction
- exit conditions are unclear
- the reader cannot trace an event from arrival to effect

## 6. Poor teaching quality

- the lesson is harder to understand than necessary
- comments do not clarify purpose or contract
- README and source teach different lessons
- the example normalizes shortcuts instead of disciplined structure

## 7. Untrustworthy reference quality

- the code works but teaches bad habits
- naming is vague
- ownership is sloppy
- cleanup is weak
- the lesson is not safe to recommend as a reference starting point

## Final rule

If a lesson runs but teaches the wrong structure, it is still a failed ReXamples lesson.

