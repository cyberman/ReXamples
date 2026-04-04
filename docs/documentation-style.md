# ReXamples Documentation Style

ReXamples uses structured in-code documentation to make teaching code easier to read,
easier to review, and easier to promote into future reference material.

Documentation in ReXamples is not decorative prose.
It is part of the teaching contract.

## 1. Purpose

In-code documentation should make four things obvious:

- what a function or definition is for
- what it owns or affects
- what happens on failure
- where it belongs in the lifecycle

If the reader still has to guess those things, the documentation is too weak.

## 2. Scope

Document what matters.

This includes:

- non-trivial functions
- resource-owning functions
- initialization and shutdown functions
- event handlers and dispatch functions
- state structs
- important IDs and constants
- non-obvious design decisions

Do not document trivial statements line by line.

## 3. Style

Documentation should be:

- technical
- concise
- structured
- written in English
- close to the code it describes

Comments must reduce uncertainty.

Comments that merely restate the code add noise and should be removed.

Weak:

```c
/* Open the library. */
````

Better:

```c
/* Open required classes before object construction so partial
 * initialization can be rolled back linearly on failure.
 */
```

## 4. Documentation as contract

Function documentation should describe the practical contract between caller and callee.

Make clear:

- what the function does
    
- what the caller must provide
    
- what the function returns
    
- what ownership changes, if any
    
- what side effects occur
    
- what happens when the function fails
    

In native AmigaOS code, this is especially important because ownership, lifetime,  
and side effects are often central to correctness.

## 5. Preferred function fields

For important functions, document these fields when they apply:

- Purpose
    
- Inputs
    
- Returns
    
- Ownership
    
- Side effects
    
- Failure behaviour
    
- Notes
    

Not every function needs every field, but ownership and failure behaviour should  
be treated as first-class concerns whenever they matter.

## 6. Preferred function format

Use a simple block format that remains readable in plain source form and is easy  
to adapt into future reference documentation.

Example:

```c
/*****************************************************************************
 *
 *  App_Init
 *
 *  Purpose:
 *      Initialize application state, open required libraries and classes,
 *      create the window object tree, and prepare the main event loop.
 *
 *  Inputs:
 *      app - Pointer to application state structure.
 *
 *  Returns:
 *      Non-zero on success, zero on failure.
 *
 *  Ownership:
 *      On success, the application structure owns all acquired resources.
 *      On failure, all partially acquired resources are released before return.
 *
 *  Side effects:
 *      Modifies the application state structure.
 *
 *  Failure behaviour:
 *      Rolls back all completed initialization steps before returning failure.
 *
 *  Notes:
 *      Establishes the top-level application lifetime.
 *
 *****************************************************************************/
```

## 7. Structs, IDs, and constants

Document structs and definitions that carry teaching value.

This includes:

- application state structs
    
- window state structs
    
- ownership-bearing structs
    
- gadget IDs
    
- menu IDs
    
- non-obvious constants
    
- example-specific conventions
    

The purpose of such documentation is not verbosity.  
It is to make structure and intent visible.

## 8. Lifecycle and event comments

Lifecycle and event flow are core parts of native ReAction teaching code.

Documentation should help the reader identify:

- where initialization begins
    
- where ownership begins
    
- where the event loop takes over
    
- where shutdown begins
    
- where cleanup is guaranteed
    

For event handlers and dispatch code, comments should clarify:

- what event is being handled
    
- how it is decoded
    
- what state may change
    
- whether UI updates, resource changes, or shutdown may follow
    

If control flow is non-trivial, the reader should not have to reverse-engineer intent from code alone.

## 9. Documentation efficiency

ReXamples prefers documentation that does more than one job.

Structured in-code documentation should be:

- useful to the reader in source form
    
- useful during review
    
- inexpensive to adapt into future reference documentation
    

This reduces documentation friction and makes disciplined documentation easier to maintain.

## 10. Autodoc readiness

ReXamples does not require full classic autodoc formatting for every source file,  
but documentation should be structured so it can be promoted into future reference  
material without major rewriting.

That means:

- stable section names
    
- clear function responsibility
    
- explicit ownership language
    
- explicit failure behaviour
    
- no vague prose
    

Autodoc-friendly structure is valuable because it turns essential in-code documentation  
into reusable material instead of one-off commentary.

## 11. Final rule

If a function matters enough to teach, it matters enough to document properly.

---

