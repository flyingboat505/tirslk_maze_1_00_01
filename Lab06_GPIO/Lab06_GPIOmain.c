// Lab06_GPIOmain.c
// Runs on MSP432
// Solution to GPIO lab
// Daniel and Jonathan Valvano
// May 21, 2017
// Provide test main program for QTR-8RC reflectance sensor array
// Pololu part number 961.

/* This example accompanies the books
   "Embedded Systems: Introduction to the MSP432 Microcontroller",
       ISBN: 978-1512185676, Jonathan Valvano, copyright (c) 2017
   "Embedded Systems: Real-Time Interfacing to the MSP432 Microcontroller",
       ISBN: 978-1514676585, Jonathan Valvano, copyright (c) 2017
   "Embedded Systems: Real-Time Operating Systems for ARM Cortex-M Microcontrollers",
       ISBN: 978-1466468863, , Jonathan Valvano, copyright (c) 2017
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/

Simplified BSD License (FreeBSD License)
Copyright (c) 2017, Jonathan Valvano, All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are
those of the authors and should not be interpreted as representing official
policies, either expressed or implied, of the FreeBSD Project.
*/

// reflectance LED illuminate connected to P5.3
// reflectance sensor 1 connected to P7.0 (robot's right, robot off road to left)
// reflectance sensor 2 connected to P7.1
// reflectance sensor 3 connected to P7.2
// reflectance sensor 4 connected to P7.3 center
// reflectance sensor 5 connected to P7.4 center
// reflectance sensor 6 connected to P7.5
// reflectance sensor 7 connected to P7.6
// reflectance sensor 8 connected to P7.7 (robot's left, robot off road to right)

#include <stdint.h>
#include "msp.h"
#include "..\inc\Reflectance.h"
#include "..\inc\Clock.h"
#include "..\inc\TExaS.h"

uint8_t Data; // QTR-8RC
// Test main for section 6.4.3
int Program6_1(void){
  Clock_Init48MHz();
  Reflectance_Init(); // your initialization
  TExaS_Init(LOGICANALYZER_P7);
  while(1){
    Data = Reflectance_Read(1000); // your measurement
    Clock_Delay1ms(10);
  }
}

int32_t Position; // 332 is right, and -332 is left of center
int main2(void){
  Clock_Init48MHz();
  Reflectance_Init();
  TExaS_Init(LOGICANALYZER_P7);
  while(1){
    Data = Reflectance_Read(1000);
    Position = Reflectance_Position(Data);
    Clock_Delay1ms(10);
  }
}
//main2 rewrite
int main(void){ // main2(void){
    int32_t i;
  Clock_Init48MHz();
  Reflectance_Init();
  P4->SEL0 &= ~0x01;
  P4->SEL1 &= ~0x01;    //  P4.0 as GPIO
  P4->DIR |= 0x01;      //  make P4.0 out
  TExaS_Init(LOGICANALYZER_P7);
  while(1){
    P5->OUT |= 0x08;      // turn on 8 IR LEDs
    P7->DIR = 0xFF;       // make P7.7-P7.0 out
    P7->OUT = 0xFF;       // prime for measurement
    Clock_Delay1us(10);   // wait 10 us
    P7->DIR = 0x00;       // make P7.7-P7.0 in
    //P7->DIR &= ~0x01;
    for(i=0;i<10000;i++){
        P4->OUT = P7->IN&0x01; // convert input to digital
        //P4->OUT = P7->IN&0x01;
    }
    P5->OUT &= ~0x08;     // turn off 8 IR LEDs
    Clock_Delay1ms(10);

  }
}

