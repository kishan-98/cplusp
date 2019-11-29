# C+P: Language Manual
___
> Kishan Sangani (201501053)
___

Compiler for a hybrid C++ and Python like language `C+P`

### Description
- Keywords : `int`, `uint`, `char`, `bool`, `print`, `read`, `if`, `elif`, `else`, `while`, `for`
- Data Types : Signed and unsigned integers, character, boolean and array of integers (1D/2D).
  `int data, array[100], arr[5][5];`
  `uint sum;`
  `char ch;`
  `bool bl;`

- Operators(in order of decreaseing precedence): Unary/Binary(&,|,!,^,<<,>>), Arithmetic((),/,*,+,-,%), Comparison(==,!=,<=,>=,<,>),Logical(&&,||),Assignment(=)

- Identifiers contains only alpha-numerical characters starting with an alphabet.
    - Regex: [a-zA-Z]+[a-zA-Z0-9]*
    - Eg. `abc`, `AbCd`, `A9c5`

- Expressions
  - C like expressions
  - Expressions can have arithmetic operators and conditional operators : Both are treated same.
  - Conditional operation returns 1 for true condition and 0 for false condition.
  - Some Examples :
    - `a + b`
    - `a + (b + c) / (x + y) % p`
    - `(a == 0) + (b < (a + d))`

- for loop
  - `for i = a, b {}` is same as `for(i = a, i < b, i++)` in C.
  - `for i = a, b, x {}` is same as `for(i = a, i < b, i+=x)` in C.

- if-else statement
  - `if (expression) {}`
  - `if (expression) {} else {}`
  - `if (expression) {} elif (expression) {} else {}`

- while statment
  - `while (expression) {}`

- print/scan/read/write
  - `print(val)`       // New line at the end of each print
  - `print(expression)`       // New line at the end of each print
  - `scan(variable_list)`
  - `read(path)`
  - `write(path)`

- Functions
    - `return_type func_name(parameters) {}`
    - `int sum(int a, int b) {...}`
    - `int fact(int n) { ... fact(n-1)*n ...}`

- Tertiary comparison
    - `(expression) ? true_val:false_val`
