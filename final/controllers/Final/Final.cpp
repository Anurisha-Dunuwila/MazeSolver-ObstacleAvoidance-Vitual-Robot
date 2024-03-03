#include <sstream> 
#include <webots/DistanceSensor.hpp>
#include <webots/Motor.hpp>
#include <webots/Robot.hpp>
#include <webots/Receiver.hpp>
#include <webots/Camera.hpp>
#include<webots/Compass.hpp>
#include <string.h>
#include <stdio.h>
#include<vector>
#include<cstring>
#include <typeinfo>


#include<iostream>
using namespace std;
#define TIME_STEP 64  
using namespace webots;

double get_bearing_in_degrees();  //for get direction 
void speed(int a,int b);          //to execute speed
int tot=-1;
int condition =0;

Compass *compass;
Motor *wheels[4];
Receiver *receiver;

vector<int> leftn;
vector<int> rightn;

int main(int argc, char **argv) {
 
 
  
  Robot *robot = new Robot();
  DistanceSensor *ds[3];
  char dsNames[3][10] = {"ds_left", "ds_right","ds_mid"};
  DistanceSensor *ds_cam[2];
  char dsCam[2][15] = {"distance_left", "distance_right"};
  DistanceSensor *ds_c[2];
  char dsC[2][4] = {"L", "R"};

    
  receiver=robot->getReceiver("receiver");
  receiver->enable(TIME_STEP);
  

  /*Three distace sensors used  (ds -- detect walls)   (ds_cam -- detect whether man is passed)   (ds_c -- detect sanitiser and man) */
  
  for (int i = 0; i < 3; i++) {
    ds[i] = robot->getDistanceSensor(dsNames[i]);
    ds[i]->enable(TIME_STEP);
  }
  
  for (int i = 0; i < 2; i++) {
    ds_cam[i] = robot->getDistanceSensor(dsCam[i]);
    ds_cam[i]->enable(TIME_STEP);
  }
  
   for (int i = 0; i < 2; i++) {
    ds_c[i] = robot->getDistanceSensor(dsC[i]);
    ds_c[i]->enable(TIME_STEP);
  }
  
  
  char wheels_names[4][8] = {"wheel1", "wheel2", "wheel3", "wheel4"};   //velocities of 4 wheels
  for (int i = 0; i < 4; i++) {
    wheels[i] = robot->getMotor(wheels_names[i]);
    wheels[i]->setPosition(INFINITY);
    wheels[i]->setVelocity(0.0);
  }
  
  Camera *camera;    // camera node
  camera=robot->getCamera("camera");
  camera->enable(TIME_STEP);
  
  
  compass=robot->getCompass("compass");  //compass node
  compass->enable(TIME_STEP);
  
  
  int avoidObstacleCounter = 0;  //to detect walls
  int obstacle=1;        
  int c=4;         //obstacle type c==4 -- no obstacle
  int count=0;   
  int leftSpeed=1,rightSpeed=1;  //speed for wheels
  int x_out=129;   //x_out pixel used to detect objects deteted by camera like man,sanitizer
  int x_re=10;    //To detect forward and backward motion
  int C_counter=3;  //enable and disable camera
  int counta=0;   
  int countb=0;
  
  int nop=0;

  int man=0;  //To check man 
  int fb=0;  //to forward and backward Receiver

  int pass=0;

  double receiverStrength=0;  //strength on reciever signal

  string old;  //two strings are used to detect whether new reciever signal is not equal to previous 
  string new1;
 

  int aaaa=0;
  int yy=1;
 
  while (robot->step(TIME_STEP) != -1) {
  condition = 0;
  double val;
  val=get_bearing_in_degrees();  //direction assigned to val
      
  while (x_re==10 && receiver->getQueueLength() > 0) {   //reciever signal taken
    const void *msg = receiver->getData();       
    
    std::string s( reinterpret_cast<char const*>(msg));
    std::string delimiter = ",";
    
    size_t pos = 0;
    std::string token;
    new1.assign(s);
      int g=0;
      while ((pos = s.find(delimiter)) != std::string::npos) {  //sort by ","
          g=g+1;
          token = s.substr(0, pos);
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
      if(yy!=0)
        {  
        receiverStrength=receiver->getSignalStrength();  //stregth is assigned
           
        if(fb==0 && old!=new1 && receiverStrength>10.3 && (ds_cam[1]->getValue()==1000 || ds_cam[0]->getValue()==1000))  //i
             {
              old.assign(new1);  //if not equal to new1 assigned to old
              x_re=20;
              if(359<=val && val<=361)   
              {
               switch(yy)  //cases to assign values to speed with relative to val
               {
               case 1:
                leftSpeed=3.0;
                rightSpeed=3.0;
                break;
               case 2:
                leftSpeed=3.0;
                rightSpeed=-3.0;
                break;
               case 3:
                leftSpeed=-3.0;
                rightSpeed=-3.0;
                break;
               case 4:
                leftSpeed=-3.0;
                rightSpeed=3.0;
                break;
               }
              } 
              if(269<=val && val<=271)
              {
               switch(yy)   //cases to assign values to speed with relative to val
               {
               case 1:
                leftSpeed=3.0;
                rightSpeed=-3.0;
                break;
               case 2:
                leftSpeed=-3.0;
                rightSpeed=-3.0;
                break;
               case 3:
                leftSpeed=-3.0;
                rightSpeed=3.0;
                break;
               case 4:
                leftSpeed=3.0;
                rightSpeed=3.0;
                break;
               }
             }
             if(179<=val && val<=181)
              { 
               switch(yy)  //cases to assign values to speed with relative to val
               {
               case 1:
                leftSpeed=-3.0;
                rightSpeed=-3.0;
                break;
               case 2:
                leftSpeed=-3.0;
                rightSpeed=3.0;
                break;
               case 3:
                leftSpeed=3.0;
                rightSpeed=3.0;
                break;
               case 4:
                leftSpeed=3.0;
                rightSpeed=-3.0;
                break;
               }
             }
             if((449<=val && val<=451)||(89<=val && val<=91))
              { 
               switch(yy)  //cases to assign values to speed with relative to val
               {
               case 1:
                leftSpeed=-3.0;
                rightSpeed=3.0;
                break;
               case 2:
                leftSpeed=3.0;
                rightSpeed=3.0;
                break;
               case 3:
                leftSpeed=3.0;
                rightSpeed=-3.0;
                break;
               case 4:
                leftSpeed=-3.0;
                rightSpeed=-3.0;
                break;
               }
             }
             if(x_out!=500)   //if receiver detected rotated speed is taken to two vectors
             {
              leftn.push_back(-leftSpeed);
              rightn.push_back(-rightSpeed);
              tot+=1;
             }
          }
           /* In backward motion fb=1 so motor executes the value taken in forward motion */
      if(fb==1 && old!=new1 && receiverStrength>10.3 && (ds_cam[1]->getValue()==1000 || ds_cam[0]->getValue()==1000))  
            { 
               old.assign(new1);
               x_re=20;               // executes using if(x_re==20) 
               if(leftn[tot]!=rightn[tot])
               {
                 leftSpeed=leftn[tot];  
                 rightSpeed=rightn[tot];
               }
               else
               {
                 leftSpeed=-leftn[tot];  
                 rightSpeed=-rightn[tot];
               }
               tot-=1;     //when reciever detected tot value reduced
               if(tot==-1)
               {
               tot=-10;  //if  tot==-1  then last reciever also detected
               }
      
            }
    }
    
    yy=0;
    aaaa=0;
    receiver->nextPacket();
    }
  
  while(C_counter==-5)  //camera node on
    {
    
    const unsigned char *image = camera->getImage();
    int image_width=camera->getWidth();
    int image_height=camera->getHeight();
     int x=0;
     while(x<image_width)  //x value <128
     {
       int y =0;
       while(y<image_height)  //y value <128
       {
         int r = camera->imageGetRed(image, image_width, x, y);  // r and g and b values are taken for image
         int b = camera->imageGetBlue(image, image_width, x, y);
         int g = camera->imageGetBlue(image, image_width, x, y);
         
        
           if(ds_c[0]->getValue()<700)   //if front left ds sensor <700
           { 
               
             if(ds[2]->getValue()==1000 &&((b<100 && r>150)||(r>b && b>150 && r>180)) && y<60 && c==4) 
             {
                 if(x<40 && pass==0)  //x<40 is man is in left corner
                 {
                   pass=1;
                   x_out=x;
                   leftSpeed=3;
                   rightSpeed=-3;
                   man=-1;
                   cout<<"hello user"<<endl;
                   x=128;  // x and y set to 128 to get out from while loops
                   y=128;
                 }
             }
             else
             {
                 if(x<64 && y>80 && r<150 && g>200 && b>200)  //sanitiser is in y>80
                 {
                     cout<<"sanitiser"<<endl;  //detect sanitizer
                     x_out=600;
                     x=128;
                     y=128;
                     nop=1;
                 }
             }
           }
          if(ds_c[1]->getValue()<700)    //if front right ds sensor <700
          {
             
             if(ds[2]->getValue()==1000 && ((b<100 && r>150)||(r>b && b>150 && r>180)) && y<60 && c==4)
             {  
                  if(x<128 && x>80 && pass==0)  //x<128 is man is in right corner
                  {
                    pass=1;
                    x_out=x;
                    leftSpeed=-3;
                    rightSpeed=3;
                    man=1;
                    cout<<"hello tharushi"<<endl;
                    x=128;
                    y=128;
                   }
              }
              else
             {
                 if(x>64 && y>80 && r<150 && g>200 && b>200 )   //sanitiser is in y>80
                 {
                   cout<<"sanitiser"<<endl;   //detect sanitizer
                   x_out=600;
                   x=128;
                   nop=1;
                   y=128;
                 }
             }
          }
          
       
         if(ds[2]->getValue()<1000)  //front ds sensor value taken  
         {
           if(r<100 && b>150 && g>150 && y>40 && c==4 && pass==0)  //condition for supermarket
           {
             x_out=500;  //x_out=500 
             fb=1;       //set fb =1 to get backward motion
             x_re=5;
             old="2";
             leftSpeed=-3;
             rightSpeed=3;
             x=128;
             y=128;
           }
         }
          
         y+=1;
         }

        x+=1;      
         }
      
     C_counter=0; //set to 0 to exit from loop
    }
 
   if(x_out==600)  //sanitiser
   {
    
      if(nop==1)
      { 
       if(ds[0]->getValue()<300)  //check left ultra sonic sensor
       {
           leftSpeed=-3;
           rightSpeed=3;
           nop=-1;
       }
       if(ds[1]->getValue()<300)  //check  right one
       {
           leftSpeed=3;
           rightSpeed=-3;
           nop=-1;
       }
       if(nop!=-1)
       {
           speed(3,3);
           if(counta==30)
           {
             x_out=129;
             counta=-1;
             countb=0;
           }
       }
       
      }
     if(nop==-1 || nop==-2)
     {
       if(countb==20)
       {
          countb=0;
          leftSpeed=-leftSpeed;
          rightSpeed=-rightSpeed;
          if(nop==-1)
          {
            nop=-2;
          }
          else
          {
            leftSpeed=0;
            rightSpeed=0;
            countb=-1;
            counta=-1;
            x_out=129;
          }
       }
       else
       {
         speed(leftSpeed,rightSpeed);
       }
       countb+=1;
     }        
     counta+=1;
   }
         
  
   if(x_out==129 && x_re==10)
   {

    if ((avoidObstacleCounter > 0)&&(c==1)) { //left obstacle
        avoidObstacleCounter--;
        leftSpeed = 3.0;
        rightSpeed =-3.0;
        obstacle=1;
        count++;
     
    } 
    else if((avoidObstacleCounter > 0)&&(c==2)){ //right obstacle
        avoidObstacleCounter--;
        leftSpeed = -3.0;
        rightSpeed =3.0;
        obstacle=1;
        count++;
    }
    else if((avoidObstacleCounter > 0)&&(c==3)){ //right obstacle
        avoidObstacleCounter--;
        leftSpeed = 3.0;
        rightSpeed =-3.0;
        obstacle=1;
        count++;
     
    }    
    else { // read sensors
        if (ds[2]->getValue() < 1000.0){
          if (ds[0]->getValue() < 1000.0){
            avoidObstacleCounter = 18;
            c=1;
          }
          else if (ds[1]->getValue() < 1000.0){
            avoidObstacleCounter = 18;
            c=2;
          
          }
          else{
            avoidObstacleCounter = 18;
            c=3;
          }
          }
        else
        { 
          if((val<359 && val>350 )||(val<269 && val>260)||(val<179 && val>170)||(val<449 && val>440))  //check whether direction is good for front movement
          {
           
           leftSpeed = 1;
           rightSpeed = -1;
           C_counter=1; //set values for C_counter
     
          }
          if((val>361 && val<370) || (val<280 && val>271) ||(val<190 && val>181)||(val<100 && val>91)) //check whether direction is good for front movement
          {
           
           leftSpeed = -1;
           rightSpeed = 1;
           C_counter=1; //set values for C_counter
          }
          
          if((359<val && val<=361) || (269<val && val<=271)||(179<val && val<=181)||(449<val && val<451)||(89<val && val<=91)) 
          {
          
           leftSpeed = 3.0;  //direction is good for front movement
           rightSpeed = 3.0;
           C_counter=1; 
           c=4;  //let c=4 to move forward
          }
        }
           
         
     
    }
     speed(leftSpeed,rightSpeed);  //call for speed
    }
    
    else{
  
    if(x_out<128)  //detected man
    {
    
    if(counta==20)
    {speed(0,0);}
    
    else
    {speed(leftSpeed,rightSpeed);}  //rotate 
    
    if(counta==21 && man!=0)
    {
      x_out=130;  //if equal 21 then x_out =130
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
     speed(3,3); //movement forward
     counta+=1;  
     if(counta==15 && man!=0)
     { 
      x_out=132;
      leftSpeed=-leftSpeed;
      rightSpeed=-rightSpeed;  //invert speeds
      counta=0;
     }
     if(man==0 && ds[2]->getValue()<1000)   //check wether robo is in middle
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
      pass=0;
     }
    }

    
    if(x_out==134)
    {
     if(man==-1 || man==-2){
      if(ds[2]->getValue()==1000 && ds_cam[0]->getValue()<500)  //check wether man passed
      {
        man=-2;

      }
      else
      {
        if(man==-1)
        {
        
        
        if((val<359 && val>350 )||(val<269 && val>260)||(val<179 && val>170)||(val<449 && val>440))  //align rotation
          {speed(1,-1);}
        if((val>361 && val<370) || (val<280 && val>271) ||(val<190 && val>181)||(val<100 && val>91))
          {speed(-1,1);}
         if((359<=val && val<=361) || (269<=val && val<=271)||(179<=val && val<=181)||(449<=val && val<=451)||(89<=val && val<=91)) 
          {speed(3,3);}
        }
        else
        {
          counta+=1;
          if(counta==3)
          {
          
            x_out=20;
            counta=0;
            man=0;
          }
          }
       } 
       
       if(ds[2]->getValue()<600)
       {
          x_out=129;
          counta=0;
          pass=0;
       }
     }     
     if(man==1 || man==2){
      if(ds[2]->getValue()==1000 && ds_cam[1]->getValue()<500)  //check wether man passed
      {
        man=2;

      }
      else{
        if(man==1)
        {
       
        if((val<359 && val>350 )||(val<269 && val>260)||(val<179 && val>170)||(val<449 && val>440))  //allign rotation
          {speed(1,-1);
           }
        if((val>361 && val<370) || (val<280 && val>271) ||(val<190 && val>181)||(val<100 && val>91))
          {speed(-1,1);
          ;}
        if((359<=val && val<=361) || (269<=val && val<=271)||(179<=val && val<=181)||(449<=val && val<=451)||(89<=val && val<=91)) 
          {speed(3,3);
           }
         }
        else{
          counta+=1;
          if(counta==3)
          {
            
            x_out=20;
            counta=0;
            man=0;
          }}}
       if(ds[2]->getValue()<600)
       {
          x_out=129;
          counta=0;
          pass=0;
       }
          
       
      }
    
   }
   }
  
   if(x_re==20)  //activate in backward motion
   {
    speed(leftSpeed,rightSpeed);
    if(countb==20)
    {
     leftSpeed=3.0;
     rightSpeed=3.0;
     x_re=10;
     countb=0;
     
    }
    countb+=1;
    
   }
   
   else
   {
   if(x_re==5)  
   {
    speed(leftSpeed,rightSpeed);
    
    if(countb==39)
    {
     leftSpeed=3.0;
     rightSpeed=3.0;
     x_re=10;
     countb=0;
     C_counter=1;
     x_out=129;
     
    }
    countb+=1;
    
   }
   }
   
  if(ds_cam[0]->getValue()==1000 && ds_cam[1]->getValue()==1000 && ds_c[0]->getValue()==1000 && ds_c[1]->getValue()==1000  && tot==-10 )
  {
   speed(3,-3);
   C_counter=3;
   x_out=10000;   //back to start place
   x_re=50;
   break;
  }
 }
  
  delete robot;
  return 0;  // EXIT_SUCCESS
}


double get_bearing_in_degrees()  //function to get direction
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


void speed(int leftSpeed,int rightSpeed)  //execute speed 
{

 wheels[0]->setVelocity(leftSpeed);
 wheels[1]->setVelocity(rightSpeed);
 wheels[2]->setVelocity(leftSpeed);
 wheels[3]->setVelocity(rightSpeed);
         
}


  