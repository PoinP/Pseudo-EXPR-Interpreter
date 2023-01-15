@page languagespecifics Language Specifics
@tableofcontents

@section variable Declaring a variable
```
myvar = 5
```
You can assign every supported in the language expression to a variable.

If you want to read to a variable you can do the following:
```
read anothervar
```
Notice that there is no prior mention about `anothervar`.

> Note: Variables are only lowercase words

@section function Declaring a function
```
FUNC[x] = x + 10
```
You can assign to a function every supported in the language expression as well.
If a parameter matches with a variable in the expressen then the passed argument
when invoking the function will be used.

> Note: Only single parameter functions are supported. You can not make a function without a parameter or with mutiple parameters

> Note: Functions are only uppercase words.

@section comments Comments
There are two kinds of comments in the language.

- The first one is a single lined comment
```
// I am a single lined comment
```
- The second one is a multilined comment
```
/* Woah
   I am on mutiple
   lines*/
```

@section logic Logical operators
The language supports logical or and logical and operators.
```
istrue = 1 or 0 and 0 or 1
```

As you can see the language uses `or` and `and` instead of the widely used `&&` and `||` operators.

@section conditionals Conditional operators
There are two types of conditional operators you can use.
You can use the classic if-else operator or the ternary operator

> Note if-else operator can act as both an expression and an instruction while the ternary operator can only act as an expression

- Ternary operator
```
myvar = 1 > 2 ? 10 : 100
```
- If-Else as an expression
```
myvar = if 1 > 2 then 10 else 100
```
- If-Else as an Instruction
```
if 10 > 5 then print 8 else x = 50
```

@section comparison Comparison operators
You can use the following comparisson and equality operators:
`>`, `<`, `>=`, `<=`, `==` and `!=`

@section io I/O operators
The only supported operators are `read` and `print`.
They work directly with the console I/O.

- `read` takes a variable as a parameter.
- `print` takes an expression as a parameter

@section loops Loops
The language supports only while loops. They have the following syntax:

```
while {expression} do
    {instruction 1}
    {instruction 2}
    {instruction 3}
    ...
done
```