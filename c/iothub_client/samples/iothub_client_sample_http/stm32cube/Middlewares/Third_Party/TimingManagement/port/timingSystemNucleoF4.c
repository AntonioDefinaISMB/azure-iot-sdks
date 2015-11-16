#include "timingSystem.h"

#include "main.h"
#include "ThreadAPI.h"

typedef enum __TTimeSyncStatus_t
{
  TIMESYNCSTATUS_NONE,
  TIMESYNCSTATUS_SET,
  TIMESYNCSTATUS_NOTSET,
}TTimeSyncStatus;

static RTC_HandleTypeDef        RtcHandle;
static volatile time_t          timeSyncSystem;
static volatile TTimeSyncStatus isTimeSynchonized;

/**
 * @brief  Configures the RTC
 * @param  None
 * @retval None
 */
static void RTC_Config(void)
{
    /*##-1- Configure the RTC peripheral #######################################*/
    RtcHandle.Instance = RTC;

    /* Configure RTC prescaler and RTC data registers */
    /* RTC configured as follow:
  - Hour Format    = Format 12
  - Asynch Prediv  = Value according to source clock
  - Synch Prediv   = Value according to source clock
  - OutPut         = Output Disable
  - OutPutPolarity = High Polarity
  - OutPutType     = Open Drain */
    
    RtcHandle.Instance = RTC;

    /* Configure RTC prescaler and RTC data registers */
    /* RTC configured as follow:
  - Hour Format    = Format 12
  - Asynch Prediv  = Value according to source clock
  - Synch Prediv   = Value according to source clock
  - OutPut         = Output Disable
  - OutPutPolarity = High Polarity
  - OutPutType     = Open Drain */
    RtcHandle.Init.HourFormat = RTC_HOURFORMAT_12;
    RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
    RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
    RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
    RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
    
        
    if(HAL_RTC_Init(&RtcHandle) != HAL_OK)
    {
        /* Initialization Error */
        while(1);
    }
}

void    TimingSystemInitialize(void)
{
  RTC_Config();
  
  timeSyncSystem = 1446741778;//FIXME: TEMPO INIZIALE "INVALID"
  
  isTimeSynchonized = TIMESYNCSTATUS_NONE;
}

int TimingSystemSetSystemTime(time_t epochTimeNow)
{
    RTC_DateTypeDef sdatestructure;
    RTC_TimeTypeDef stimestructure;

    time_t now = epochTimeNow;
    struct tm *calendar = gmtime(&now);
    
    /*##-3- Configure the Date #################################################*/
    sdatestructure.Year = calendar->tm_year - 100;
    sdatestructure.Month = calendar->tm_mon + 1;
    sdatestructure.Date = calendar->tm_mday;
    sdatestructure.WeekDay = calendar->tm_wday + 1;

    if(HAL_RTC_SetDate(&RtcHandle,&sdatestructure,FORMAT_BIN) != HAL_OK)
    {
        /* Initialization Error */
      return 0;
    }

    /*##-4- Configure the Time #################################################*/
    stimestructure.Hours                = calendar->tm_hour;
    stimestructure.Minutes              = calendar->tm_min;
    stimestructure.Seconds              = calendar->tm_sec;
    stimestructure.TimeFormat           = RTC_HOURFORMAT12_AM;
    stimestructure.DayLightSaving       = RTC_DAYLIGHTSAVING_NONE ;
    stimestructure.StoreOperation       = RTC_STOREOPERATION_RESET;

    if(HAL_RTC_SetTime(&RtcHandle,&stimestructure,FORMAT_BIN) != HAL_OK)
    {
      return 0;
    }
        
    timeSyncSystem      = epochTimeNow;
    
    gmtime(&epochTimeNow);
    isTimeSynchonized   = TIMESYNCSTATUS_SET;
    
    return 1;
}


time_t 	TimingSystemGetSystemTime(void)
{
  time_t        returnTime;
  struct tm*    pCalendar;
  
  RTC_DateTypeDef sdatestructure;
  RTC_TimeTypeDef stimestructure;
  
  returnTime = 0;
  
  while(isTimeSynchonized!=TIMESYNCSTATUS_SET)
  {
    ThreadAPI_Sleep(10);
  }  
  
  pCalendar             = gmtime((const time_t*)&timeSyncSystem);
 
  if(HAL_RTC_GetTime(&RtcHandle,&stimestructure,FORMAT_BIN)==HAL_OK && HAL_RTC_GetDate(&RtcHandle,&sdatestructure,FORMAT_BIN)==HAL_OK)
  {
      pCalendar->tm_year           = sdatestructure.Year +100;
      pCalendar->tm_mon            = sdatestructure.Month-1;
      pCalendar->tm_mday           = sdatestructure.Date;
      pCalendar->tm_wday           = sdatestructure.WeekDay - 1;
      pCalendar->tm_hour           = stimestructure.Hours;
      pCalendar->tm_min            = stimestructure.Minutes;
      pCalendar->tm_sec            = stimestructure.Seconds;
      pCalendar->tm_isdst          = 0;
      #if  _DLIB_SUPPORT_FOR_AEABI
      pCalendar->__BSD_bug_filler1 = 0;
      pCalendar->__BSD_bug_filler2 = 0;
      #endif
            
      returnTime        = mktime(pCalendar);
      timeSyncSystem    = returnTime;
  }
    
  return returnTime;
}

