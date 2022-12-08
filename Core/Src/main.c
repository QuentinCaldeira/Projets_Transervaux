/* USER CODE BEGIN Header */
/*VILLETTE Lou-Anne & CALDEIRA Quentin*/
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ACC_ADR 		0x32 //Adresse i2c de l'acceloremetre
#define MAG_ADR 		0x3C //Adresse i2c du magnetometre
#define WHO_AM_I_A 		0x0F
#define WHO_AM_I_M		0x4F


#define CTRL_REG1_A 	0x20
#define CTRL_REG5_A 	0x24
#define OUT_X_L_A		0x28

#define CFG_REG_A_M 	0x60
#define	OUTX_L_REG_M	0x68
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
 char acc_data_w[12];
 char acc_data_r[12];
 char mag_data_w[12];
 char mag_data_r[12];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
int who_am_i_sensors();
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/*------------Fonction permettant de relier le printf et l'uart---------------*/
int __io_putchar(int ch) {
	uint8_t c = ch & 0x00FF;
	HAL_UART_Transmit(&huart2, &c, 1, 10);
	return ch;
}
/*----------------------------------------------------------------------------*/

/*---Fonction permettant d'attester la présence du magneto et de l'accelero---*/
int who_am_i_sensors(){
	uint8_t buf[1];                                                                                     //Buffer de 1 octet car on ne lit que une case mémoire
	HAL_StatusTypeDef ret;                                                                              //Variable HAL permettant de voir l'état de la transmission I2C
	buf[0] = WHO_AM_I_A;                                                                                //On affecte l'adresse de la case mémoire stockant WHOAMI pour l'accelero
	ret = HAL_I2C_Master_Transmit(&hi2c1, ACC_ADR, buf, 1, HAL_MAX_DELAY);                              //On effectue la transmission sur l'accelero
	if ( ret != HAL_OK ) {                                                                              //Si la transmission s'est mal passé, on affiche une erreur
		printf("Error Tx\r\n");
	}
	else {
	    ret = HAL_I2C_Master_Receive(&hi2c1, ACC_ADR, buf, 1, HAL_MAX_DELAY);                           //Sinon, on recupere la valeur dans la case memoire
	    if ( ret != HAL_OK ) {                                                                          //Si la réception s'est mal passée, on affiche une erreur
	      printf("Error Rx\r\n");
	    }
	    else {
	    	if ( buf[0]==0x33 ) {                                                                         //On teste si la valeur présente dans la case memoire est identique à celle indiquée dans la doc
	        printf("Detection de accelerometre \n\r");
	    	}
	    }
  }
  buf[0] = WHO_AM_I_M;                                                                                //On affecte l'adresse de la case mémoire du WHOAMI pour le magneto
  ret = HAL_I2C_Master_Transmit(&hi2c1, MAG_ADR, buf, 1, HAL_MAX_DELAY);                              //On effectue la transmission sur le magneto
  if ( ret != HAL_OK ) {                                                                              //Si la transmission s'est mal passé, on affiche une erreur
	  printf("Error Tx\r\n");
  }
  else {
  	ret = HAL_I2C_Master_Receive(&hi2c1, MAG_ADR, buf, 1, HAL_MAX_DELAY);                             //Sinon, on recupere la valeur dans la case memoire
  	if ( ret != HAL_OK ) {                                                                            //Si la réception s'est mal passée, on affiche une erreur
  	  printf("Error Rx\r\n");
  	}
    else {
    	if ( buf[0]==0x40 ) {                                                                           //On teste si la valeur présente dans la case memoire est identique à celle indiquée dans la doc
    		printf("Detection de magneto \n\r");
    	}
    }
  }
}

int reset_acc(){
  
}
/*---------------------------------------------------------------------------------*/
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */



  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1){
	  who_am_i_sensors();
  }
}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  
  /* USER CODE END 3 */

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
	  printf("error i2c");
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
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
