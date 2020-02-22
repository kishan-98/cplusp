# CFG for C+P

C+P is a hybrid programming language inspired from C++ and Python
- Simplicity of Python language
- Reliability of C++ language

Following is the Context Free Grammar for C+P programming language:

<pre>
program: statement_list;

statement_list: statement statement_list | statement;

statement:      declaration_statement
            |   assignment_statement
            |   expression
            |   loops
            |   control_statement
            |   break
            |   return
            |   function_declaration
            |   function_call
            |   io_statements;

operators: unary | comparison | arithmatic | logical | assignment;

unary: & | | | ! | ^ | << | >>;

comparison: == | != | <= | >= | < | >;

arithmatic: / | * | + | - | %;

logical: && | ||;

assignment: =;

data_type: int | uint | char | bool;

variable: [a-zA-Z]+[a-zA-Z0-9_]*;

value: [a-zA-Z0-9]+;

variable_list: variable variable_list | variable;

declaration_statement: data_type variable | data_type variable assignment value;

assignment_statement: variable = expression;

expression: expression operators expression | variable | value;

loops: for variable = value, value {statement} | for variable = value, value, value {statement} | while(expression) {statement};

control_statement: if(expression) {statement} | if(expression) {statement} else {statement} | if(expression) {statement} elif(expression) {statement} else {statement} | (expression) ? statement : statement;

parameter_list: data_type variable parameter_list | data_type variable;

function_declaration: data_type func_name(parameter_list) {statement};

arguments: variable arguments | variable | ε;

func_name: variable;

function_call: func_name(arguments);

io_statements: print(expression) | print(value) | scan(variable_list) | read(filename) | write(filename);

</pre>

The _unary_, _comparison_, _arithmatic_, _logical_,  _assignment_, _data_type_, _variable_ and _value_ are the micro-syntax in the CFG of C+P and the rest are macro-syntax.
