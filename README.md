# Kaleidoscope
This is the impl from of Kaleidoscope(A toy language) by llvm. ref:https://llvm.org/docs/tutorial/#llvm-tutorial-table-of-contents
Kaleidoscope means beautiful, form, and view. It is a procedural language that allows you to define functions, use conditionals, math, etc.

A compiler contains lexer ^^^^. The above parts will be discussed in order.
## Lexer
The effect of a lexer is to spilt number, variable, identifier and so on. It is the first part of the compiler.
We can identify the number and variable(beginning with a letter and only contains number and alpha) by ASCII.
