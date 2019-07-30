//Hardware settings
#if defined(ARDUINO_ESP32_DEV)
    #define BOARD_TYPE_ESP32 true
    #define BOARD_TYPE "ESP32"
#elif defined(ARDUINO_ESP8266_WEMOS_D1MINI)
    #define BOARD_TYPE "ESP8266"
    #define BOARD_TYPE_ESP8266 true
    extern "C" {
        #include "user_interface.h"
   }
#endif

    //One main color
uint8_t ledRed   = 0;
uint8_t ledGreen = 0;
uint8_t ledBlue  = 0;
//All locations Mixed to one color
uint8_t ledMRed   = 0;
uint8_t ledMGreen = 0;
uint8_t ledMBlue  = 0;
//Top
uint8_t ledTRed   = 0;
uint8_t ledTGreen = 0;
uint8_t ledTBlue  = 0;
//Bottom
uint8_t ledBRed   = 0;
uint8_t ledBGreen = 0;
uint8_t ledBBlue  = 0;
//Left
uint8_t ledLRed   = 0;
uint8_t ledLGreen = 0;
uint8_t ledLBlue  = 0;
//Right
uint8_t ledRRed   = 0;
uint8_t ledRGreen = 0;
uint8_t ledRBlue  = 0;


/***
 *    ╔╦╗  ╔═╗  ╔═╗  ╔═╗  ╦ ╦  ╦    ╔╦╗
 *     ║║  ║╣   ╠╣   ╠═╣  ║ ║  ║     ║ 
 *    ═╩╝  ╚═╝  ╚    ╩ ╩  ╚═╝  ╩═╝   ╩ 
 *                                     
 *                                     
 *                                     
 */
//Default start
String addTabs(String text, int amount)
{
    for(int iPos = amount; iPos < amount; iPos++)
    {
        text = "\t" + text;
    }
}

long millisLast = millis();//Used in functions
//Default stop



/***
 *    ╔╦╗  ╦  ╔╦╗  ╔═╗  ╦═╗  
 *     ║   ║  ║║║  ║╣   ╠╦╝  
 *     ╩   ╩  ╩ ╩  ╚═╝  ╩╚═  
 * 
 * 
 * 
 */
//Timer start
long millisMain;
long millisStart[10];
String timerNames[10];
void timerSet(int iPos = 0,String addString = "")
{ 
    long millisDiff = millis() - millisMain;
    millisMain = millis();
    millisStart[iPos] = millisMain;//Set start time new action
    timerNames[iPos] = addString;//Set name of action
    
    Serial.print("\n Seconds online:" + String( millisMain/1000 ));
    Serial.print("\n Start : " + timerNames[iPos] + "[" + millisDiff + "ms total]" );
}

void timerPrint(int iPos = 0)
{
    long millisDiff = millis() - millisStart[iPos]; 
    String insertSpace = "";

    if(millisDiff < 10){ insertSpace = "   "; }
    else if (millisDiff < 100){ insertSpace = "  "; }
    else if (millisDiff < 1000){ insertSpace = " "; }

    //Serial.print("\n[" + String( millisMain ) + "ms onlinetime ] done : " + timerNames[iPos] + "[" + millisDiff + "ms total]" );
    //Serial.print("\n\t" + String(millisDiff) + insertSpace + String(" ms <- ")  + timerNames[iPos]);
    Serial.print("\n Seconds online:" + String( millisMain/1000 ));
    Serial.print("\n Done : " + timerNames[iPos] + "[" + millisDiff + "ms total]" );
}
void timerLogToSerial(String itemName)
{
    long millisDiff = millis() - millisLast;
    String insertSpace = "";

    if(millisDiff < 10){ insertSpace = "   "; }
    else if (millisDiff < 100){ insertSpace = "  "; }
    else if (millisDiff < 1000){ insertSpace = " "; }

    Serial.print("\n\t" + String(millisDiff) + insertSpace + String(" ms <- ")  + itemName); /* Reset */ millisLast = millis();
}
// Timer stop



/***
 *    ╦ ╦  ┬  ╔═╗  ┬
 *    ║║║  │  ╠╣   │
 *    ╚╩╝  ┴  ╚    ┴
 * 
 * 
 * 
 */
// WiFi start
#include <WiFi.h>

WiFiClient client;

const char *ssid = "ssidHERE";
const char *password = "passwordHERE";

void wifiConnect()
{
    //connecting to a WiFi network
    Serial.println("Connecting to " + String(ssid));
    Serial.println("Wifi begin : " + String(WiFi.begin(ssid, password)));

    while (WiFi.status() != WL_CONNECTED)
    {

        Serial.print(".");
        delay(500);
    }

    delay(1500);
    Serial.print("WiFi connected. ");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}
// WiFi Stop



/***
 *    ╦ ╦  ╔╦╗  ╔╦╗  ╔═╗       ╔═╗  ╦    ╦  ╔═╗  ╔╗╔  ╔╦╗
 *    ╠═╣   ║    ║   ╠═╝  ───  ║    ║    ║  ║╣   ║║║   ║ 
 *    ╩ ╩   ╩    ╩   ╩         ╚═╝  ╩═╝  ╩  ╚═╝  ╝╚╝   ╩ 
 * 
 * 
 * 
 */
#include <HTTPClient.h>

#define HTTP_PRINT_TO_SERIAL false
const int httpBuffLength = 600;
 int httpBuffPosition = 0;
char httpBuff[httpBuffLength];
char* httpBufName = "";
void httpBuffFill()
{
    httpBuffPosition++;
    for(int iBuffFillPosition = httpBuffPosition; iBuffFillPosition < httpBuffLength; iBuffFillPosition++)
    {
        httpBuff[iBuffFillPosition] = '\0';
    }
}
bool HTTPgetUpdates = false;
void HTTPget(char* host, int port, char* path, char* buffer = "")
{
    #if HTTP_PRINT_TO_SERIAL
        timerLogToSerial("HTTPget - Start");
    #endif

    char * httpVersion = "1.1";
    bool bJSON = true;
    //bool bClose = false;

    //or char httpVersion[] = "1.1";
       
    // Use WiFiClient class to create TCP connections
    //WiFiClient client;

    if (WiFi.status() == WL_CONNECTED)
    {
        //Disconnect from host
        //client.stop();

        //Connect to host
        if (!client.connect(host, port))
        {
            #if HTTP_PRINT_TO_SERIAL
                timerLogToSerial("Connection failed");
            #endif
            return;
        }
        
        #if HTTP_PRINT_TO_SERIAL
            timerLogToSerial("HTTPget - connected");
        #endif

        //Start of request
        client.print("GET " + String(path) + " HTTP/" + String(httpVersion) + "\r\n");
        if(bJSON)client.print("Content-Type: application/json; charset=UTF-8\r\n"); //Request JSON content      
        //if(bClose)client.print("Connection: close\r\n"); //Request JSON content
        client.print("\r\n");
        //End of request

        #if HTTP_PRINT_TO_SERIAL
            timerLogToSerial("HTTPget - request  - send");
        #endif
        
        //Wait for the response and timeout
        unsigned long timeout = millis();
        while (client.available() == 0) {
            if (millis() - timeout > 5000) {
                #if HTTP_PRINT_TO_SERIAL
                    timerLogToSerial("HTTPget - Client Timeout");
                #endif                
                client.stop();
                return;
            }
        }

        #if HTTP_PRINT_TO_SERIAL
            timerLogToSerial("HTTPget - response - read");
        #endif

        //Read the headers
        bool headersCompleted = false;

        // Header example:        
        //  1  HEADERS: >>>|    HTTP/1.0 200 OK//n/r                                |<<<
        //  2  HEADERS: >>>|    Date: Sat, 13 Jul 2019 19:22:56 GMT//n/r            |<<<
        //  3  HEADERS: >>>|    Accept-Ranges: bytes//n/r                           |<<<
        //  4  HEADERS: >>>|    Server: Restlet-Framework/2.1//n/r                  |<<<
        //  5  HEADERS: >>>|    Access-Control-Allow-Origin: *//n/r                 |<<<
        //  6  HEADERS: >>>|    Content-Length: 536//n/r                            |<<<
        //  7  HEADERS: >>>|    Content-Type: application/json; charset=UTF-8//n/r  |<<<
        //  8  HEADERS: >>>|    //n/r                                               |<<<

              //56ms
            #if HTTP_PRINT_TO_SERIAL
                timerLogToSerial("HTTPget - response - headers read");
            #endif
            
            char c;
            while (!headersCompleted)
            {
                c = client.read();
                if( c == char('\r') )//CR1
                {
                    c = client.read();
                    if( c == char('\n') )//LF1
                    {
                        c = client.read();
                        if( c == char('\r') )//CR2
                        {
                            c = client.read();
                            if( c == char('\n') )//CRLF
                            {
                                headersCompleted = true;
                                #if HTTP_PRINT_TO_SERIAL
                                    timerLogToSerial("HTTPget - response - headers done");
                                #endif
                                //break;
                            }
                        }
                    }
                }
            }


        //Serial.print("\n\t ms: " + String( millis() - millisLast ) + String("ms")  + " HTTPget - headers"); /* Reset */ millisLast = millis();

        #define WIFICLIENT_HTTP_BODY_MODE_2 true
        bool bodyCompleted = false;

        /* Read source in once - SLOW */
           
            #if HTTP_PRINT_TO_SERIAL
                timerLogToSerial("HTTPget - response - body read");
            #endif
            
            httpBuffPosition = 0;
            while (client.available() > 0)
            {
                char c = client.read();
                //Serial.print(c);
                httpBuff[httpBuffPosition++] = c;
            }
            httpBuffFill();
            bodyCompleted = true;

            #if HTTP_PRINT_TO_SERIAL
                timerLogToSerial("HTTPget - response - body done");
            #endif

    }
    else
    {
        #if HTTP_PRINT_TO_SERIAL
            timerLogToSerial("HTTPget - ERROR");
        #endif  

        wifiConnect();
    }

    #if HTTP_PRINT_TO_SERIAL
        timerLogToSerial("HTTPget - response done");
    #endif    

    HTTPgetUpdates = false;
    client.stop();
    
    #if HTTP_PRINT_TO_SERIAL
        timerLogToSerial("HTTPget - Stop");
    #endif
}



/***
 *    ╔═╗  ╔═╗  ╔═╗  ╔╦╗       ╦    ╔═╗  ╔╦╗
 *    ╠╣   ╠═╣  ╚═╗   ║   ───  ║    ║╣    ║║
 *    ╚    ╩ ╩  ╚═╝   ╩        ╩═╝  ╚═╝  ═╩╝
 * 
 * 
 * 
 */
#include "SPI.h"
#include <FastLED.h>

#define DATA_PIN 18
#define CLOCK_PIN 19

/*  How many leds in your strip? */
#define NUM_LEDS 300

/*  Define the array of leds (current state) */
CRGB leds_current[NUM_LEDS];

/*  Define the array of leds (wanted state) */
CRGB leds_update[NUM_LEDS];

/************************************
*   Set the global brightness scaling
*   @param scale a 0-255 value for how much to scale all leds before writing them out
*/
bool bLed_Brightness_SetEnabled = true;
int8_t iLed_Brightness_Level = 96;  //Default:96
void LedStrip_Set_Brightness()
{
    FastLED.setBrightness(iLed_Brightness_Level);
    return;
}

/*****************************************************************************************************************************************************************************************************************************************************************************************************************
*   FastLED users can easily scale the brightness and power consumption of their projects with FastLED.setBrightness( 0..255 ). FastLED v2.1 (and later) includes automatic "temporal dithering" which helps preserve color and light when the brightness is turned down. To take advantage of temporal dithering:
*   Code your animations as if they were going to run at normal full brightness; use the full range of 0..255 for RGB and HSV values as much as you wish.
*   Use FastLED.setBrightness( 0..255) to adjust the brightness of your whole animation. The FastLED temporal dithering will 'kick in' automatically as you lower the master brightness control.
*   In place of the standard "delay(...)" function, use "FastLED.delay(...)". FastLED will use the 'delay time' to keep the LEDs refreshed with dithered pixel values.
*
*   Other details:
*   
*   To disable temporal dithering, for POV or light painting projects, use "FastLED.setDither( 0 );". If you turn off dithering, the library reverts back to 'flooring' integer values, instead of dithering them.
*   Temporal dithering has no effect at full brightness (255). It exists to preserve high quality color and accurate light output when the master brightness control is turned down to save power or manage brightness.
*   The more often your code calls FastLED.show(), or FastLED.delay(), the higher-quality the dithering will be, because FastLED can refresh the LEDs faster and more often.
*   If you are refreshing the LEDs less frequently (e.g., if you have a hundreds of LEDs, or computationally intensive animations), and you are running at a low brightness level, you may see the dithered pixel output as flickering, and you may want to turn it off if the effect is distracting. It's not magic; it's up to you what looks good in your projects.﻿
*/
bool bLed_Set_Dithering_Enabled = false;
int8_t iLed_Dithering_Level = 0;
void Led_Set_Dithering()
{
    if(!bLed_Set_Dithering_Enabled)FastLED.setDither( iLed_Dithering_Level );
}

/**************************************
*   In what order and where to update leds
*   1 - Normal -> led update order -> while(!done)loop -> starts from postition 0 to postition NUM_LEDS
*   2 - Random -> led update order -> while(!done)loop -> starts from a random postion, and will get the next led when this led is updated already.
*   3 - Fast -> Update order -> Realtime.
*/
int iLed_Update_OrderMode = 3; //Default:2

/************************************************************************************************************************************************************
*   How fast showing the updated led, Leds will be updated in steps(Stepper) to avoid flickering, this also gives the chance to update in between some updates.
*   how higher the level, how higher the updates will be, this option can be useless, since level 1 till 5, slow down the script.
*
*   In stepper function:
*   level 1 - Stepper level -> LED ( R or G or B ) level - After ( R or G or B ) of (LED)has 1 step down/up - with usefull updates
*   level 2 - Stepper level -> LED ( R or G or B ) level - After ( R or G or B ) o (LED) has 1 step down/up - with useless updates
*   level 3 - Stepper level -> LED ( Full RGB )    level - After RGB update (LED) - (1 step down/up)
*   level 4 - Stepper level -> LED ( Full RGB )    level - After RGB update (LED) - (even when we need to run an other cycle)
*   level 5 - Stepper level -> LED ( Full RGB )    level - After fullfill update (LED) - (even when value is the same as wanteed)
*
*   After stepper function:
*   level 6 - Runner level  -> LED ( Full RGB ) - After fullfill update (LED)
*   level 7 - Runner level  -> LED ( Full RGB ) - After fullfill update (LEDSTRIP)
*/
int iLed_Update_SpeedMode = 7; //Default:7 !! 6 and lower can be fine in some situations

/**********************************************************************************************
*   The speed updating the led to the new color:
*
*    0 - {254,128,96,64,32,16,8,4,2,1};   //Disco mode XD
*    1 - {128,96,64,32,16,8,4,2,1};       //Intense mode
*    2 - {96,64,32,16,8,4,2,1};           //Game mode
*    3 - {64,32,16,8,4,2,1};              //Compelling mode
*    4 - {32,16,8,4,2,1};                 //Normal mode (default)
*    5 - {16,8,4,2,1};                    //Comfort mode
*    6 - {8,4,2,1};                       //Quiet mode
*    7 - {4,2,1};                         //Relax mode
*    8 - {2,1};                           //Slow mode
*    9 - {1};                             //Slowest mode
*   
*    Example : 
*    We use StepMode = 4   -> {32,16,8,4,2,1}
*    With LEDvalueCurent = 0
*    And LEDvalueWanted = 127
*       
*    Then the led will be updated like: 
*       
*    ValueCurrent=0 -> (+32) 
*    ValueCurrent=32 -> (+32) 
*    ValueCurrent=64 -> (+32) 
*    ValueCurrent=96 -> (+32) 
*    ValueCurrent=112 -> (+16) 
*    ValueCurrent=120 -> (+8)
*    ValueCurrent=124 -> (+4)
*    ValueCurrent=126 -> (+2)
*    ValueCurrent=126 -> (+1)
*    ValueCurrent=valueWanted
*/
int iLed_Update_StepMode = 3; //Default:3

/* FastLED 2.1 Power management set at 5V, 500mA. */
//set_max_power_in_volts_and_milliamps(5, 500);

/*****************************************************************************************************
*   Set the maximum refresh rate.  This is global for all leds.  Attempts to
*   call show faster than this rate will simply wait.  Note that the refresh rate
*   defaults to the slowest refresh rate of all the leds added through addLeds.  If
*   you wish to set/override this rate, be sure to call setMaxRefreshRate _after_
*   adding all of your leds.
*   @param refresh - maximum refresh rate in hz
*   @param constrain - constrain refresh rate to the slowest speed yet set
*
*   FastLED3.1 now has FastLED.setMaxRefreshRate(uint16_t hz) that lets you set the maximum refresh rate, and it now defaults to 400Hz.
*   if we're constraining, the new value of m_nMinMicros _must_ be higher than previously (because we're only allowed to slow things down if constraining)
*/
bool bLed_RefreshRateEnabled = false;    //Default:true
int16_t iLed_RefreshRateMax = 16000;    //Default:400,Custom:16000
bool iLed_RefreshConstrain = true;      //Default:true
void LedStrip_Set_RefreshRate()
{
    FastLED.setMaxRefreshRate(iLed_RefreshRateMax,iLed_RefreshConstrain);
    return;
}


/* FastLED.setMaxRefreshRate(uint16_t hz) */
bool bLedShowDelayed = false;
int16_t iLedShowFramesSecond = 500;
void ledStrip_Update()
{
    if(bLedShowDelayed)
    {
        FastLED.delay(1000/iLedShowFramesSecond); 
        return;
    }
    FastLED.show();
    return;
}


void LedStrip_Setup()
{
    FastLED.addLeds<LPD8806, DATA_PIN, CLOCK_PIN, BRG>(leds_current, NUM_LEDS);
    if(bLed_Brightness_SetEnabled)LedStrip_Set_Brightness();
    if(bLed_RefreshRateEnabled)LedStrip_Set_RefreshRate();
    if(bLed_Set_Dithering_Enabled)Led_Set_Dithering();
}

// Fast-/LED -> Method 1 -> ledStrip_UpdateOrderModeNormal Start
void ledStripLedUpdateWithStepper(int ledPosition)
{
    int iLed_Update_SpeedMode_tmp = (leds_current[ledPosition] == leds_update[ledPosition]?0:iLed_Update_SpeedMode);

    while (leds_current[ledPosition] != leds_update[ledPosition])
    //if (leds_current[ledPosition] != leds_update[ledPosition])
    {
        int ledValueSteps[] = {254,128,96,64,32,16,8,4,2,1}; //Disco mode XD
        int valStepPos = iLed_Update_StepMode; // Startposition

        //Step increase/decrease RGB value
        int ledValueStepsLength = sizeof(ledValueSteps)/sizeof(ledValueSteps[0]);
        
        //RGB update value                
        for(; valStepPos < ledValueStepsLength; valStepPos++)
        {
            //RGB value iteration
            for(int iRGB = 0; iRGB < 3; iRGB++)
            {
                if( leds_current[ledPosition][iRGB] > leds_update[ledPosition][iRGB] )//bDecreaseLed
                {   
                        int valueDecrease = leds_current[ledPosition][iRGB] - ledValueSteps[valStepPos];
                        if(valueDecrease >= leds_update[ledPosition][iRGB])
                        {
                            leds_current[ledPosition][iRGB] = valueDecrease;
                            if( iLed_Update_SpeedMode_tmp == 1 )ledStrip_Update();//LED color Real-time ( R or G or B ) - After ( R or G or B ) has 1 step down/up - with usefull updates
                        }
                }       

                else if( leds_current[ledPosition][iRGB] < leds_update[ledPosition][iRGB] )//bIncreaseLed
                {
                        int valueIncrease = leds_current[ledPosition][iRGB] + ledValueSteps[valStepPos];
                        if(valueIncrease <= leds_update[ledPosition][iRGB])
                        {
                            leds_current[ledPosition][iRGB] = valueIncrease;
                            if( iLed_Update_SpeedMode_tmp == 1 )ledStrip_Update();//LED color Real-time ( R or G or B ) - After ( R or G or B ) has 1 step down/up - with usefull updates
                        }
                }

                if( iLed_Update_SpeedMode_tmp == 2 )ledStrip_Update();//LED color Real-time ( R or G or B ) - After ( R or G or B ) has 1 step down/up - with useless updates
            }
            if( iLed_Update_SpeedMode_tmp == 3 )ledStrip_Update(); //LED color Real-time ( RGB ) - After fullfill update (1 step down/up)
        }
        if( iLed_Update_SpeedMode_tmp == 4 )ledStrip_Update(); //LED RGB Real-time ( RGB ) - - After fullfill update - (even when we need to run an other cycle)
    }
    if( iLed_Update_SpeedMode_tmp == 5 )ledStrip_Update(); //LED RGB Real-time ( RGB ) - After fullfill update - (even when value is the same as wanteed)
}

// Fast-/LED -> Method 1 (default) -> ledStrip_UpdateOrderModeNormal Start
void ledStrip_UpdateOrderModeNormal()
{
    //int updatesDone = 0;

    for (int ledPosition = 0; ledPosition < NUM_LEDS; ledPosition++)
    {
        if(leds_current[ledPosition] != leds_update[ledPosition])
        {
            ledStripLedUpdateWithStepper(ledPosition);
            //updatesDone++;
            if( iLed_Update_SpeedMode == 6 )ledStrip_Update();
        } 
    }
    if( iLed_Update_SpeedMode == 7 )ledStrip_Update();
    //if(updatesDone == 0) 
    HTTPgetUpdates = true;
}
// Fast-/LED -> Method 1 -> ledStrip_UpdateOrderModeNormal End



// Fast-/LED -> Method 2 -> ledStrip_UpdateOrder_Random Start

// bool array for leds that did get an uptdate 
bool bItemDone[NUM_LEDS];
//A counter that shows how many times a unique item is created
int ledStrip_UpdateOrder_Random_Pos_Generated = 0;
void ledStrip_UpdateOrder_Random_Reset(int start, int stop)//This function will reset the global array,to make it ready for next use
{
    for ( int itemPosition = start; itemPosition < stop; itemPosition++ )
    {
        bItemDone[itemPosition] = false;
    }
    ledStrip_UpdateOrder_Random_Pos_Generated = 0;
}
//This function will look for an unique location that isnt hand out.
//We start with some random location, and return the first one not set,
int ledStrip_UpdateOrder_Random_Pos(int start, int stop)
{
    int itemPosition = random(start, stop);//Create random number

    while( ledStrip_UpdateOrder_Random_Pos_Generated < stop )//While we did set all locations
    {
        if( itemPosition == stop ) //If we did reach the last led
        {
            itemPosition = start; //Maybe we still need to set some others with an lower array location
        }
        if( !bItemDone[itemPosition] ) //if item is not set yet
        {
            bItemDone[itemPosition] = true; //set the status to SET
            ledStrip_UpdateOrder_Random_Pos_Generated++; //Count the items we have set
            return itemPosition; //Return this posistion
        }
        itemPosition++; //The location was set already, check the next location
    }
    //will never be reached
    Serial.print("\n ERROR ledStrip_UpdateOrder_Random_Pos done, reached " + String(stop) );
    return stop;
}

void ledStrip_UpdateOrder_Random(int start, int stop)
{   
    bool printMsg = false;
    int updatesDone = 0;
    if( printMsg )timerLogToSerial("ledStrip_UpdateOrder_Random - start");
    while( ledStrip_UpdateOrder_Random_Pos_Generated < stop )
    {
        int itemPosition = ledStrip_UpdateOrder_Random_Pos(start, stop);
        if(leds_current[itemPosition] != leds_update[itemPosition])
        {
            ledStripLedUpdateWithStepper(itemPosition);
            if( iLed_Update_SpeedMode == 6 )ledStrip_Update();
            updatesDone++;
        }       
    }
    ledStrip_UpdateOrder_Random_Reset(0, NUM_LEDS);
    //if(updatesDone == 0) HTTPgetUpdates = true;
    HTTPgetUpdates = true;


    /// Update all our controllers with the current led colors, using the passed in brightness
	/// @param scale temporarily override the scale
    //if( iLed_Update_SpeedMode == 6 )FastLED.show();

    if( iLed_Update_SpeedMode == 7 )ledStrip_Update();

	/// Set all leds on all controllers to the given color/scale
	/// @param color what color to set the leds to
	/// @param scale what brightness scale to show at
	//void showColor(const struct CRGB & color, uint8_t scale);

	/// Set all leds on all controllers to the given color
	/// @param color what color to set the leds to
	//void showColor(const struct CRGB & color) { showColor(color, m_Scale); }

    if( printMsg )timerLogToSerial("ledStrip_UpdateOrder_Random - stop");
}
// Fast-/LED -> Method 2 -> ledStrip_UpdateOrder_Random End


// Fast-/LED -> Method 3 (default) -> ledStrip_UpdateOrderModeFast Start
void ledStrip_UpdateOrderModeFast()
{
    //int updatesDone = 0;

    for (int ledPosition = 0; ledPosition < NUM_LEDS; ledPosition++)
    {
        if(leds_current[ledPosition] != leds_update[ledPosition])
        {
            leds_current[ledPosition] = leds_update[ledPosition];
            //updatesDone++;
            if( iLed_Update_SpeedMode == 6 )ledStrip_Update();
        } 
    }
    if( iLed_Update_SpeedMode == 7 )ledStrip_Update();
    //if(updatesDone == 0) 
    HTTPgetUpdates = true;

    //fill_solid( &(leds_current[0]), 300, CRGB( ledTRed, ledTGreen, ledTBlue) );
    //    if( iLed_Update_SpeedMode == 7 )ledStrip_Update();
}
// Fast-/LED -> Method 3 -> ledStrip_UpdateOrderModeFast End


/***
 *    ╔═╗  ╦    ╔═╗  ╔═╗  ╔═╗       ╔═╗  ╔╦╗  ╔╗   ╦       ╔╦╗  ╦  ╦
 *    ║    ║    ╠═╣  ╚═╗  ╚═╗  ───  ╠═╣  ║║║  ╠╩╗  ║  ───   ║   ╚╗╔╝
 *    ╚═╝  ╩═╝  ╩ ╩  ╚═╝  ╚═╝       ╩ ╩  ╩ ╩  ╚═╝  ╩        ╩    ╚╝                                                                
 *                                                                  
 */
// AmbiTV start
class classAmbiLightTV
{
    //Jointspace information : http://jointspace.sourceforge.net/projectdata/documentation/jasonApi/1/doc/API-gettingstarted.html

    protected:
        //A protected member variable or function is very similar to a private member but it provided one additional benefit,
        //that they can be accessed in child classes which are called derived classes.

    private:
        //A private member variable or function cannot be accessed, or even viewed from outside the class. 
        //Only the class and friend functions can access private members.

          String className                  = "classAmbiLightTV";

    public:
        //A public member is accessible from anywhere outside the class but within a program. 
        //You can set and get the value of public variables without any member.

    //Special layout :P

    class { public : /* AmbiLightTV.Info */

        class { public : /* AmbiLightTV.Info.Audio */

            class { public : /* AmbiLightTV.Info.Audio.Volume */
                //Returns the TV volume.

                   bool muted = false; //Set to true if the sound is muted, false if not.
                uint8_t current = 18;  //The actual volume.
                uint8_t min = 0;       //The minimum volume.
                uint8_t max = 60;      //The maximum volume.

            }Volume;

        }Audio;

        class { public : /* AmbiLightTV.Info.AmbiLight */

            class { public : /* AmbiLightTV.Info.AmbiLight.Topology */
                //Returns the number of layers and the number of pixels on each side.
            
                uint8_t layers = 2; //(integer number): The number of layers.
                uint8_t left = 4;       //(integer number): The number of pixels on the left.
                uint8_t top = 8;        //(integer number): The number of pixels on the top.
                uint8_t right = 4;      //(integer number): The number of pixels on the right.
                uint8_t bottom = 2;     //(integer number): The number of pixels on the bottom.

            }Topology;
            
            class { public : /* AmbiLightTV.Info.AmbiLight.Cached */
                //Returns the ambilight colours stored in the cache.
            } Cached;

            class { public : /* AmbiLightTV.Info.AmbiLight.Measured */
                //Returns the measured ambilight colours before processing by the internal ambilight algorithm.
            } Measured;

            class { public : /* AmbiLightTV.Info.AmbiLight.Processed */
                //Returns the ambilight colours calculated by the internal ambilight algorithm.
            
                class { public : /* AmbiLightTV.Info.AmbiLight.Processed.Layer[x] */

                    class { public : /* AmbiLightTV.Info.AmbiLight.Processed.Layer[x].bottom[x] */

                        uint8_t r = -1;  //(integer): The red value for a pixel, between 0 and 255.
                        uint8_t g = -1;  //(integer): The green value for a pixel, between 0 and 255.
                        uint8_t b = -1;  //(integer): The blue value for a pixel, between 0 and 255.

                    }bottom[2];

                    class { public : /* AmbiLightTV.Info.AmbiLight.Processed.Layer[x].left[x] */

                        uint8_t r = -1;  //(integer): The red value for a pixel, between 0 and 255.
                        uint8_t g = -1;  //(integer): The green value for a pixel, between 0 and 255.
                        uint8_t b = -1;  //(integer): The blue value for a pixel, between 0 and 255.

                    }left[4];

                    class { public : /* AmbiLightTV.Info.AmbiLight.Processed.Layer[x].right[x] */

                        uint8_t r = -1;  //(integer): The red value for a pixel, between 0 and 255.
                        uint8_t g = -1;  //(integer): The green value for a pixel, between 0 and 255.
                        uint8_t b = -1;  //(integer): The blue value for a pixel, between 0 and 255.

                    }right[4];

                    class { public : /* AmbiLightTV.Info.AmbiLight.Processed.Layer[x].top[x] */

                        uint8_t r = -1;  //(integer): The red value for a pixel, between 0 and 255.
                        uint8_t g = -1;  //(integer): The green value for a pixel, between 0 and 255.
                        uint8_t b = -1;  //(integer): The blue value for a pixel, between 0 and 255.

                    }top[7];

                }Layer[2];

            }Processed;

        }AmbiLight;
 
    }Info;

} AmbiLightTV;
classAmbiLightTV *pAmbiLightTV = &AmbiLightTV;
// AmbiTV stop



#include <ArduinoJson.h>

//void JSONAmbTvFetch(String jsonString)
//void JSONAmbTvFetch(char* json)


void JSONAmbTvFetch()
{
    bool printMsg = false;
    if( printMsg )timerLogToSerial("JSONAmbTvFetch - start");

    const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + 18*JSON_OBJECT_SIZE(3) + 3*JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(8) + 200;
    DynamicJsonDocument doc(capacity);
    deserializeJson(doc, httpBuff);
    JsonObject layer1 = doc["layer1"];
    
    if( printMsg )timerLogToSerial("JSONAmbTvFetch - deserializeJson");

    int layerPosition = 0;
    int posQuarter  = NUM_LEDS/4;
    int posHalf     = NUM_LEDS/2;
    int pos3Quarter = posQuarter + posHalf;

    //mode
    bool modeSingle = true;
    int  mainMode = 0;      //Set mainmode to 0=top,1=bottom,2=left,3=right

    //locations to add for end result
    bool addTop = true;
    bool addBottom = true;
    bool addLeft = true;
    bool addRight = true;

    for(int iPosition = 0; iPosition < AmbiLightTV.Info.AmbiLight.Topology.top && addTop; iPosition++)
    {
        AmbiLightTV.Info.AmbiLight.Processed.Layer[layerPosition].top[iPosition].r = uint8_t(layer1["top"][String(iPosition)]["r"]);
        AmbiLightTV.Info.AmbiLight.Processed.Layer[layerPosition].top[iPosition].g = uint8_t(layer1["top"][String(iPosition)]["g"]);
        AmbiLightTV.Info.AmbiLight.Processed.Layer[layerPosition].top[iPosition].b = uint8_t(layer1["top"][String(iPosition)]["b"]);

        ledTRed   =+ AmbiLightTV.Info.AmbiLight.Processed.Layer[layerPosition].top[iPosition].r;
        ledTGreen =+ AmbiLightTV.Info.AmbiLight.Processed.Layer[layerPosition].top[iPosition].g;
        ledTBlue  =+ AmbiLightTV.Info.AmbiLight.Processed.Layer[layerPosition].top[iPosition].b;
        if( iPosition > 0 ) ledTRed   /= 2;
        if( iPosition > 0 ) ledTGreen /= 2;
        if( iPosition > 0 ) ledTBlue  /= 2;

        if(mainMode == 0) ledRed   = ledTRed  ; 
        if(mainMode == 0) ledGreen = ledTGreen; 
        if(mainMode == 0) ledBlue  = ledTBlue ; 
    }
    //timerLogToSerial("JSONAmbTvFetch - top");


    for(int iPosition = 0; iPosition < AmbiLightTV.Info.AmbiLight.Topology.bottom && addBottom; iPosition++)
    {
        AmbiLightTV.Info.AmbiLight.Processed.Layer[layerPosition].bottom[iPosition].r = uint8_t(layer1["bottom"][String(iPosition)]["r"]); //int(layer1["bottom"][String(iPos)]["r"]);
        AmbiLightTV.Info.AmbiLight.Processed.Layer[layerPosition].bottom[iPosition].g = uint8_t(layer1["bottom"][String(iPosition)]["g"]);
        AmbiLightTV.Info.AmbiLight.Processed.Layer[layerPosition].bottom[iPosition].b = uint8_t(layer1["bottom"][String(iPosition)]["b"]);

        ledBRed   =+ AmbiLightTV.Info.AmbiLight.Processed.Layer[layerPosition].bottom[iPosition].r;
        ledBGreen =+ AmbiLightTV.Info.AmbiLight.Processed.Layer[layerPosition].bottom[iPosition].g;
        ledBlue   =+ AmbiLightTV.Info.AmbiLight.Processed.Layer[layerPosition].bottom[iPosition].b;
        if( iPosition > 0 ) ledBRed   /= 2;
        if( iPosition > 0 ) ledBGreen /= 2;
        if( iPosition > 0 ) ledBlue   /= 2;

        if(mainMode == 1) ledRed   = ledBRed  ;
        if(mainMode == 1) ledGreen = ledBGreen;
        if(mainMode == 1) ledBlue  = ledBlue  ;
    }
    //timerLogToSerial("JSONAmbTvFetch - bottom");


    for(int iPosition = 0; iPosition < AmbiLightTV.Info.AmbiLight.Topology.left && addLeft; iPosition++)
    {
        AmbiLightTV.Info.AmbiLight.Processed.Layer[layerPosition].left[iPosition].r = uint8_t(layer1["left"][String(iPosition)]["r"]);
        AmbiLightTV.Info.AmbiLight.Processed.Layer[layerPosition].left[iPosition].g = uint8_t(layer1["left"][String(iPosition)]["g"]);
        AmbiLightTV.Info.AmbiLight.Processed.Layer[layerPosition].left[iPosition].b = uint8_t(layer1["left"][String(iPosition)]["b"]);

        ledLRed   =+ AmbiLightTV.Info.AmbiLight.Processed.Layer[layerPosition].left[iPosition].r;
        ledLGreen =+ AmbiLightTV.Info.AmbiLight.Processed.Layer[layerPosition].left[iPosition].g;
        ledLBlue  =+ AmbiLightTV.Info.AmbiLight.Processed.Layer[layerPosition].left[iPosition].b;
        if( iPosition > 0 ) ledLRed   /= 2;
        if( iPosition > 0 ) ledLGreen /= 2;
        if( iPosition > 0 ) ledLBlue  /= 2;

        if(mainMode == 2) ledRed   = ledLRed  ;
        if(mainMode == 2) ledGreen = ledLGreen;
        if(mainMode == 2) ledBlue  = ledLBlue ;
    }
    //timerLogToSerial("JSONAmbTvFetch - left");


    for(int iPosition = 0; iPosition < AmbiLightTV.Info.AmbiLight.Topology.right && addRight; iPosition++)
    {
        AmbiLightTV.Info.AmbiLight.Processed.Layer[layerPosition].right[iPosition].r = uint8_t(layer1["right"][String(iPosition)]["r"]);
        AmbiLightTV.Info.AmbiLight.Processed.Layer[layerPosition].right[iPosition].g = uint8_t(layer1["right"][String(iPosition)]["g"]);
        AmbiLightTV.Info.AmbiLight.Processed.Layer[layerPosition].right[iPosition].b = uint8_t(layer1["right"][String(iPosition)]["b"]);

        ledRRed   =+ AmbiLightTV.Info.AmbiLight.Processed.Layer[layerPosition].right[iPosition].r;
        ledRGreen =+ AmbiLightTV.Info.AmbiLight.Processed.Layer[layerPosition].right[iPosition].g;
        ledRBlue  =+ AmbiLightTV.Info.AmbiLight.Processed.Layer[layerPosition].right[iPosition].b;
        if( iPosition > 0 ) ledRRed   /= 2;
        if( iPosition > 0 ) ledRGreen /= 2;
        if( iPosition > 0 ) ledRBlue  /= 2;

        if(mainMode == 3) ledRed   = ledRRed  ;
        if(mainMode == 3) ledGreen = ledRGreen;
        if(mainMode == 3) ledBlue  = ledRBlue ;
    }
    //timerLogToSerial("JSONAmbTvFetch - right");

    if( printMsg )timerLogToSerial("JSONAmbTvFetch - saved json to object");

    if(modeSingle)
    {
        for (int iPosition = 0; iPosition < NUM_LEDS; iPosition++)
        {
            leds_update[iPosition] = CRGB(ledRed,ledGreen,ledBlue);
        }
    }
    else
    {
        //Top
        for (int iPosition = 0; iPosition < posQuarter && addTop; iPosition++)
        {
            leds_update[iPosition] = CRGB(ledTRed,ledTGreen,ledTBlue);
        }

        //Bottom
        for (int iPosition = pos3Quarter; iPosition < NUM_LEDS && addBottom; iPosition++)
        {
            leds_update[iPosition] = CRGB(ledBRed,ledBGreen,ledBBlue);
        }
            
        //Left
        for (int iPosition = posQuarter; iPosition < posHalf && addLeft; iPosition++)
        {
            leds_update[iPosition] = CRGB(ledLRed,ledLGreen,ledLBlue);
        }
    
        //Right
        for (int iPosition = posHalf; iPosition < pos3Quarter && addRight; iPosition++)
        {
            leds_update[iPosition] = CRGB(ledRRed,ledRGreen,ledRBlue);
        }
    
    }
 
    if( printMsg )timerLogToSerial("JSONAmbTvFetch - saved from object to ledarray");
}

//Timer loop average - START
class cMyTimer
{    public :
        class cAvg
        {    public :
                bool printRound = false;                                                  //print the time value of round
                bool printArray = false;                                                  //print the average time value of array
                long valueStart = millis();                                               //Start value of timer new timer
    static const int valueArrLength = 500;                                                //Amount of timers (History length) to calculate average 
                 int valueArrPos = valueArrLength - 1;                                    //Position to save the current value
                long valueAverage = valueStart;                                           //The average value in valueArr
                long valueArr[valueArrLength];                                            //Array with timer values

                long SetAndGet()                                                          
                {   
                    //1 - Get and set single timer
                    long previousStartVal = round(millis() - valueStart);                 //Value of last round
                    valueArrPos++;                                                        //Create a position to save the new time average
                    if ( valueArrPos >= valueArrLength )                                  //Check if this position is over limit set with valueArrLength, otherwise reset
                    {
                        valueArrPos = 0;                                                  //Reset position back to start
                    }
                    valueArr[valueArrPos] = previousStartVal;                             //Get and save the value
                    valueStart = millis();  //Reset the value
                    if(printRound) Serial.print( "\n Average - Round: " + String(valueArr[valueArrPos]) );//Print the value

                    //2 - Get and set average time
                    long itemCounter = 1;                                                 //Items to math the average +1 for the value aready existing in valueAverage
                    for (int positie = 0; positie < (int)valueArrLength; positie++ )      //Loop through items
                    {
                                                                                          //if ( !(positie % 10) ) { Serial.print("\n"); };   //Print a new line every 10 items
                        if ( positie == valueArrPos ) { }                                 // Not used, plan was to add startvalue of array position 0
                        else if ( valueArr[positie] > long(0) )                           // Skip 0 value, empty array
                        {
                            valueAverage = valueAverage + valueArr[positie];              //Add the new value to our average
                            itemCounter++;                                                //Add 1 to the itemcounter
                        }
                        else
                        {
                            break;                                                        //We got a null value, break the for loop
                        }
                    }
                    valueAverage = valueAverage / itemCounter;                            // Set valueAverage, divide by ItemCounter
                    if(printArray)Serial.print( " -> Array: " + String(valueAverage));    //Print the value
                    
                    if( itemCounter < 500 ){
                        Serial.print(" building! need ");
                        Serial.print( valueArrLength - itemCounter );
                        Serial.print(" more items. Temp average ");
                    }

                    return valueAverage;                                                  //Return array average
                };
        } Avg; //Create object
} Timer; //Create object
//Timer loop average - STOP

/***
 *    ╦    ╔═╗  ╔═╗  ╔═╗
 *    ║    ║ ║  ║ ║  ╠═╝
 *    ╩═╝  ╚═╝  ╚═╝  ╩  
 * 
 * 
 * 
 */
const int touchSensorTotal = 7;
bool touchSensorValue[touchSensorTotal];
uint8_t readTouchPin(int touchPos = 0)
{
    switch(touchPos)
    {
        case 0 : return touchRead(T0); break;
        case 1 : return touchRead(T4); break;
        case 2 : return touchRead(T5); break;
        case 3 : return touchRead(T6); break;
        case 4 : return touchRead(T7); break;
        case 5 : return touchRead(T8); break;
        case 6 : return touchRead(T9); break;
    }
}
bool readTouch(int touchPos = 0)
{
    uint8_t retValue = readTouchPin(touchPos);
    if( readTouchPin(touchPos) < 100 ){
        delay(2);
        retValue = readTouchPin(touchPos);
    }
    touchSensorValue[touchPos] = bool(retValue < 100);
    return touchSensorValue[touchPos];
}

void touchInterrupt()
{
    Serial.print("\n touchInterrupt" );
}

struct Button {
    const uint8_t PIN;
    uint32_t numberKeyPresses;
    bool pressed;
};
Button button0 = {4, 0, false};

void IRAM_ATTR isr() {
  button0.numberKeyPresses += 1;
  button0.pressed = true;
}

void loop()
{
    //if(WiFi.status() != WL_CONNECTED)
    //{
    //    wifiConnect();
    //    return;
    //}

    if(HTTPgetUpdates)
    {
        HTTPget("tv.local",1925,"/5/ambilight/measured","AmbiLightTV");       //

        Serial.print("\n Timer : ");
        Serial.print(Timer.Avg.SetAndGet());//Set, Get and print loop time value
    }
    else
    {
        JSONAmbTvFetch();

        switch(iLed_Update_OrderMode)
        {
            case 2:
                ledStrip_UpdateOrder_Random(0, NUM_LEDS);//Ledstart-LedStop
                break;
            case 3:
                ledStrip_UpdateOrderModeFast();
                break;
            default://mode 1
                ledStrip_UpdateOrderModeNormal();
                break;
        }
    }

    if (button0.pressed) {
        Serial.printf("Button 0 has been pressed %u times\n", button0.numberKeyPresses);
        button0.pressed = false;
    }

    //Serial.println("\n");
    //for(int touchSensor = 0; touchSensor < touchSensorTotal; touchSensor ++)
    //{
  //      readTouch(touchSensor);
    //    Serial.print("Touch " + String(touchSensor) + " value is = ");
    //    Serial.println(readTouch(touchSensor) == true?"true":"false");
    //}
}


/***
 *    ╔═╗  ╔═╗  ╔╦╗  ╦ ╦  ╔═╗
 *    ╚═╗  ║╣    ║   ║ ║  ╠═╝
 *    ╚═╝  ╚═╝   ╩   ╚═╝  ╩  
 * 
 * 
 * 
 */
void setup()
{
    Serial.begin(115200);
    timerLogToSerial("\n Serial monitor started.");
    timerLogToSerial("setup - start");

    LedStrip_Setup();
    
    //LOW	    Triggers interrupt whenever the pin is LOW
    //HIGH	    Triggers interrupt whenever the pin is HIGH
    //CHANGE	Triggers interrupt whenever the pin changes value, from HIGH to LOW or LOW to HIGH
    //FALLING	Triggers interrupt when the pin goes from HIGH to LOW
    //RISING	Triggers interrupt when the pin goes from LOW to HIGH
    // attachInterrupt(GPIOPin, ISR, Mode);

    pinMode(button0.PIN, INPUT_PULLUP);
    attachInterrupt(button0.PIN, isr, FALLING);

    timerLogToSerial("setup - Stop");
}


//Touch0 >> GPIO4
//Touch1 >> GPIO13
//Touch2 >> GPIO12
//Touch3 >> GPIO14
//Touch4 >> GPIO27
//Touch5 >> GPIO33
//Touch6 >> GPIO32





//NOT USED




//char tBuf[64];
//char offarray[] ="off";
//if(strstr(offarray,tBuf)!=0 )//checks for off in tBuf
