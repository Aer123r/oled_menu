//
// Created by WuGaoyuan on 2022/12/13.
//

#ifndef STM32F103C8_DEMO_MAIN_MENU_H
#define STM32F103C8_DEMO_MAIN_MENU_H
#include "main.h"
#include "u8g2.h"

#define Button1_PIN    GPIO_PIN_3
#define Button2_PIN    GPIO_PIN_4

#define  Flex_Direction  Colmn;
#define  X_StrBegin    5

struct oled_menu{
    float Style;
    float CurHeight;
    float CurWidth;
    float DestHeight;
    float DestWidth;
    float H_Speed;
    float V_Speed;
    uint8_t IDLE;
    uint8_t curNum;
    uint8_t numOfAll;
    char items[20][30];
};
typedef struct oled_menu Menu;
void Menu_init(Menu *menu,char* lists);
void Menu_Render(Menu *menu);
void Menu_ShowList(Menu *menu);
void Menu_DoTask(Menu *menu);
void Menu_Go2Menu(Menu *menu);




#endif //STM32F103C8_DEMO_MAIN_MENU_H
