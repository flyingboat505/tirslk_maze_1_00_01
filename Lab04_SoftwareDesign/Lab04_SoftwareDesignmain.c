// main.c
// Runs on MSP432
// Student version of Software Design lab
// Daniel and Jonathan Valvano
// May 28, 2017


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
#include <stdint.h>
enum scenario {
    Error = 0,
    LeftTooClose = 1,
    RightTooClose = 2,
    CenterTooClose = 4,
    Straight = 8,
    LeftTurn = 9,
    RightTurn = 10,
    TeeJoint = 11,
    LeftJoint = 12,
    RightJoint = 13,
    CrossRoad = 14,
    Blocked = 15
};
typedef enum scenario scenario_t;

#define SIDEMAX 354    // largest side distance to wall in mm
#define SIDEMIN 212    // smallest side distance to wall in mm
#define CENTEROPEN 600 // distance to wall between open/blocked
#define CENTERMIN 150  // min distance to wall in the front

int LOpen(){

}
int LClosed(){

}
int ROpen(){
}
int RClosed(){
}

int COpen(){
}

int CClosed(){
}

scenario_t Classify(int32_t Left, int32_t Center, int32_t Right){
    //case 0
  scenario_t result=Error;
  if(Left<50 || Right<50 || Center<50 || Left>800 || Right>800 || Center>800 ){
      return result;
  }

  if(Left<SIDEMIN){
      result=LeftTooClose;
  }
  //case 2
  if(Right<SIDEMIN){
      result=RightTooClose;
  }
  //case 3
  //double check this
  if(Center<CENTERMIN){
      result=CenterTooClose;
  }

  if(Left<SIDEMIN && Right<SIDEMIN){
      result=LeftTooClose+RightTooClose;
  }
  //case 4

  //case 5
  if(Center<CENTERMIN && Left<SIDEMIN ){
      result=CenterTooClose+LeftTooClose ;
  }
  //case 6
  //double check
  if (Center<CENTERMIN && Right<SIDEMIN ){
      result=CenterTooClose+RightTooClose ;
  }
  //case 7
  //double check
  if (Center<CENTERMIN && Right<SIDEMIN && Left<SIDEMIN ){
        result=CenterTooClose+RightTooClose+LeftTooClose ;
  }
  //case 8-Straight
  //check range
  if(Center>=CENTEROPEN && Right>=SIDEMIN && Right<SIDEMAX  && Left>=SIDEMIN && Left<SIDEMAX){
        result=Straight;
  }
  //case 9-Left Turn
  if(Center<CENTEROPEN && Center>=CENTERMIN && Right>=SIDEMIN && Right<SIDEMAX && Left>=SIDEMAX){
         result=LeftTurn;
   }



  //case 10-Right Turn
  if(Center<CENTEROPEN && Center>=CENTERMIN && Left>=SIDEMIN && Left<SIDEMAX && Right>=SIDEMAX){
          result=RightTurn ;
  }
  //case 11-T-Joint
  //double check- add constaitns
  if(Center>=CENTERMIN && Center<CENTEROPEN && Left>=SIDEMAX && Right>=SIDEMAX ){
      result=TeeJoint ;
  }
  //case 12
    //Right is blocked,everything else open
  //double check left is open
  if(Right>=SIDEMIN && Right<SIDEMAX && Center>=CENTEROPEN && Left>=SIDEMAX){
       result=LeftJoint ;
  }
  //case 13 Right Joint
  //right is open
  if(Left<SIDEMAX && Left>=SIDEMIN  && Center>=CENTEROPEN && Right>=SIDEMAX){
      result=RightJoint ;
  }
  //case 14 CrossRoad
  if(Center>=CENTEROPEN && Left>=SIDEMAX && Right>=SIDEMAX){
        result=CrossRoad ;
  }
  //Case 15 bloacked
  if(Center>=CENTERMIN && Center<CENTEROPEN && Left>=SIDEMIN  && Left<SIDEMAX && Right>=SIDEMIN  && Right<SIDEMAX){
          result=Blocked ;
    }

  return result;
}

#define IRSlope 1195172
#define IROffset -1058
#define IRMax 2552
//Let n be a 14-bit sample from the ADC, and D be the distance in mm.
//The basic form of this nonlinear transfer relation
//is D = 1195172/(n � 1058)
// so if the ADC value is less than 2552 (IRMAX),
//your function should return 800
//where 1195172 and -1058 are calibration coefficients to be empirically determined in the ADC lab section.
int32_t Convert(int32_t n){
  if(n<IRMax){
      return 800;
  }
  n=IRSlope / (n + IROffset) ; // replace this line

  return n;
}
// ***********testing of Convert*********
int32_t const ADCBuffer[16]={2000, 2733, 3466, 4199, 4932, 5665, 6398, 7131, 7864, 8597, 9330, 10063, 10796, 11529, 12262, 12995};
int32_t const DistanceBuffer[16]={800, 713, 496, 380, 308, 259, 223, 196, 175, 158, 144, 132, 122, 114, 106, 100};
//renaming P_4 to main
/*void main(void){
  int i;
  int32_t adc,distance,errors,diff;
  errors = 0;
  for(i=0; i<16; i++){
    adc = ADCBuffer[i];
    distance = Convert(adc); // call to your function
    diff = distance-DistanceBuffer[i];
    if((diff<-1)||(diff>1)){
      errors++;
    }
  }
  while(1){};
}*/
// ***********end of testing of Convert*********



// ***********testing of classify
scenario_t Solution(int32_t Left, int32_t Center, int32_t Right);
int32_t const CornerCases[18]={49,50,51,149,150,151,211,212,213,353,354,355,599,600,601,799,800,801};
int32_t errors;
int32_t correct;
void main(void){
  enum scenario result,truth;
  int i,j,k;
  int32_t left, right, center; // sensor readings
  errors = 0;
  correct=0;
  for(i=0; i<18; i++){
    left = CornerCases[i];
    for(j=0; j<18; j++){
      center = CornerCases[j];
      for(k=0; k<18; k++){
        right = CornerCases[k];
        result = Classify(left,center,right); // your solution
        truth = Solution(left,center,right);  // correct answer
        if(result != truth){
           errors++;
           //Classify(left,center,right);
        }
        else{
            correct++;

        }
      }
    }
  }
  while(1){
  }
}

void Program4_3(void){ // will take over 16 hours to complete
  enum scenario result,truth;
  int32_t left, right, center; // sensor readings
  errors = 0;
  for(left=0; left<1000; left++){
    for(center=0; center<1000;  center++){
      for(right=0; right<1000; right++){
        result = Classify(left,center,right); // your solution
        truth = Solution(left,center,right);  // correct answer
        if(result != truth){
           errors++;
        }
      }
    }
  }
  while(1){
  }
}

void main2(void){
  // run one of these
  //Program4_1();
//  Program4_2();
//  Program4_3();
}
