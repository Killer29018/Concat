# SBIMCL
### Stack Based Interpreted Maybe Compiled Language

A language that could be considered similar to Forth or [Porth](https://gitlab.com/tsoding/porth)

### Future Features
- [x] Save the intermediary Opcode to a binary format to speed up compilation (Preferred over ASM)
- [ ] Possibly Write ASM code to compile the language ()
- [ ] Variables, Constants and continuous Memory access
- [ ] Functions

### Currently being added
```
@8 !8 for 8 bit access to memory
<< and >> bitwise operators
+! -! *! /! for writing directly to the pointer
```

## Current Instructions
### Format
```
( 'input' -- 'output' )
'input' represents the current values at the top of the stack
'output' represents what the top of the stack will look like after the operation
```
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

mod ( a b -- b%a )
Get the Modulos of the top two values
```

### Condtionals
```
== (a b -- a==b)
Pop the top two values and then push a == b

!= (a b -- a!=b)
Pop the top two values and then push a != b

> (a b -- a>b)
Pop the top two values and then push a > b

< (a b -- a<b)
Pop the top two values and then push a < b

>= (a b -- a>=b)
Pop the top two values and then push a >= b

<= (a b -- a<=b)
Pop the top two values and then push a <= b
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

### Comments
```
//
Single Line Comment

/* .... */
Multi Line Comment
```
### Macros
````
macro <name> <body...> end
Whenever 'name' occurs in the program it gets expanded into <body...>
````

### Planned Commands
```
```
