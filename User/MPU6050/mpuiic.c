#include "mpuiic.h"
//#include "delay.h"
//#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK NANO STM32F103开发板
//MPU6050 IIC驱动 代码
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2018/7/28
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2018-2028
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////
static uint32_t fac_us=0;							//us延时倍乘数
void delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told,tnow,tcnt=0;
    uint32_t reload=SysTick->LOAD;				//LOAD的值
    ticks=nus*fac_us; 						//需要的节拍数
    told=SysTick->VAL;        				//刚进入时的计数器值
    while(1)
    {
        tnow=SysTick->VAL;
        if(tnow!=told)
        {
            if(tnow<told)tcnt+=told-tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
            else tcnt+=reload-tnow+told;
            told=tnow;
            if(tcnt>=ticks)break;			//时间超过/等于要延迟的时间,则退出.
        }
    };
}

//MPU IIC 延时函数
void MPU_IIC_Delay(void)
{
    delay_us(2);
}

//初始化IIC
void MPU_IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOA_CLK_ENABLE();           //开启GPIOA时钟

    GPIO_Initure.Pin=GPIO_PIN_2|GPIO_PIN_3; //PA2、PA3
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;    	//高速

    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2|GPIO_PIN_3,GPIO_PIN_SET);//PB11,PB12 输出高

    HAL_GPIO_Init(GPIOA,&GPIO_Initure);

}
//产生IIC起始信号
void MPU_IIC_Start(void)
{
    MPU_SDA_OUT();     //sda线输出
    MPU_IIC_SDA=1;
    MPU_IIC_SCL=1;
    MPU_IIC_Delay();
    MPU_IIC_SDA=0;//START:when CLK is high,DATA change form high to low
    MPU_IIC_Delay();
    MPU_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据
}
//产生IIC停止信号
void MPU_IIC_Stop(void)
{
    MPU_SDA_OUT();//sda线输出
    MPU_IIC_SCL=0;
    MPU_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
    MPU_IIC_Delay();
    MPU_IIC_SCL=1;
    MPU_IIC_SDA=1;//发送I2C总线结束信号
    MPU_IIC_Delay();
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t MPU_IIC_Wait_Ack(void)
{
    uint8_t ucErrTime=0;
    MPU_SDA_IN();      //SDA设置为输入
    MPU_IIC_SDA=1;MPU_IIC_Delay();
    MPU_IIC_SCL=1;MPU_IIC_Delay();
    while(MPU_READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            MPU_IIC_Stop();
            return 1;
        }
    }
    MPU_IIC_SCL=0;//时钟输出0
    return 0;
}
//产生ACK应答
void MPU_IIC_Ack(void)
{
    MPU_IIC_SCL=0;
    MPU_SDA_OUT();
    MPU_IIC_SDA=0;
    MPU_IIC_Delay();
    MPU_IIC_SCL=1;
    MPU_IIC_Delay();
    MPU_IIC_SCL=0;
}
//不产生ACK应答
void MPU_IIC_NAck(void)
{
    MPU_IIC_SCL=0;
    MPU_SDA_OUT();
    MPU_IIC_SDA=1;
    MPU_IIC_Delay();
    MPU_IIC_SCL=1;
    MPU_IIC_Delay();
    MPU_IIC_SCL=0;
}
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void MPU_IIC_Send_Byte(uint8_t txd)
{
    uint8_t t;
    MPU_SDA_OUT();
    MPU_IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {
        MPU_IIC_SDA=(txd&0x80)>>7;
        txd<<=1;
        MPU_IIC_SCL=1;
        MPU_IIC_Delay();
        MPU_IIC_SCL=0;
        MPU_IIC_Delay();
    }
}
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
uint8_t MPU_IIC_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    MPU_SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
    {
        MPU_IIC_SCL=0;
        MPU_IIC_Delay();
        MPU_IIC_SCL=1;
        receive<<=1;
        if(MPU_READ_SDA)receive++;
        MPU_IIC_Delay();
    }
    if (!ack)
        MPU_IIC_NAck();//发送nACK
    else
        MPU_IIC_Ack(); //发送ACK
    return receive;
}
