#include "main.h"

void SystemClockConfigUpdate(void);
void GPIOConfig(void);
void NVIC_Config(void);
void SysTick_Configuration(void);


int main(void)
{

	// Bu sefer 1000ms de bir SysTick ISR sini çalıştıran bir SysTick konfigürasyonunu yapacağım

  SystemClockConfigUpdate();
  NVIC_Config();
  SysTick_Configuration();
  GPIOConfig();


  while (1)
  {

  }
  /* USER CODE END 3 */
}

int _write(int file, char *ptr, int len)
{
  (void)file;
  int DataIdx;

  for (DataIdx = 0; DataIdx < len; DataIdx++)
  {
    ITM_SendChar(*ptr++);
  }
  return len;
}

void NVIC_Config(){

	// CMSIS ile konfigürasyon;

	NVIC_SetPriority(SysTick_IRQn, 3); // priority ayarı
	// NVIC_EnableIRQ(SysTick_IRQn);  TICKINT = 1 → NVIC’e otomatik yönlendirilir. Bu yüzden bu satıra gerek yok
}

void SysTick_Configuration(){


	// Artık SysTick timer ın konfigürasyonunu yapalım

	// SysTick frekans=16MHz olsun istediğim için  HCLK kaynağını direkt kullanacağım.
	// Bu nedenle STK_CTRL register ında clock source olarak HCLK yı seçmek istediğim için 2. biti "1" olarak ayarla
	SysTick->CTRL |= (1<<2);


	SysTick->LOAD = 16000000-1; // Başlangıç değeri yükle
	SysTick->CTRL |= (1<<1); // sayaç 0 olduğunda interrupt oluşsun diye TICKINT bitini 1 yap

	SysTick->CTRL |= (1<<0); // SysTick enable et(başlat)

}

void SystemClockConfigUpdate(){

	// Amaç: SYSCLK=16MHz çalıştırmak

	/* FLASH ayarları */
	FLASH->ACR |= (5<<0); // Bu satır LATENCY ayarını yapıyor, LATENCY = 5 → 5 wait states (STM32F4’de 168 MHz çalıştırmak için datasheet’e göre 5 wait state gerekiyor)
	FLASH->ACR |= (1<<8); // PRFTEN (Prefetch enable) bitini açar.
	FLASH->ACR |= (1<<9); // ICEN (Instruction cache enable) bitini açar.
	FLASH->ACR |= (1<<10); // DCEN (Data cache enable) bitini açar.


	RCC->CR |= (1<<0); // HSI enable edildi kullanılmak üzere
	while((RCC->CR&0x00000002)!=0x00000002); // HSI ready flag i 1 olup HSI nin çalışmaya hazır olduğunu söyleyene kadar bekle


	RCC->CFGR &= ~(3<<0); // HSI oscillator selected as system clock

	while((RCC->CFGR & (3<<2)) != (0<<2)); // HSI aktif olana kadar bekle

	// AHB Prescaler(HPRE biti) = 1 olmalı ki HCLK=168MHz olsun**********
	RCC->CFGR &= ~(0xF<<4);



	//  APB1=16, APB2=16 MHz de çalışsın diye PPRE1=1, PPRE2=1 olarak ayalarnmalı RCC_CFGR register ında
	RCC->CFGR &= ~(0x3F<<10); // bitleri temizle


	SystemCoreClockUpdate(); // Donanımda değiştirdiğimiz SYSCLK frekansını CMSIS tarafındaki SystemCoreClock değişkenine güncelleyerek,
	                         // yazılımın ve kütüphanelerin doğru CPU frekansını kullanmasını sağlamaktır.
}


void GPIOConfig(){

	RCC->AHB1ENR |= (1<<3); // PortD_12. pin led olarak kullanılacak


	// PortD nin 12. pinini led olarak kullanacağım
	// Pini çıkış pini olarak ayarla
	GPIOD->MODER &= ~(3<<24);
	GPIOD->MODER |= (1<<24);

	// OTYPER
	GPIOD->OTYPER &= ~(1<<12);

	//OSPEEDR
	GPIOD->OSPEEDR &= ~(3<<24);
	GPIOD->OSPEEDR |= (2<<24); // Medium


	// Gerçek zamanlı işlemler için BSRR ile dijital çıkış vermek daha doğru.
	// Bu uygulamada şart değil ama örnek olarak kullandım
	GPIOD->BSRR = (1<<28);

}
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
