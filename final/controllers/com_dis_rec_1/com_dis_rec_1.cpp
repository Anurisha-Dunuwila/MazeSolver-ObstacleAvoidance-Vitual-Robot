#include <webots/DistanceSensor.hpp>
#include <webots/Motor.hpp>
#include <webots/Robot.hpp>
#include <webots/Receiver.hpp>
#include <iostream>
#include <string>
//#include <vector>
//#include <stdio.h>
#include <typeinfo>
//#include "utf8.h"
//#include <fstream>
#include <Windows.h>
#include<cstring>
#include <sstream> 
#include <webots/Compass.hpp>
//#include <struct>

#define TIME_STEP 64  
using namespace std;
using namespace webots;

 
double get_bearing_in_degrees();
void speed(int ,int );
Compass *compass;

Motor *wheels[4];
char wheels_names[4][8] = {"wheel1", "wheel2", "wheel3", "wheel4"};
  
int main(int argc, char **argv) {
  Robot *robot = new Robot();
  DistanceSensor *ds[3];
  char dsNames[3][10] = {"ds_left", "ds_right","ds_mid"};
  
  for (int i = 0; i < 3; i++) {
    ds[i] = robot->getDistanceSensor(dsNames[i]);
    ds[i]->enable(TIME_STEP);
  }
 
  for (int i = 0; i < 4; i++) {
    wheels[i] = robot->getMotor(wheels_names[i]);
    wheels[i]->setPosition(INFINITY);
    wheels[i]->setVelocity(0.0);
  }
  
  compass=robot->getCompass("comp1");
  compass->enable(TIME_STEP);

  Receiver *receiver; 
  receiver=robot->getReceiver("recei");
  receiver->enable(TIME_STEP);
  
  int avoidObstacleCounter = 0;
  int recicounter=0;
  int c=0;
  int count=0;
  int aaaa=0;
  int yy=1;
  int ide=0;
  //int m=0;
  double leftSpeed=1,rightSpeed=1;
  while (robot->step(TIME_STEP) != -1) {
  
    double val = get_bearing_in_degrees();
    std::cout << val << std::endl;
 /*   
    while ((receiver->getQueueLength() > 0)&&(m<67)) {
      const void *msg = receiver->getData();
      wprintf(L"%s\n",msg);
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
      
       cout << yy<< endl;
       
       switch(yy)
       {
      
       case 1:
           if(((val>80) || (val<100))&& (recicounter>0))
           {
             leftSpeed = -1.0;
             rightSpeed =1.0;
             speed(-1,1);
           }
           
           else if (((val>260) || (val<280))&& (recicounter>0))
           {
             leftSpeed = 1.0;
             rightSpeed =-1.0;
             speed(1,-1);
           }
           ide=1;
          
           break;
       case 2:     
           if(((val>350) || (val<10))&& (recicounter>0))
           {
             leftSpeed = 1.0;
             rightSpeed =-1.0;
             speed(1,-1);
           }
           
           else if (((val>170) || (val<190))&& (recicounter>0))
           {
             leftSpeed = -1.0;
             rightSpeed =1.0;
             speed(-1,1);
           }
           ide=1;
          
           break;
       case 3:   
           if(((val>80) || (val<100))&& (recicounter>0))
           {
             leftSpeed = 1.0;
             rightSpeed =-1.0;
             //speed(1,-1);
           }
           
           else if (((val>260) || (val<280))&& (recicounter>0))
           {
             recicounter--;
             leftSpeed = -1.0;
             rightSpeed =1.0;
             //speed(-1,1);
             cout << "Hello" << endl;
           }
           ide=1;
           speed(leftSpeed,rightSpeed);
           break;
       case 4:    
           if(((val>350) || (val<10))&& (recicounter>0))
           {
             recicounter--;
             leftSpeed = -1.0;
             rightSpeed =1.0;
             //speed(-1,1);
           }
           
           else if (((val>170) || (val<190))&&(recicounter>0))
           {
             recicounter--;
             leftSpeed = 1.0;
             rightSpeed =-1.0;
             //speed(1,-1);
             cout << "Hello" << endl;
           }
           ide=1;
           speed(leftSpeed,rightSpeed);
           break;
        default:
            
       
       }
       speed(3,3);
       yy=0;
       aaaa=0;
       
       //m=m+1;
     
      receiver->nextPacket();
   }
*/   
   
   
    if ((recicounter> 0)&&(yy==1)) { //left obstacle
        if((val>80) || (val<100))
        {
           recicounter--;
           leftSpeed = -1.0;
           rightSpeed =1.0;
        }
           
        else if ((val>260) || (val<280))
        {
           recicounter--;
           leftSpeed = 1.0;
           rightSpeed =-1.0;
        }
        ide=1;
    } 
    else if((recicounter> 0)&&(yy==2)){ //right obstacle
        if((val>350) || (val<10))
        {
           recicounter--;
           leftSpeed = 1.0;
           rightSpeed =-1.0;
        }
           
        else if ((val>170) || (val<190))
        {
           recicounter--;
           leftSpeed = -1.0;
           rightSpeed =1.0;
        }
           ide=1;
          
    }
    else if((recicounter> 0)&&(yy==3)){ //right obstacle
        if((val>80) || (val<100))
        {
           recicounter--;
           leftSpeed = 1.0;
           rightSpeed =-1.0;
             //speed(1,-1);
        }
           
        else if ((val>260) || (val<280))
        {
           recicounter--;
           leftSpeed = -1.0;
           rightSpeed =1.0;
             //speed(-1,1);
           cout << "Hello" << endl;
        }
        ide=1;
    } 
    else if((recicounter> 0)&&(yy==4)){ //right obstacle
        if((val>350) || (val<10))
        {
           recicounter--;
           leftSpeed = -1.0;
           rightSpeed =1.0;
             //speed(-1,1);
        }
           
        else if ((val>170) || (val<190))
        {
           recicounter--;
           leftSpeed = 1.0;
           rightSpeed =-1.0;
             //speed(1,-1);
           cout << "Hello" << endl;
        }
        ide=1;
    }   
    else { // read sensors
      if((receiver->getQueueLength() > 0)){
        while (receiver->getQueueLength() > 0) {
        const void *msg = receiver->getData();
        wprintf(L"%s\n",msg);
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
        
        cout << yy<< endl;
        recicounter=56;
      }
     }
     else{
       leftSpeed = 3.0;
       rightSpeed = 3.0; 
       yy=0;
       aaaa=0;
     }
   }
   speed(leftSpeed,rightSpeed) ; 
   
   
   
   
   
    //m=m+1;
    /*
   if(m==150)
   {
      m=0;
   }
   */
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
        else if(ide==0)
        {
          leftSpeed = 3.0;
          rightSpeed = 3.0;
        }
        
    }
    speed(leftSpeed,rightSpeed);
    ide=0;
   /*
      wheels[0]->setVelocity(leftSpeed);
      wheels[1]->setVelocity(rightSpeed);
      wheels[2]->setVelocity(leftSpeed);
      wheels[3]->setVelocity(rightSpeed);      
  */
  //m=0;
  } 
  //m=0; 
  delete robot;
  return 0;  // EXIT_SUCCESS

}

double get_bearing_in_degrees() {
  const double *north = compass->getValues();
  double rad = atan2(north[0], north[2]);
  double bearing = (rad - 1.5708) / M_PI * 180.0;
  if (bearing < 0.0)
    bearing = bearing + 360.0;
  return bearing;
}

void speed(int leftSpeed,int rightSpeed)
{

    wheels[0]->setVelocity(leftSpeed);
    wheels[1]->setVelocity(rightSpeed);
    wheels[2]->setVelocity(leftSpeed);
    wheels[3]->setVelocity(rightSpeed); 
     
}