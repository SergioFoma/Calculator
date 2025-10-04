PUSH 2 ; ADD Element
PUSH 2
MUL ; Do multiplication
PUSH 4
PUSH 1
PUSH -10
MUL
MUL
SUB ; Do subtraction
OUT ; Get result

PUSH 10
PUSH 20
PUSH 30
PUSH 40
PUSH 50

POPR RAX ; Put in regs
POPR RDX
POPR RBX
POPR RCX

PUSHR RAX ; from regs to stack
PUSHR RDX
PUSHR RCX
PUSHR RBX

IN

HLT ; Stop
