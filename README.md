# ITE2031_Computer_Architecture
*Hanyang University, Computer Architecture, Spring 2023, by prof. Woong Sul*

## Project 01
<p>This project is intended to help understand the instructions of a very simple assembly language and how to assemble programs into machine language.</p>

### LC-2K Instruction-Set Architecture
The LC-2K is another 32-bit RISC architecture which includes   
> – 8 x 32-bit registers: reg01 to reg7   
> – Each address is 32-bits and it stores a word   
> – 65536 words of memory (addressed by word index, not byte)
<br>
<p>There are 4 instruction formats (bit 0 is the least-significant bit).
Bits 31-25 are unused for all instructions, and should always be 0.</p>

***R-type instructions (add, nor):***   
> bits 24-22: opcode   
> bits 21-19: reg A    
> bits 18-16: reg B     
> bits 15-3: unused (should all be 0)    
> bits 2-0: destReg    

***I-type instructions (lw, sw, beq):***   
> bits 24-22: opcode     
> bits 21-19: reg A     
> bits 18-16: reg B     
> bits 15-0: offsetField (a 16-bit value with a range of -32768 to 32767)     

***J-type instructions (jalr):***    
> bits 24-22: opcode    
> bits 21-19: reg A     
> bits 18-16: reg B      
> bits 15-0: unused (should all be 0)     

***O-type instructions (halt, noop):***     
> bits 24-22: opcode     
> bits 21-0: unused (should all be 0)     

<br>

### 1. LC-2K Assembly Language and Assembler
<p>The first part of this project is to write a program to take an assembly-language program and translate it into machine language.</p>

The format for a line of assembly code is :
> label`<white>`instruction`<white>`fld0`<white>`fld1`<white>`fld2`<white>`comments   
> (``<white>`` means a series of tabs and/or spaces)

<br>

**<p> Description of Machine Instructions </p>**
<img width="905" alt="스크린샷 2023-07-18 오후 2 15 17" src="https://github.com/najiyeon/ITE2031_Computer_Architecture/assets/113894257/b1b063bf-a3d4-4ad8-ad6d-7faed56e1250">

***R-type instructions (add, nor) instructions require 3 fields:***    
> field0 is regA, field1 is regB, and field2 is destReg.

***I-type instructions (lw, sw, beq) require 3 fields:***        
> field0 is regA, field1 is regB, and field2 is either a numeric value for offsetField or a symbolic address. Numeric offsetFields can be positive or negative; symbolic addresses are discussed below.
   
***J-type instructions (jalr) require 2 fields:***       
> field0 is regA, and field1 is regB.

***O-type instructions (noop, halt) require no fields.***       

<br>

### 2. Behavioral Simulator
<p>The second part of this assignment is to write a program that can simulate any legal LC-2K machine-code program.</p>



<br>

### 3. Assembly-Language Multiplication
<p>The third part of this assignment is to write an assembly-language program to multiply two numbers.</p>

