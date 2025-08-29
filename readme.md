# SLUG

Slug is a minimalist, interpreted programming language written in C. It features a lexical analyzer, parser, environment management, and an interpreter capable of evaluating expressions and statements including control flow, functions, and recursion.

Designed to be simple yet powerful, Slug supports higher order functions, closures, conditionals, and looping constructs. Through examples such as the factorial function, the deeply recursive Ackermann function, and advanced recursion patterns, Slug demonstrates its ability to express any computable function and is therefore **Turing complete**.

Slug is suitable for exploring language interpreter design, functional programming concepts, and computational theory in a compact and clear environment.

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

## Slug Language: Features and Turing Completeness Proof

This document describes several scripts and core language features of the Slug interpreted programming language, demonstrating its capabilities and proving it is Turing complete.

---

## Core Language Features

### Arithmetic, Variables, and Conditionals (`scripts/core_language_test.slg`)
```
{
    let a = 10;
    const b = 5;
    let c = a + b * 2;
    outn(c);

    if (c > 15) {
        outn(1);
    } elif (c > 10) {
        outn(2);
    } else {
        outn(3);
    }

    let count = 0;
    while (count < 5) {
        outn(count);
        count = count + 1;
    }

    let multiply = func (x, y) => {
        x * y;
    };

    let result = multiply(4, 5);
    outn(result);

    let outer = func (x) => {
        func (y) => {
            x + y;
        };
    };
    let add5 = outer(5);
    outn(add5(10));

    let val = true && false || true;
    if (val == true) {
        outn(42);
    }
}
```

- Supports variables (mutable `let` and immutable `const`).
- Arithmetic operations and operator precedence.
- Conditional `if`, `elif`, and `else` blocks.
- Loops using `while`.
- Function declarations, including closures and nested functions.
- Boolean logic operations.

---

### Simple Recursion (`scripts/turing.slg`)
```
let factorial = func(n) => {
    if (n == 0) {
        1;
    } else {
        n * factorial(n - 1);
    }
};

let result = factorial(5);
outn(result);
```
- Demonstrates recursion and function calls.
- Computes factorial demonstrating non trivial mathematical functions.

---

### Anonymous Functions (`scripts/anon_func.slg`)
```
let increment = func(x) => { x + 1; };
outn(increment(7));
```
- Supports anonymous functions (lambdas).
- Direct application of function values.

---

### Advanced Recursion: Ackermann Function (`scripts/ackermann.slg`)
```
let ackermann = func(m, n) => {
    if (m == 0) {
        n + 1;
    } else {
        if (n == 0) {
            ackermann(m - 1, 1);
        } else {
            ackermann(m - 1, ackermann(m, n - 1));
        }
    }
};

let result = ackermann(3, 7);
outn(result);
```
- A classic computationally intensive recursive function.
- Demonstrates deep recursion and complex control flow capabilities.
- Running this successfully shows the language supports computations beyond simple loops.

---

### Loop Control without `break` (`scripts/unbounded_loop.slg`)
```
let i = 0;
let done = false;
while (!done) {
    i = i + 1;
    if (i == 10) {
        outn(i);
        done = true;
    }
}
```
- Demonstrates looping and condition controlled termination without `break`.
- Shows language handles boolean variables and `while` loops effectively.

---

### Higher Order Functions and Closures (`scripts/higher_order_functions_and_closures.slg`)
```
let apply = func(f, x) => {
    f(x);
};

let square = func(n) => {
    n * n;
};

outn(apply(square, 5));
```
- Supports functions as first class values.
- Demonstrates passing functions as arguments and returning values.

---

### Tail Recursive Function (`scripts/recursion.slg`)
```
let fact = func(n, acc) => {
    if (n == 0) {
        acc;
    } else {
        fact(n - 1, acc * n);
    }
};

outn(fact(5, 1));
```
- Shows support for tail recursion style functions with parameters as accumulators.
- Demonstrates ability to write efficient recursive code patterns.

---

### Truth Table Testing Script (`scripts/truth_table_testing.slg`)
```
let not = func(b) => {
        if (b) { false; } else { true; }
};

let and = func(a, b) => a && b;
let or = func(a, b) => a || b;

let get_bit = func(num, bit) => {
        if ((num / bit) % 2 == 0) { false; } else { true; }
};

let truth_table = func(f) => {
        let i = 0;
        while (i < 4) {
                let a = get_bit(i, 2);
                let b = get_bit(i, 1);
                outn(f(a, b));
                i = i + 1;
        }
};

let de_morgan_1 = func(a, b) => {
        not(and(a, b)) == or(not(a), not(b));
};

truth_table(de_morgan_1);
```
- Defines basic boolean logic functions `not`, `and`, `or`.
- Programmatically generates all possible Boolean input pairs using `get_bit`.
- Automates truth table testing with the `truth_table` function by evaluating `f` over all input pairs.
- Tests De Morgan's Law, outputting `true` or `false` for each case.

---

### De Morgan's Law Script (`scripts/demorgan_law.slg`)
```
let not = func(b) => {
    if (b) { false; } else { true; }
};

let and = func(a, b) => { a && b; };
let or = func(a, b) => { a || b; };

let de_morgan_1 = func(a, b) => {
    not(and(a, b)) == or(not(a), not(b));
};

outn(de_morgan_1(true, true));
outn(de_morgan_1(true, false));
outn(de_morgan_1(false, true));
outn(de_morgan_1(false, false));
```
- Implements the boolean negation, AND, and OR functions.
- Defines the De Morgan equivalence function `de_morgan_1`.
- Outputs the result of the equivalence on all possible two boolean inputs manually.
- Demonstrates the fundamental logic identity holds for every input.

---

### Gödel Halting Paradox Script (`scripts/godel_halt_paradox.slg`)
```
let halts = func(prog, input) => {
    false;
};

let diagonal = func(f) => {
    if (halts(f, f)) {
        diagonal(f);
    } else {
        0;
    }
};

let paradox = func(haltFunc) => {
    diagonal(haltFunc);
};

let result = paradox(diagonal);

outn(result);
```
- Defines a false halting oracle `halts` always returning `false`.
- Defines `diagonal` applying oracle to function applied to itself, recursing infinitely if halting predicted.
- Sets up `paradox` to apply `diagonal` to a halting oracle itself.
- Outputs `0`, illustrating the contradiction proving halting problem undecidable.


## Proof of Turing Completeness

Slug language supports:

- **Mutable state (variables)**
- **Conditional branching** (`if`, `else`, `elif`)
- **Loops** (`while`)
- **Function definitions, recursion, and higher order functions**

By these properties alone, it meets the minimal criteria to be Turing complete.

Furthermore, the implementation of:

- The **factorial function** (`scripts/turing.slg`),
- The **Ackermann function** (`scripts/ackermann.slg`), and
- Tail recursion with accumulators (`scripts/recursion.slg`),

demonstrate the ability to compute any computable function given enough memory and time.

These scripts prove Slug can simulate the core logic patterns of a Turing machine, thereby being **Turing complete**.

---

## Conclusion

Slug is a fully featured interpreted language with all fundamental capabilities necessary for general computation.

The advanced recursion and higher order function examples prove its ability to express any computable function, fulfilling the definition of Turing completeness.

---

## License

This project is provided under the GPL3 License.

---
