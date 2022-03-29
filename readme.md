# Concat
### Concatenative Imperitive language

A language that could be considered similar to Forth or [Porth](https://gitlab.com/tsoding/porth)

### Future Features
- [ ] Variables, Constants and continuous Memory access
- [ ] Functions

### Currently being added
```
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
Prints a difference interpreation of the value at the top of the stack depending on its value
    int     --  ascii representation
    bool    --  int representation
    char    --  int representation
    string  --  first character
```

### Arithmetic

```
+ ( a b -- b+a )
Pop the top two values and then push the sum

- ( a b -- b-a )
Pop the top two values and then push the difference

* ( a b -- b*a )
Pop the top two values and then push the product

/ ( a b -- b/a )
Pop the top two values and then push the division

mod ( a b -- b%a )
Get the Modulos of the top two values
```

### Condtionals
```
== ( a b -- a==b )
Pop the top two values and then push a == b

!= ( a b -- a!=b )
Pop the top two values and then push a != b

> ( a b -- a>b )
Pop the top two values and then push a > b

< ( a b -- a<b )
Pop the top two values and then push a < b

>= ( a b -- a>=b )
Pop the top two values and then push a >= b

<= ( a b -- a<=b )
Pop the top two values and then push a <= b
```

### Stack Operations
```
dup ( a -- a a )
Duplicate the value at the top of the stack

swap ( a b -- b a )
Swap the top two elements on the stack

over ( a b -- a b a )
Dup the second element in the stack

rot ( a b c -- b c a )
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

### Loops
```
while <condition> do 
    <body...> 
endwhile

e.g.
0 while dup 10 <= do
    dup print cr
    1 +
endwhile

Print the values 0 - 10
```

### Conditionals
```
if <condition> then
    <body...>
elseif <condition> then
    <body...>
else
    <body...>
endif
```

### Memory
```
mem <name> <size> endmem
Whatever the value at the top of the stack is will get used for mem size
Whenever 'name' gets used in the program the memory address is placed at the top of the stack

! ( address value -- )
Writes the 4 byte value into the place in the memory

@ ( address -- value)
Reads the 4 byte value from address onto the stack

!8 ( address value -- )
Writes the 1 byte value into the place in the memory

@8 ( address -- value)
Reads the 1 byte value from address onto the stack

+! ( address value -- )
Adds value to the value at address
-! ( address value -- )
subtracts value from the the value at address
*! ( address value -- )
Multiplies the value at address by value
/! ( address value -- )
Divides the value at address by value

// 1 Byte variants of above
+!8 ( address value -- )
-!8 ( address value -- )
*!8 ( address value -- )
/!8 ( address value -- )
```

### Strings
```
"<string>"
When using string the pointer to the start of the string is placed at the top of the string
This pointer can be treated like a memptr to access different parts of the string
```

### Casting
```
"cast(int)"
"cast(bool)"
"cast(char)"
"cast(string)"

Using any of the casts will take the value at the top of the stack and try to cast it to
the requested value
```
