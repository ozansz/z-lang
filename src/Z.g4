grammar Z;

// Z Parser

program
    : LPAREN expression RPAREN;

expression
    : atomic_expression
    | k_expression
    | function_call_expression
    | conditional_expression;

atomic_expression
    : constant_statement 
    | ID
    | S;

function_call_expression
    : LPAREN function_identifier function_call_arg_list RPAREN;

function_identifier
    : ID
    | PUTCH
    | PEEK
    | POPUSH
    | SUB
    | ADD
    | MUL
    | DIV;

function_call_arg_list
    : expression+;

conditional_expression
    : conditional_expression_condition LPAREN expression RPAREN (LPAREN expression RPAREN)?;

conditional_expression_condition
    : LPAREN conditional_operator conditional_operand conditional_operand RPAREN;

conditional_operator
    : EQ
    | NEQ
    | GT
    | LT
    | LEQ
    | GEQ;

conditional_operand
    : atomic_expression
    | function_call_expression;

k_expression
    : K k_expression_param+ atomic_expression;

k_expression_param
    : LPAREN k_expression RPAREN
    | LPAREN z_expression RPAREN
    | LPAREN lambda_decl RPAREN
    | ID;

z_expression
    : Z lambda_decl;

lambda_decl
    : BACKSLASH lambda_identifier lambda_param_list lambda_body;

lambda_identifier
    : QUESTIONMARK
    | ID;

lambda_param_list
    : LPAREN lambda_param* RPAREN;

lambda_param
    : (lambda_param_integer | lambda_param_set);

lambda_param_integer
    : ID;

lambda_param_set
    : VECTID;

lambda_body
    : LPAREN expression RPAREN;

constant_statement
    : set_statement
    | INT;

set_statement
    : LCB INT (COMMA INT)* RCB;

// Z Lexer

EQ:
    '=';

NEQ:
    '!=';

GT:
    '>';

LT:
    '<';

LEQ:
    '<=';

GEQ:
    '>=';

BACKSLASH:
    '\\';

QUESTIONMARK:
    '?';

COMMA:
    ',';

LCB:
    '{';

RCB:
    '}';

LPAREN:
    '(';

RPAREN:
    ')';

S:
    'S';

Z:
    'Z';

K:
    'K';

PUTCH:
    '\'';

PEEK:
    '^';

POPUSH:
    '<<';

SUB:
    '-';

ADD:
    '+';

MUL:
    '*';

DIV:
    '/';

VECTID:
    '#' ID;

ID:
    LowercaseLetter+ ;

INT:
    Digit+ ;

fragment Digit:
    [0-9] ;

fragment LowercaseLetter:
    [a-z] ;

WS:
    [ \t\n\r]+ -> skip ;