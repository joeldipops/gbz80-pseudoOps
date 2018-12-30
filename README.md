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

### ldAny

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

### ldhAny

**ldhAny [$ff00 + n8], n8**
* Store value *n8* in HRAM or IO space at $ff*n8*
* Cycles: 5
* Bytes: 4 
* Flags: None 

**ldhAny [$ff00 + n8], r8**
* Store value in register *r8* in HRAM or IO space at $ff*n8*
* Cycles: 4
* Bytes: 3 
* Flags: None

**ldhAny [$ff00 + n8], [r16]**
* Store value at address pointed to by *r16* in HRAM or IO space at $ff*n8*
* Cycles: 5
* Bytes: 3 
* Flags: None 

**ldhAny [$ff00 + n8], [n16]**
* Store value at address *n16* in HRAM or IO space at $ff*n8*
* Cycles: 7
* Bytes: 5 
* Flags: None 

### ldiAny

**ldiAny r8, [HL]**
* Store value at address pointed to by HL into register *r8* then increments **HL**
* Cycles: 3
* Bytes: 2
* Flags: None

**ldiAny [r16], [HL]**
* Store value at address pointed to by HL into address pointed to by *r16* then increments **HL**
* Cycles: 4
* Bytes: 2
* Flags: None

**ldiAny [n16], [HL]**
* Store value at address pointed to by HL into address *n16* then increments **HL**
* Cycles: 6
* Bytes: 4
* Flags: None

### orAny

**orAny r8, r8**
* Bitwise OR between the values in two registers *r8*.  Result in **A**
* Cycles: 2
* Bytes: 2
* Flags: 
    * Z: Set if result is 0, reset otherwise
    * N: 0
    * H: 0 
    * C: 0

**orAny r8, n8**
* Bitwise or between the value in *r8* and *n8*. Result in **A**
* Cycles: 4
* Bytes: 4 
* Flags: 
    * Z: Set if result is 0, reset otherwise
    * N: 0
    * H: 0 
    * C: 0

**orAny r8, [HL]**
* Bitwise OR between the value in *r8* and the value at address pointed to by **HL**. Result in **A**
* Cycles: 3
* Bytes: 2
* Flags: 
    * Z: Set if result is 0, reset otherwise
    * N: 0
    * H: 0 
    * C: 0

**orAny [r16], r8**
* Bitwise OR between the value at address pointed to by *r16* and the value in *r8*. Result in **A**
* Cycles: 3
* Bytes: 2
* Flags: 
    * Z: Set if result is 0, reset otherwise
    * N: 0
    * H: 0 
    * C: 0

**orAny [r16], n8**
* Bitwise OR between the value at address pointed to by *r16* and *n8*. Result in **A**
* Cycles: 4
* Bytes: 3 
* Flags: 
    * Z: Set if result is 0, reset otherwise
    * N: 0
    * H: 0 
    * C: 0

**orAny [r16], [HL]**
* Bitwise OR between the value at address pointed to by *r16* and the value at address pointed to by **HL**. Result in **A**
* Cycles: 4
* Bytes: 2
* Flags: 
    * Z: Set if result is 0, reset otherwise
    * N: 0
    * H: 0 
    * C: 0

**orAny [n16], r8**
* Bitwise OR between the value at address *n16* and the value of *r8*. Result in **A**
* Cycles: 5
* Bytes: 4
* Flags: 
    * Z: Set if result is 0, reset otherwise
    * N: 0
    * H: 0 
    * C: 0

**orAny [n16], n8**
* Bitwise OR between the value at address *n16* and *n8*. Result in **A**
* Cycles: 6
* Bytes: 5 
* Flags: 
    * Z: Set if result is 0, reset otherwise
    * N: 0
    * H: 0 
    * C: 0

**orAny [n16], [HL]**
* Bitwise OR between the value at address *n16* and the value at address pointed to by **HL**. Result in **A**
* Cycles: 6
* Bytes: 4
* Flags: 
    * Z: Set if result is 0, reset otherwise
    * N: 0
    * H: 0 
    * C: 0

## andAny

**andAny r8, r8**
* Bitwise AND between two registers *r8*.  Result in **A**
* Cycles: 2
* Bytes: 2
* Flags: 
    * Z: Set if result is 0, reset otherwise
    * N: 0 
    * H: 1
    * C: 0

**andAny r8, n8**
* Bitwise AND between the value in *r8* and *n8*.  Result in **A**
* Cycles: 4
* Bytes: 4 
* Flags: 
    * Z: Set if result is 0, reset otherwise
    * N: 0 
    * H: 1
    * C: 0

**andAny r8, [HL]**
* Bitwise AND between the value in *r8* and the value at address pointed to by **HL**.  Result in **A**
* Cycles: 3
* Bytes: 2
* Flags: 
    * Z: Set if result is 0, reset otherwise
    * N: 0 
    * H: 1
    * C: 0

**andAny [r16], r8**
* Bitwise AND between the value at address pointed to by *r16* and the value in *r8*. Result in **A**
* Cycles: 3
* Bytes: 2
* Flags: 
    * Z: Set if result is 0, reset otherwise
    * N: 0 
    * H: 1
    * C: 0

**andAny [r16], n8**
* Bitwise AND between the value at address pointed to by *r16* and *n8*. Result in **A**
* Cycles: 4
* Bytes: 3 
* Flags: 
    * Z: Set if result is 0, reset otherwise
    * N: 0 
    * H: 1
    * C: 0

**andAny [r16], [HL]**
* Bitwise AND between the value at address pointed to by *r16* and the value at address pointed to by **HL**. Result in **A**
* Cycles: 4
* Bytes: 2
* Flags: 
    * Z: Set if result is 0, reset otherwise
    * N: 0 
    * H: 1
    * C: 0

**andAny [n16], r8**
* Bitwise AND between the value at address *n16* and the value in *r8*. Result in **A**
* Cycles: 5
* Bytes: 4
* Flags: 
    * Z: Set if result is 0, reset otherwise
    * N: 0 
    * H: 1
    * C: 0

**andAny [n16], n8**
* Bitwise AND between the value at address pointed to by *r16* and *n8*. Result in **A**
* Cycles: 6
* Bytes: 5 
* Flags: 
    * Z: Set if result is 0, reset otherwise
    * N: 0 
    * H: 1
    * C: 0

**andAny [n16], [HL]**
* Bitwise AND between the value at address pointed to by *r16* and the value at address pointed to by **HL**. Result in **A**
* Cycles: 6
* Bytes: 4
* Flags: 
    * Z: Set if result is 0, reset otherwise
    * N: 0 
    * H: 1
    * C: 0

### xorAny

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

### resAny

**resAny u3, [n16]**
* Resets bit *u3* of value at address *n16* 
* Cycles: 10 
* Bytes: 8
* Flags: None

**resAny u3, [r16]**
* Resets bit *u3* of value at address pointed to by *r16* 
* Cycles: 6
* Bytes: 4
* Flags: None

### resIO

**resIO u3, [$ff00 + n8]**
* Reset but *u3* of value at address in HRAM or IO space at $ff*n8*
* Cycles: 8 
* Bytes: 6
* Flags: None

### cpIO

**cpIO [$ff00 + n8], n8**
* Subtracts the value *n8* from the value in HRAM or IO space at address $ff*n8* and sets flags accordingly, without storing the result.
* Cycles: 5
* Bytes: 4
* Flags: 
    * Z: Set if result is 0, reset otherwise
    * N: 1
    * H: Reset if borrow from bit 4, set otherwise.
    * C: Set if *n8* > [$ff00 + n8]

cpIO [$ff00 + n8], r8 
Cycles: 4
Bytes: 3
Flags: Z=? N=1, H=? C=?

cpIO [$ff00 + n8], [HL] 
Cycles: 5
Bytes: 3
Flags: Z=? N=1, H=? C=?

### cpAny

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

### incAny

**incAny [n16]**
* Increment value in address at *n16* by 1
* Cycles: 9
* Bytes: 7
* Flags: 
    * Z: Set if result is 0, reset otherwise
    * N: 0
    * H: Set if overflow from bit 3
    * C: Not affected

**incAny [r16]**
* Increment value at address pointed to by *r16* by 1
* Cycles: 5
* Bytes: 3
* Flags: 
    * Z: Set if result is 0, reset otherwise
    * N: 0
    * H: Set if overflow from bit 3
    * C: Not affected

### decAny

**decAny [n16]**
* Decrement the value at address *n16* by 1
* Cycles: 9
* Bytes: 7
* Flags: 
    * Z: Set if result is 0, reset otherwise
    * N: 1
    * H: Reset if borrow from bit 4, set otherwise
    * C: Not affected

**decAny [r16]**
* Cycles: 5
* Bytes: 3
* Flags: 
    * Z: Set if result is 0, reset otherwise
    * N: 1
    * H: Reset if borrow from bit 4, set otherwise
    * C: Not affected

### ld16

**ld16 r16high,r16low, r16high,r16low**
* Stores the value of the register made from the combination of *r16high* and *r16low* into another 16bit register. 
* eg. ld16 H,L, B,C
* Cycles: 2
* Bytes: 2
* Flags: None

### sub16

**sub16 r16high,r16low, r16high,r16low**
*
* eg. sub16 H,L, B,C
* Cycles: 6
* Bytes: 6
* Flags: Z=? N=1 H=? C=?

### mult

mult 
Multiplies two numbers
@param \1 the first number
@param \2 the second number
@return A the multiplied result.
Don't use this as a macro if you're running out of ROM space


