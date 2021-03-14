
	#include "stm32f4xx.h"
	#include "stm32f4_discovery.h"

	int8_t x,y,z;  //DEÐÝÞTÝRDÝ!!!.Önceki videoda burasý iþaretsiz tam sayý idi.

	GPIO_InitTypeDef GPIOStructure;
	SPI_InitTypeDef SPIStructure;

	uint16_t delay_cnt=0;

	void SysTick_Handler(void)
	{

		if(delay_cnt>0)
		{
			delay_cnt--;
		}

	}
	void delay_ms(uint16_t delay)
	{
	   delay_cnt=delay;

	   while(delay_cnt)
	   {


	   }


	}
	void SPI_Tx(uint8_t adress , uint8_t data)
	{

		GPIO_ResetBits(GPIOE,GPIO_Pin_3);
		while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE));
		SPI_I2S_SendData(SPI1,adress);
		while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE));
		SPI_I2S_ReceiveData(SPI1);

		while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE));
		SPI_I2S_SendData(SPI1,data);
		while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE));
		SPI_I2S_ReceiveData(SPI1);

		GPIO_SetBits(GPIOE,GPIO_Pin_3);



	}

	uint8_t SPI_Rx(uint8_t adress)
	{
		GPIO_ResetBits(GPIOE,GPIO_Pin_3);
		adress = 0x80 | adress ;
		while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE));
		SPI_I2S_SendData(SPI1,adress);
		while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE));
		SPI_I2S_ReceiveData(SPI1);

		while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE));
		SPI_I2S_SendData(SPI1,0x00); //0x00 sahte deðer
		while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE));
		SPI_I2S_ReceiveData(SPI1);
		GPIO_SetBits(GPIOE,GPIO_Pin_3);
		return SPI_I2S_ReceiveData(SPI1);

	}

	void config(void)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);

		GPIOStructure.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
		GPIOStructure.GPIO_Mode=  GPIO_Mode_OUT;
		GPIOStructure.GPIO_OType=0x00;
		GPIOStructure.GPIO_PuPd=0x00;
		GPIOStructure.GPIO_Speed=GPIO_Speed_100MHz;
		GPIO_Init(GPIOD, &GPIOStructure);

		// Configuring SCK , MOSI ,MISO
		GPIOStructure.GPIO_Pin=GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
		GPIOStructure.GPIO_Mode=GPIO_Mode_AF;
		GPIOStructure.GPIO_OType=GPIO_OType_PP;

		GPIO_Init(GPIOA, & GPIOStructure);


		//alternate function olarak ayarlanan pinlerde mutlaka tanýmlama yapýlmalý

		GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1);
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1);
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1);

		// Chipselect =pin E3
		GPIOStructure.GPIO_Pin=GPIO_Pin_3;
		GPIOStructure.GPIO_Mode=GPIO_Mode_OUT;
		GPIOStructure.GPIO_OType=GPIO_OType_PP;
		GPIO_Init(GPIOE,&GPIOStructure);



	SPIStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_2 ; 		SPIStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex; //hem yazma hem okuma
		SPIStructure.SPI_Mode=SPI_Mode_Master  ; //stm kartý master olucak
		SPIStructure.SPI_DataSize=SPI_DataSize_8b;  //8 bitlik veri gönderimi
		SPIStructure.SPI_FirstBit=SPI_FirstBit_MSB;
		SPIStructure.SPI_CPOL=SPI_CPOL_High;
		SPIStructure.SPI_CPHA=SPI_CPHA_2Edge;
		SPIStructure.SPI_NSS=SPI_NSS_Soft | SPI_NSSInternalSoft_Set ; //yazýlýmsal kontrol aktifleþtirildi
		SPI_Init(SPI1,&SPIStructure);

		SPI_Cmd(SPI1,ENABLE);

		SPI_Tx(0x20,0x77);

		SysTick_Config(SystemCoreClock/1000); //1ms
	}

	int main(void)
	{
		 config();


	  while (1)
	  {

			 x= SPI_Rx(0x29);

			 y=SPI_Rx(0x2B);


			 z=SPI_Rx(0x2D);

			 delay_ms(50);


			 if(y<-1 && y>-27 && z<-95 && z>-115 )
			 {

				 GPIO_SetBits(GPIOD,GPIO_Pin_15);
				 GPIO_ResetBits(GPIOD,GPIO_Pin_14);
				 GPIO_ResetBits(GPIOD,GPIO_Pin_12);
				 GPIO_ResetBits(GPIOD,GPIO_Pin_13);
			 }
			 else GPIO_ResetBits(GPIOD,GPIO_Pin_15);

				if(x<-8 && x>-20 && z<-95 && z>-110  ) {

					GPIO_ResetBits(GPIOD,GPIO_Pin_14);
					GPIO_SetBits(GPIOD,GPIO_Pin_12);
					GPIO_ResetBits(GPIOD,GPIO_Pin_15);
					GPIO_ResetBits(GPIOD,GPIO_Pin_13);
				}
				 else GPIO_ResetBits(GPIOD,GPIO_Pin_12);

				if(y>-128 && y<-115 && z<-95 && z>-100  )
				{

					GPIO_ResetBits(GPIOD,GPIO_Pin_15);
					GPIO_ResetBits(GPIOD,GPIO_Pin_14);
					GPIO_ResetBits(GPIOD,GPIO_Pin_12);
					GPIO_SetBits(GPIOD,GPIO_Pin_13);
				}
				else GPIO_ResetBits(GPIOD,GPIO_Pin_13);

				if(x>-128 && x<-103 && z<-96 && z>-110)
				{

				GPIO_SetBits(GPIOD,GPIO_Pin_14);
				GPIO_ResetBits(GPIOD,GPIO_Pin_12);
				GPIO_ResetBits(GPIOD,GPIO_Pin_15);
				GPIO_ResetBits(GPIOD,GPIO_Pin_13);
				 }
				else GPIO_ResetBits(GPIOD,GPIO_Pin_14);

	  }
	}

	//Burdan gerisi önemli deðil.
	void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
	  /* TODO, implement your code here */
	  return;
	}

	/*
	 * Callback used by stm324xg_eval_audio_codec.c.
	 * Refer to stm324xg_eval_audio_codec.h for more info.
	 */
	uint16_t EVAL_AUDIO_GetSampleCallBack(void){
	  /* TODO, implement your code here */
	  return -1;
	}

