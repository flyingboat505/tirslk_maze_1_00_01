; LinearInterpolation.s
; Runs on any Cortex M
; Test the Sin() function by testing numbers from 0 to 255 and comparing
; the result of the linear interpolation function to pre-calculated sine
; values.
; Daniel Valvano
; September 11, 2013

; This example accompanies the books
;   "Embedded Systems: Introduction to the MSP432 Microcontroller",
;       ISBN: 978-1512185676, Jonathan Valvano, copyright (c) 2017
;   "Embedded Systems: Real-Time Interfacing to the MSP432 Microcontroller",
;       ISBN: 978-1514676585, Jonathan Valvano, copyright (c) 2017
;   "Embedded Systems: Real-Time Operating Systems for ARM Cortex-M Microcontrollers",
;       ISBN: 978-1466468863, , Jonathan Valvano, copyright (c) 2017
; For more information about my classes, my research, and my books, see
; http://users.ece.utexas.edu/~valvano/
;
;Simplified BSD License (FreeBSD License)
;Copyright (c) 2017, Jonathan Valvano, All rights reserved.
;
;Redistribution and use in source and binary forms, with or without modification,
;are permitted provided that the following conditions are met:
;
;1. Redistributions of source code must retain the above copyright notice,
;   this list of conditions and the following disclaimer.
;2. Redistributions in binary form must reproduce the above copyright notice,
;   this list of conditions and the following disclaimer in the documentation
;   and/or other materials provided with the distribution.
;
;THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
;AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
;IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
;ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
;LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
;DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
;LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
;AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
;OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
;USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;
;The views and conclusions contained in the software and documentation are
;those of the authors and should not be interpreted as representing official
;policies, either expressed or implied, of the FreeBSD Project.

       .thumb
       .text
       .align 2
       .global Sin
       .global main

Expected .long 0,3,6,9,12,16,19,22,25,28,31,34,37,40
         .long 43,46,49,51,54,57,60,63,65,68,71,73,76,78
         .long 81,83,85,88,90,92,94,96,98,100,102,104,106,107
         .long 109,111,112,113,115,116,117,118,120,121,122,122,123,124
         .long 125,125,126,126,126,127,127,127,127,127,127,127,126,126
         .long 126,125,125,124,123,122,122,121,120,118,117,116,115,113
         .long 112,111,109,107,106,104,102,100,98,96,94,92,90,88
         .long 85,83,81,78,76,73,71,68,65,63,60,57,54,51
         .long 49,46,43,40,37,34,31,28,25,22,19,16,12,9
         .long 6,3,0,-3,-6,-9,-12,-16,-19,-22,-25,-28,-31,-34
         .long -37,-40,-43,-46,-49,-51,-54,-57,-60,-63,-65,-68,-71,-73
         .long -76,-78,-81,-83,-85,-88,-90,-92,-94,-96,-98,-100,-102,-104
         .long -106,-107,-109,-111,-112,-113,-115,-116,-117,-118,-120,-121,-122,-122
         .long -123,-124,-125,-125,-126,-126,-126,-127,-127,-127,-127,-127,-127,-127
         .long -126,-126,-126,-125,-125,-124,-123,-122,-122,-121,-120,-118,-117,-116
         .long -115,-113,-112,-111,-109,-107,-106,-104,-102,-100,-98,-96,-94,-92
         .long -90,-88,-85,-83,-81,-78,-76,-73,-71,-68,-65,-63,-60,-57
         .long -54,-51,-49,-46,-43,-40,-37,-34,-31,-28,-25,-22,-19,-16
         .long -12,-9,-6,-3

main:   .asmfunc
    MOV R4, #0                      ; R4 = 0 = Correct    (0 <= error < 1) or (-1 < error <= 0)
    MOV R5, #0                      ; R5 = 0 = OffByOne   (1 <= error < 2) or (-2 < error <= -1)
    MOV R6, #0                      ; R6 = 0 = OffByTwo   (2 <= error < 3) or (-3 < error <= -2)
    MOV R7, #0                      ; R7 = 0 = OffByThree (3 <= error < 4) or (-4 < error <= -3)
    MOV R8, #0                      ; R8 = 0 = OffByFour  (4 <= error < 5) or (-5 < error <= -4)
    MOV R9, #0                      ; R9 = 0 = OffByFiveOrMore (error >= 5) or (error <= -5)
    MOV R10, #0                     ; R10 = 0 = index
    LDR R11, ExpectedAddr           ; R11 = &Expected (pointer)
TestLoop
    MOV R0, R10                     ; R0 = R10 = index
    BL  Sin                         ; R0 = Sin(R10) = Sin(index)
    LDR R1, [R11], #4               ; R1 = [R11] = Expected[index] (value)
                                    ; R11 = R11 + 4 (increment pointer 'Expected')
    SUB R2, R0, R1                  ; R2 = R0 - R1 = Sin(index) - Expected[index]
    CMP R2, #0                      ; is Sin(index) - Expected[index] (R2) == 0
    BEQ Correct                     ; if so, skip to 'Correct'
    CMP R2, #1                      ; is Sin(index) - Expected[index] (R2) == 1
    BEQ OffByOne                    ; if so, skip to 'OffByOne'
    CMN R2, #1                      ; is Sin(index) - Expected[index] (R2) == -1
    BEQ OffByOne                    ; if so, skip to 'OffByOne'
    CMP R2, #2                      ; is Sin(index) - Expected[index] (R2) == 2
    BEQ OffByTwo                    ; if so, skip to 'OffByTwo'
    CMN R2, #2                      ; is Sin(index) - Expected[index] (R2) == -2
    BEQ OffByTwo                    ; if so, skip to 'OffByTwo'
    CMP R2, #3                      ; is Sin(index) - Expected[index] (R2) == 3
    BEQ OffByThree                  ; if so, skip to 'OffByThree'
    CMN R2, #3                      ; is Sin(index) - Expected[index] (R2) == -3
    BEQ OffByThree                  ; if so, skip to 'OffByThree'
    CMP R2, #4                      ; is Sin(index) - Expected[index] (R2) == 4
    BEQ OffByFour                   ; if so, skip to 'OffByFour'
    CMN R2, #4                      ; is Sin(index) - Expected[index] (R2) == -4
    BEQ OffByFour                   ; if so, skip to 'OffByFour'
    B   OffByFiveOrMore             ; unconditional branch to 'OffByFiveOrMore'
Correct
    ADD R4, R4, #1                  ; R4 = R4 + 1 (Correct = Correct + 1)
    B   HistoDone                   ; unconditional branch to 'HistoDone'
OffByOne
    ADD R5, R5, #1                  ; R5 = R5 + 1 (OffByOne = OffByOne + 1)
    B   HistoDone                   ; unconditional branch to 'HistoDone'
OffByTwo
    ADD R6, R6, #1                  ; R6 = R6 + 1 (OffByTwo = OffByTwo + 1)
    B   HistoDone                   ; unconditional branch to 'HistoDone'
OffByThree
    ADD R7, R7, #1                  ; R7 = R7 + 1 (OffByThree = OffByThree + 1)
    B   HistoDone                   ; unconditional branch to 'HistoDone'
OffByFour
    ADD R8, R8, #1                  ; R8 = R8 + 1 (OffByFour = OffByFour + 1)
    B   HistoDone                   ; unconditional branch to 'HistoDone'
OffByFiveOrMore
    ADD R9, R9, #1                  ; R9 = R9 + 1 (OffByFiveOrMore = OffByFiveOrMore + 1)
HistoDone                           ; done incrementing histogram bins
    ADD R10, R10, #1                ; R10 = R10 + 1 (index = index + 1)
    CMP R10, #256                   ; is index (R10) < 256?
    BLO TestLoop                    ; if so, skip to 'TestLoop'
loop  B  loop                           ; stall here and observe the registers
    .endasmfunc
ExpectedAddr .field Expected,32
    .end
