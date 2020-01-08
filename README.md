# The Z Programming Language

Z is a functional, Turing-complete programming language that uses paradigms of lambda calculus.

## Generic Types

### Integer

### Set

## Built-in Constructs

### S (Global Stack)

`S` is **the only global variable** in the whole Z program code. It is mainly to use as an outout buffer.

### Z (Z-combinator)

Grammar: `Z <lambda-decl>`

`Z` is used to recursively call lambda functions. As there are no iterative loops in **Z**, recursive calls are the way we do iteration :)

### K (Kombine)

Grammar: `K <lambda-identifier|lambda-decl|Z-expression|K-expression>+ <int|set|S>`

Lambda function bodies in **Z** consists of exactly one expression. So, we need to *'kombine'* the expressions as a chain. The `K` combinator does this.

```
K f g h t x == f(g(h(t x)))
```

### Lambda

Lambda functions are what the **Z** language is built on. Lambda functions,

* Can be anonymous
* Can have zero number of parameters
* Can only be passed `integer`, `set` type of constant or variables, or the `S` as arguments.

## Built-in Functions

### ' (Put one char)

Grammar: `' <constant|integer|set|S>`

Puts the ASCII character equivalent of the given integer argument's value to standard output. If the given argument is of set type or `S`, puts the ASCII character equivalent of the top/first element.

If `S` is given as argument, this function pops the top item from `S`. No such action is taken if a set is given.

Usage:
```
' S
\? (x) (' x)
\? () (' 10)
```

### ^ (Peek)

Grammar: `^ <integer|set|S>`

Returns the first element of given set. If the given argument is `S`, returns the top item. Otherwise, returns the value of the integer argument itself.

Usage:
```
^ S
\y (x) ((!= 0 (^ x)) (' x))
```

### << (Pop from && push to)

Grammar: `<integer|set|S> << <constant|integer|set|S>`

Pops one item from `y` and pushes it to `x`. If `y` is of type integer, takes its value and pushes it to `x`. If `x` is type integer, sets its value to taken integer value.