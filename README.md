# gbz80-pseudoOps
Set of rgbasm macros and constants to facilitate readability for modern programmers.

I've only just started using ASM, but like most programmers born in the 90s I've gotten used to 
a) Doing things in one step instead of two or three and 
b) camelCase.  
So this is my attempt to simplify the writing and most relevantly the reading of asm code without comprimising the control over optimisation required to run gameboy software as well as it needs to run.

This is a work in progress, so more pseudo-ops will be added as I come across a case for them in the software I write.

## Legend
List of abbreviations, as used by rgbasm in https://rednex.github.io/rgbds/gbz80.7.html

Symbol|Def
------|----------
*r8*  | Any of the 8-bit registers (A, B, C, D, E, H, L).
*r16* | Any of the general-purpose 16-bit registers (BC, DE, HL).
*n8*  | 8-bit integer constant.
*n16* | 16-bit integer constant.
*e8*  | 8-bit offset (-128 to 127).
*u3*  | 3-bit unsigned integer constant (0 to 7).

## The Ops

**ldAny r8, [n16]**
* Store the value at address *n16* into register *r8*.
* Cycles: 5 
* Bytes: 4 
* Flags: None

**ldAny r8, [r16]**
* Store the value pointed to by register *r16* into register *r8*
* Cycles: 3
* Bytes: 2 
* Flags: None

**ldAny [n16], r8**
* Store the value in register *r8* to address *n16*
* Cycles: 5
* Bytes: 4
* Flags: None

**ldAny [r16], r8**
* Store the value in register *r8* to address pointed to by register *r16*
* Cycles: 3
* Bytes: 2
* Flags: None

**ldAny [r16], [r16]**
* Store the value pointed to by register *r16* in to address pointed to by other register *r16*
* Cycles: 4
* Bytes: 2
* Flags: None

**ldAny [r16], [n16]**
* Store the value at address *n16* into address pointed to by register *r16*
* Cycles: 6
* Bytes: 4
* Flags: None

**ldAny [n16], [r16]**
* Store the value at address pointed to by *r16* to address *n16*
* Cycles: 6
* Bytes: 4
* Flags: None

**ldAny [n16], [n16]**
* Store the value at address *n16* into other address *n16*
* Cycles: 8
* Bytes: 6
* Flags: None

**ldAny [n16], n8**
* Store value *n8* to address *n16*
* Cycles: 6
* Bytes: 5
* Flags: None

Loads to an address in IO space
ldhAny [$ff00 + n8], n8
Cycles: 5
Bytes: 4 
Flags: None 

ldhAny [$ff00 + n8], r8
Cycles: 4
Bytes: 3 
Flags: None

ldhAny [$ff00 + n8], [r16]
Cycles: 5
Bytes: 3 
Flags: None 

ldhAny [$ff00 + n8], [n16]
Cycles: 7
Bytes: 5 
Flags: None 

ldhAny: macro
    ld A, \2
    ldh \1, A
endm


Loads from [HL] then increments HL
ldiAny r8, [HL]
Cycles: 3
Bytes: 2
Flags: None

ldiAny [r16], [HL]
Cycles: 4
Bytes: 2
Flags: None

ldiAny [n16], [HL]
Cycles: 6
Bytes: 4
Flags: None

ldiAny: macro
    ldi A, [HL]
    ld \1, A
endm


ORs the bits of two registers, result in A

orAny r8, r8
Cycles: 2
Bytes: 2
Flags: Z=? N=0 H=0 C=0

orAny r8, n8
Cycles: 4
Bytes: 4 
Flags: Z=? N=0 H=0 C=0

orAny r8, [HL]
Cycles: 3
Bytes: 2
Flags: Z=? N=0 H=0 C=0

orAny [r16], r8
Cycles: 3
Bytes: 2
Flags: Z=? N=0 H=0 C=0

orAny [r16], n8
Cycles: 4
Bytes: 3 
Flags: Z=? N=0 H=0 C=0

orAny [r16], [HL]
Cycles: 4
Bytes: 2
Flags: Z=? N=0 H=0 C=0

orAny [n16], r8
Cycles: 5
Bytes: 4
Flags: Z=? N=0 H=0 C=0

orAny [n16], n8
Cycles: 6
Bytes: 5 
Flags: Z=? N=0 H=0 C=0

orAny [n16], [HL]
Cycles: 6
Bytes: 4
Flags: Z=? N=0 H=0 C=0

orAny: macro
    ld A, \1
    or \2
endm


ANDs the bit of two registers, result in A

andAny r8, r8
Cycles: 2
Bytes: 2
Flags: Z=? N=0 H=1 C=0

andAny r8, n8
Cycles: 4
Bytes: 4 
Flags: Z=? N=0 H=1 C=0

andAny r8, [HL]
Cycles: 3
Bytes: 2
Flags: Z=? N=0 H=1 C=0

andAny [r16], r8
Cycles: 3
Bytes: 2
Flags: Z=? N=0 H=1 C=0

andAny [r16], n8
Cycles: 4
Bytes: 3 
Flags: Z=? N=0 H=1 C=0

andAny [r16], [HL]
Cycles: 4
Bytes: 2
Flags: Z=? N=0 H=1 C=0

andAny [n16], r8
Cycles: 5
Bytes: 4
Flags: Z=? N=0 H=1 C=0

andAny [n16], n8
Cycles: 6
Bytes: 5 
Flags: Z=? N=0 H=1 C=0

andAny [n16], [HL]
Cycles: 6
Bytes: 4
Flags: Z=? N=0 H=1 C=0

andAny: macro
    ld A, \1
    and \2
endm


Performs an XOR on any two 8bit registers. Usual flags affected and A set to the result.

xorAny r8, r8
Cycles: 2
Bytes: 2
Flags: Z=? N=0 H=0 C=0

xorAny r8, n8
Cycles: 4
Bytes: 4 
Flags: Z=? N=0 H=0 C=0

xorAny r8, [HL]
Cycles: 3
Bytes: 2
Flags: Z=? N=0 H=0 C=0

xorAny [r16], r8
Cycles: 3
Bytes: 2
Flags: Z=? N=0 H=0 C=0

xorAny [r16], n8
Cycles: 4
Bytes: 3 
Flags: Z=? N=0 H=0 C=0

xorAny [r16], [HL]
Cycles: 4
Bytes: 2
Flags: Z=? N=0 H=0 C=0

xorAny [n16], r8
Cycles: 5
Bytes: 4
Flags: Z=? N=0 H=0 C=0

xorAny [n16], n8
Cycles: 6
Bytes: 5 
Flags: Z=? N=0 H=0 C=0

xorAny [n16], [HL]
Cycles: 6
Bytes: 4
Flags: Z=? N=0 H=0 C=0

xorAny: macro
    ld A, \1
    xor \2
endm


Resets a bit of an 8bit piece of memory

resAny u3, [n16]
Cycles: 10 
Bytes: 8
Flags: None

resAny u3, [r16]
Cycles: 6
Bytes: 4
Flags: None

resAny: macro
    ld A, \2
    res \1, A
    ld \2, A
endm


Resets a bit of an 8bit piece of IO space memory

resIO u3, [$ff00 + n8]
Cycles: 8 
Bytes: 6
Flags: None

resIO: macro
    ldh A, \2
    res \1, A
    ldh \2, A
endm


Compares a value with a value in the IO space of memory and sets flags.

cpIO [$ff00 + n8], n8
Cycles: 5
Bytes: 4
Flags: Z=? N=1, H=? C=?

cpIO [$ff00 + n8], r8 
Cycles: 4
Bytes: 3
Flags: Z=? N=1, H=? C=?

cpIO [$ff00 + n8], [HL] 
Cycles: 5
Bytes: 3
Flags: Z=? N=1, H=? C=?

cpIO: macro
    ldh A, \1
    cp \2
endm


Compares two values, setting appropriate flags.

cpAny r8, n8
Cycles: 4
Bytes: 4
Flags: Z=? N=1, H=? C=?

cpAny r8, r8
Cycles: 3
Bytes: 3
Flags: Z=? N=1, H=? C=?

cpAny r8, [HL]
Cycles: 4
Bytes: 3
Flags: Z=? N=1, H=? C=?

cpAny [r16], n8
Cycles: 4
Bytes: 3
Flags: Z=? N=1, H=? C=?

cpAny [r16], r8
Cycles: 3
Bytes: 2
Flags: Z=? N=1, H=? C=?

cpAny [r16], [HL]
Cycles: 4
Bytes: 2
Flags: Z=? N=1, H=? C=?

cpAny [n16], n8
Cycles: 6
Bytes: 5
Flags: Z=? N=1, H=? C=?

cpAny [n16], r8
Cycles: 5
Bytes: 4
Flags: Z=? N=1, H=? C=?

cpAny [n16], [HL]
Cycles: 6
Bytes: 4
Flags: Z=? N=1, H=? C=?

cpAny: macro
    ld A, \1
    cp \2
endm


Increment an 8bit value

incAny [n16]
Cycles: 9
Bytes: 7
Flags: Z=? N=0 H=? C=C

incAny [r16]
Cycles: 5
Bytes: 3
Flags: Z=? N=0 H=? C=C


Decrement an 8bit value

decAny [n16]
Cycles: 9
Bytes: 7
Flags: Z=? N=1 H=? C=C

decAny [r16]
Cycles: 5
Bytes: 3
Flags: Z=? N=1 H=? C=C


Loads one 16 bit register into another

ld16 r16high,r16low, r16high,r16low
ld16 H,L, B,C
Cycles: 2
Bytes: 2
Flags: None



Subtracts one 16 bit register from another with result in \1,\2.

sub16 r16high,r16low, r16high,r16low
sub16 H,L, B,C
Cycles: 6
Bytes: 6
Flags: Z=? N=1 H=? C=?

mult 
Multiplies two numbers
@param \1 the first number
@param \2 the second number
@return A the multiplied result.
Don't use this as a macro if you're running out of ROM space


