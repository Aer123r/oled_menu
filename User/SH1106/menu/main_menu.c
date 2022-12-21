//
// Created by WuGaoyuan on 2022/12/13.
//
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "main_menu.h"
#include "stm32_u8g2.h"
u8g2_t u8g2;
void __Default_Task(){
    u8g2_ClearBuffer(&u8g2);
    u8g2_SetFont(&u8g2,u8g2_font_courB12_tr);
    u8g2_DrawStr(&u8g2,10,55,"Not");
    u8g2_DrawStr(&u8g2,10,85,"Found");
    u8g2_SendBuffer(&u8g2);
}
__weak void Menu_DoTask0(Menu* menu){__Default_Task();}
__weak void Menu_DoTask1(Menu* menu){__Default_Task();}
__weak void Menu_DoTask2(Menu* menu){__Default_Task();}
__weak void Menu_DoTask3(Menu* menu){__Default_Task();}
__weak void Menu_DoTask4(Menu* menu){__Default_Task();}
__weak void Menu_DoTask5(Menu* menu){__Default_Task();}
__weak void Menu_DoTask6(Menu* menu){__Default_Task();}
__weak void Menu_DoTask7(Menu* menu){__Default_Task();}
__weak void Menu_DoTask8(Menu* menu){__Default_Task();}
__weak void Menu_DoTask9(Menu* menu){__Default_Task();}
#define TASK_ADC

void Menu_init(Menu *menu,char* lists)
{
    u8g2Init(&u8g2);
    uint8_t i=0;
    uint8_t index=0;
    while(lists[i] != '\0')
    {
        if(lists[i] != ';')
        {
            menu->items[menu->numOfAll][index++] = lists[i];
        }else
        {
            index=0;
            menu->numOfAll++;
        }
        i++;
    }
    menu->IDLE=1;
    uint8_t curNum = menu->curNum;
    menu->DestHeight = curNum*14+2;
    menu->DestWidth = 12* strlen(menu->items[curNum])+5;
}

void Menu_ShowList(Menu *menu)
{
    uint8_t i;
    uint8_t numOfAll = menu->numOfAll;
    for(i=0;i<numOfAll;i++)
    {
        printf("%d---%s\n",i,menu->items[i]);
    }
}

void Menu_Render(Menu *menu)
{
    u8g2_ClearBuffer(&u8g2);
    u8g2_SetFont(&u8g2,u8g2_font_courB10_tf);
    uint8_t i;
    for(i=0;i<menu->numOfAll;i++)
    {
        u8g2_DrawStr(&u8g2,X_StrBegin,12+i*14,menu->items[i]);
        u8g2_DrawHLine(&u8g2,0,14+i*14,120);
        u8g2_DrawTriangle(&u8g2,5,11,5,4,10,7);
    }
    u8g2_SendBuffer(&u8g2);
}


void Menu_DoTask(Menu *menu)
{
    if(menu->IDLE == 1)
    {
        HAL_NVIC_EnableIRQ(EXTI3_IRQn);
        Menu_Go2Menu(menu);
        return;
    }
    HAL_NVIC_DisableIRQ(EXTI3_IRQn);
    switch (menu->curNum) {
        case 0:
            Menu_DoTask0(menu);
            break;
        case 1:
            Menu_DoTask1(menu);
            break;
        case 2:
            Menu_DoTask2(menu);
            break;
        case 3:
            Menu_DoTask3(menu);
            break;
        case 4:
            Menu_DoTask4(menu);
            break;
        case 5:
            Menu_DoTask5(menu);
            break;
        case 6:
            Menu_DoTask6(menu);
            break;
        case 7:
            Menu_DoTask7(menu);
            break;
    }



}



void Menu_Go2Menu(Menu *menu)
{
    u8g2_ClearBuffer(&u8g2);
    u8g2_SetFont(&u8g2,u8g2_font_courB10_tf);
    uint8_t i;
    for(i=0;i<menu->numOfAll;i++)
    {
        u8g2_DrawStr(&u8g2,X_StrBegin,12+i*14,menu->items[i]);
        u8g2_DrawHLine(&u8g2,0,14+i*14,120);
    }
    uint8_t curNum = menu->curNum;
//    u8g2_DrawTriangle(&u8g2,5,11+curNum*14,5,4+curNum*14,10,7+curNum*14);
//stylesheet function
    u8g2_DrawBox(&u8g2,X_StrBegin-5,menu->DestHeight,menu->DestWidth,12);

    u8g2_SetDrawColor(&u8g2,0);
    u8g2_DrawStr(&u8g2,X_StrBegin,12+curNum*14,menu->items[curNum]);
    u8g2_SetDrawColor(&u8g2,1);
    u8g2_SendBuffer(&u8g2);
}
