/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2015 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include <time.h>   // time functions
#include <stdint.h>
#include <string.h>
#include "main.h"
#include "InternetInterfaceDriver.h"
#include "iothub_client_sample_http.h"
#include "cmsis_os.h"
#include "stdiospecialization.h"
#include "ThreadAPI_Port.h"
#include "threadapi.h"
#include "AzureIOTSDKConfig.h"
#include "CompilerCheckError.h"
#include "timingSystem.h"
#include "SynchronizationAgent.h"
#include "InternetAdapter.h"
    
#include "agenttime.h"//FIXME: RIMUOVERE
    
#if ENABLE_TEST_THREADAPI>0
#include "AdditionalUserTask.h"
#endif
  
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

static TInternetAdapterSettings adapterInternetSettings;

#if ENABLE_TEST_THREADAPI>0
static THREAD_HANDLE testThreadHandle;
#endif

void SystemClock_Config(void);

void RTC_Config(void);

/* Private functions ---------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/


/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

void AssignInternetSettings(void)
{
  adapterInternetSettings.pSSID          = DEFAULT_SSID;
  adapterInternetSettings.pSecureKey     = DEFAULT_SECUREKEY;
}

int CheckAzureIOTConfigurationParameters(void)
{
    if(strcmp(AZUREIOTHUBCONNECTIONSTRING,AZUREIOTHUBCONNECTIONSTRING_INVALID)==0)
    {
      printf("\r\nUSER ERROR! The Connection String has not been set \r\n(Check AzureIOTSDKConfig.h headers)\r\n");
      return -1;
    }
    if(strcmp(DEFAULT_SSID,DEFAULT_SSID_INVALID)==0 || strcmp(DEFAULT_SECUREKEY,DEFAULT_SECUREKEY_INVALID)==0)
    {
      printf("\r\nUSER ERROR! WiFi Parameters are not set! \r\n(Check AzureIOTSDKConfig.h headers)\r\n");
      return -1;
    }
  
    return 1;
}

static void PrintAssemblyInformation(void)
{
  printf("\r\nSTM32 Nucleo F4 Board Azure IOT SDK Test: %s\r\n",AZUREIOTSDK_STM32NUCLEOF4PORTING_VERSION);
  
  #if defined(__IAR_SYSTEMS_ICC__) && defined (__ICCARM__) && defined(__VER__)
  printf("IAR Compiler Platform: %d Compiler: %d. Version: %d\r\n",__IAR_SYSTEMS_ICC__,__ICCARM__,__VER__);
  #endif
  printf("Build Date: %s Time: %s\r\n",__DATE__,__TIME__);
  
  printf("\r\n");
}

int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();
 
  /* configure the timers  */
  Timer_Config(); 
  
  UART_Configuration();
  
  #ifdef USART_PRINT_MSG
    stdioinitialize();
  #endif      
   
  if(CheckAzureIOTConfigurationParameters()<=0)
  {
    printf("\r\nWrong Configuration. Application Exit\r\n");
    return 0;
  }
    
  TimingSystemInitialize();
  
  AssignInternetSettings();
  InternetInterfaceInitialize(&adapterInternetSettings);
  ThreadAPI_Initialize();
  
  PrintAssemblyInformation();
  
  InternetAdapterStart(CALLINGPROCEDURETYPE_MAIN);
  SynchronizationAgentStartDefaultConfig();
  
  osThreadDef(defaultTask, (os_pthread)iothub_client_sample_http_run, osPriorityNormal, 0, AZURERUNEXAMPLE_STACKSIZE);
  osThreadCreate(osThread(defaultTask), NULL);
    
  #if ENABLE_TEST_THREADAPI>0
  ThreadAPI_Create(&testThreadHandle,taskTest,0);
  #endif

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
 

  /* Start scheduler */
  osKernelStart();
  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    
  while (1)
  {
    
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */
}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

