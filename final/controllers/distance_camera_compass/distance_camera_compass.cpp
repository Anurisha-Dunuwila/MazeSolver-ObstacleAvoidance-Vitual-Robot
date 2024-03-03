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
using namespace std;
using namespace webots;

void speed(int ,int );
double get_bearing_in_degrees();

Compass *compass;
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
  
  for (int i = 0; i < 4; i++) {
    wheels[i] = robot->getMotor(wheels_names[i]);
    wheels[i]->setPosition(INFINITY);
    wheels[i]->setVelocity(0.0);
  }
  
  compass=robot->getCompass("compass");
  compass->enable(TIME_STEP);

  Camera *cm;
  cm=robot->getCamera("camera");
  cm->enable(TIME_STEP);
  cm->recognitionEnable(TIME_STEP);
  
  int avoidObstacleCounter = 0;
  int c=0; 
  double leftSpeed=1,rightSpeed=1;
  int x_out=129,x_outt=129;
  int man=0;
  int counta=0;
  double dscam_0=1000, dscam_1=1000;
  int hello2=0,hello3=0,a=0;

  
  while (robot->step(TIME_STEP) != -1) {
  
  double val = get_bearing_in_degrees();
  std::cout << val << std::endl;
  
  const unsigned char *image = cm->getImage();
  int k=200;
  dscam_0 = ds_cam[0]->getValue();
  dscam_1 = ds_cam[1]->getValue();

  for (int x =0; x <128; x++){
    for (int y = 0; y <128; y++) {
      int r = cm->imageGetRed(image, 128, x, y);
      int g = cm->imageGetGreen(image, 128, x, y);
      int b = cm->imageGetBlue(image, 128, x, y);
      //std::cout <<"red= , green= , blue= "<< r <<' '<< g <<' '<< b << std::endl;
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
  
  if(x_out==129)
  {
    if ((avoidObstacleCounter > 0)&&(c==1)) { //left obstacle
        avoidObstacleCounter--;
        leftSpeed = 1.0;
        rightSpeed =-1.0;
    } 
    else if((avoidObstacleCounter > 0)&&(c==2)){ //right obstacle
        avoidObstacleCounter--;
        leftSpeed = -1.0;
        rightSpeed =1.0;
    }
    else if((avoidObstacleCounter > 0)&&(c==3)){ //right obstacle
        avoidObstacleCounter--;
        leftSpeed = 1.0;
        rightSpeed =-1.0;
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
          //leftSpeed = 3.0;
          //rightSpeed = 3.0;
          if((val<359 && val>350 )||(val<269 && val>260)||(val<179 && val>170)||(val<449 && val>440))
          {
           
           leftSpeed = 1;
           rightSpeed = -1;
           //C_counter=1; 
     
          }
          if((val>361 && val<370) || (val<280 && val>271) ||(val<190 && val>181)||(val<460 && val>451))
          {
           
           leftSpeed = -1;
           rightSpeed = 1;
           //C_counter=1;
         
           //c=4;
           //cout<<val<<endl;
          }
          
          if((359<val && val<=361) || (269<val && val<=271)||(179<val && val<=181)||(449<val && val<451)||(89<val && val<=91)) 
          {
          
           leftSpeed = 3.0;
           rightSpeed = 3.0;
           //C_counter=1; 
           //condition=1;
           c=4;
          }
        
     }
     }
      speed(leftSpeed,rightSpeed);    
   } 
  else{
    if((x_out<128)&&((hello2==1)||(hello3==1)))
    {
      if(counta<20)
      {
        speed(leftSpeed,rightSpeed);
        a=leftSpeed;
      }
      else if((counta>=20)&&(counta<35))
      {
        speed(3,3);
      }
      else if((counta>=35)&&(counta<55))
      {
        if(a>0)
        {
          speed(-a,a);
        }
        else if(a<0)
        {
          speed(-a,a);
        }
      }
      else if((counta>=55)&&(counta<150))
      {
        speed(3,3);
      }
      else if((counta>=150)&&(counta<170))
      {
        if(a>0)
        {
          speed(-a,a);
        }
        else if(a<0)
        {
          speed(-a,a);
        }
      }
      else if((counta>=170)&&(counta<185))
      {
        speed(3,3);
      }
      else if((counta>=185)&&(counta<203))
      {
        if(a>0)
        {
          speed(a,-a);
        }
        else{
          speed(a,-a);
        }
      }
      else
      {
        counta=-1;
        x_out=129;
        a=0;
        hello2=0;
        hello3=0;
      }
      counta+=1;
     }
  }
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

double get_bearing_in_degrees() {
  const double *north = compass->getValues();
  double rad = atan2(north[0], north[2]);
  double bearing = (rad - 1.5708) / M_PI * 180.0;
  if (bearing < 0.0)
    bearing = bearing + 360.0;
  return bearing;
}
