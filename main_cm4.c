/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "stdio.h"
#include "tty_utils.h"
#include "clcd.h"
#include "func_list.h"
#include "stdlib.h"
void do_help(void) ;
void do_clear(void) ;
void do_write(void) ;
void do_SetDDRAddr(void) ;
void do_SetCGRAMAddr(void) ;
void do_Command(void) ;
void do_print(void) ;
void do_pos(void) ;

f_list_type main_func_list[] = {
    { "help",    do_help,          "show help message"   },
    { "clear",   do_clear,         "Clear LCD"           },
    { "write",   do_write,         "Write data to LCD"   },
    { "print",   do_print,         "Write string to LCD" },
    { "ddr",     do_SetDDRAddr,    "Set DDR Address"     },
    { "cg",      do_SetCGRAMAddr,  "Set CG RAM Address"  },
    { "command", do_Command,       "send instruction"    },
    { "pos",     do_pos,           "pos line col"        },
    {     0,     0,                0} /* list terminator */
} ;


int main(void)

{
    func_ptr func ;
    uint8_t data ;
    char cmd[32] ;
    char buffer[5];
    char buffer2[5];
    __enable_irq();
 
    ADC_Init();
    ADC_Start();
    ADC_StartConvert();
    tty_init(USE_CM4) ;
    UART_Start();
    CLCD_Init() ;
     uint8 Proximity_1 = 0;
     uint8 Proximity_2 = 0;
     uint8 Proximity_3 = 0;
     uint8 Proximity_4 = 0;
    splash("PSoC 6 CLCD Test\n") ;

    
    prompt("> ") ;
    for(;;)
    {  
        if (Cy_GPIO_Read(P9_0_PORT, P9_0_NUM) == 0 && Proximity_1 == 0)
        {
        CLCD_PutString("Height: 10 cm");
        Proximity_1 = 1;
        CyDelay(1000);
        CLCD_Clear() ;
        }
        if (Cy_GPIO_Read(P9_1_PORT, P9_1_NUM) == 0 && Proximity_2 == 0)
        {
        CLCD_PutString("Height: 20 cm");
        Proximity_2 = 1;
        CyDelay(1000);
        CLCD_Clear() ;
        }
        if (Cy_GPIO_Read(P9_2_PORT, P9_2_NUM) == 0 && Proximity_3 == 0)
        {
        CLCD_PutString("Height: 30 cm");
        Proximity_3 = 1;
        CyDelay(1000);
        CLCD_Clear() ;
        }
        if (Cy_GPIO_Read(P9_3_PORT, P9_3_NUM) == 0 && Proximity_4 == 0)
        {
        CLCD_PutString("Height: 40 cm");
        Proximity_4 = 1;
        CyDelay(1000);
        CLCD_Clear() ;
        }
        else
        {
      CLCD_PutString("Moisture: ");
      itoa(ADC_GetResult16(0), buffer, 10);
      splash(buffer) ;
      CLCD_PutString(buffer) ;
   
      CyDelay(1000);
      CLCD_Clear() ;
        }
      
        if (get_line()) {
            sscanf(str, "%s", cmd) ;
            func = get_func(main_func_list, cmd) ;
            if (func) {
                func() ;
            } else {
                print("Unknown Command: ") ;
                print(str) ;
                print("\r\n") ;
                do_help() ;
            }
            print("\r\n");
            prompt("> ") ;
        }
    }
}

void do_help(void)
{
    show_help(main_func_list) ;
}

void do_clear(void) 
{
    CLCD_Clear() ;
}

void do_write(void) 
{
    int c ;
    sscanf(str, "%*s %x", &c) ;
    CLCD_WriteData(c & 0xFF) ;
}

void do_SetDDRAddr(void) 
{
    int addr ;
    sscanf(str, "%*s %x", &addr) ;
    CLCD_SetDDRAMAddr(addr) ;
}

void do_SetCGRAMAddr(void) 
{
    int addr ;
    sscanf(str, "%*s %x", &addr) ;
    CLCD_SetCGRAMAddr(addr) ;    
}

void do_Command(void) 
{
    int cmd ;
    
    sscanf(str, "%*s %x", &cmd) ;
    CLCD_WriteCommand(cmd) ;
}

void do_print(void)
{
    char buf[81] ;
    sscanf(str, "%*s %s", buf) ;
    CLCD_PutString(buf) ;
}

void do_pos(void)
{
    int line, col ;
    int addr ;
    sscanf(str, "%*s %d %d", &line, &col) ;
    if (line < 1) { 
        line = 1 ;
    }
    if (line > CLCD_NUM_ROWS) {
        line = CLCD_NUM_ROWS ;
    }
    if (col < 1) {
        col = 1 ;
    } 
    if (col > CLCD_NUM_COLS) {
        col = CLCD_NUM_COLS ;
    }
    addr = 0x40 * (line-1) + (col-1) ;
    CLCD_SetDDRAMAddr(addr) ;
}

/* [] END OF FILE */
