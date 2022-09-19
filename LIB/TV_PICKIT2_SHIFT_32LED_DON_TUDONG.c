UNSIGNED LONG LONG    Y,YCHAYP=1,YCHAYT=0X80000000,YCODINH=0;
UNSIGNED INT16 ZT,ZP;
UNSIGNED CHAR DEM_LED; 
SIGNED I=32,J=0,TTCT=0;

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//32 LED SANG DAN ROI TAT DAN TU NGOAI VAO
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
VOID SANG_TAT_DAN_NGOAI_VAO_32LED()
{ 


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//32 LED SANG DAN ROI TAT DAN TU NGOAI VAO
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
VOID SANG_TAT_DAN_NGOAI_VAO_32LED()
{  
   IF (DEM_LED<16)
   {  
      XUAT_32LED_DON_2WORD(ZP,ZT);
      DELAY_MS(50);   
      ZP= (ZP<<1)+1; 
      ZT= (ZT>>1)+0X8000;
      DEM_LED++;
   }
   ELSE  IF (DEM_LED<32)
   {  
      XUAT_32LED_DON_2WORD(ZP,ZT);
      DELAY_MS(50);   
      ZP= (ZP<<1); 
      ZT= (ZT>>1);
      DEM_LED++;
   }
   ELSE 
   {
      DEM_LED = 0;
      TTCT++;
   }
}   
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//32 LED SANG DAN ROI TAT DAN TU TRONG RA
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
VOID SANG_TAT_DAN_TRONG_RA_32LED()
{  
   IF (DEM_LED<16)
   {   
      XUAT_32LED_DON_2WORD(ZP,ZT);
      DELAY_MS(50);   
      ZP= (ZP>>1)+0X8000; 
      ZT= (ZT<<1)+1;
      DEM_LED++;
   }
   ELSE  IF (DEM_LED<32)
   {  
      XUAT_32LED_DON_2WORD(ZP,ZT);
      DELAY_MS(50);   
      ZP= (ZP>>1); 
      ZT= (ZT<<1);
      DEM_LED++;
    }
   ELSE 
      DEM_LED = 0;               
}   
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//32 LED CHOP TAT 
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
VOID SANG_TAT_32LED()
{     
      XUAT_32LED_DON_1DW(Y); 
      DELAY_MS(200);
      Y=~Y;  
}
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//32 LED SANG DAN ROI TAT DAN TU PHAI SANG TRAI
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
VOID SANG_TAT_DAN_PST_32LED()
{        
   IF (DEM_LED<32)
   {  
      XUAT_32LED_DON_1DW(Y);
      Y= (Y<<1)+1;
      DEM_LED++;
      DELAY_MS(50);
   }
   ELSE  IF (DEM_LED<64)
   {  
      XUAT_32LED_DON_1DW(Y);   
      Y= (Y<<1); 
      DEM_LED++;
      DELAY_MS(50);
   } 
   ELSE 
      DEM_LED = 0;
}    
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//32 LED SANG DAN ROI TAT DAN TU TRAI SANG PHAI
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
VOID SANG_TAT_DAN_TSP_32LED()
{           
      IF (DEM_LED<32)
      {  
         XUAT_32LED_DON_1DW(Y);
         Y= (Y>>1)+0x80000000; 
         DEM_LED++;
         DELAY_MS(50);                   
      }
      ELSE
      IF (DEM_LED<64)
      {  
         XUAT_32LED_DON_1DW(Y); 
         Y= (Y>>1);
         DEM_LED++;
         DELAY_MS(50);
      } 
      ELSE DEM_LED = 0;
}         

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//32 LED SANG DON  PHAI SANG TRAI
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
VOID SANG_DON_PST_32LED()
{       
   IF(I>0)
   {
      IF(J<I)
      {
         Y = YCODINH|YCHAYP;
         XUAT_32LED_DON_1DW(Y);
         DELAY_MS(50); 
         YCHAYP = YCHAYP <<1;
         J++;
      }
      IF (J==I) 
      {
         I--;
         J=0;         
         YCODINH=Y;
         YCHAYP = 1;
      }
   }
   ELSE 
   {
      YCODINH=0;
      YCHAYP = 1;
      I=32;
   }
}   
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//32 LED SANG DON  PHAI SANG TRAI
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
VOID SANG_DON_TSP_32LED()
{       
   IF(I>0)
   {
      IF(J<I)
      {
         Y = YCODINH|YCHAYT;
         XUAT_32LED_DON_1DW(Y);
         DELAY_MS(50); 
         YCHAYT = YCHAYT >>1;
         J++;
      }
      IF (J==I) 
      {
         I--;
         J=0;         
         YCODINH=Y;
         YCHAYT = 0X80000000;
      }
   }
   ELSE 
   {
      YCODINH=0;
      YCHAYT = 0X80000000;
      I=32;
   }
}  
