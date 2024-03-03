#include <webots/DistanceSensor.hpp>
#include <webots/Motor.hpp>
#include <webots/Robot.hpp>
#include <webots/Receiver.hpp>
#include <iostream>
#include <webots/Keyboard.hpp>
#include <webots/GPS.hpp>
#include <webots/InertialUnit.hpp>
#include <webots/Camera.hpp>
#include <webots/Compass.hpp>

#define TIME_STEP 64  
using namespace webots;
using namespace std;

float wb_range_finder_image_get_depth(const float *a, int width, int x, int y);
virtual void wb_range_finder_enable(TIME_STEP);

int main(int argc, char **argv) {
  Robot *robot = new Robot();
  DistanceSensor *ds[3];
  char dsNames[3][10] = {"ds_left", "ds_right","ds_mid"};
  
  for (int i = 0; i < 3; i++) {
    ds[i] = robot->getDistanceSensor(dsNames[i]);
    ds[i]->enable(TIME_STEP);
  }
  
  Motor *wheels[4];
  char wheels_names[4][8] = {"wheel1", "wheel2", "wheel3", "wheel4"};
  for (int i = 0; i < 4; i++) {
    wheels[i] = robot->getMotor(wheels_names[i]);
    wheels[i]->setPosition(INFINITY);
    wheels[i]->setVelocity(0.0);
  }
  
  //Receiver *receiver;
  //receiver=robot->getReceiver("receiver");
  //receiver->enable(TIME_STEP);
  
  Camera *cm;
  cm=robot->getCamera("CAM");
  cm->enable(TIME_STEP);
  cm->recognitionEnable(TIME_STEP);
  
  class webots::RangeFinder *rf;
  //rf=robot->getRangeFinder("range");
  rf=wb_range_finder_enable(TIME_STEP);
  //rf->enable(TIME_STEP); 
  //rf->recognitionEnable(TIME_STEP);
  
  int avoidObstacleCounter = 0;
  int c=0; 
  double leftSpeed=1,rightSpeed=1;
  int x_out=129,x_outt=129;
  int man=0;
  int counta=0;
  double dscam_0=1000, dscam_1=1000;
  int hello2=0,hello3=0;
  float depth=0;
  
  while (robot->step(TIME_STEP) != -1) {
    const unsigned char *image = cm->getImage();
    int k=200;
    const float *image1 = rf->getRangeImage();
    for (int x =0; x <128; x++){
      for (int y = 0; y <128; y++) {
        int r = cm->imageGetRed(image, 128, x, y);
        int g = cm->imageGetGreen(image, 128, x, y);
        int b = cm->imageGetBlue(image, 128, x, y);
        //std::cout <<"red= , green= , blue= "<< r <<' '<< g <<' '<< b << std::endl;
        
        depth = wb_range_finder_image_get_depth(*image1,128,x,y);
        std::cout <<"depth"<<depth<< std::endl;
        
        if(r>60)
           {
              std::cout <<"red= , green= , blue= "<< r <<' '<< g <<' '<< b << std::endl;
              k=x;
           }
           if(b>100 && k==x)
           {  
               if(x<32)
               {
                 x_outt=x;
                 man=-1;
               }
               else if (x>96){
                 x_outt=x;
                 man=1;
               }
           }
            k=200;
      }
   }
  
    if((man==-1) && (dscam_0<1000)&&(ds[2]->getValue()==1000)){
        cout<<" hello2 "<<endl;
        hello2=1;
        leftSpeed=3;
        rightSpeed=-3;
        x_out=x_outt;
        man=0;
    }
    
    else if((man==1) && (dscam_1<1000)&&(ds[2]->getValue()==1000))
    {
      cout<<" hello3 "<<endl;
      x_out=x_outt;
      hello3=1;
      leftSpeed=-3;
      rightSpeed=3;
      man=0;
    }
  
    if ((avoidObstacleCounter > 0)&&(c==1)) { //left obstacle
        avoidObstacleCounter--;
        leftSpeed = 1.0;
        rightSpeed =-1.0;
        //count++;
    } 
    else if((avoidObstacleCounter > 0)&&(c==2)){ //right obstacle
        avoidObstacleCounter--;
        leftSpeed = -1.0;
        rightSpeed =1.0;
        //count++;
    }
    else if((avoidObstacleCounter > 0)&&(c==3)){ //right obstacle
        avoidObstacleCounter--;
        leftSpeed = 1.0;
        rightSpeed =-1.0;
        //count++;
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
      wheels[0]->setVelocity(leftSpeed);
      wheels[1]->setVelocity(rightSpeed);
      wheels[2]->setVelocity(leftSpeed);
      wheels[3]->setVelocity(rightSpeed);      
  
  }    
  delete robot;
  return 0;  // EXIT_SUCCESS

}