# SBIMCL
### Stack Based Interpreted Maybe Compiled Language

A language that could be considered similar to Forth or [Porth](https://gitlab.com/tsoding/porth)

## Current Instructions

### IO
```
print ( a -- )
Directly print what the value at the top of the stack is

cr ( -- )
Print a Carriage Return
```

### Arithmetic

```
+ ( a b -- b+a )
Pop the top two values and then push the sum

- (a b -- b-a)
Pop the top two values and then push the difference

* (a b -- b*a)
Pop the top two values and then push the product

/ (a b -- b/a)
Pop the top two values and then push the division

```
