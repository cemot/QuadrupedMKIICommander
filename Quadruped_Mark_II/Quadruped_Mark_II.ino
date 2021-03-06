/*
 * Auto-Generated by NUKE! (Hand optimized)
 *   http://arbotix.googlecode.com
 */

#include <ax12.h>
#include <BioloidController.h>
#include <Commander.h>
#include "nuke.h"

Commander command = Commander();

void setup(){
    pinMode(0,OUTPUT);
    // setup IK
    setupIK();
    gaitSelect(SMOOTH_AMBLE);
    // setup serial
    Serial.begin(38400);

    // wait, then check the voltage (LiPO safety)
    delay (1000);
    float voltage = (ax12GetRegister (1, AX_PRESENT_VOLTAGE, 1)) / 10.0;
    Serial.print ("System Voltage: ");
    Serial.print (voltage);
    Serial.println (" volts.");
    if (voltage < 10.0)
        while(1);

    // stand up slowly
    bioloid.poseSize = 12;
    bioloid.readPose();
    doIK();
    bioloid.interpolateSetup(1000);
    while(bioloid.interpolating > 0){
        bioloid.interpolateStep();
        delay(3);
    }
}

/* this is the main loop, it repeats over and over again */
void loop(){
  // take commands from a commander or computer
  if(command.ReadMsgs() > 0){
    if(command.buttons&BUT_R3){ gaitSelect(RIPPLE);}
    if(command.buttons&BUT_R2){ gaitSelect(AMBLE);}
    if(command.buttons&BUT_R1){ gaitSelect(SMOOTH_AMBLE);}
    digitalWrite(0,HIGH-digitalRead(0));
    
      Yspeed = (3*command.walkH);
      Rspeed = -(command.lookH)/100.0;
    if(currentGait == AMBLE){
      Xspeed = 3*command.walkV;
      (bodyPosX) = ((((float)command.walkV))*(-1))/2;      
    }else if(currentGait == SMOOTH_AMBLE){
      Xspeed = 3*command.walkV;
      (bodyPosX) = ((((float)command.walkV))*(-1))/2;      
    }else{
      Xspeed = 2*command.walkV;
    }
    
   
//    if((command.buttons&BUT_RT) > 0)
//      bodyRotX = ((float)command.lookH)/250.0;
//      bodyRotY = (((float)command.lookV))/250.0;      
//    
  }

  // if our previous interpolation is complete, recompute the IK
  if(bioloid.interpolating == 0){
    doIK();
    bioloid.interpolateSetup(tranTime);
  }

  // update joints
  bioloid.interpolateStep();

}


