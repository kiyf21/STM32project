#include "stm32f10x.h"  
#include "Delay.h"// Device header//使用延时模块要引用
int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	
//STM32时钟系统主要的目的就是给相对独立的外设模块提供时钟，也是为了降低整个芯片的耗能。
//系统时钟，是处理器运行时间基准（每一条机器指令一个时钟周期）
//时钟是单片机运行的基础，时钟信号推动单片机内各个部分执行相应的指令。
//一个单片机内提供多个不同的系统时钟，可以适应更多的应用场合。
//不同的功能模块会有不同的时钟上限，因此提供不同的时钟，也能在一个单片机内放置更多的功能模块。
//对不同模块的时钟增加开启和关闭功能，可以降低单片机的功耗
//STM32为了低功耗，他将所有的外设时钟都设置为disable(不使能)，用到什么外设，只要打开对应外设的时钟就可以， 其他的没用到的可以还是
//使能)，这样耗能就会减少。 这就是为什么不管你配置什么功能都需要先打开对应的时钟的原因
	
	
	GPIO_InitTypeDef GPIO_InitStructure;//结构体类型+名字（GPIO_InitStructure）局部变量
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;//推挽输出Out_PP
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_All;//只初始化了0用|来实现多初始化GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2  初始化引脚
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//初始化外设
	//GPIO_ResetBits(GPIOA,GPIO_Pin_0 );//使pa0设置为低电平二极管发亮
	//GPIO_SetBits(GPIOA,GPIO_Pin_0 );//使pa0设置为高电平二极管熄灭
	//因为二极管的单向导电性，普遍将二极管设置为低电平点亮即由外界3.3v点亮，而不采用芯片的高电平点亮，若将二极管反接则为高电平点亮
	
	//	GPIO_WriteBit(GPIOA, GPIO_Pin_0,Bit_SET);
	//GPIO_WriteBit(GPIOA, GPIO_Pin_0,(BitAction)0);//若用0/1表示高低电平需要bitaction的强制类型转换
	//GPIO_Write(GPIOA,0x0001)//用于同时控制16个端口，stm32识别16进制，~0x0001表示按位取反即这个亮，其余灭（因为是低电平点亮所以按位取反）
	while(1)
	{
		GPIO_Write(GPIOA,~0x0001);
		Delay_ms(500);
		GPIO_Write(GPIOA,~0x0002);
		Delay_ms(500);
		GPIO_Write(GPIOA,~0x0004);
		Delay_ms(500);
		GPIO_Write(GPIOA,~0x0008);
		Delay_ms(500);
		GPIO_Write(GPIOA,~0x0010);
		Delay_ms(500);
		GPIO_Write(GPIOA,~0x0020);
		Delay_ms(500);
		GPIO_Write(GPIOA,~0x0040);
		Delay_ms(500);
		GPIO_Write(GPIOA,~0x0080);
		Delay_ms(500);
	}


}
