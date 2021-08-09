
# TODO

Things to be done for next milestone.  
Each thing on the checklist needs to be implemented fully, from lexer to parser to Bytecode and VM execution.  
e.g.: integers need to be parsed, converted, put in to a chunk, and executed.  

## Version goals

v 0.0.3

- [x] Add bool and None `True`, `False`, `None`
- [x] Add runtime truthiness evaluation
- [x] Add unary Not
- [ ] Add comparisons operators `==`, `!=`, `<`, `>`, `<=`, `>=`
- [ ] Make sure correct operation precedence is implemented  
      (see: https://docs.python.org/3/reference/expressions.html#operator-precedence)


v 0.0.2:

- [x] Add floats (c++ doubles under the hood)
- [x] Base math ops float and int
- [x] Add integer divide `//`
- [x] Add unary negative
- [x] Add power `**`


v 0.0.1:

- [x] add base types `interger` and `float`
- [x] add operations `+ - * /`
- [x] add parentheses
- [x] Correct math order parsing

## Should be done at some point

- [ ] output artifacts from docker build
