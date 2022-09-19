#include "lib/tv_pickit2_shift_1 - d503.c"
#include "lib/tv_pickit2_shift_dc_motor.c"
#include "lib/tv_pickit2_shift_ds1307_i2c.c"
#include "lib/tv_pickit2_shift_key4x4_138.c"

#define pir pin_a3

signed INT8 bdn,sec,mp; 
unsigned int8 start_time = 8, end_time = 20;
unsigned int8 this_hour;
unsigned int8 dcmotor_runtime = 2;
unsigned int8 false_cnt;

unsigned int16 t0,guest_count,guest_total;
unsigned int1 pir_temp, is_correct;

unsigned int8 len = 8;
unsigned int8 password_set[8] = {1,9,1,4,2,2,9,8};
unsigned int8 password_enter[8] = {0,0,0,0,0,0,0,0};

#INT_timer1 
void interrupt_timer1() 
{  
   if(dcmotor_onoff == 1 || false_cnt >= 3)
   {
      bdn++;
      if(bdn >= 10)
      {
         bdn = 0;
         sec++;
      }
   }
   else 
   {
      bdn = 0;
      sec = 0;
   }
   
   set_timer1 (3036);
}

void count_guest()
{
   t0 = get_timer0();
   guest_total = guest_count * 255 + t0;
   xuat_4led_7doan_4so(ma7doan[guest_total/1000],ma7doan[guest_total/100%10], ma7doan[guest_total/10%10], ma7doan[guest_total%10]);
   
   if(t0 == 255)
   {
      guest_count++;
      set_timer0(0);
   }
   
   if(guest_total == 10000)
   {
      guest_count = 0;
      set_timer0(1);
   }
}

void run_motor()
{
   if(input(pir) == 1)
   {
      dcmotor_tn = 0;
      setup_timer_0(t0_off);
   }
   else
   {
      dcmotor_tn = 1;
      setup_timer_0(t0_ext_l_to_h | t0_div_1| t0_8_bit);
      count_guest();
   }
   if(pir_temp != input(pir))
   {
      dcmotor_onoff = 1;
      pir_temp = input(pir);
   }
}

int check_password(unsigned int8 arr_1[], arr_2[], length)
{
    for (int i = 0; i < length; i++)
   {
      if(arr_1[i] != arr_2[i])
      {
         return 0;
      }
   }
   return 1;
}

void delete_all()
{
   for (int i = 0; i < len; i++)
   {
      password_enter[i] = 0;
   }
}

void on_alarm()
{
   if(sec <= 2)
   {
      buzzer_on();
   }
   else if(sec == 3)
   {
      buzzer_off();
   }
   else
   {
      sec = 0;
   }  
}

void print_password()
{  
   lcd_goto_xy(2,12);
   for (int i = 0; i < len; i++)
   {
      lcd_data(password_enter[i] + 0x30);
   }
}

void main()
{
   set_up_port_ic_chot();
   
   setup_timer_0(t0_off);
   set_timer0(0);
   
   setup_timer_1 (t1_internal|t1_div_by_8);
   set_timer1 (3036);
   enable_interrupts (global);
   enable_interrupts (INT_timer1);
   
   dc_motor_enable();
   dc_motor_stop();
   
   setup_lcd () ;
   lcd_goto_xy(0,0);
   lcd_data("Thoi gian:");
   lcd_goto_xy(1,0);
   lcd_data("Ng/Th/Nam:");
   
   doc_thoi_gian_tu_realtime () ;
   if (ma_ds13 != ma_ds)
   {
      thiet_lap_thoi_gian_hien_tai () ;
      nap_thoi_gian_htai_vao_ds13b07 () ;
   }
   doc_thoi_gian_tu_realtime () ;
   giaytam = giay_ds13;
   
   is_correct = 0;
   false_cnt = 0;
   sec = 0;
   guest_count = 0;
   pir_temp = input(pir);
   
   while (true)
   {
      doc_thoi_gian_tu_realtime () ;
      giaytam = giay_ds13;
      hien_thi_thoi_gian_ds13b07 () ;
      //Run the motor automatically during working hours when someone is at the door
      this_hour = gio_ds13 / 16 * 10 + gio_ds13 % 16;
      if(this_hour >= start_time && this_hour <= end_time)
      {
         run_motor();
         lcd_goto_xy(2,0);
         lcd_data("                    ");
      }
      //password required when out of business hours
      else
      {
         lcd_goto_xy(2,0);
         lcd_data("Nhap pass:");
         print_password();
         mp = key_4x4_dw();

         if(mp != 0xff)
         {
            if(mp <= 0x09)
            {
               for (int i = 0; i < len - 1; i++)
               {
                  password_enter[i] = password_enter[i + 1];
               }
               password_enter[len - 1] = mp;
            }
            //delete password
            if(mp == 0x0a)
            {
               delete_all();
            }
            // password confirmation
            if(mp == 0x0f)
            {
               is_correct = check_password(password_set, password_enter, len);
               if(is_correct)
               {
                  false_cnt = 0;
                  lcd_goto_xy(3,0);
                  lcd_data("Ban da nhap dung !!!");
                  delete_all();
               }
               else
               {
                  false_cnt++;
                  lcd_goto_xy(3,0);
                  lcd_data("Nhapsai hay nhap lai");
               }
            }
         }
      }
      
      if(is_correct)
      {
         run_motor();
      }
      
      dc_motor_control();
      
      if(sec >= dcmotor_runtime)
      {
         dcmotor_onoff = 0;
         if(dcmotor_tn == 0 && is_correct == 1)
         {
            is_correct = 0;
         }
      }
      // wrong input 3 times turn on the alarm speaker
      if(false_cnt >= 3)
      {
         lcd_goto_xy(3,0);
         lcd_data("Ban nhapsai qua 3lan");
         on_alarm();
      }
      else
      {
         buzzer_off();
      }
   }
}
