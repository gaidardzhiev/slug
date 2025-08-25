# SLUG

A minimalist programming language interpreter written in C, featuring lexical analysis (tokenizer), parsing (parser), environment management, and interpretation of expressions and statements including control flow and functions.

---

## Features

- Lexical tokenizer supporting keywords, identifiers, literals, operators, and symbols.
- Recursive descent parser producing an Abstract Syntax Tree (AST).
- Environment model with variable scoping and constants.
- Primitive data types: numbers and booleans.
- Functions.
- Control flow constructs: `if`, `elif`, `else`, `while`.
- Built in output function `outn` for printing values.
- Runtime error handling with descriptive messages.
- Interpreter that evaluates the AST directly.

---

## Language Overview

The language supports:

- Variable declarations with `let` and `const`.
- Arithmetic operators (`+`, `-`, `*`, `/`, `%`).
- Comparison and equality operators (`<`, `<=`, `>`, `>=`, `==`, `!=`).
- Logical operators (`&&`, `||`, `!`).
- Function declarations via `func(params) => expression`.
- Control structures: `if`, `elif`, `else`, `while`.
- Statements end with semicolons `;`.
- Output via `outn(expression);`.

---

## Code Structure

### Tokenizer

Converts source text into tokens for keywords, identifiers, literals, operators, and punctuation.

### Parser

Implements recursive descent to produce an AST representing the program structure, supporting expressions, statements, blocks, and functions.

### AST Nodes & Types

Enumerates node types and expresses program structure:
- Literals (number, boolean)
- Identifiers
- Variable declarations (`let`, `const`)
- Assignments
- Binary and unary operations
- Blocks (`{ ... }`)
- Control flow (`if/elif/else`, `while`)
- Function declarations and calls
- Built in function calls (currently `outn`)

### Runtime Environment

Linked list environment with support for nested scopes, variable binding, constants, and lookup.

### Values

Supports numbers, booleans, functions (closures), and null.

### Interpreter

Walks the AST to evaluate expressions and statements:

- Evaluates literals and identifiers.
- Variable declaration and assignment with constant checks.
- Arithmetic and logical operations with type checking.
- Control flow execution (`if/elif/else`, `while`).
- Function calls with closure environments.
- Built in `outn` prints evaluated values.

Errors (e.g., undefined variables, type mismatches, division by zero) cause the interpreter to print a descriptive message and terminate.

---
