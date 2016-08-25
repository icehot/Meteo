/** OS **/

//#define DEBUG

void Task_Init_Callback() 
{
  #ifdef DEBUG
    Serial.println("#OS: Init Task");
  #endif 
  
  init_UART();
  
  #ifdef USE_SD
    init_SD();
  #endif 
  
  #ifdef USE_DEBUG_LED
    init_DEBUG_LED();
  #endif 

  #ifdef USE_FACTDEF_BTN
    init_FACT_DEF_BTN();
  #endif

  #ifdef USE_SOUND_DETECT
    init_SoundDetect();
  #endif 

  #ifdef USE_RELAY
    init_Relay();
  #endif 

  #ifdef USE_PIR
    init_PIR();
  #endif

  #ifdef USE_RGB
    init_RGB();
  #endif
  
  #ifdef USE_DHT11
    init_DHT11();
  #endif

  #ifdef USE_BMP085
    init_BMP085(); 
  #endif

  #ifdef USE_MS5611
    init_MS5611();
  #endif

  #ifdef USE_ETH_SHIELD
    init_NetSetup();
  #endif
  
  #ifdef USE_WEBDUINO
    init_Webduino();
  #endif
  
  #ifdef USE_NTP
    init_NTP();
  #endif

   #ifdef USE_DS1302
    init_DS1302();
  #endif

  #ifdef USE_LCD
    init_LCD();
  #endif
}

void Task_Acquisition_Callback()
{
    #ifdef DEBUG
      Serial.println("#OS: Acquisition Task");
    #endif 
  
    TimeStamps.cycleStart = millis();
    #ifdef USE_DHT11
    read_DHT11();
    TimeStamps.dht11 = millis();
    #endif
    #ifdef USE_BMP085
    read_BMP085(); 
    TimeStamps.bmp085 = millis();
    #endif
    #ifdef USE_MS5611
    read_MS5611();
    TimeStamps.ms5611 = millis();
    #endif
    #ifdef USE_NTP
    read_time(); 
    //TimeStamps.ntp = millis();
    #endif
    #ifdef USE_DS1302
    read_DS1302();
    TimeStamps.ds1302 = millis();
    #endif
    //sync_DS1302withNTP(); //Uncomment for DS1302 NTP sync
    #ifdef USE_PIR
    get_PIR_State();
    #endif
    #ifdef USE_SOUND_DETECT
    get_SoundDetect_State();
    #endif
    #ifdef USE_RGB
    analogWrite(PIN_RGBLED_R, DataPool.RGB_Red);
    analogWrite(PIN_RGBLED_G, DataPool.RGB_Green);
    analogWrite(PIN_RGBLED_B, DataPool.RGB_Blue);
    #endif

   if (Task_Acquisition.isFirstIteration())
   {
      Task_Display.enable();
      Task_Webduino.enable();
      Task_Log.enable();
   }
}

void Task_Display_Callback()
{
    #ifdef DEBUG
      Serial.println("#OS: Display Task");
    #endif 
    
    TimeStamps.task2s = TimeStamps.current;
    #ifdef USE_LCD
    updateLCD();
    #endif
}

void Task_Webduino_Callback()
{
    #ifdef DEBUG
      Serial.println("#OS: Webduino Task");
    #endif 
  
  #ifdef USE_WEBDUINO
  TimeStamps.webStart = millis();
  WebduinoServerLoop();
  TimeStamps.webEnd = millis();
  #endif
}

void Task_Log_Callback()
{
    #ifdef DEBUG
      Serial.println("#OS: Log Task");
    #endif 
    
    TimeStamps.task1m = TimeStamps.current;
    #ifdef USE_SD
    saveDataToLog();
    #endif
}

void Task_RenewDHCP_Callback()
{
   #ifdef DEBUG
     Serial.println("#OS: Renew DHCP Task");
   #endif 
  
  #ifdef USE_ETH_SHIELD
  
    /* renew DHCP lease */
  
    dhcp_state=Ethernet.maintain();
    
    switch(dhcp_state)
    {
      case NothingHappened:
        #ifdef USE_SERIAL_MONITOR
          Serial.println("#DHCP: Nothing Happened");
        #endif
        #ifdef USE_SYS_LOG
          add2SysLog("#DHCP: Nothing Happened");
        #endif
      break;
      case RenewFailed:
        #ifdef USE_SERIAL_MONITOR
          Serial.println("#DHCP: Renew Failed");
        #endif
        #ifdef USE_SYS_LOG
          add2SysLog("#DHCP: Renew Failed");
        #endif
      break;
      case RenewSuccess:
        #ifdef USE_SERIAL_MONITOR
          Serial.println("#DHCP: Renew Success");
        #endif
        #ifdef USE_SYS_LOG
          add2SysLog("#DHCP: Renew Success");
        #endif
      break;
      case RebindFailed:
        #ifdef USE_SERIAL_MONITOR
          Serial.println("#DHCP: Rebind Failed");
        #endif
        #ifdef USE_SYS_LOG
          add2SysLog("#DHCP: Rebind Failed");
        #endif
      break;
      case RebindSuccess:
        #ifdef USE_SERIAL_MONITOR
          Serial.println("#DHCP: Rebind Success");
        #endif
        #ifdef USE_SYS_LOG
          add2SysLog("#DHCP: Rebind Success");
        #endif
      break;
      default:
        #ifdef USE_SERIAL_MONITOR
          Serial.println("#DHCP: Unknown Error");
        #endif
        #ifdef USE_SYS_LOG
          add2SysLog("#DHCP: Unknown Error");
        #endif
      break;
    }
  #endif
}

void OS_calcRunTime()
{
  /*
  RunTime.dht11  = TimeStamps.dht11  - TimeStamps.cycleStart;
  RunTime.bmp085 = TimeStamps.bmp085 - TimeStamps.dht11;
  RunTime.ms5611 = TimeStamps.ms5611 - TimeStamps.bmp085;
  RunTime.ds1302 = TimeStamps.ds1302 - TimeStamps.ms5611;
  RunTime.total  = TimeStamps.end    - TimeStamps.start;
  RunTime.web    = TimeStamps.webEnd - TimeStamps.webStart; 
  */
}

#ifdef DEBUG
  #undef DEBUG
#endif