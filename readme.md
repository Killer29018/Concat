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

. ( a -- )
Print the value at the top of the stack as its ascii representation
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

### Stack Operations
```
dup ( a -- a a )
Duplicate the value at the top of the stack

swap ( a b -- b a)
Swap the top two elements on the stack

over ( a b -- a b a)
Dup the second element in the stack

rot ( a b c -- b c a)
Rotate the top 3 elements in the stack
```

### Planned Commands
```
mod

if <condition> do <body> <else> <condition> do <endif>
while <condition> then <body> endwhile

macro <body> endmacro
```
