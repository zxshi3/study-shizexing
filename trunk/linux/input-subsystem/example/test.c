#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <fcntl.h>

#include <string.h>

#include <linux/input.h>

#include <sys/select.h>

#include <poll.h>

#define BITS_PER_LONG 32

#define BIT_WORD(nr) ((nr) / BITS_PER_LONG)

static int test_bit(int nr, const volatile unsigned long *addr)
{
   return 1UL & (addr[BIT_WORD(nr)] >> (nr & (BITS_PER_LONG-1)));
}

int main(int argc, char ** argv)
{
   struct pollfd  mypoll;
   int fd;
   unsigned long *evtype_b = (unsigned long*)malloc(sizeof(int));
   int yalv;
   if ((fd = open(argv[1], O_RDONLY)) < 0) 
   {
      perror("evdev open");
      exit(1);
   }

   memset(evtype_b, 0, sizeof(evtype_b));

   if (ioctl(fd, EVIOCGBIT(0, EV_MAX), evtype_b) < 0) {

     perror("evdev ioctl");
    }
   //size_t rb;
  // struct input_event ev[64];
  
  // rb=read(fd,ev,sizeof(struct input_event)*64);

    mypoll.fd =  fd;
    mypoll.events = POLLIN;
    while(1)
    {

       if(poll( &mypoll, 1, 10) > 0)
      {
            unsigned char data[4] ={0};   
            /*
            data的数据格式:
            data0:00xx 1xxx   ----低三位是按键值---左中右分别为01 02 04, 第4/5位分别代表x、y移动方向，右上方x/y>0,左下方xy<0
            data1:取值范围-127~127，代表x轴移动偏移量
            data2:取值范围-127~127，代表y轴移动偏移量            
            */
            usleep(50000);   
            read(fd, data, 4); //MOUSEDEV_EMUL_PS2方式每次采样数据为3个字节，多读不会出错，只返回成功读取的数据数
            if(data[0] & 0x1)
               printf("\nLeft key of mouse press\n\n");
            else if(data[0] & 0x2)
               printf("\nRight key of mouse press\n\n");
            else if(data[0] & 0x4)
               printf("\nMiddle key of mouse press\n\n");
            printf("mouse data=%02x%02x%02x%02x\n", data[0],data[1], data[2], data[3]);
        }
    }

    printf("Supported event types:\n");
 /*
    for (yalv = 0; yalv < EV_MAX; yalv++) 
    {

       if (test_bit(yalv, evtype_b))
        {

           

          printf(" Event type 0x%02x ", yalv);

         switch(yalv)
         {

             case EV_SYN :
               printf(" (Synch Events)\n");
               break;
             case EV_KEY :
                printf(" (Keys or Buttons)\n");
                break;
             case EV_REL :
                printf(" (Relative Axes)\n");
                break;
            case EV_ABS :
               printf(" (Absolute Axes)\n");
               break;
            case EV_MSC :
               printf(" (Miscellaneous)\n");
               break;
            case EV_LED :
               printf(" (LEDs)\n");
               break;
           case EV_SND :
               printf(" (Sounds)\n");
               break;
           case EV_REP :
               printf(" (Repeat)\n");
               break;
           case EV_FF :
           case EV_FF_STATUS:
              printf(" (Force Feedback)\n");
              break;

          case EV_PWR:
              printf(" (Power Management)\n");
              break; 
          default:
              printf(" (Unknown: 0x%04hx)\n",yalv);
        }
     }
    }*/
   close(fd);

}
