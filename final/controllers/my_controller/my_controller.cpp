#include <sstream> 
#include <webots/DistanceSensor.hpp>
#include <webots/Motor.hpp>
#include <webots/Robot.hpp>
#include <webots/Receiver.hpp>
#include <webots/Camera.hpp>
#include<webots/Compass.hpp>
#include <string.h>
#include <stdio.h>

#include<cstring>
#include <typeinfo>


#include<iostream>
using namespace std;
#define TIME_STEP 64  
using namespace webots;

double get_bearing_in_degrees();
void speed(int a,int b);

Compass *compass;
Motor *wheels[4];
Receiver *receiver;

int main(int argc, char **argv) {

  
  Robot *robot = new Robot();
  DistanceSensor *ds[3];
  char dsNames[3][10] = {"ds_left", "ds_right","ds_mid"};
  DistanceSensor *ds_cam[2];
  char dsCam[2][15] = {"distance_left", "distance_right"};

    
  receiver=robot->getReceiver("receiver");
  receiver->enable(TIME_STEP);
  
  
  
  for (int i = 0; i < 3; i++) {
    ds[i] = robot->getDistanceSensor(dsNames[i]);
    ds[i]->enable(TIME_STEP);
  }
  
  for (int i = 0; i < 2; i++) {
    ds_cam[i] = robot->getDistanceSensor(dsCam[i]);
    ds_cam[i]->enable(TIME_STEP);
  }
  
  
  char wheels_names[4][8] = {"wheel1", "wheel2", "wheel3", "wheel4"};
  for (int i = 0; i < 4; i++) {
    wheels[i] = robot->getMotor(wheels_names[i]);
    wheels[i]->setPosition(INFINITY);
    wheels[i]->setVelocity(0.0);
  }
  
  Camera *camera;
  camera=robot->getCamera("camera");
  camera->enable(TIME_STEP);
  
  
  compass=robot->getCompass("compass");
  compass->enable(TIME_STEP);
  
  
  int Mvalue=0;
  int avoidObstacleCounter = 0;
  int obstacle=1;
  int c=4;
  int count=0;
  double leftSpeed=1,rightSpeed=1;
  int x_out=129;
  int C_counter=1;
  int counta=0;
  int man=0;

  int aaaa=0;
  int yy=1;
 
  while (robot->step(TIME_STEP) != -1) {

   
  while (receiver->getQueueLength() > 0) {
    const void *msg = receiver->getData();
    
    wprintf(L"%s\n",msg);
    //cout<<"wenujucs"<<endl;
    std::string s( reinterpret_cast<char const*>(msg));
    std::string delimiter = ",";
    size_t pos = 0;
    std::string token;
      int g=0;
      while ((pos = s.find(delimiter)) != std::string::npos) {
          g=g+1;
          token = s.substr(0, pos);
          //std::cout << token << std::endl;
          stringstream geek(token); 
          int x = 0; 
          geek >> x; 
          if(aaaa<x)
          {
            aaaa=x;
            yy=g;
          }
          s.erase(0, pos + delimiter.length());
        }
      stringstream geek(s); 
      int x = 0; 
      geek >> x; 
      if(aaaa<x)
      {
        aaaa=x;
        yy=4;
      }
      
    cout <<"hhh"<<yy<< endl;
    yy=0;
    aaaa=0;
     receiver->nextPacket();
    }
  

  double val;
  val=get_bearing_in_degrees();  
  while(C_counter==1)
    {
    
    const unsigned char *image = camera->getImage();
    int image_width=camera->getWidth();
    int image_height=camera->getHeight();
    int k=200;
    for (int x = 0; x < image_width; x++){
      for (int y = 0; y < image_height; y++) {
         
         int r = camera->imageGetRed(image, image_width, x, y);
         int b = camera->imageGetBlue(image, image_width, x, y);
        
        
         if(r>200)
         {
            k=x;
            //cout<<r<<" "<<b<<endl;
         }
         if(b>100 && k==x)
         {  
             if(x<64)
             {
               
               x_out=x;
               leftSpeed=3.0;
               rightSpeed=-3.0;
               man=-1;
              
             }
             else{
               //cout<<"man_2"<<x<<endl;
               x_out=x;
               leftSpeed=-3.0;
               rightSpeed=3.0;
               man=1;
               
             }
          
         }
         k=200;
         }
         }
   
     C_counter=0;
     //cout<<"man_1"<<x<<endl;
     }
     
   
   if(x_out==129)
   {
 
    if ((avoidObstacleCounter > 0)&&(c==1)) { //left obstacle
        avoidObstacleCounter--;
        leftSpeed = 1.0;
        rightSpeed =-1.0;
        obstacle=1;
       
        count++;
        //cout<<"efnrg"<<endl;
    } 
    else if((avoidObstacleCounter > 0)&&(c==2)){ //right obstacle
        avoidObstacleCounter--;
        leftSpeed = -1.0;
        rightSpeed =1.0;
        obstacle=1;
        count++;
        //cout<<"efnrg"<<endl;
    }
    else if((avoidObstacleCounter > 0)&&(c==3)){ //right obstacle
        avoidObstacleCounter--;
        leftSpeed = 1.0;
        rightSpeed =-1.0;
        obstacle=1;
        count++;
        //cout<<"efnrg"<<endl;
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
        { if((val<359 && val>350 )||(val<269 && val>260)||(val<179 && val>170)||(val<449 && val>440))
          {
           
           leftSpeed = 1;
           rightSpeed = -1;
           C_counter=1; 
        
           //cout<<val<<endl;
          }
          if((val>361 && val<370) || (val<280 && val>271) ||(val<190 && val>181)||(val<460 && val>451))
          {
           
           leftSpeed = -1;
           rightSpeed = 1;
           C_counter=1; 
           c=4;
           //cout<<val<<endl;
          }
          else
          {
          if((359<val && val<361) || (269<val && val<=271)||(179<val && val<=181)||(449<val && val<451)||(89<val && val<=91)) 
          {
          
           leftSpeed = 3.0;
           rightSpeed = 3.0;
           C_counter=1; 
           c=4;
           //cout<<"bbb"<<endl;
          }
          }
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
    if(x_out==130)
    {
     speed(3,3);
     counta+=1;  
     if(counta==10)
     { 
      x_out=132;
      leftSpeed=-leftSpeed;
      rightSpeed=-rightSpeed;
      counta=0;
     }
    }
    
    if(x_out==132)
    {
     speed(leftSpeed,rightSpeed);
     counta+=1;  
     if(counta==20 && man!=0)
     { 
      x_out=134;
      counta=0;
     }
     if(man==0 && counta==21)
     { 
      x_out=129;
      counta=0;
     }
      }

    
    if(x_out==134)
    {
     if(man==-1){
      if(ds_cam[0]->getValue()<500)
      {
       man=0;
       x_out=20;
      }
      else
      {
       speed(3,3);
      }  
      }
     if(man==1){
      if(ds_cam[1]->getValue()<500)
      {
       man=0;
       x_out=20;
      }
      else
      {
       speed(3,3);
      }
    }
   }
   
   
 }
 //cout<<(news)<<endl;
 }
  
  delete robot;
  return 0;  // EXIT_SUCCESS

}

double get_bearing_in_degrees()
{
  const double *north =compass->getValues();
  double rad =atan2(north[0],north[2]);
  double bearing = (rad-1.5708)/M_PI*180;
  if(bearing <0.0)
  {
  bearing +=360.0;
  return bearing;
  }
}

void speed(int leftSpeed,int rightSpeed)
{

 wheels[0]->setVelocity(leftSpeed);
 wheels[1]->setVelocity(rightSpeed);
 wheels[2]->setVelocity(leftSpeed);
 wheels[3]->setVelocity(rightSpeed);
          
}

  