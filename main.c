#include "main.h"

void usart_init(void);
void send_to_uart(uint8_t data);
void send_str(char * string);
void adc_init(void);
u16 readADC1(u8 channel);

void usart_init(void){
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;

RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);

GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOA, &GPIO_InitStructure);

GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
GPIO_Init(GPIOA, &GPIO_InitStructure);

USART_StructInit(&USART_InitStructure);

USART_Init(USART2, &USART_InitStructure);
USART_Cmd(USART2, ENABLE);
}

void send_to_uart(uint8_t data){
 while(!(USART2->SR & USART_SR_TC));
 USART2->DR=data;
}

void send_str(char * string){
 uint8_t i=0;
 while(string[i]){
  send_to_uart(string[i]);
  i++;
}
}

void adc_init(void){
 ADC_InitTypeDef ADC_InitStructure;
 ADC_StructInit(&ADC_InitStructure);

 ADC_CommonInitTypeDef adc_init;
 ADC_CommonStructInit(&adc_init);
 
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
 ADC_DeInit();

 adc_init.ADC_Mode = ADC_Mode_Independent;
 adc_init.ADC_Prescaler = ADC_Prescaler_Div2;

 ADC_InitStructure.ADC_ScanConvMode = DISABLE;
 ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
 ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;
 ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;

 ADC_CommonInit(&adc_init);
 ADC_Init(ADC1, &ADC_InitStructure);
 ADC_Cmd(ADC1, ENABLE);
}

u16 readADC1(u8 channel){
 ADC_RegularChannelConfig(ADC1, channel, 1, ADC_SampleTime_3Cycles);
 ADC_SoftwareStartConv(ADC1);
 while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
 return ADC_GetConversionValue(ADC1);
}

void main(void){
usart_init();
adc_init();
ADC_TempSensorVrefintCmd(ENABLE);
 
while(1){
 uint16_t Temperature = readADC1(16);
 send_to_uart(Temperature);
 }
}

