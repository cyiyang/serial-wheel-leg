#include "k210_msg_deal.h"

#define K210Serial  Serial 
msg_k210 k210_msg ;
char buff_com[50];

void setup()
{
  K210Serial.begin(115200);
}


void loop()
{
  while (K210Serial.available()) 
  {
      recv_k210msg(K210Serial.read());
      
      if (k210_msg.class_n != 0)//例程号不为空
      {
        if(k210_msg.class_n == 10)//自主学习分类
        {
          sprintf(buff_com,"id = %c\r\n",k210_msg.id);
          K210Serial.print(buff_com);
      
          k210_msg.class_n = 0;//清除例程号  
        }
        
      }
    
  }
}
