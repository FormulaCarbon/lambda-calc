a = any letter
_ = any template

templates:
a
(lambda a._) - function definition (lambda input.output)
(_ _) - function application (func arg)

beta reduction:
just replace all instances of the parameter in the function body with the argument and return only the body

methodology:
Parse tokens
Convert to De Brujin indices
Create AST
Do Beta Reduction

language design:

(lambda p1 p2 : p1)             function definition
name FUNCNAME {EXPR}            named expression (must be all caps)
(FUNCNAME a)                    application
out {EXPR}                      print to terminal
all other non-keywords          variables
include {FILENAME}                include stuff from another file

tokens:
left and right parens
left and right brackets
"lambda"
variables
"name"
exprname
"include"
colon
"out"