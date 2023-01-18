# Pseudo-EXPR-Interpreter

## Base
EXPR is a language which is used by students.
The programs of this language consist of many rows
of operations for assigning and printing values.
It supports only unsigned integers that span from 0
to the max value of `unsigned long int`.
The assignment operators are two.
 - Assigning a number or an expression to a variable
 - Declaring a function with only 1 parameter
The `read` operators allows the user to input a value to
a variable from the console, while the `print` operator
prints an expression to the console.

## Extension
The Pseudo EXPR Interpreter changes some of the syntax
described above and also adds new functionality.

It adds a
 - Support for recursive functions
 - Support for arbitrary sized unsigned integers
 - Support for conditions(if-else, ternary operator)
 - Support for loops(while loop)
 - Support for conditional operators(and, or)
 - Support for equality and comparison operators

### Syntax of the Pseudo EXPR language

```
Line → Instruction
Instruction → Var = Expr | Fun[Var] = Expr | if LogicalOr then Instruction else Instruction |
while LogicalOr do (Instruction)* done | print Expr | read Var

Fun → A | B | … | Z | AFun | BFun | … | ZFun
Var → a | b | … | z | aVar | bVar | … | zVar
Num → 0 | … | 9 | 1Num | … | 9Num

Expr       → If
If         → if LogicalOr then Expr else Expr | Ternary
Ternary    → LogicalOr ? Ternary : Ternary | LogicalOr
LogicalOr  → LogicalOr or LogicalAnd | LogicalAnd
LogicalAnd → LogicalAnd and Equaliry | Equality
Equality   → Equality == Comparison | Equality != Comparison | Comparison
Comparison → Comparsion (> | >= | <= | <) Arithmetic | Arithmetic  
Arithmetic → Arithmetic + Factor | Arithmetic - Factor | Factor
Factor     → Factor * Unary | Factor / Unary | Factor % Unary | Unary
Unary      → !Unary | Primitive
Primitive  → Var | Num | (Expr) | Fun[Expr]
```

# You can check the docs branch for more information on the language!
