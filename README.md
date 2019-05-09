# gbz80-pseudoOps
Set of rgbasm macros and constants to facilitate readability for modern programmers.

I've only just started using ASM, but like most programmers born in the 90s I've gotten used to 
a) Doing things in one step instead of two or three and 
b) camelCase.  
So this is my attempt to simplify the writing and most relevantly the reading of asm code without comprimising the control over optimisation required to run gameboy software as well as it needs to run.

This is a work in progress, so more pseudo-ops will be added as I come across a case for them in the software I write.

## Usage
The ops can be included with
`INCLUDE "ops.inc"`

There's also a list of hardware memory locations, with names like `BackgroundScrollX` that you may prefer to the traditional gbhw.inc names such as `rSCY`.  That's in
`INCLUDE "addresses.inc"`

As well as some random utility macros you may find a use for.

You can include everything with `INCLUDE "gbz80-pseudoOps.inc`.  It's all macros so won't take up any cart space or cycles, but may increase compliation time in larger projects.

## Acknowledgements
Thanks to [DonaldHays](/DonaldHays) for writing the 'jump if greather than or equal to' style ops.

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

*Unless otherwise specified, all ops will overwrite A, and have no other side-effects*

**Don't take these timings as gospel, I've overhauled the ldAny macro to make certain optimisations where possible, which has had a flow on effect to everything and I haven't counted every cycle/byte affected yet.**

### ldAny

Will perform the fastest/smallest ld it can, such as using the ldh instruction where appropriate.

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

**ldAny [r16], 0**
**ldAny [n16], 0**
**ldAny [$ff00 + n8], [$ff00 + n8]**
**ldAny [$ff00 + n8], n8**
**ldAny [$ff00 + n8], r8**
**ldAny [$ff00 + n8], [r16]**
**ldAny [$ff00 + n8], [n16]**

**ldAny r8, [$ff00 + n8]**
**ldAny [r16], [$ff00 + n8]**
**ldAny [n16], [$ff00 + n8]**


### ldiAny

**ldiAny r8, [HL]**
* Store value at address pointed to by **HL** into register *r8* then increments **HL**
* Cycles: 3
* Bytes: 2
* Flags: None

**ldiAny [r16], [HL]**
* Store value at address pointed to by **HL** into address pointed to by *r16* then increments **HL**
* Cycles: 4
* Bytes: 2
* Flags: None

**ldiAny [n16], [HL]**
* Store value at address pointed to by **HL** into address *n16* then increments **HL**
* Cycles: 6
* Bytes: 4
* Flags: None

**ldiAny [HL], r8**
* Storevalue of *r8* into address pointed to by **HL** then increments **HL**
* Cycles: 3
* Bytes: 2
* Flags: None

**ldiAny [HL], [r16]**
* Store value at address pointed to by *n16* in to address pointed to by **HL** then increments **HL**
* Cycles: 4
* Bytes: 2
* Flags: None

**ldiAny [HL], [n16]**
* Store value at address *n16* in to address pointed to by **HL** then increments **HL**
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

### andAny

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

**xorAny r8, r8**
* Bitwise XOR between the value of two registers *r8*.  Result in **A**
* Cycles: 2
* Bytes: 2
* Flags: 
    * Z: Set if result is 0, reset otherwise
    * N: 0
    * H: 0
    * C: 0

**xorAny r8, n8**
* Bitwise XOR between the value of *r8* and *n8*. Result in **A**.
* Cycles: 4
* Bytes: 4 
* Flags: 
    * Z: Set if result is 0, reset otherwise
    * N: 0
    * H: 0
    * C: 0

**xorAny r8, [HL]**
* Bitwise XOR between the value of *r8* and the value at address pointed to by **HL**.  Result in **A**
* Cycles: 3
* Bytes: 2
* Flags: 
    * Z: Set if result is 0, reset otherwise
    * N: 0
    * H: 0
    * C: 0

**xorAny [r16], r8**
* Bitwise XOR between the value at address pointed to by *r16* and the value in *r8*.  Result in **A**
* Cycles: 3
* Bytes: 2
* Flags: 
    * Z: Set if result is 0, reset otherwise
    * N: 0
    * H: 0
    * C: 0

**xorAny [r16], n8**
* Bitwise XOR between the value at address pointed to by *r16* and *n8*.  Result in **A**
* Cycles: 4
* Bytes: 3 
* Flags: 
    * Z: Set if result is 0, reset otherwise
    * N: 0
    * H: 0
    * C: 0

**xorAny [r16], [HL]**
* Bitwise XOR between the value at address pointed to by *r16* and the value at address pointed to be **HL**.  Result in **A**
* Cycles: 4
* Bytes: 2
* Flags: 
    * Z: Set if result is 0, reset otherwise
    * N: 0
    * H: 0
    * C: 0

**xorAny [n16], r8**
* Bitwise XOR between the value at address *n16* and the value of *r8*.  Result in **A**
* Cycles: 5
* Bytes: 4
* Flags: 
    * Z: Set if result is 0, reset otherwise
    * N: 0
    * H: 0
    * C: 0

**xorAny [n16], n8**
* Bitwise XOR between the value at address *n16* and *n8*. Result in **A**
* Cycles: 6
* Bytes: 5 
* Flags: 
    * Z: Set if result is 0, reset otherwise
    * N: 0
    * H: 0
    * C: 0

**xorAny [n16], [HL]**
* Bitwise XOR between the value at address *n16* and the value at address pointed to by **HL**.  Result in **A**
* Cycles: 6
* Bytes: 4
* Flags: 
    * Z: Set if result is 0, reset otherwise
    * N: 0
    * H: 0
    * C: 0

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

### cpAny

**cpAny r8, n8**
* Subtract the *n8* from the value in *r8* and set flags accordingly, without storing the result.
* Cycles: 4
* Bytes: 4
* Flags: 
    * Z: Set if result is 0, reset otherwise
    * N: 1
    * H: Reset if borrow from bit 4, set otherwise.
    * C: Set if *n8* > *r8*, reset otherwise

**cpAny r8, r8**
* Subtract the value in one register *r8* from another register *r8* and set flags accordingly, without storing the result.
* Cycles: 3
* Bytes: 3
* Flags: 
    * Z: Set if result is 0, reset otherwise
    * N: 1
    * H: Reset if borrow from bit 4, set otherwise.
    * C: Set if second *r8* > than first *r8*, reset otherwise
    
**cpAny r8, [HL]**
* Subtract the value at address pointed to by **HL** from value of *r8* and set flags accordingly, without storing the result.
* Cycles: 4
* Bytes: 3
* Flags: 
    * Z: Set if result is 0, reset otherwise
    * N: 1
    * H: Reset if borrow from bit 4, set otherwise.
    * C: Set if value at [**HL**] > *r8*, reset otherwise

**cpAny [r16], n8**
* Subtract *n8* from value at address pointed to by *r16* and set flags accordingly, without storing the result.
* Cycles: 4
* Bytes: 3* Flags: 
    * Z: Set if result is 0, reset otherwise
    * N: 1
    * H: Reset if borrow from bit 4, set otherwise.
    * C: Set if *n8* > value at [*r16*], reset otherwise

**cpAny [r16], r8**
* Subtract value in *r8* from value at address pointed to by *r16* and set flags accordinly, without storing the result.
* Cycles: 3
* Bytes: 2
* Flags: 
    * Z: Set if result is 0, reset otherwise.
    * N: 1
    * H: Reset if borrow from bit 4, set otherwise.
    * C: Set if *r8* > value at [*r16*], reset otherwise

**cpAny [r16], [HL]**
* Subtract value at address pointed to by **HL** from value at address pointed to by *r16* and set flags accordinly, without storing the result.
* Cycles: 4
* Bytes: 2
* Flags: 
    * Z: Set if result is 0.
    * N: 1
    * H: Reset if borrow from bit 4, set otherwise.
    * C: Set if [**HL**] > [*r16*], reset otherwise

**cpAny [n16], n8**
* Subtract *n8* from value at address *n16* and set flags accordinly without storing the result.
* Cycles: 6
* Bytes: 5
* Flags: 
    * Z: Set if result is 0, reset otherwise.
    * N: 1
    * H: Reset if borrow from bit 4, set otherwise.
    * C: Set if *n8* > [*n16*], reset otherwise.

**cpAny [n16], r8**
* Subtract value of *r8* from value at address *n16* and set flags accordinly without storing the result.
* Cycles: 5
* Bytes: 4
* Flags: 
    * Z: Set if result is 0, reset otherwise.
    * N: 1
    * H: Reset if borrow from bit 4, set otherwise.
    * C: Set if *r8* > [*n16*], reset otherwise.
        
**cpAny [n16], [HL]**
* Subtract value of address pointed to by **HL** from value at address *n16* and set flags accordinly, without storing the result.
* Cycles: 6
* Bytes: 4
* Flags: 
    * Z: Set if result is 0, reset otherwise.
    * N: 1
    * H: Reset if borrow from bit 4, set otherwise.
    * C: Set if [**HL**] > [*r16*], reset othwerwise.

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

**ld16 r16, r16**
* Stores the value of one 16 bit register in to another.
* eg. ld16 HL, BC
* Cycles: 2
* Bytes: 2
* Flags: None

**ld16 r16, [n16]**
* Stores the 16bit value at address *n16* and following byte in to register r16
* eg. ld16 HL, $c123
* Cycles: 10
* Bytes: 8
* Flags: None

**ld16 [n16], r16**
* Stores the value in *r16* to address *n16* and following address
* eg. ld16 $c123, HL
* Cycles: 10
* Bytes: 8
* Flags: None

**ld16 [n16], [n16]**
* Stores the 16bit value at one address *n16* (and next address) in to another address *n16* and next address.
* eg. ld16 $d321, $c123
* Cycles: 10
* Bytes: 8
* Flags: None

### sub16

**sub16 r16, r16**
* Subtracts the value of 1 16bit register from another and stores the result into the first register.
* eg. sub16 HL, BC
* Cycles: 6
* Bytes: 6
* Flags: 
    * Z: Set if value in high byte is 0, reset otherwise
    * N: 1
    * H: Reset if borrow from bit 11, set otherwise.
    * C: Set if first *r16* < second *r16*, reset otherwise.

### mult 

**mult r8, ?r16**
* Multiplies **A** with register *r8*.  Result in **HL**.
* Optionally, specify BC or DE to be affected and save 8 cycles, 2 bytes
* Cycles: Depends on value of operands.
* Bytes: 16/18
* Flags:
    * Z: 1
    * N: Set if result is non-zero, reset otherwise
    * H: 0
    * C: 0

**mult n8, ?r16**
* Multiplies **A** with *n8*.  Result in **HL**.
* Optionally, specify BC or DE to be affected and save 8 cycles, 2 bytes
* Cycles: Depends on value of operands.
* Bytes: 26
* Flags: 17/19
    * Z: 1
    * N: Set if result is non-zero, reset otherwise
    * H: 0
    * C: 0