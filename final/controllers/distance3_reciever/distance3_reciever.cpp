#include <webots/DistanceSensor.hpp>
#include <webots/Motor.hpp>
#include <webots/Robot.hpp>
#include <webots/Receiver.hpp>
#include <iostream>
#include <webots/Keyboard.hpp>
#include <webots/GPS.hpp>
#include <webots/InertialUnit.hpp>
#include <webots/Camera.hpp>

#define TIME_STEP 64  
using namespace std;
using namespace webots;

void speed(int ,int );

Motor *wheels[4];
char wheels_names[4][8] = {"wheel1", "wheel2", "wheel3", "wheel4"};

int main(int argc, char **argv) {
  Robot *robot = new Robot();
  DistanceSensor *ds[3];
  char dsNames[3][10] = {"ds_left", "ds_right","ds_mid"};
  DistanceSensor *ds_cam[2];
  char dsCam[2][15] = {"distance_left", "distance_right"};
  
  for (int i = 0; i < 3; i++) {
    ds[i] = robot->getDistanceSensor(dsNames[i]);
    ds[i]->enable(TIME_STEP);
  }
  
  for (int i = 0; i < 2; i++) {
    ds_cam[i] = robot->getDistanceSensor(dsCam[i]);
    ds_cam[i]->enable(TIME_STEP);
  }
  
  //Motor *wheels[4];
  //char wheels_names[4][8] = {"wheel1", "wheel2", "wheel3", "wheel4"};
  for (int i = 0; i < 4; i++) {
    wheels[i] = robot->getMotor(wheels_names[i]);
    wheels[i]->setPosition(INFINITY);
    wheels[i]->setVelocity(0.0);
  }

  Camera *cm;
  cm=robot->getCamera("CAM");
  cm->enable(TIME_STEP);
  cm->recognitionEnable(TIME_STEP);
/*
  Receiver *receiver;
  receiver=robot->getReceiver("receiver");
  receiver->enable(TIME_STEP);
 */ 
  int avoidObstacleCounter = 0;
  int c=0; 
  int count=0;
  double leftSpeed=1,rightSpeed=1;
  int x_out=129;
  int man=0;
  int counta=0;
  double dscam_0=1000, dscam_1=1000;

  
  while (robot->step(TIME_STEP) != -1) {
    //std::cout << "Hello World!" << std::endl;
  const unsigned char *image = cm->getImage();
  int k=200;
  //cout<<ds[0]->getValue()<<endl;
  dscam_0 = ds_cam[0]->getValue();
  dscam_1 = ds_cam[1]->getValue();
  //cout<<dscam_0<<endl;
  for (int x =0; x <128; x++){
    for (int y = 0; y <128; y++) {
      int r = cm->imageGetRed(image, 128, x, y);
      int g = cm->imageGetGreen(image, 128, x, y);
      int b = cm->imageGetBlue(image, 128, x, y);
      //std::cout <<"red= , green= , blue= "<< r <<' '<< g <<' '<< b << std::endl;
     
      if(r>200)
         {
            k=x;
            std::cout <<"red= "<< b <<std::endl;
         }
         if(b>100 && k==x)
         {  
             if(x<32)
             {
               x_out=x;
               man=-1;
               //cout<<" hello2 "<<endl;
             }
             else if (x>96){
               x_out=x;
               man=1;
              //cout<<" hello3 "<<endl;
             }
             //speed(leftSpeed,rightSpeed);
             x_out=129; 
         }
          k=200;
         
    }
    
  }
  //cout<<dscam_1<<endl;
  if((man==-1) && (dscam_0<1000)&&(ds[2]->getValue()==1000)){
      cout<<" hello2 "<<endl;
      leftSpeed=3;
      rightSpeed=-3;
      man=0;
  }
  
  else if((man==1) && (dscam_1<1000)&&(ds[2]->getValue()==1000))
  {
    cout<<" hello3 "<<endl;
    leftSpeed=-3;
    rightSpeed=3;
    man=0;
  }
    /*
    if (receiver->getQueueLength()>0)
    {
      std::cout << "Hello World!" << std::endl;
    }
  
  while (receiver->getQueueLength() > 0) {
    const void *message = receiver->getData();
    const double *dir = receiver->getEmitterDirection();
    double signal = receiver->getSignalStrength();
    std::cout <<"received: (signal=, dir=[])\n"<< message << signal << dir[0] << dir[1] << dir[2];
    receiver->nextPacket();
  }
*/  
  if(x_out==129)
  {
    if ((avoidObstacleCounter > 0)&&(c==1)) { //left obstacle
        avoidObstacleCounter--;
        leftSpeed = 1.0;
        rightSpeed =-1.0;
        count++;
    } 
    else if((avoidObstacleCounter > 0)&&(c==2)){ //right obstacle
        avoidObstacleCounter--;
        leftSpeed = -1.0;
        rightSpeed =1.0;
        count++;
    }
    else if((avoidObstacleCounter > 0)&&(c==3)){ //right obstacle
        avoidObstacleCounter--;
        leftSpeed = 1.0;
        rightSpeed =-1.0;
        count++;
    }    
    
    else { // read sensors
        if (ds[2]->getValue() < 1000.0){
          if (ds[0]->getValue() < 1000.0){
            avoidObstacleCounter = 56;
            c=1;
          }
          else if (ds[1]->getValue() < 1000.0){
            avoidObstacleCounter = 56;
            c=2;
          }
          else{
            avoidObstacleCounter = 56;
            c=3;
          }
        }
        else
        {
          leftSpeed = 3.0;
          rightSpeed = 3.0;
        }
        
    }
      speed(leftSpeed,rightSpeed);    
     
   } 
   else{
    
    if(x_out<128)
    {
    speed(leftSpeed,rightSpeed);
    if(counta==19 && man!=0)
    {
      speed(0,0);
      
    }
    if(counta==20 && man!=0)
    {
      x_out=130;
      counta=0;
    }
    
    if(man==0 && counta==21)
     { 
      x_out=130;
      counta=0;
     }
    counta+=1;
     
    }
    }
    //x_out=129; 
  }  
  delete robot;
  return 0;  // EXIT_SUCCESS

}

void speed(int leftSpeed,int rightSpeed)
{

 wheels[0]->setVelocity(leftSpeed);
 wheels[1]->setVelocity(rightSpeed);
 wheels[2]->setVelocity(leftSpeed);
 wheels[3]->setVelocity(rightSpeed);   
}
