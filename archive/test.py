from enum import Enum

# SCANNER

source = input()

tokens = []

class TokenType(Enum):
    LEFT_PAREN = "("
    RIGHT_PAREN = ")"
    DOT = "."
    STRING = "str"
    LAMBDA = "lambda"
    
class Token:
    def __init__(self, tokentype, val):
        self.tokentype = tokentype
        self.val = val
        
    def __repr__(self):
        return str(self.tokentype) + (f" {self.val}" if self.val else "")

current = 0

def scanString():
    global current
    start = current
    while current+1 < len(source) and source[current+1].isalnum():
        current += 1
    return source[start:current+1]
    
while current < len(source):
    char = source[current]
    print(char, current)
    match char:
        case "(":
            tokens.append(Token(TokenType.LEFT_PAREN, None))
        case ")":
            tokens.append(Token(TokenType.RIGHT_PAREN, None))
        case ".":
            tokens.append(Token(TokenType.DOT, None))
        case " ":
            pass
        case _:
            string = scanString()
            if string == "lambda":
                tokens.append(Token(TokenType.LAMBDA, None))
            else:
                tokens.append(Token(TokenType.STRING, string))
    current += 1
print(tokens)
    
# RECURSIVE PARSER

class Expr:
    def getVars(self, out=[]):
        print("WRONG")

class Var(Expr):
    def __init__(self, id: str, token):
        self.id = id
        self.token = token
        
    def evaluate(self):
        return self
    
    def getVars(self, out: list = []):
        out.append(self)
        
    def __str__(self):
        return f"({self.id})"
    
    def __repr__(self):
        self.__str__()

class Func(Expr):
    def __init__(self, param: Var, definition: Expr):
        self.param = param
        self.definition = definition
    
    def __str__(self):
        return f"(lambda {self.param}.{self.definition})"
    
    def evaluate(self):
        return self
    
    def getVars(self, out: list = []):
        self.definition.getVars(out)
        return out
    
    def __repr__(self):
        self.__str__()

class Appl(Expr):
    def __init__(self, func: Func, arg: Expr):
        self.func = func
        self.arg = arg
        
    def __str__(self):
        return f"({self.func} {self.arg})"
    
    def evaluate(self):
        param = self.func.param
        body = self.func.definition
    
    def getVars(self, out: list = []):
        self.func.getVars(out)
        self.arg.getVars(out)
        
    
    def __repr__(self):
        self.__str__()

current = 0 

def peek() -> Token:
    return tokens[current]

def peeknext():
    return tokens[current+1]

def match(t: TokenType):
    global current
    if (peek().tokentype == t):
        current += 1
        return True
    return False

def consume() -> Token:
    global current
    current += 1
    return tokens[current-1]

def expression():
    global current
    print(tokens[current])
    if match(TokenType.LEFT_PAREN):
        if match(TokenType.LAMBDA):
            
            return function()
        else:
            return apply()
    elif peek().tokentype == TokenType.STRING:
        
        return variable()
    else:
        print("none",consume())
        
def apply():
    print("appl")
    left = expression()
    right = expression()
    consume()
    return Appl(left, right)
        
def function():
    print("func")
    global current
    param = variable()
    consume()
    definition = expression()
    consume()
    return Func(param, definition) # type: ignore

def variable():
    print("var")
    global current
    token = consume()
    return Var(token.val, token)

expr = expression()
print(expr)

# BETA REDUCTION
#print(expr.getVars())