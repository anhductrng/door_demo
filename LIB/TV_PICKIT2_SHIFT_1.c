#INCLUDE    <18F4550.H>
#DEVICE     ADC=10
#FUSES      NOWDT,PUT,HS,NOPROTECT,NOLVP,CPUDIV1
#USE        I2C(MASTER,SLOW,SDA=PIN_B0,SCL=PIN_B1)
#USE        DELAY(CLOCK=20000000)
#USE        rs232(baud=9600, xmit=PIN_C6,rcv=PIN_C7)
//NUT NHAN:
#DEFINE  BT0     PIN_B5
#DEFINE  BT1     PIN_B4
#DEFINE  BT2     PIN_B3
#DEFINE  BT3     PIN_B2

#DEFINE  ON      BT0       //DK LED DON, MOTOR
#DEFINE  OFF     BT1
#DEFINE  INV     BT2

#DEFINE  UP      BT0    //DK LED DON
#DEFINE  DW      BT1
#DEFINE  CLR     BT2
#DEFINE  MOD     BT3

#DEFINE  STOP    BT3

#DEFINE  ON1     BT0     
#DEFINE  OFF1    BT1
#DEFINE  ON2     BT2       
#DEFINE  OFF2    BT3


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#DEFINE      SSDO             PIN_E2         //CHUNG TAT CA
#DEFINE      SSCK             PIN_E0         //CHUNG TAT CA

#DEFINE      G_ALL            PIN_D0         //A - 4 BYTE
#DEFINE      RCK_LCD20X4      PIN_D2         //A - 2 BYTE DAO
#DEFINE      RCK_BUZERELAY    PIN_D3     //B - 1 BYTE
#DEFINE      RCK_32LED        PIN_D4         //A - 4 BYTE
#DEFINE      RCK_8LEDQUET     PIN_D5         //B - 2 BYTE DAO
#DEFINE      RCK_4LED7DOAN    PIN_D6         //A - 4 BYTE
#DEFINE      RCK_MATRANLED    PIN_D7         //B - 3 BYTE DAO

VOID XUAT_LCD20X4(UNSIGNED INT8 LCD_SIGNAL,LCD_INS_HTHI);
VOID XUAT_BUZZER_RELAY();

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//CHUONG TRINH KIEM TRA PHIM BT1
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
UNSIGNED INT8 DL_BTN=0;
INT1 PHIM_BT0(UNSIGNED INT16 DL)
{  
    IF(!INPUT(BT0))
    {  
      DL_BTN++;
      IF(DL_BTN==1) RETURN(1);
      ELSE
      {
         IF (DL_BTN>= DL)
         DL_BTN = 0;
      }
    }
    ELSE   
    {
      RETURN(0);
    }
} 
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//CHUONG TRINH KIEM TRA PHIM BT1
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
INT1 PHIM_BT1(UNSIGNED INT16 DL )
{  
    IF (!INPUT(BT1))
    {  
      DL_BTN++;
      IF(DL_BTN==1) RETURN(1);
      ELSE
      {
         IF (DL_BTN>= DL)
         DL_BTN =0;
      }
    }
    ELSE   
    {
      RETURN(0);
    }
} 
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//CHUONG TRINH KIEM TRA PHIM BT2      
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX                                    
INT1 PHIM_BT2(UNSIGNED INT16 DL )
{                                        
    IF (!INPUT(BT2))
    {  
      DL_BTN++;
      IF(DL_BTN==1) RETURN(1);
      ELSE
      {
         IF (DL_BTN>= DL)
         DL_BTN =0;
      }
    }
    ELSE   
    {
      RETURN(0);
    }                                          
}      

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//CHUONG TRINH KIEM TRA PHIM BT3
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
INT1 PHIM_BT3(UNSIGNED INT16 DL,  UNSIGNED INT8 SOLAN  )
{  
    IF (!INPUT(BT3))
    {  
      DL_BTN++;
      IF(DL_BTN==1) RETURN(1);
      ELSE
      {
         IF (DL_BTN>= DL)
         DL_BTN =SOLAN;
      }
    }
    ELSE   
    {
      //DL_BTN =0;
      RETURN(0);
    }
} 
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//KHAI BAO DU LIEU 16 BIT CHO MODULE RELAY, TRIAC, BUZZER, DONG CO
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
UNSIGNED INT16 RBDC;
#BIT STEP_MOTOR_ENABLE  = RBDC.0    //1=ENA,0=DIS
#BIT STEP_MOTOR_IN1     = RBDC.1    
#BIT STEP_MOTOR_IN2     = RBDC.2
#BIT STEP_MOTOR_IN3     = RBDC.3
#BIT STEP_MOTOR_IN4     = RBDC.4
#BIT DC_ENABLE          = RBDC.5    //1=ENA,0=DIS
#BIT PWRKEY             = RBDC.6    //KHOI TAO SIM900

#BIT BUZZER             = RBDC.8    //1=ON,0=OFF
#BIT TRIAC_1            = RBDC.9    //1=ON,0=OFF
#BIT TRIAC_2            = RBDC.10   //1=ON,0=OFF
#BIT RELAY_1            = RBDC.11   //0=ON,1=OFF
#BIT RELAY_2            = RBDC.12   //0=ON,1=OFF



//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX   
CONST UNSIGNED CHAR MA7DOAN[16]= {0XC0,0XF9,0xA4,0XB0,0X99,0X92,0X82,0XF8,
0X80,0X90,0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E};
#DEFINE     MASO0    0XC0
#DEFINE     MASO1    0XF9
#DEFINE     MASO2    0XA4
#DEFINE     MASO3    0XB0
#DEFINE     MASO4    0X99
#DEFINE     MASO5    0X92
#DEFINE     MASO6    0X82
#DEFINE     MASO7    0XF8

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX 
//HAM 102 XUAT 1 BYTE RA THANH GHI DICH
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX    
VOID XUAT_1BYTE(UNSIGNED INT8 X)
{
   UNSIGNED INT8 SB;   
   #BIT BSERI  = X.7
   FOR(SB=0;SB<8;SB++)
      {                                                 
         OUTPUT_BIT(SSDO,BSERI);    
         OUTPUT_LOW(SSCK); OUTPUT_HIGH(SSCK);
         X= X<<1;         
     }
}
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX 
//HAM 301
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//CHUONG TRINH CON XUAT 4 BYTE RA 32 LED DON
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
VOID XUAT_32LED_DON_4BYTE(UNSIGNED INT8 BLD3,BLD2,BLD1,BLD0)
{     
      XUAT_1BYTE(BLD3);          
      XUAT_1BYTE(BLD2);
      XUAT_1BYTE(BLD1);          
      XUAT_1BYTE(BLD0);            
                      
      OUTPUT_HIGH(RCK_32LED);    
      OUTPUT_LOW(RCK_32LED);
    
}
//HAM 302
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//CHUONG TRINH CON XUAT 2 WORD 16 BITRA 32 LED DON
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
VOID XUAT_32LED_DON_2WORD(UNSIGNED INT16 WLD1, UNSIGNED INT16 WLD0)
{     
     UNSIGNED INT8  B3,B2,B1,B0;
     B3 = WLD1>>8;  B2 = WLD1; 
     B1 = WLD0>>8;  B0 = WLD0;
     XUAT_32LED_DON_4BYTE(B3,B2,B1,B0);
}
//HAM 303
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//CHUONG TRINH CON XUAT 1 DOUBLE WORD RA 32 LED DON
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
VOID XUAT_32LED_DON_1DW(UNSIGNED LONG LONG DWLD)
{     
     UNSIGNED INT16  WD1,WD0;
     WD1 = DWLD>>16;  WD0 = DWLD;
     XUAT_32LED_DON_2WORD(WD1,WD0);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX    
//XUAT 1 BIT ROI TRA LAI KET QUA SAU KHI DICH DI 1 BIT
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX    
UNSIGNED  INT8 XUAT_1BIT(UNSIGNED INT8 BYTEXUAT)
{
      UNSIGNED INT8   XBITX;   
      #BIT BSERIX  = XBITX.0
      XBITX = BYTEXUAT;
                                            
      OUTPUT_BIT(SSDO,BSERIX);    
      OUTPUT_LOW(SSCK); OUTPUT_HIGH(SSCK);
      XBITX= XBITX>>1;   
      RETURN(XBITX);  
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx    
//ham 103: xuat 1 word ra thanh ghi dich
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx    
void xuat_1word(unsigned int16 w)
{  
   unsigned int8 b1,b0;
   b1 = w>>8; b0 = w;
   xuat_1byte(b1);    
   xuat_1byte(b0);
}   
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx     
//ham 104: xoa cac module 
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx  
void xoa_tat_6module()                          
{                                                                                              
   xuat_1word(0);                             
   xuat_1word(0); 
   xuat_1word(0);                             
   xuat_1word(0);                                    
   
   output_high(rck_buzerelay);   
   output_low(rck_buzerelay);
   
   output_high(rck_32led);
   output_low(rck_32led); 
   
   output_high(rck_8ledquet);   
   output_low(rck_8ledquet);   
                                 
   output_high(rck_matranled);          
   output_low(rck_matranled);   
                           
   output_high(rck_lcd20x4);   
   output_low(rck_lcd20x4);      
                                                                          
   xuat_1word(0xffff); 
   xuat_1word(0xffff); 
   output_high(rck_4led7doan);       
   output_low(rck_4led7doan);                                                                      
}             
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX 
//HAM 101
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX    
//HAM KHOI TAO CAC PORT VA IC CHOT
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX    
VOID SET_UP_PORT_IC_CHOT()
{     
   set_tris_a(0xff);    set_tris_b(0x3c);                                                   
   set_tris_c(0xf1);    set_tris_e(0x00);                                               
   set_tris_d(0x00);    
   output_d(0x00); 
   xoa_tat_6module();
   port_b_pullups(0xff);          
   rbdc=0;               
            
   setup_adc(adc_clock_div_32);        
   setup_adc_ports(an0_to_an2|vss_vdd); 
   output_low(g_all);        
}
//HAM 104

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//CHUONG TRINH CON XUAT 4 BYTE RA 4 LED 7 DOAN
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
VOID XUAT_4LED_7DOAN_4SO(UNSIGNED INT BL743,BL742,BL741,BL740)
{     
      XUAT_1BYTE(BL740);   XUAT_1BYTE(BL741);
      XUAT_1BYTE(BL742);   XUAT_1BYTE(BL743);       
      OUTPUT_LOW(RCK_4LED7DOAN);   
      OUTPUT_HIGH(RCK_4LED7DOAN);

}
//ham 404
VOID XUAT_4LED_7DOAN_3SO(UNSIGNED INT BL742,BL741,BL740)
{     
     XUAT_4LED_7DOAN_4SO(0XFF,BL742,BL741,BL740);
}
//ham 403
VOID XUAT_4LED_7DOAN_2SO(UNSIGNED INT BL741,BL740)
{     
     XUAT_4LED_7DOAN_4SO(0XFF,0XFF,BL741,BL740);
}
//ham 402
VOID XUAT_4LED_7DOAN_1SO(UNSIGNED INT BL740)
{     
     XUAT_4LED_7DOAN_4SO(0XFF,0XFF,0XFF,BL740);
}

//ham 405
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//CHUONG TRINH CON HIEN THI DU LIEU 16 BIT TREN 4 LED 7 DOAN
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
UNSIGNED CHAR DONVI4,CHUC4,TRAM4,NGAN4;
VOID  XUAT_4LED_7DOAN_GIAIMA_XOA_SO0 (UNSIGNED INT16 TAM)
{         
      DONVI4 = MA7DOAN[TAM %10];         
      CHUC4  = MA7DOAN[TAM/10%10];
      TRAM4  = MA7DOAN[TAM/100%10]; 
      NGAN4  = MA7DOAN[TAM/1000%10];       
      IF (NGAN4==0XC0)
      {
         NGAN4=0XFF;
         IF (TRAM4==0XC0)
         {
            TRAM4=0XFF;
            IF (CHUC4==0XC0)   CHUC4=0XFF;
         }
      }
      XUAT_4LED_7DOAN_4SO(NGAN4,TRAM4,CHUC4,DONVI4);   
}

void  XUAT_4LED_7DOAN_2_SO (unsigned int8 BH,BL)
{         
 XUAT_4LED_7DOAN_4SO(MA7DOAN[BH/10],MA7DOAN[BH%10],MA7DOAN[BL/10],MA7DOAN[BL%10]);   
}

UNSIGNED INT8 LCDDATA=0; 
UNSIGNED INT8 LCDCONTROL=0;
UNSIGNED INT8 GLCDDATA=0;
UNSIGNED INT8 GLCDCONTROL=0; 
//HAM 701
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//CHUONG TRINH CON XUAT DU LIEU 4 BYTE RA GLCD VA LCD
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
VOID XUAT_GLCD_LCD()
{      
    XUAT_1BYTE(GLCDCONTROL);             
    XUAT_1BYTE(GLCDDATA);
    XUAT_1BYTE(LCDCONTROL);       
    XUAT_1BYTE(LCDDATA);      
         
    OUTPUT_HIGH(RCK_LCD20X4);     
    OUTPUT_LOW(RCK_LCD20X4);
}
//HAM 702
VOID XUAT_LCD20X4(UNSIGNED INT8 LCD_SIGNAL,LCD_INS_HTHI)
{     
    LCDCONTROL = ~LCD_SIGNAL;
    LCDDATA    = ~LCD_INS_HTHI;      
    XUAT_GLCD_LCD();
}
//HAM 703
VOID XUAT_GLCD128X64(UNSIGNED INT8 GLCD_SIGNAL,GLCD_INS_HTHI)
{     
    GLCDCONTROL = ~GLCD_SIGNAL;
    GLCDDATA    = ~GLCD_INS_HTHI;     
    XUAT_GLCD_LCD();
}
//HAM 501
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//CHUONG TRINH CON XUAT 2 BYTE RA DK 1 LED QUET SANG
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
VOID XUAT_8LED_7DOAN_QUET_1(UNSIGNED INT MA,SO_HTHI)
{     
      XUAT_1BYTE(~MA);   
      XUAT_1BYTE(~SO_HTHI);
      OUTPUT_HIGH(RCK_8LEDQUET); 
      OUTPUT_LOW(RCK_8LEDQUET);
}
//HAM 502
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//CHUONG TRINH CON XUAT 2 BYTE RA TAT 8 LED QUET
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
VOID XUAT_8LED_7DOAN_QUET_TAT_LED()
{
   XUAT_8LED_7DOAN_QUET_1(0XFF,0XFF);
}
//HAM 503
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//CHUONG TRINH CON HIEN THI LED THEO THU TU
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
CONST UNSIGNED CHAR TTLEDQUET[8]= {0x7F,0xBF,0xDF,0xEF,0xF7,0xFB,0xFD,0xFE};
VOID XUAT_8LED_7DOAN_QUET_2(UNSIGNED INT THUTULED,SO_HTHI)
{         
      XUAT_1BYTE(~ (TTLEDQUET[THUTULED]));   
      XUAT_1BYTE(~SO_HTHI);     
      OUTPUT_HIGH(RCK_8LEDQUET); 
      OUTPUT_LOW(RCK_8LEDQUET);
}
//HAM 504
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//CHUONG TRINH CON HIEN THI 8 SO TREN 8 LED QUET - CO KIEM TRA
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
UNSIGNED CHAR LED_7DQ[8]={0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF};
UNSIGNED INT8 TT8LED=0;
VOID HIEN_THI_8LED_7DOAN_QUET()
{     
      FOR(TT8LED=0;TT8LED<8;TT8LED++)
         IF (LED_7DQ[TT8LED]!=0XFF)
         {           
            XUAT_8LED_7DOAN_QUET_2(TT8LED, LED_7DQ[TT8LED]);
            DELAY_US(100);
            XUAT_8LED_7DOAN_QUET_TAT_LED();           
            }
}
//HAM 505
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//CHUONG TRINH CON HIEN THI 8 SO TREN 8 LED QUET - KHONG KIEM TRA
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
VOID HIEN_THI_8LED_7DOAN_QUET_ALL()
{     
      FOR(TT8LED=0;TT8LED<8;TT8LED++)         
         {           
            XUAT_8LED_7DOAN_QUET_2(TT8LED, LED_7DQ[TT8LED]);
            DELAY_US(100);
            XUAT_8LED_7DOAN_QUET_TAT_LED();           
            }
}

VOID DELAY_QUET_8LED(UNSIGNED INT16 DL)
{ 
      UNSIGNED INT8 I;
      FOR (I=0; I<DL;I++)
      HIEN_THI_8LED_7DOAN_QUET_ALL();
} 

VOID GIAI_MA_GAN_CHO_8LED_QUET_16_XOA(UNSIGNED INT16 X)
{     
      LED_7DQ[0]= MA7DOAN [X %10];    
      LED_7DQ[1]= MA7DOAN [X/10%10];
      LED_7DQ[2]= MA7DOAN [X/100%10];    
      LED_7DQ[3]= MA7DOAN [X/1000%10];
      LED_7DQ[4]= MA7DOAN [X/10000%10];
      IF (LED_7DQ[4]==MASO0) 
      {
            LED_7DQ[4]=0XFF;
            IF (LED_7DQ[3]==MASO0) 
            {
               LED_7DQ[3]=0XFF;
               IF (LED_7DQ[2]==MASO0) 
               {
                  LED_7DQ[2]=0XFF;
                  IF (LED_7DQ[1]==MASO0) 
                  {
                     LED_7DQ[1]=0XFF;
                  }
               }
            }
      }
            
}


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX         
VOID XUAT_2BYTE(UNSIGNED INT16 X)
{
   INT8 SBB;
   #BIT BSERIX  = X.15
   FOR (SBB=0;SBB<16;SBB++)
      {                                                 
         OUTPUT_BIT(SSDO,BSERIX);    
         OUTPUT_LOW(SSCK);         
         OUTPUT_HIGH(SSCK);
         X= X<<1;         
     }
}
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//CHUONG TRINH CON XUAT RA LED MA TRAN
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
VOID XUAT_MATRANLED(UNSIGNED INT16 MAHANG,UNSIGNED INT16 MACOT1,
UNSIGNED INT16 MACOT2,UNSIGNED INT16 MACOT3)
{      
      XUAT_2BYTE(MACOT3);
      XUAT_2BYTE(MACOT2);
      XUAT_2BYTE(MACOT1);
      XUAT_2BYTE(MAHANG);     

      OUTPUT_HIGH(RCK_MATRANLED); 
      OUTPUT_LOW(RCK_MATRANLED);
}
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//CHUONG TRINH CON XUAT RA LED MA TRAN
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
VOID TAT_MATRANLED()
{      
      XUAT_2BYTE(0);
      XUAT_2BYTE(0);
      XUAT_2BYTE(0);
      XUAT_2BYTE(0);           
      OUTPUT_HIGH(RCK_MATRANLED); OUTPUT_LOW(RCK_MATRANLED); 
}
UNSIGNED  INT8 HIEU_CHINH_4BIT_CAO(UNSIGNED INT8 XZY)
{
      INT1  BTG;
      UNSIGNED INT8   BX;   
      #BIT BIT4  = BX.4
      #BIT BIT5  = BX.5
      #BIT BIT6  = BX.6
      #BIT BIT7  = BX.7
      BX=XZY;
      BTG=BIT4;   BIT4=BIT7;  BIT7=BTG;
      BTG=BIT5;   BIT5=BIT6;  BIT6=BTG;      
      RETURN(BX);  
}
//HAM 601
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//MODULE DIEU KHIEN RELAY,TRIAC, BUZZER, DONG CO, SIM
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
VOID XUAT_BUZZER_RELAY()
{     
   UNSIGNED INT8 RBDC1,RBDC2;
      RBDC1=RBDC; RBDC2=RBDC>>8;      
      XUAT_1BYTE(RBDC2);      
      XUAT_1BYTE(RBDC1);
      
      OUTPUT_HIGH(RCK_BUZERELAY);   
      OUTPUT_LOW(RCK_BUZERELAY);
}
//HAM 602
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
VOID BUZZER_ON()
{    
      BUZZER=1;
      XUAT_BUZZER_RELAY();         
}
//HAM 603
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
VOID BUZZER_OFF()
{     
      BUZZER=0;
      XUAT_BUZZER_RELAY();
}
//HAM 604
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
VOID RELAY_1_ON()
{    
      RELAY_1=1;
      XUAT_BUZZER_RELAY();         
}
//HAM 603
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
VOID RELAY_1_OFF()
{    
      RELAY_1=0;
      XUAT_BUZZER_RELAY();         
}
//HAM 604
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
VOID RELAY_2_ON()
{    
      RELAY_2=1;
      XUAT_BUZZER_RELAY();         
}
//HAM 607
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
VOID RELAY_2_OFF()
{    
      RELAY_2=0;
      XUAT_BUZZER_RELAY();         
}
//HAM 608
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
VOID RELAY_1_RELAY_2_ON()
{    
      RELAY_1=1;  RELAY_2=1;
      XUAT_BUZZER_RELAY();         
}
//HAM 609
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
VOID RELAY_1_RELAY_2_OFF()
{    
      RELAY_1=0;  RELAY_2=0;
      XUAT_BUZZER_RELAY();         
}
//HAM 610
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
VOID TRIAC_1_ON()
{    
      TRIAC_1=1;
      XUAT_BUZZER_RELAY();         
}
//HAM 611
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
VOID TRIAC_1_OFF()
{    
      TRIAC_1=0;
      XUAT_BUZZER_RELAY();         
}
//HAM 612
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
VOID TRIAC_2_ON()
{    
      TRIAC_2=1;
      XUAT_BUZZER_RELAY();         
}
//HAM 613
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
VOID TRIAC_2_OFF()
{    
      TRIAC_2=0;
      XUAT_BUZZER_RELAY();         
}
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

VOID GGMMAA()
{
   UNSIGNED INT8   X;   
   X=MA7DOAN[1];
}
