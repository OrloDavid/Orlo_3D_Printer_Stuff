


#include "../../inc/MarlinConfigPre.h"
#if 1//ENABLED(ANYCUBIC_LCD_DGUS)
#include "encoder.h"
#include "ui_api.h"

#include "AC_spi_dgus.h"
#include "bsp_spi_tft.h"  
//#include "dgus_tft.h"
//#include "dgus_Tunes.h"
//#include "dgus_FileNavigator.h"

#include "../../../../gcode/queue.h"
#include "../../../../sd/cardreader.h"
#include "../../../../libs/numtostr.h"
//#include "../../../../libs/build_date.h"
#include "../../../../MarlinCore.h"
#include "../../../../feature/powerloss.h"





namespace Anycubic {

void color_test()
{
    static uint16_t Rcolor[3]={RED,0,0};
    static uint16_t Gcolor[3]={0,RED,0};
    static uint16_t Bcolor[3]={0,0,RED};
    static char seclect_color_bit,R,G,B=0;
     EncoderState  button_status=ENCODER_DIFF_NO;
     button_status=Encoder_ReceiveAnalyze();
    if(button_status==ENCODER_DIFF_CW)
    {
      if(seclect_color_bit==0)
      {
        R++;
        R=R%32;
      }
      else if(seclect_color_bit==1)
      {
        G++;
        G=G%64;
      }
      else
      {
        B++;
        B=B%32;
      }
    
       LCD_Fill(0,0,320,240,  ((R<<11)+(G<<5)+B));
      
    }
    else if(button_status==ENCODER_DIFF_CCW)
    {
      if(seclect_color_bit==0)
      {
        if(R>0)
        {
          R--;
        }
        else
        {
          R=31;
        }
      }
      else if(seclect_color_bit==1)
      {
        if(G>0)
        {
          G--;
        }
        else
        {
          G=63;
        }
      }
      else
      {
        if(B>0)
        {
          B--;
        }
        else
        {
          B=31;
        }
      }
         LCD_Fill(0,0,320,240, ((R<<11)+(G<<5)+B));
    }
    else if(button_status==ENCODER_DIFF_ENTER)
    {
      seclect_color_bit++;
      seclect_color_bit=seclect_color_bit%3;
    }
      if(button_status!=ENCODER_DIFF_NO)
      {
        LCD_Show_all_String(30,50,"红:",0,0xffff,16,24);
        LCD_ShowIntNum(70,50,R,2,Rcolor[seclect_color_bit],0xffff,16,24);
      
        LCD_ShowString(50,100,"G:",0,0xffff,16,24);
        LCD_ShowIntNum(70,100,G,2,Gcolor[seclect_color_bit],0xffff,16,24);
      
        LCD_ShowString(50,150,"B:",0,0xffff,16,24);
        LCD_ShowIntNum(70,150,B,2,Bcolor[seclect_color_bit],0xffff,16,24);
      }
   }

  void DgusTFT::IdleLoop()  {
        color_test();
    }
  DgusTFT::p_fun fun_array[] = {
                        // DgusTFT::page1_handle,

                       };
       lcd_info_t   DgusTFT::lcd_info;
          FileList  DgusTFT::filelist;    
   printer_state_t  DgusTFT::printer_state;
         uint32_t   DgusTFT::page_index;
         uint32_t   DgusTFT::last_page_index;
         uint32_t   DgusTFT::page_max_item;
         uint32_t   DgusTFT::page_now_item;  
             char   DgusTFT::item_string[max_item_buf_num][max_string_buf];                       
//  printer_state_t  DgusTFT::printer_state;
//  paused_state_t   DgusTFT::pause_state;
//  heater_state_t   DgusTFT::hotend_state;
//  heater_state_t   DgusTFT::hotbed_state;
//  xy_uint8_t       DgusTFT::selectedmeshpoint;
//  char             DgusTFT::selectedfile[MAX_PATH_LEN];
//  char             DgusTFT::panel_command[MAX_CMND_LEN];
//  uint8_t          DgusTFT::command_len;
//  float            DgusTFT::live_Zoffset;
//  file_menu_t      DgusTFT::file_menu;
//  
//  bool             DgusTFT::data_received;
//  uint8_t          DgusTFT::data_buf[64];
//  uint8_t          DgusTFT::data_index;
//  uint32_t         DgusTFT::page_index_last;
//  uint32_t         DgusTFT::page_index_now;
//  uint8_t          DgusTFT::message_index;
//  uint8_t          DgusTFT::pop_up_index;
//  uint32_t         DgusTFT::key_index;
//  uint32_t         DgusTFT::key_value;
//  uint16_t         DgusTFT::filenumber;
//  uint16_t         DgusTFT::filepage;
//  uint8_t          DgusTFT::lcd_txtbox_index;
//  uint8_t          DgusTFT::lcd_txtbox_page;
//  uint16_t         DgusTFT::change_color_index;
//  uint8_t          DgusTFT::TFTpausingFlag;
//  uint8_t          DgusTFT::TFTStatusFlag;
//  uint8_t          DgusTFT::TFTresumingflag;
//  uint8_t          DgusTFT::ready;
//
//  lcd_info_t       DgusTFT::lcd_info_back;  // back for changing on lcd, to save flash lifecycle


  DgusTFT Dgus;

  DgusTFT::DgusTFT() {

  }
    void DgusTFT::PAGE_choose_oper()
    {
      static uint8_t now_page;
      if(last_page_index!=page_index)
      {
        last_page_index=page_index;
        page_now_item=0;
        if(lcd_info.language==CHS)
        {
          strcpy(item_string[0],"界面信息");
          strcpy(item_string[1],"准备");
          strcpy(item_string[2],"控制");
          strcpy(item_string[3],"打印");
        }
        else
        {
          strcpy(item_string[0],"界面信息");
          strcpy(item_string[1],"准备");
          strcpy(item_string[2],"控制");
          strcpy(item_string[3],"打印");          
        }
        page_max_item=4;
        if(printer_state==AC_printer_printing)
        {
         if(lcd_info.language==CHS)
        {
          strcpy(item_string[0],"界面信息");
          strcpy(item_string[1],"准备");
          strcpy(item_string[2],"控制");
          strcpy(item_string[3],"打印");
        }
        else
        {
          strcpy(item_string[0],"界面信息");
          strcpy(item_string[1],"准备");
          strcpy(item_string[2],"控制");
          strcpy(item_string[3],"打印");          
        }         
        }
        else if(printer_state== AC_printer_paused)
        {
           if(lcd_info.language==CHS)
          {
            strcpy(item_string[0],"界面信息");
            strcpy(item_string[1],"准备");
            strcpy(item_string[2],"控制");
            strcpy(item_string[3],"打印");
          }
          else
          {
            strcpy(item_string[0],"界面信息");
            strcpy(item_string[1],"准备");
            strcpy(item_string[2],"控制");
            strcpy(item_string[3],"打印");          
          }
        }

        
      }

      
    }
    
    
    
     void DgusTFT::show_item(uint32_t num,char seclect)
      {
        if(seclect)
        {
            LCD_Show_all_String(0,num*Line_height,(const uint8_t*)item_string[num],Seclect_f_color,Default_back_color,16,24);
        }
        else
        {
          LCD_Show_all_String(0,num*Line_height,(const uint8_t*)item_string[num],N_Seclect_f_color,Default_back_color,16,24);
        }
      }
//  void DgusTFT::Startup() {
//    selectedfile[0]   = '\0';
//    panel_command[0]  = '\0';
//    command_len       = 0;
//    printer_state     = AC_printer_idle;
//    pause_state       = AC_paused_idle;
//    hotend_state      = AC_heater_off;
//    hotbed_state      = AC_heater_off;
//    live_Zoffset      = 0.0;
//    file_menu         = AC_menu_file;

//    // Setup pins for powerloss detection
//    // Two IO pins are connected on the Trigorilla Board
//    // On a power interruption the OUTAGECON_PIN goes low.

////    #if ENABLED(POWER_LOSS_RECOVERY)
////      OUT_WRITE(OUTAGECON_PIN, HIGH);
////    #endif

//    // Filament runout is handled by Marlin settings in Configuration.h
//    // opt_set    FIL_RUNOUT_STATE HIGH  // Pin state indicating that filament is NOT present.
//    // opt_enable FIL_RUNOUT_PULLUP

//    TFTSer.begin(115200);

//    // Signal Board has reset
////    SendtoTFTLN(AC_msg_main_board_has_reset);

//    // Enable levelling and Disable end stops during print
//    // as Z home places nozzle above the bed so we need to allow it past the end stops
//    injectCommands_P(AC_cmnd_enable_levelling);

//    // Startup tunes are defined in Tunes.h
//    // PlayTune(BEEPER_PIN, Anycubic_PowerOn, 1);
//    // PlayTune(BEEPER_PIN, GB_PowerOn, 1);
//    #if ACDEBUGLEVEL
//      SERIAL_ECHOLNPAIR("AC Debug Level ", ACDEBUGLEVEL);
//    #endif
////    SendtoTFTLN(AC_msg_ready);
//  }

//  void DgusTFT::ParamInit() {

//    if(lcd_info.language == ExtUI::CHS) {
//      page_index_now = 1;
//    } else if(lcd_info.language == ExtUI::ENG) {
//      page_index_now = 121;
//    }

//    LcdAudioSet(lcd_info.audio);

//#if ACDEBUG(AC_MARLIN)
//    if(lcd_info.language == ExtUI::CHS) {
//      SERIAL_ECHOLN("lcd language: CHS");
//    } else if(lcd_info.language == ExtUI::ENG) {
//      SERIAL_ECHOLN("lcd language: ENG");
//    }

//    if(lcd_info.audio == ExtUI::ON) {
//      SERIAL_ECHOLN("lcd audio: ON");
//    } else if(lcd_info.audio == ExtUI::OFF) {
//      SERIAL_ECHOLN("lcd audio: OFF");
//    }
//#endif

//    RequestValueFromTFT(0x14);  // get page ID
//  }



//#if ACDEBUG(AC_MARLIN)
//    if(key_value) {
//      SERIAL_ECHOLNPAIR("page: ", page_index_now);
//      SERIAL_ECHOLNPAIR("key: ", key_value);
//    }
//#endif

//    if(page_index_now == 115) {
//      page115_handle();
//    } else if(page_index_now == 117) {
//      page117_handle();
//    } else if(page_index_now == 170) {
//      page170_handle();
//    } else if(page_index_now == 171) {
//      page171_handle();
//    } else if(page_index_now == 173) {
//      page173_handle();
//    } else if(page_index_now == 175) {
//      page175_handle();
//    } else if(page_index_now == 176) {
//      page176_handle();
//    } else {

//      if(lcd_info.language == ExtUI::CHS) {
//        if(0 < page_index_now && page_index_now < 36) {
//          fun_array[page_index_now-1]();
//        } else {
//#if ACDEBUG(AC_MARLIN)
//          SERIAL_ECHOLNPAIR("line: ", __LINE__);
//          SERIAL_ECHOLNPAIR("fun not exists: ", page_index_now);
//#endif
//        }
//      } else if(lcd_info.language == ExtUI::ENG) {
//        if(120 < page_index_now && page_index_now < 156) {
//          fun_array[page_index_now-1-120]();  // ENG page_index is 120 more than CHS
//        } else {

//#if ACDEBUG(AC_MARLIN)
//          SERIAL_ECHOLNPAIR("line: ", __LINE__);
//          SERIAL_ECHOLNPAIR("fun not exists: ", page_index_now);
//#endif
//        }
//      }

//    }

//    pop_up_manager();
//    key_value = 0;

//    CheckHeaters();
//  }

//  void DgusTFT::PrinterKilled(PGM_P error,PGM_P component)  {
////    SendtoTFTLN(AC_msg_kill_lcd);
//    #if ACDEBUG(AC_MARLIN)
//      SERIAL_ECHOLNPAIR("PrinterKilled()\nerror: ", error , "\ncomponent: ", component);
//    #endif

//    if(strncmp(error, "Heating Failed", strlen("Heating Failed")) == 0) {

//        if(strncmp(component, "Bed", strlen("Bed")) == 0) {
//            SERIAL_ECHOLNPAIR("Check Bed heater");
//        } else if(strncmp(component, "E1", strlen("E1")) == 0) {
//            ChangePageOfTFT(PAGE_ABNORMAL);
//            SERIAL_ECHOLNPAIR("Check E1 heater");
//        }

//    } else if(strncmp(error, "Err: MINTEMP", strlen("Err: MINTEMP")) == 0) {

//        if(strncmp(component, "Bed", strlen("Bed")) == 0) {
//            SERIAL_ECHOLNPAIR("Check Bed thermistor");
//        } else if(strncmp(component, "E1", strlen("E1")) == 0) {
//            ChangePageOfTFT(PAGE_ABNORMAL);
//            SERIAL_ECHOLNPAIR("Check E1 thermistor");
//        }

//    } else if(strncmp(error, "THERMAL RUNAWAY", strlen("THERMAL RUNAWAY")) == 0) {

//        if(strncmp(component, "Bed", strlen("Bed")) == 0) {
//            SERIAL_ECHOLNPAIR("Check Bed thermal runaway");
//        } else if(strncmp(component, "E1", strlen("E1")) == 0) {
//            ChangePageOfTFT(PAGE_ABNORMAL);
//            SERIAL_ECHOLNPAIR("Check E1 thermal runaway");
//        }
//    }

//  }

  void DgusTFT::MediaEvent(media_event_t event)  {
       uint16_t  num;
    switch(event) {
      case AC_media_inserted:
//
         LCD_Show_all_String(30,50,"SD inserted",0,0xffff,16,24);
      
         num=filelist.count();
         LCD_ShowIntNum(70,150,num,2,0,0xffff,16,24);
      break;

      case AC_media_removed:

      break;

      case AC_media_error:
//        SendtoTFTLN(AC_msg_no_sd_card);
      break;
    }
  }

//  void DgusTFT::TimerEvent(timer_event_t event)  {
//    char str_buf[20];

//    #if ACDEBUG(AC_MARLIN)
//      SERIAL_ECHOLNPAIR("TimerEvent() ", event);
//      SERIAL_ECHOLNPAIR("Printer State: ", printer_state);
//    #endif
//         SERIAL_ECHOLNPGM("TimerEvent call event =:",event);
//         SERIAL_ECHOLNPGM("Timer event printer_state= :",printer_state);
//    switch (event) {
//      case AC_timer_started: {
//        live_Zoffset = 0.0; // reset print offset
//        setSoftEndstopState(false);  // disable endstops to print
//        if(printer_state!=AC_printer_stopping)
//        {
//              printer_state = AC_printer_printing;
//        }
//     
////        SendtoTFTLN(AC_msg_print_from_sd_card);
//      } break;

//      case AC_timer_paused: {
////        printer_state = AC_printer_paused;
////        pause_state   = AC_paused_idle;
////        SendtoTFTLN(AC_msg_paused);
//      }
//      break;

//      case AC_timer_stopped: {
//      
//        if ((printer_state != AC_printer_idle)&&(printer_state!=AC_printer_stopping)) {

//          if(printer_state == AC_printer_stopping_from_media_remove) {

//            ChangePageOfTFT(PAGE_NO_SD);

//          } else if(printer_state != AC_printer_stopping){

//            printer_state = AC_printer_stopping;

//            // Get Printing Time
//            uint32_t time = getProgress_seconds_elapsed() / 60;
//            sprintf(str_buf, "%s H ", utostr3(time/60));
//            sprintf(str_buf+strlen(str_buf), "%s M", utostr3(time%60));
//            SendTxtToTFT(str_buf, TXT_FINISH_TIME);
//            ChangePageOfTFT(PAGE_PRINT_FINISH);
//          }          
//        }
//        setSoftEndstopState(true); // enable endstops
//          printer_state     = AC_printer_idle;
//          pause_state       = AC_paused_idle;
//          hotend_state      = AC_heater_off;
//          hotbed_state      = AC_heater_off;
//      } 
//      break;
//    }
//  }

//  void DgusTFT::FilamentRunout()  {
//    #if ACDEBUG(AC_MARLIN)
//      SERIAL_ECHOLNPAIR("FilamentRunout() printer_state ", printer_state);
//    #endif
//    // 1 Signal filament out
////    SendtoTFTLN(isPrintingFromMedia() ? AC_msg_filament_out_alert : AC_msg_filament_out_block);
////    printer_state = AC_printer_filament_out;
//    pop_up_index = 15;  // show filament lack

////    SERIAL_ECHOLNPAIR("getFilamentRunoutState: ", getFilamentRunoutState());
////    if(READ(FIL_RUNOUT_PIN) == FIL_RUNOUT_STATE) {
////      PlayTune(BEEPER_PIN, FilamentOut, 1);

//      if(isPrintingFromMedia()) {
//        pausePrint();
//        printer_state = AC_printer_pausing;
//        pause_state = AC_paused_filament_lack;
//      }
////    }
//  }

//  void DgusTFT::ConfirmationRequest(const char * const msg)  {
//    // M108 continue
//    #if ACDEBUG(AC_MARLIN)
//      SERIAL_ECHOLNPAIR("ConfirmationRequest() ", msg);
//      SERIAL_ECHOLNPAIR("printer_state:", printer_state);
//      SERIAL_ECHOLNPAIR("pause_state:", pause_state);
//    #endif
//      SERIAL_ECHOLNPGM("ConfirmationRequest call msg :",msg);
//     if (strcmp_P(msg, "RESUME OPTIONS:") == 0)
//     {
//         SERIAL_ECHOLNPGM("ConfirmationRequest call msg in");
//         setPauseMenuResponse(PAUSE_RESPONSE_RESUME_PRINT);
//     }
//   
//    switch (printer_state) {

//       case AC_printer_stopping:
//               // if (strcmp_P(msg, "Heating...")==0)
//              //  {
//                 //  SERIAL_ECHOLNPGM("AC_printer_stopping and send M108");
//                   injectCommands_P(PSTR("M108"));
//             //   }
//       break;

//      case AC_printer_pausing: {
//        if (strcmp_P(msg, MARLIN_msg_print_paused) == 0 || strcmp_P(msg, MARLIN_msg_nozzle_parked) == 0) {
//          if(pause_state != AC_paused_filament_lack) {
//            ChangePageOfTFT(PAGE_STATUS1);    // enable continue button
//          }
//          printer_state = AC_printer_paused;
//        }
//      } break;

//      case AC_printer_resuming_from_power_outage:
//      case AC_printer_printing:
//      case AC_printer_paused: {
//        // Heater timout, send acknowledgement
//        if (strcmp_P(msg, MARLIN_msg_heater_timeout) == 0) {
//         // pause_state = AC_paused_heater_timed_out;     被我注释掉了
////          SendtoTFTLN(AC_msg_paused); // enable continue button
//          PlayTune(BEEPER_PIN,Heater_Timedout,1);
//        }
//        // Reheat finished, send acknowledgement
//        else if (strcmp_P(msg, MARLIN_msg_reheat_done) == 0) {

//#if ACDEBUG(AC_MARLIN)
//          SERIAL_ECHOLNPAIR("send M108 ", __LINE__);
//#endif
//          injectCommands_P(PSTR("M108"));

//          if(pause_state != AC_paused_filament_lack) {
//            pause_state = AC_paused_idle;
//          }

////          SendtoTFTLN(AC_msg_paused); // enable continue button
//        }
//        // Filament Purging, send acknowledgement enter run mode
//        else if (strcmp_P(msg, MARLIN_msg_filament_purging) == 0) {
//          pause_state = AC_paused_purging_filament;
////          SendtoTFTLN(AC_msg_paused); // enable continue button
//        } else if(strcmp_P(msg, MARLIN_msg_nozzle_parked) == 0) {
//#if ACDEBUG(AC_MARLIN)
//            SERIAL_ECHOLNPAIR("send M108 ", __LINE__);
//#endif
//            injectCommands_P(PSTR("M108"));

//            if(pause_state != AC_paused_filament_lack) {
//              pause_state = AC_paused_idle;
//            }
//        }
//      } break;

//      default:
//      break;
//    }
//  }

//  void DgusTFT::StatusChange(const char * const msg)  {
//    #if ACDEBUG(AC_MARLIN)
//      SERIAL_ECHOLNPAIR("StatusChange() ", msg);
//      SERIAL_ECHOLNPAIR("printer_state:", printer_state);
//      SERIAL_ECHOLNPAIR("pause_state:", pause_state);
//    #endif
//    bool msg_matched = false;
//    static uint8_t probe_cnt = 0;


//    SERIAL_ECHOLNPGM("StatusChange call msg :",msg);
//    // The only way to get printer status is to parse messages
//    // Use the state to minimise the work we do here.
//    switch (printer_state) {
//      case AC_printer_probing: {

//        if (strncmp_P(msg, MARLIN_msg_probing_point, strlen_P(MARLIN_msg_probing_point)) == 0) {
//            probe_cnt++;
//        }

//        // If probing completes ok save the mesh and park
//        // Ignore the custom machine name
//        if (strcmp_P(msg + strlen(CUSTOM_MACHINE_NAME), MARLIN_msg_ready) == 0) {
//          if(probe_cnt == GRID_MAX_POINTS_X*GRID_MAX_POINTS_Y) {
//            probe_cnt = 0;
//            injectCommands_P(PSTR("M500"));    // G27 park nozzle
//            ChangePageOfTFT(PAGE_PreLEVEL);
//            printer_state = AC_printer_idle;
//            msg_matched = true;
//          }
//        }

//        // If probing fails dont save the mesh raise the probe above the bad point
//        if (strcmp_P(msg, MARLIN_msg_probing_failed) == 0) {
//          PlayTune(BEEPER_PIN, BeepBeepBeeep, 1);
//          injectCommands_P(PSTR("G1 Z50 F500"));
////          SendtoTFTLN(AC_msg_probing_complete);
//          printer_state = AC_printer_idle;
//          msg_matched = true;
//        }

//        if (strcmp_P(msg, MARLIN_msg_probe_preheat_start) == 0) {
//          ChangePageOfTFT(PAGE_CHS_PROBE_PREHEATING);
//        }

//        if (strcmp_P(msg, MARLIN_msg_probe_preheat_stop) == 0) {
//          ChangePageOfTFT(PAGE_LEVELING);
//        }

//      } break;

//      case AC_printer_printing: {
//        if (strcmp_P(msg, MARLIN_msg_reheating) == 0) {
////          SendtoTFTLN(AC_msg_paused); // enable continue button
//          ChangePageOfTFT(PAGE_STATUS2);
//          msg_matched = true;
//        } else if(strcmp_P(msg, MARLIN_msg_media_removed) == 0) {
//          msg_matched = true;
//          printer_state = AC_printer_stopping_from_media_remove;
//        } else {
////          SERIAL_ECHOLNPAIR("setFilamentRunoutState: ", __LINE__);
////          setFilamentRunoutState(false);
//        }
//      } break;

//      case AC_printer_pausing: {
//        if (strcmp_P(msg, MARLIN_msg_print_paused) == 0) {

//          if(pause_state != AC_paused_filament_lack) {
//            ChangePageOfTFT(PAGE_STATUS1);        // show resume
//            pause_state = AC_paused_idle;
//          }

//          printer_state = AC_printer_paused;

//          msg_matched = true;
//         }
//      } break;

//      case AC_printer_paused: {
//        if (strcmp_P(msg, MARLIN_msg_print_paused) == 0) {

//          if(pause_state != AC_paused_filament_lack) {
//            ChangePageOfTFT(PAGE_STATUS1);        // show resume
//            pause_state = AC_paused_idle;
//          }

//          printer_state = AC_printer_paused;

//          msg_matched = true;
//         }
//      } break;

//      case AC_printer_stopping: {
//       /* if (strcmp_P(msg, MARLIN_msg_print_aborted) == 0) {
//          ChangePageOfTFT(PAGE_MAIN);
//          printer_state = AC_printer_idle;
//          msg_matched = true;
//        }
//        */
//      } break;
//      default:
//      break;
//    }

//    // If not matched earlier see if this was a heater message
//    if (!msg_matched) {
//      if (strcmp_P(msg, MARLIN_msg_extruder_heating) == 0) {
////        SendtoTFTLN(AC_msg_nozzle_heating);
//        hotend_state = AC_heater_temp_set;
//      }
//      else if (strcmp_P(msg, MARLIN_msg_bed_heating) == 0) {
////        SendtoTFTLN(AC_msg_bed_heating);
//        hotbed_state = AC_heater_temp_set;
//      }
//    }
//  }

//  void DgusTFT::PowerLoss()  {
//  // On:  5A A5 05 82 00 82 00 00
//  // Off: 5A A5 05 82 00 82 00 64

//    uint8_t data_buf[8] = { 0x5A, 0xA5, 0x05, 0x82, 0x00, 0x82, 0x00, 0x00 };

//    for(uint8_t i=0; i<8; i++) {
//      TFTSer.write(data_buf[i]);
//    }
//  }

//  void DgusTFT::PowerLossRecovery()  {
//    printer_state = AC_printer_resuming_from_power_outage; // Play tune to notify user we can recover.
//  }

//  void DgusTFT::SendtoTFT(PGM_P str) {  // A helper to print PROGMEN string to the panel
//    #if ACDEBUG(AC_SOME)
//      serialprintPGM(str);
//    #endif
//    while (const char c = pgm_read_byte(str++)) TFTSer.print(c);
//  }

//  void DgusTFT::SendValueToTFT(uint32_t value, uint32_t address) {

//    uint8_t data_buf[32] = {0};
//    uint8_t data_index = 0;

//    uint8_t *p_u8 =  (uint8_t *)(&address)+1 ;

//    data_buf[data_index++] = 0x5A;
//    data_buf[data_index++] = 0xA5;
//    data_buf[data_index++] = 0x05;
//    data_buf[data_index++] = 0x82;
//    data_buf[data_index++] = *p_u8;
//    p_u8--;
//    data_buf[data_index++] = *p_u8;
//    p_u8 =  (uint8_t *)(&value)+1;
//    data_buf[data_index++] = *p_u8;
//    p_u8--;
//    data_buf[data_index++] = *p_u8;

//    for(uint8_t i=0; i<data_index; i++) {
//      TFTSer.write(data_buf[i]);
//    }
//  }

//  void DgusTFT::RequestValueFromTFT(uint32_t address) {

//    uint8_t data_buf[20] = {0};
//    uint8_t data_index = 0;
//    uint8_t data_len = 0;

//    uint8_t *p_u8 =  (uint8_t *)(&address)+1 ;

//    data_buf[data_index++] = 0x5A;
//    data_buf[data_index++] = 0xA5;
//    data_buf[data_index++] = 0x04;
//    data_buf[data_index++] = 0x83;
//    data_buf[data_index++] = *p_u8;
//    p_u8--;
//    data_buf[data_index++] = *p_u8;
//    data_buf[data_index++] = 0x01;

//    for(uint8_t i=0; i<data_index; i++) {
//      TFTSer.write(data_buf[i]);
//    }
//  }
//  

//  void DgusTFT::SendTxtToTFT(const char *pdata, uint32_t address) {

//    char data_buf[128] = {0};
//    uint8_t data_index = 0;
//    uint8_t data_len = 0;

//    uint8_t *p_u8 =  (uint8_t *)(&address)+1 ;
//    data_len = strlen(pdata);

//    data_buf[data_index++] = 0x5A;
//    data_buf[data_index++] = 0xA5;
//    data_buf[data_index++] = data_len + 5;
//    data_buf[data_index++] = 0x82;
//    data_buf[data_index++] = *p_u8;
//    p_u8--;
//    data_buf[data_index++] = *p_u8;

//    strncpy(&data_buf[data_index], pdata, data_len);
//    data_index += data_len;

//    data_buf[data_index++] = 0xFF;
//    data_buf[data_index++] = 0xFF;

//    for(uint8_t i=0; i<data_index; i++) {
//      TFTSer.write(data_buf[i]);
//    }
//  }

//  void DgusTFT::SendColorToTFT(uint32_t color, uint32_t address) {

//    uint8_t data_buf[32] = {0};
//    uint8_t data_index = 0;

//    uint8_t *p_u8 =  (uint8_t *)(&address)+1 ;
//    address += 3;

//    data_buf[data_index++] = 0x5A;
//    data_buf[data_index++] = 0xA5;
//    data_buf[data_index++] = 0x05;
//    data_buf[data_index++] = 0x82;
//    data_buf[data_index++] = *p_u8;
//    p_u8--;
//    data_buf[data_index++] = *p_u8;
//    p_u8 =  (uint8_t *)(&color)+1;
//    data_buf[data_index++] = *p_u8;
//    p_u8--;
//    data_buf[data_index++] = *p_u8;

//    for(uint8_t i=0; i<data_index; i++) {
//      TFTSer.write(data_buf[i]);
//    }
//  }

//  void DgusTFT::SendReadNumOfTxtToTFT(uint8_t number, uint32_t address) {
//    uint8_t data_buf[32] = {0};
//    uint8_t data_index = 0;

//    uint8_t *p_u8 =  (uint8_t *)(&address)+1 ;

//    data_buf[data_index++] = 0x5A;
//    data_buf[data_index++] = 0xA5;
//    data_buf[data_index++] = 0x04;      //frame length
//    data_buf[data_index++] = 0x83;
//    data_buf[data_index++] = *p_u8;
//    p_u8--;
//    data_buf[data_index++] = *p_u8;
//    data_buf[data_index++] = number;    //how much bytes to read

//    for(uint8_t i=0; i<data_index; i++) {
//      TFTSer.write(data_buf[i]);
//    }
//  }

//  void DgusTFT::ChangePageOfTFT(uint32_t page_index) {

//    uint8_t data_buf[20] = {0};
//    uint8_t data_index = 0;
//    uint32_t data_temp = 0;

//    if(lcd_info.language == ExtUI::CHS) {
//        data_temp = page_index;

//    } else if(lcd_info.language == ExtUI::ENG) {
//        if(PAGE_OUTAGE_RECOVERY == page_index) {
//            data_temp = PAGE_ENG_OUTAGE_RECOVERY;
//        } else if(PAGE_CHS_PROBE_PREHEATING == page_index) {
//            data_temp = PAGE_ENG_PROBE_PREHEATING;
//        } else {
//            data_temp = page_index+120;
//        }
//    }

//    uint8_t *p_u8 = (uint8_t *)(&data_temp)+1 ;

//    data_buf[data_index++] = 0x5A;
//    data_buf[data_index++] = 0xA5;
//    data_buf[data_index++] = 0x07;
//    data_buf[data_index++] = 0x82;
//    data_buf[data_index++] = 0x00;
//    data_buf[data_index++] = 0x84;
//    data_buf[data_index++] = 0x5A;
//    data_buf[data_index++] = 0x01;
//    data_buf[data_index++] = *p_u8;
//    p_u8--;
//    data_buf[data_index++] = *p_u8;

//    for(uint8_t i=0; i<data_index; i++) {
//      TFTSer.write(data_buf[i]);
//    }

//    page_index_last = page_index_now;
//	page_index_now = data_temp ;
//  }

//  void DgusTFT::LcdAudioSet(ExtUI::audio_t audio) {
//  // On:  5A A5 07 82 00 80 5A 00 00 1A
//  // Off: 5A A5 07 82 00 80 5A 00 00 12
//    uint8_t data_buf[20] = { 0x5A, 0xA5, 0x07, 0x82, 0x00, 0x80, 0x5A, 0x00, 0x00 };

//    if(audio == ExtUI::ON) {
//        data_buf[9] = 0x1A;
//    } else if(audio == ExtUI::OFF) {
//        data_buf[9] = 0x12;
//    }

//    for(uint8_t i=0; i<10; i++) {
//      TFTSer.write(data_buf[i]);
//    }
//  }

//  bool DgusTFT::ReadTFTCommand() {

//    static uint8_t length = 0, cnt = 0, tft_receive_steps = 0;
//	uint8_t data;

//	if(0 == TFTSer.available() || data_received) {
//	  return false;
//	}

//	data = TFTSer.read();

//    if(tft_receive_steps==0) {
//      if(data != 0x5a) {
//        cnt = 0;
//        length = 0;
//        data_index = 0;
//        data_received = 0;
//        return false;
//      }

//      while(!TFTSer.available()) {
//        TERN_(USE_WATCHDOG, HAL_watchdog_refresh());
//      }

//      data = TFTSer.read(); 
//      //MYSERIAL.write(data );
//      if(data == 0xa5) {
//        tft_receive_steps=2;
//      }
//    } else if(tft_receive_steps==2) {  
//      length = data;
//      tft_receive_steps = 3;
//      data_index = 0;
//      cnt = 0 ;
//    } else if(tft_receive_steps==3) {
//      if(data_index >= 63) {
//#if ACDEBUG(AC_MARLIN)
//        SERIAL_ECHOLNPAIR("lcd uart buff overflow: ", data_index);
//#endif
//        data_index = 0;
//        data_received = 0;
//        return false;
//      }
//      data_buf[data_index++] = data;
//      cnt++;
//      if(cnt>=length) {   //Receive complete
//        tft_receive_steps = 0;
//        cnt = 0;
//        data_index = 0;
//        data_received = 1;
//        return true;
//      }
//    }

//    return false;
//  }

//#if 0
//  {

////    SERIAL_ECHOLNPAIR("ReadTFTCommand: ", millis());
////    return -1;

//    bool command_ready = false;
//    uint8_t data = 0;

//    while(TFTSer.available() > 0 && command_len < MAX_CMND_LEN) {
//      data = TFTSer.read();
//      if(0 == command_len) {
//        if
//      }
//      
//      panel_command[command_len] = 
//      if (panel_command[command_len] == '\n') {
//        command_ready = true;
//        break;
//      }
//      command_len++;
//    }

//    if (command_ready) {
//      panel_command[command_len] = 0x00;
////      #if ACDEBUG(AC_ALL)
//        SERIAL_ECHOLNPAIR("< ", panel_command);
////      #endif
//      #if ACDEBUG(AC_SOME)
//        // Ignore status request commands
//        uint8_t req = atoi(&panel_command[1]);
//        if (req > 7 && req != 20) {
//          SERIAL_ECHOLNPAIR("> ", panel_command);
//          SERIAL_ECHOLNPAIR("printer_state:", printer_state);
//        }
//      #endif
//    }
//    return command_ready;
//  }
//#endif

//  int8_t DgusTFT::Findcmndpos(const char * buff, char q) {
//    int8_t pos = 0;
//    do { if (buff[pos] == q) return pos; } while(++pos < MAX_CMND_LEN);
//    return -1;
//  }

//  void DgusTFT::CheckHeaters() {
//    static uint8_t faultE0Duration = 0, faultBedDuration = 0;
//    float temp = 0;
//    static uint32_t time_last = 0;

//    if(millis() < time_last + 500) {
//        return;
//    }

//    time_last = millis();

//    // if the hotend temp is abnormal, confirm state before signalling panel
//    temp = getActualTemp_celsius(E0);
//    if(!WITHIN(temp, HEATER_0_MINTEMP, HEATER_0_MAXTEMP)) {
//        faultE0Duration++;
//        if (faultE0Duration >= AC_HEATER_FAULT_VALIDATION_TIME) {
////          SendtoTFTLN(AC_msg_nozzle_temp_abnormal);
//#if ACDEBUG(AC_MARLIN)
//          SERIAL_ECHOLNPAIR("Extruder temp abnormal! : ", temp);
//#endif
//          faultE0Duration = 0;
//        }
//    }

//    temp = getActualTemp_celsius(BED);
//    if(!WITHIN(temp, BED_MINTEMP, BED_MAXTEMP)) {
//        faultBedDuration++;
//        if (faultBedDuration >= AC_HEATER_FAULT_VALIDATION_TIME) {
////            SendtoTFTLN(AC_msg_bed_temp_abnormal);
//#if ACDEBUG(AC_MARLIN)
//            SERIAL_ECHOLNPAIR("Bed temp abnormal! : ", temp);
//#endif
//            faultBedDuration = 0;
//        }
//    }

//#if 0
//    // Update panel with hotend heater status
//    if (hotend_state != AC_heater_temp_reached) {
//      if (WITHIN(getActualTemp_celsius(E0) - getTargetTemp_celsius(E0), -1, 1)) {
//        SendtoTFTLN(AC_msg_nozzle_heating_done);
//        hotend_state = AC_heater_temp_reached;
//      }
//    }

//    // Update panel with bed heater status
//    if (hotbed_state != AC_heater_temp_reached) {
//      if (WITHIN(getActualTemp_celsius(BED) - getTargetTemp_celsius(BED), -0.5, 0.5)) {
//        SendtoTFTLN(AC_msg_bed_heating_done);
//        hotbed_state = AC_heater_temp_reached;
//      }
//    }
//#endif
//  }

//  void DgusTFT::SendFileList(int8_t startindex) {
//    // Respond to panel request for 4 files starting at index
//    #if ACDEBUG(AC_INFO)
//      SERIAL_ECHOLNPAIR("## SendFileList ## ", startindex);
//    #endif
//    filenavigator.getFiles(startindex);
//  }

//  void DgusTFT::SelectFile() {
//    strncpy(selectedfile, panel_command + 4, command_len - 4);
//    selectedfile[command_len - 5] = '\0';
//    #if ACDEBUG(AC_FILE)
//      SERIAL_ECHOLNPAIR_F(" Selected File: ",selectedfile);
//    #endif
//    switch (selectedfile[0]) {
//      case '/':   // Valid file selected
////        SendtoTFTLN(AC_msg_sd_file_open_success);
//        break;

//      case '<':   // .. (go up folder level)
//        filenavigator.upDIR();
////        SendtoTFTLN(AC_msg_sd_file_open_failed);
//        SendFileList( 0 );
//        break;
//      default:   // enter sub folder
//        filenavigator.changeDIR(selectedfile);
////        SendtoTFTLN(AC_msg_sd_file_open_failed);
//        SendFileList( 0 );
//        break;
//    }
//  }

//  void DgusTFT::InjectCommandandWait(PGM_P cmd) {
//    //injectCommands_P(cmnd); queue.enqueue_now_P(cmd);
//    //SERIAL_ECHOLN(PSTR("Inject>"));
//  }

//  void DgusTFT::ProcessPanelRequest() {
//    unsigned char * p_u8 ;
//    unsigned char i,j;
//    unsigned int control_index  = 0;
//    unsigned int control_value;
//    unsigned int temp;
//    char str_buf[20];

//    if(data_received) {
//    
//      data_received = 0;
//  	
//    if(0x83 == data_buf[0]) {
//  	    p_u8 =  (unsigned char *)(&control_index) ;//get control address
//  		*p_u8 = data_buf[2];
//  		p_u8++;
//  		*p_u8 = data_buf[1] ;

//#if ACDEBUG(AC_MARLIN)
//        SERIAL_ECHOLNPAIR("control_index: ", control_index);
//#endif

//        if((control_index&0xF000) == KEY_ADDRESS)// is KEY
//        {
//          key_index = control_index;
//           p_u8 =  (unsigned char *)(&key_value) ;//get key value
//          *p_u8 = data_buf[5];
//           p_u8++;
//          *p_u8 = data_buf[4];
//        }
//  	    else if(control_index==TXT_HOTEND_TARGET||control_index==TXT_ADJUST_HOTEND)//hotend target temp
//  	  	{
//            p_u8 =  (unsigned char *)(&control_value) ;//get  value
//           *p_u8 = data_buf[5];
//            p_u8++;
//           *p_u8 = data_buf[4];
//            temp=constrain((uint16_t)control_value, 0, HEATER_0_MAXTEMP);
//            setTargetTemp_celsius(temp, E0);
//  		 
//  	    }
//  	    else if(control_index==TXT_BED_TARGET||control_index==TXT_ADJUST_BED)//bed target temp
//  	  	{
//            p_u8 =  (unsigned char *)(&control_value) ;//get value
//           *p_u8 = data_buf[5];
//           p_u8++;
//           *p_u8 = data_buf[4];

//  		  temp=constrain((uint16_t)control_value, 0, BED_MAXTEMP);
//          setTargetTemp_celsius(temp, BED);
//  	    }
//  	  	else if(control_index==TXT_FAN_SPEED_TARGET)//fan speed
//  	  	{
//  		   p_u8 =  (unsigned char *)(&control_value) ;//get value
//  		  *p_u8 = data_buf[5];
//  		   p_u8++;
//  		  *p_u8 = data_buf[4];
//  		  
//  		    temp=constrain((uint16_t)control_value,0,100); 
//  		    SendValueToTFT((uint16_t)temp,TXT_FAN_SPEED_NOW);
//  			SendValueToTFT((uint16_t)temp,TXT_FAN_SPEED_TARGET);
//            setTargetFan_percent(temp, FAN0);
//  	    }
//  		else if(control_index==TXT_PRINT_SPEED_TARGET||control_index==TXT_ADJUST_SPEED)//prinf speed
//  	  	{
//  		   p_u8 =  (unsigned char *)(&control_value) ;//get  value
//  		  *p_u8 = data_buf[5];
//  		   p_u8++;
//  		  *p_u8 = data_buf[4];

//  		   float feedrate = constrain((uint16_t)control_value,40,999);
////  		   feedrate_percentage=constrain(control_value,40,999);
//  		   sprintf(str_buf,"%u",(uint16_t)feedrate);
//  	       SendTxtToTFT(str_buf, TXT_PRINT_SPEED);
//           SendValueToTFT((uint16_t)feedrate, TXT_PRINT_SPEED_NOW);
//  		   SendValueToTFT((uint16_t)feedrate, TXT_PRINT_SPEED_TARGET);
//  		   setFeedrate_percent(feedrate);
//  	    }

//  	    else if(control_index == TXT_PREHEAT_HOTEND_INPUT)
//  	    {
//            p_u8 =  (unsigned char *)(&control_value) ;//get  value
//           *p_u8 = data_buf[5];
//            p_u8++;
//           *p_u8 = data_buf[4];

//            temp=constrain((uint16_t)control_value, 0, HEATER_0_MAXTEMP);
//            setTargetTemp_celsius(temp, E0);
//  	    }

//  	    else if(control_index == TXT_PREHEAT_BED_INPUT)
//  	    {
//            p_u8 =  (unsigned char *)(&control_value) ;//get  value
//           *p_u8 = data_buf[5];
//            p_u8++;
//           *p_u8 = data_buf[4];

//            temp=constrain((uint16_t)control_value, 0, BED_MAXTEMP);
//            setTargetTemp_celsius(temp, BED);
//  	    }

//  	    else if(control_index == REG_LCD_READY)
//  	    {
//           p_u8 =  (unsigned char *)(&control_value) ;//get  value
//           *p_u8 = data_buf[5];
//           p_u8++;
//           *p_u8 = data_buf[4];
//           p_u8++;
//           *p_u8 = data_buf[3];
//           if((control_value&0x00FFFFFF) == 0x010072) { // startup last gif

//             LcdAudioSet(lcd_info.audio);

//             SendValueToTFT(2, ADDRESS_MOVE_DISTANCE);

//#if ENABLED(CASE_LIGHT_ENABLE)
//             SendValueToTFT(getCaseLightState(), ADDRESS_SYSTEM_LED_STATUS);
//             SendValueToTFT(getCaseLightState(), ADDRESS_PRINT_SETTING_LED_STATUS);
//#endif

//             if(AC_printer_resuming_from_power_outage == printer_state) {
//                char filename[64] = { '\0' };
//                ChangePageOfTFT(PAGE_OUTAGE_RECOVERY);
//                //card.getLongPath(str, recovery.info.sd_filename);
//                //   SendTxtToTFT(str, TXT_OUTAGE_RECOVERY_FILE);

//                 SendTxtToTFT( recovery.info.sd_filename, TXT_OUTAGE_RECOVERY_FILE);
//                //SERIAL_ECHOLNPAIR("info.print_progress:", recovery.info.print_progress);     //没有这一行了
//                //SendTxtToTFT(ui8tostr3rj(recovery.info.print_progress), TXT_OUTAGE_RECOVERY_PROGRESS);
//                PlayTune(BEEPER_PIN, SOS, 1);
//             } else {
//                ChangePageOfTFT(PAGE_MAIN);
//             }

//           } else if((control_value&0x00FFFFFF) == 0x010000) {  // startup first gif
//             PlayTune(BEEPER_PIN, Anycubic_PowerOn, 1);         // take 3500 ms
//           }
//  	    }

//  	    else
//  	    {

//  	    }

//       /* else if((control_index&0xF000)==0x2000)// is TXT ADDRESS
//        	{
//        	  tft_txt_index = control_index;
//        	  j=0;
//         	  for(i=4; ;i++)
//            {
//              tft_txt_buf[j] = data_buf[i];
//  			if(tft_txt_buf[j]==0xFF)
//  			{
//  			  tft_txt_buf[j]= 0 ;
//  			  break;
//  			}
//  			j++;
//         	  }
//        	}
//  		*/
//  	 }
//  	 else  if(data_buf[0]==0x82)
//  	 {
//  	 // send_cmd_to_pc(cmd ,start );
//  	 }
//    }
//  
//  
//  
//  }
//#if 0
//  {
//    // Break these up into logical blocks // as its easier to navigate than one huge switch case!
//    int8_t req = atoi(&panel_command[1]);

//    // Information requests A0 - A8 and A33
//    if (req <= 8 || req == 33) PanelInfo(req);

//    // Simple Actions A9 - A28
//    else if ( req <= 28) PanelAction(req);

//    // Process Initiation
//    else if (req <= 34) PanelProcess(req);

//    else SendtoTFTLN();
//  }
//#endif

//  void DgusTFT::page1_handle(void) {

//	static millis_t flash_time = 0;
//	char str_buf[20];

//	switch (key_value) {
//	  case 0:
//      break;

//	  case 1:   // main page, print
//	  {

//        lcd_txtbox_page = 0;

//        if(lcd_txtbox_index) {
//          SendColorToTFT(COLOR_BLUE, TXT_DISCRIBE_0+0x30*(lcd_txtbox_index-1));
//          lcd_txtbox_index = 0;
//        }

//	    ChangePageOfTFT(PAGE_FILE);
//	    SendFileList(0);
//	  }
//	  break;

//	  case 2:   // tool
//	  {
//		ChangePageOfTFT(PAGE_TOOL);

//#if ENABLED(CASE_LIGHT_ENABLE)
//		SendValueToTFT(getCaseLightState(), ADDRESS_SYSTEM_LED_STATUS);
//#endif

//	  }
//	  break;

//	  case 3:   // prepare
//       ChangePageOfTFT(PAGE_PREPARE);
//	  break;

//	  case 4:   // system
//      {
//        if(lcd_info.language == ExtUI::ENG) {
//          if(lcd_info.audio == ExtUI::ON) {
//            ChangePageOfTFT(11);    // PAGE_SYSTEM_ENG_AUDIO_ON - 120
//          } else if(lcd_info.audio == ExtUI::OFF) {
//            ChangePageOfTFT(50);   // PAGE_SYSTEM_ENG_AUDIO_OFF - 120
//          }
//        } else if(lcd_info.language == ExtUI::CHS) {
//          if(lcd_info.audio == ExtUI::ON) {
//            ChangePageOfTFT(PAGE_SYSTEM_CHS_AUDIO_ON);
//          } else if(lcd_info.audio == ExtUI::OFF) {
//            ChangePageOfTFT(PAGE_SYSTEM_CHS_AUDIO_OFF);
//          }
//        }
//      }
//	  break;
//	}

//#if 0
//	if(message_index < 30) {
//	 SendTxtToTFT(p_mesage[message_index], TXT_MAIN_MESSAGE);
//	 message_index = 30;
//	}
//#endif

//	if(millis() < (flash_time + 1500) ) {
//	  return;
//	}

//	flash_time = millis();

//	sprintf(str_buf,"%u/%u",(uint16_t)getActualTemp_celsius(E0), (uint16_t)getTargetTemp_celsius(E0));
//	SendTxtToTFT(str_buf, TXT_MAIN_HOTEND);

//	sprintf(str_buf,"%u/%u",(uint16_t)getActualTemp_celsius(BED), (uint16_t)getTargetTemp_celsius(BED));
//	SendTxtToTFT(str_buf, TXT_MAIN_BED);
//  }


//  void DgusTFT::page2_handle(void) {

//    char file_index = 0;

//	switch (key_value) {
//	case 0:
//      break;

//	case 1: // return
//	  {
//	   ChangePageOfTFT(PAGE_MAIN);
//	   SendColorToTFT(COLOR_BLUE, TXT_DISCRIBE_0+0x30*(lcd_txtbox_index-1));
//	  }
//	  break;

//	case 2: // page up
//	{
//	  if(lcd_txtbox_page > 0) {
//	    lcd_txtbox_page--;

//        SendColorToTFT(COLOR_BLUE, TXT_DISCRIBE_0+0x30*(lcd_txtbox_index-1));
//	    lcd_txtbox_index = 0;

//	    SendFileList(lcd_txtbox_page*5);
//	  }
//	  break;
//	}

//	case 3: // page down
//    {
//      if((lcd_txtbox_page+1) * 5 < filenavigator.getFileNum()) {
//        lcd_txtbox_page++;
//     	
//        SendColorToTFT(COLOR_BLUE, TXT_DISCRIBE_0+0x30*(lcd_txtbox_index-1));
//        lcd_txtbox_index = 0;

//     	SendFileList(lcd_txtbox_page*5);
//      }
//      break;
//    }

//	  case 4: // page refresh
//	  {

//	    if (!isMediaInserted()) {
//	      safe_delay(500);
//	    }

//	    filenavigator.reset();

//        lcd_txtbox_page = 0;
//        if(lcd_txtbox_index) {
//          SendColorToTFT(COLOR_BLUE, TXT_DISCRIBE_0+0x30*(lcd_txtbox_index-1));
//          lcd_txtbox_index = 0;
//        }

//	    SendFileList(lcd_txtbox_index);
//	  }
//	  break;

//	  case 5: // resume of outage(last power off)
//#if ACDEBUG(AC_MARLIN)
//	    SERIAL_ECHOLNPAIR("printer_state: ", printer_state);
//#endif
//        if(lcd_txtbox_index > 0 && lcd_txtbox_index  < 6) {   // 1~5

//          if(filenavigator.filelist.seek(lcd_txtbox_page*5+(lcd_txtbox_index-1))) {
//  
//            SendColorToTFT(COLOR_BLUE, TXT_DISCRIBE_0+0x30*(lcd_txtbox_index-1));

//#if ENABLED(CASE_LIGHT_ENABLE)
//            setCaseLightState(1);
//#endif

//            char str_buf[20];
//            strncpy_P(str_buf, filenavigator.filelist.longFilename(), 17);
//            str_buf[17] = '\0';
//            SendTxtToTFT(str_buf, TXT_PRINT_NAME);

//            if (printer_state == AC_printer_resuming_from_power_outage) {
//              // Need to home here to restore the Z position
////              injectCommands_P(AC_cmnd_power_loss_recovery);
////              SERIAL_ECHOLNPAIR("start resumeing from power outage: ", AC_cmnd_power_loss_recovery);
//              ChangePageOfTFT(PAGE_STATUS2);    // show pause
//              injectCommands_P(PSTR("M1000"));  // home and start recovery
//            }
//          }
//        }
//	    break;

//	  case 6:   // start print
//          if(lcd_txtbox_index > 0 && lcd_txtbox_index  < 6) {   // 1~5

//            if(filenavigator.filelist.seek(lcd_txtbox_page*5+(lcd_txtbox_index-1))) {

//#if 0
//              SERIAL_ECHOLNPAIR("start print: ", lcd_txtbox_page*5+(lcd_txtbox_index-1));
//              SERIAL_ECHOLNPAIR("start print: ", filenavigator.filelist.shortFilename());
//              SERIAL_ECHOLNPAIR("start print: ", filenavigator.filelist.longFilename());
//#endif

//              SendColorToTFT(COLOR_BLUE, TXT_DISCRIBE_0+0x30*(lcd_txtbox_index-1));

//              // Allows printer to restart the job if we dont want to recover
//              if (printer_state == AC_printer_resuming_from_power_outage) {
//                injectCommands_P(PSTR("M1000 C")); // Cancel recovery
//                printer_state = AC_printer_idle;
//              }

//#if ENABLED(CASE_LIGHT_ENABLE)
//              setCaseLightState(1);
//#endif

//              printFile(filenavigator.filelist.shortFilename());

//              char str_buf[20];
//              strncpy_P(str_buf, filenavigator.filelist.longFilename(), 17);
//              str_buf[17] = '\0';
//              SendTxtToTFT(str_buf, TXT_PRINT_NAME);

//              sprintf(str_buf, "%d", (uint16_t)getFeedrate_percent());
//              SendTxtToTFT(str_buf, TXT_PRINT_SPEED);

//              sprintf(str_buf, "%u", (uint16_t)getProgress_percent());
//              SendTxtToTFT(str_buf, TXT_PRINT_PROGRESS);

//              uint32_t time = 0;
//              sprintf(str_buf, "%s H ", utostr3(time/60));
//              sprintf(str_buf+strlen(str_buf), "%s M", utostr3(time%60));
//              SendTxtToTFT(str_buf, TXT_PRINT_TIME);

//              ChangePageOfTFT(PAGE_STATUS2);
//            }
//          }
//	  break;

//	  case 7:   // txtbox 1 click
//	  case 8:   // txtbox 2 click
//	  case 9:   // txtbox 3 click
//	  case 10:  // txtbox 4 click
//	  case 11:  // txtbox 5 click
//	  {
//        static uint8_t lcd_txtbox_index_last = 0;

//        lcd_txtbox_index = key_value - 6;

//        // lcd_txtbox_page 0~...
//        // lcd_txtbox_index 1~5
//	    file_index = lcd_txtbox_page*5 + (lcd_txtbox_index-1);
//	    if(file_index < filenavigator.getFileNum()) {

//          SendColorToTFT(COLOR_RED, TXT_DISCRIBE_0 + 0x30*(lcd_txtbox_index-1));

//          if(lcd_txtbox_index_last && lcd_txtbox_index_last != lcd_txtbox_index) {   // 1~5
//            SendColorToTFT(COLOR_BLUE, TXT_DISCRIBE_0 + 0x30*(lcd_txtbox_index_last-1));
//          }
//          lcd_txtbox_index_last = lcd_txtbox_index;
//	    }
//	    break;
//	  }
//    }
//  }

//    
//    void DgusTFT::page3_handle(void)
//    {
//        
//        static millis_t flash_time = 0;
//        char str_buf[20];

//        static uint8_t progress_last = 0;
//        static uint16_t feedrate_last = 0;

//        switch (key_value)
//        {
//          case 0:

//          break;

//          case 1: // return
//          {
//            if(isPrintingFromMedia() == false) {  //only is idle status can return 
//              ChangePageOfTFT(PAGE_FILE);
//            }
//          }
//          break;

//          case 2: // resume print
//#if ACDEBUG(AC_MARLIN)
//            SERIAL_ECHOLNPAIR("printer_state: ", printer_state);
//            SERIAL_ECHOLNPAIR("pause_state: ", pause_state);
//#endif
//            if(pause_state == AC_paused_idle || pause_state == AC_paused_filament_lack ||
//               printer_state == AC_printer_resuming_from_power_outage) {

//              printer_state = AC_printer_idle;
//              pause_state = AC_paused_idle;
//              resumePrint();
//              ChangePageOfTFT(PAGE_STATUS2);    // show pasue print
//              flash_time = millis();

//            } else {
//              setUserConfirmed();
//            }
//            break;

//          case 3: // print stop
//            if(isPrintingFromMedia()) {    
//              ChangePageOfTFT(PAGE_STOP_CONF);
//            }
//             break;

//          case 4: // print change param
//            ChangePageOfTFT(PAGE_ADJUST);
//#if ENABLED(CASE_LIGHT_ENABLE)
//            SendValueToTFT(getCaseLightState(), ADDRESS_PRINT_SETTING_LED_STATUS);
//#endif
//            SendValueToTFT((uint16_t)getTargetTemp_celsius(E0), TXT_ADJUST_HOTEND);
//            SendValueToTFT((uint16_t)getTargetTemp_celsius(BED), TXT_ADJUST_BED);
//            feedrate_last = (uint16_t)getFeedrate_percent();
//            SendValueToTFT(feedrate_last, TXT_ADJUST_SPEED);
//            flash_time = millis();
//            break;
//        }

//        if(millis() < (flash_time +1500) ) {
//          return;
//        }
//        flash_time=millis();

//        if(feedrate_last != (uint16_t)getFeedrate_percent()) {
//          feedrate_last = (uint16_t)getFeedrate_percent();
//          sprintf(str_buf, "%d", feedrate_last);
//          SendTxtToTFT(str_buf, TXT_PRINT_SPEED);
//        }

//#if ACDEBUG(AC_MARLIN)
//          SERIAL_ECHOLNPAIR("print speed: ", str_buf);
//          SERIAL_ECHOLNPAIR("feedrate_back: ", feedrate_back);
//#endif

//        if(progress_last != getProgress_percent()) {
//          sprintf(str_buf, "%u", getProgress_percent());
//          SendTxtToTFT(str_buf, TXT_PRINT_PROGRESS);
//          progress_last = getProgress_percent();
//        }

//        // Get Printing Time
//        uint32_t time = getProgress_seconds_elapsed() / 60;
//        sprintf(str_buf, "%s H ", utostr3(time/60));
//        sprintf(str_buf+strlen(str_buf), "%s M", utostr3(time%60));
//        SendTxtToTFT(str_buf, TXT_PRINT_TIME);

//        sprintf(str_buf,"%u/%u",(uint16_t)getActualTemp_celsius(E0),(uint16_t)getTargetTemp_celsius(E0));
//        SendTxtToTFT(str_buf,TXT_PRINT_HOTEND );

//        sprintf(str_buf,"%u/%u",(uint16_t)getActualTemp_celsius(BED),(uint16_t)getTargetTemp_celsius(BED));
//        SendTxtToTFT(str_buf,TXT_PRINT_BED);
//    }

//    void DgusTFT::page4_handle(void)
//    {
//        
//        static millis_t flash_time = 0;
//        char str_buf[20];
//        static uint8_t progress_last = 0;
//        static uint16_t feedrate_last = 0;
//        unsigned int temp;
//    
//        switch (key_value)
//        {
//        case 0:
//          break;

//        case 1: // return
//        {
//          if(isPrintingFromMedia() == false) {
//  //            if(card.sdprinting==false)//only is idle status can return
//              ChangePageOfTFT(PAGE_FILE);
//          }
//        }
//        break;

//        case 2:  // print pause
//          if(isPrintingFromMedia()) {
//            pausePrint();
//            printer_state = AC_printer_pausing;
//            pause_state = AC_paused_idle;
//            ChangePageOfTFT(PAGE_WAIT_PAUSE);
////            injectCommands_P(PSTR("M108"));     // stop waiting temperature M109
//          }
//          break;

//        case 3: // print stop
//          if(isPrintingFromMedia()) {    
//              ChangePageOfTFT(PAGE_STOP_CONF);
//          }
//          break;

//        case 4: // print settings
//          ChangePageOfTFT(PAGE_ADJUST);
//#if ENABLED(CASE_LIGHT_ENABLE)
//          SendValueToTFT(getCaseLightState(), ADDRESS_PRINT_SETTING_LED_STATUS);
//#endif
//          SendValueToTFT((uint16_t)getTargetTemp_celsius(E0), TXT_ADJUST_HOTEND);
//          SendValueToTFT((uint16_t)getTargetTemp_celsius(BED), TXT_ADJUST_BED);
//          feedrate_last = (uint16_t)getFeedrate_percent();
//          SendValueToTFT(feedrate_last, TXT_ADJUST_SPEED);
//          SendValueToTFT((uint16_t)getActualFan_percent(FAN0), TXT_FAN_SPEED_TARGET);
//          SendTxtToTFT(ftostr(getZOffset_mm()), TXT_LEVEL_OFFSET);

//          break;
//        }

//        if(millis() < (flash_time +1500)) {
//          return;
//        }
//        flash_time=millis();

//        if(feedrate_last != (uint16_t)getFeedrate_percent()) {
//          feedrate_last = (uint16_t)getFeedrate_percent();
//          sprintf(str_buf, "%d", feedrate_last);
//          SendTxtToTFT(str_buf, TXT_PRINT_SPEED);
//        }

//        if(progress_last != getProgress_percent())
//        {
//          sprintf(str_buf, "%d", getProgress_percent());
//          SendTxtToTFT(str_buf, TXT_PRINT_PROGRESS);
//          progress_last = getProgress_percent();
//        }

//        uint32_t time = getProgress_seconds_elapsed() / 60;
//        sprintf(str_buf, "%s H ", utostr3(time/60));
//        sprintf(str_buf+strlen(str_buf), "%s M", utostr3(time%60));
//        SendTxtToTFT(str_buf, TXT_PRINT_TIME);

//        sprintf(str_buf,"%u/%u",(uint16_t)getActualTemp_celsius(E0), (uint16_t)getTargetTemp_celsius(E0));
//        SendTxtToTFT(str_buf, TXT_PRINT_HOTEND);
//   
//        sprintf(str_buf,"%u/%u",(uint16_t)getActualTemp_celsius(BED), (uint16_t)getTargetTemp_celsius(BED));
//        SendTxtToTFT(str_buf, TXT_PRINT_BED);
//    }

//    void DgusTFT::page5_handle(void)          // print settings
//    {
//      char str_buf[10];
//      float z_off;
//      static bool z_change = false;

//      switch (key_value) {
//        case 0:
//      break;

//      case 1: // return
//        if(AC_printer_printing == printer_state) {
//          ChangePageOfTFT(PAGE_STATUS2);  // show pause
//        } else if(AC_printer_paused == printer_state) {
//          ChangePageOfTFT(PAGE_STATUS1);  // show print
//        }
//      break;

//      case 2: // -
//      {
//        float z_off = getZOffset_mm();
////        SERIAL_ECHOLNPAIR("z_off: ", z_off);
////        setSoftEndstopState(false);
//        z_off -= 0.05f;
//        setZOffset_mm(z_off);

//        str_buf[0]=0;
//        strcat(str_buf, ftostr(getZOffset_mm()));
//        SendTxtToTFT(str_buf, TXT_LEVEL_OFFSET);
//  
////        if (isAxisPositionKnown(Z)) {  // Move Z axis
////          SERIAL_ECHOLNPAIR("Z now:", getAxisPosition_mm(Z));
////          const float currZpos = getAxisPosition_mm(Z);
////          setAxisPosition_mm(currZpos-0.05, Z);
////          SERIAL_ECHOLNPAIR("Z now:", getAxisPosition_mm(Z));
////        }

//        int16_t steps = mmToWholeSteps(-0.05, Z);
//        babystepAxis_steps(steps, Z);

//        GRID_LOOP(x, y) {
//          const xy_uint8_t pos { x, y };
//          const float currval = getMeshPoint(pos);
//#if ACDEBUG(AC_MARLIN)
//          SERIAL_ECHOLNPAIR("x: ", x, " y: ", y, " z: ", currval);
//#endif
//          setMeshPoint(pos, constrain(currval - 0.05f, AC_LOWEST_MESHPOINT_VAL, 5));
//        }

//        z_change = true;

////        setSoftEndstopState(true);
//      }
//      break;

//      case 3: // +
//      {
//        float z_off = getZOffset_mm();
////        SERIAL_ECHOLNPAIR("z_off: ", z_off);
////        setSoftEndstopState(false);
//        z_off += 0.05f;
//        setZOffset_mm(z_off);
//  
//        str_buf[0]=0;
//        strcat(str_buf, ftostr(getZOffset_mm()));
//        SendTxtToTFT(str_buf, TXT_LEVEL_OFFSET);
//  
//  //          int16_t steps = mmToWholeSteps(constrain(Zshift,-0.05,0.05), Z);

///*
//        if (isAxisPositionKnown(Z)) {  // Move Z axis
//          SERIAL_ECHOLNPAIR("Z now:", getAxisPosition_mm(Z));
//          const float currZpos = getAxisPosition_mm(Z);
//          setAxisPosition_mm(currZpos-0.05, Z);
//          SERIAL_ECHOLNPAIR("Z now:", getAxisPosition_mm(Z));
//        }
//*/

//        int16_t steps = mmToWholeSteps(0.05, Z);
//        babystepAxis_steps(steps, Z);

//        GRID_LOOP(x, y) {
//          const xy_uint8_t pos { x, y };
//          const float currval = getMeshPoint(pos);
////          SERIAL_ECHOLNPAIR("x: ", x, " y: ", y, " z: ", currval);
//          setMeshPoint(pos, constrain(currval + 0.05f, AC_LOWEST_MESHPOINT_VAL, 5));
//        }

//        z_change = true;

////        setSoftEndstopState(true);
//      }
//        break;

//        case 4: // light control
//#if ENABLED(CASE_LIGHT_ENABLE)
//          if(getCaseLightState()) {
//            SendValueToTFT(0, ADDRESS_PRINT_SETTING_LED_STATUS);
//            setCaseLightState(0);
//          } else {
//            SendValueToTFT(1, ADDRESS_PRINT_SETTING_LED_STATUS);
//            setCaseLightState(1);
//          }
//#endif
//          break;

//        case 5:
//          ChangePageOfTFT(PAGE_DONE);
//          break;

//        case 6:
//  
//          break;

//        case 7:
//          RequestValueFromTFT(TXT_ADJUST_BED);
//          RequestValueFromTFT(TXT_ADJUST_SPEED);
//          RequestValueFromTFT(TXT_ADJUST_HOTEND);
//          RequestValueFromTFT(TXT_FAN_SPEED_TARGET);

//          if(z_change == true) {
//            injectCommands_P(PSTR("M500"));
//            z_change = false;
//          }

//          if(AC_printer_printing == printer_state) {
//            ChangePageOfTFT(PAGE_STATUS2);  // show pause
//          } else if(AC_printer_paused == printer_state) {
//            ChangePageOfTFT(PAGE_STATUS1);  // show print
//          }

//          break; 
//      }
//    }

//    void DgusTFT::page6_handle(void)
//    {
//        switch (key_value)
//        {
//          case 0:
//          break;

//          case 1:
//          break;
//        }
//    }

//    void DgusTFT::page7_handle(void) // tools
//    {
//     unsigned char str_buf[20];
//     unsigned int temp;
//        switch (key_value)
//        {
//        
//          case 0:
//          break;

//          case 1:   // return
//          {
//           ChangePageOfTFT(PAGE_MAIN);
//          }
//          break;

//          case 2:
//          {
//             ChangePageOfTFT(PAGE_MOVE);
//          }
//          break;
//          
//          case 3:   // set temperature
//          ChangePageOfTFT(PAGE_TEMP);
//          SendValueToTFT((uint16_t)getActualTemp_celsius(E0), TXT_HOTNED_NOW);
//          SendValueToTFT((uint16_t)getTargetTemp_celsius(E0), TXT_HOTEND_TARGET);
//          SendValueToTFT((uint16_t)getActualTemp_celsius(BED), TXT_BED_NOW);
//          SendValueToTFT((uint16_t)getTargetTemp_celsius(BED), TXT_BED_TARGET);

//          break;
//          
//          case 4:
//           ChangePageOfTFT(PAGE_SPEED);
//           SendValueToTFT((uint16_t)getActualFan_percent(FAN0), TXT_FAN_SPEED_NOW);
//           SendValueToTFT((uint16_t)getTargetFan_percent(FAN0), TXT_FAN_SPEED_TARGET);
//           SendValueToTFT((uint16_t)getFeedrate_percent(), TXT_PRINT_SPEED_NOW);
//           SendValueToTFT((uint16_t)getFeedrate_percent(), TXT_PRINT_SPEED_TARGET);
//          break;

//          case 5:   // turn off the xyz motor
//            if(!isMoving()) {
//                  stepper.disable_all_steppers();
//            }
//          break;

//          case 6:   // light control
//#if ENABLED(CASE_LIGHT_ENABLE)
//            if(getCaseLightState()) {
//              setCaseLightState(0);
//              SendValueToTFT(0, ADDRESS_SYSTEM_LED_STATUS);
//            } else {
//              setCaseLightState(1);
//              SendValueToTFT(1, ADDRESS_SYSTEM_LED_STATUS);
//            }
//#endif
//          break;
//        }
//    }

//    void DgusTFT::page8_handle(void)
//    {
//     static uint16_t movespeed = 50;
//     static float move_dis = 1.0f;
//     unsigned char value[30];

//     if(key_value == 2 || key_value == 4 ||
//        key_value == 6 || key_value == 8 ||
//        key_value == 10 || key_value == 12 && !isMoving()) {

//        if(getAxisPosition_mm(Z) < 0) {
//            setAxisPosition_mm(0, Z, 8);
//        }
//     }

//      value[0]=0;
//     //  if(!planner.movesplanned())return;
//        switch (key_value) {
//          case 0:
//            break;
//          
//          case 1://return
//            ChangePageOfTFT(PAGE_TOOL);
//            break;

//          case 5:
//            if(!isMoving()) {
//              injectCommands_P(PSTR("G28 X"));
//            }
//            break;

//          case 9:
//            if(!isMoving()) {
//              injectCommands_P(PSTR("G28 Y"));
//            }
//            break;

//          case 13:
//            if(!isMoving()) {
//              injectCommands_P(PSTR("G28 Z"));
//            }
//            break;

//          case 17:
//            if(!isMoving()) {
//              injectCommands_P(PSTR("G28"));
//            }
//            break;

//          case 2:   // X-
//            if(!isMoving()) {
//              setAxisPosition_mm(getAxisPosition_mm(X) - move_dis, X, 50);
//            }
//          break;

//          case 4:   // X+
//            if(!isMoving()) {
//              setAxisPosition_mm(getAxisPosition_mm(X) + move_dis, X, 50);
//            }
//            break;

//          case 6:   // Y+
//            if(!isMoving()) {
//              setAxisPosition_mm(getAxisPosition_mm(Y) + move_dis, Y, 50);
//            }
//            break;

//          case 8:   // Y-
//            if(!isMoving()) {
//              setAxisPosition_mm(getAxisPosition_mm(Y) - move_dis, Y, 50);
//            }
//            break;

//          case 10:  // Z-
//            if(!isMoving()) {
//              setAxisPosition_mm(getAxisPosition_mm(Z) - move_dis, Z, 8);
//            }
//            break;

//          case 12:  // Z+
//            if(!isMoving()) {
//              setAxisPosition_mm(getAxisPosition_mm(Z) + move_dis, Z, 8);
//            }
//            break;

//          case 3:
//            move_dis = 0.1f ;
//            SendValueToTFT(1, ADDRESS_MOVE_DISTANCE);
//            break;

//          case 7:
//            move_dis = 1.0f ;
//            SendValueToTFT(2, ADDRESS_MOVE_DISTANCE);
//            break;

//          case 11:
//            move_dis = 10.0f ;
//            SendValueToTFT(3, ADDRESS_MOVE_DISTANCE);
//            break;

//          case 14:
//            movespeed = 3000;//SERIAL_PROTOCOLLN("3000");
//            break;

//          case 15:
//            movespeed = 2000;//SERIAL_PROTOCOLLN("2000");
//            break;

//          case 16:
//            movespeed = 1000;//SERIAL_PROTOCOLLN("1000");
//            break;
//         }
//    }

//    void DgusTFT::page9_handle(void)
//    {
//       static millis_t flash_time = 0;
//     
//        switch (key_value)
//        {
//         case 0:
//         break;
//         
//          case 1://return
//          {
//            ChangePageOfTFT(PAGE_TOOL);
//          }
//          break;
//          case 2:
//          {
//            
//          }
//          break;
//          case 3:
//    
//          break;
//          case 4:
//          break;
//          
//          case 5:
//          break;

//          case 6: // cooling
//            setTargetTemp_celsius(0, E0); 
//            setTargetTemp_celsius(0, BED);
//            ChangePageOfTFT(PAGE_TOOL);
//          break;

//          case 7: // send target temp
//            RequestValueFromTFT(TXT_HOTEND_TARGET);
//            RequestValueFromTFT(TXT_BED_TARGET);
//            ChangePageOfTFT(PAGE_TOOL);
//          break;
//    }

//        if(millis() < (flash_time +1500) ) {
//          return;
//        }
//        flash_time=millis();
//    
//        SendValueToTFT( (uint16_t)getActualTemp_celsius(E0), TXT_HOTNED_NOW);
//        SendValueToTFT( (uint16_t)getActualTemp_celsius(BED), TXT_BED_NOW);
//    }
//    
//    void DgusTFT::page10_handle(void)
//    {
//        static millis_t flash_time = 0;
//      uint16_t temp;
//        switch (key_value)
//        {
//                case 0:
//         break;
//          case 1:   // return
//          {
//              ChangePageOfTFT(PAGE_TOOL);
//          }
//          break;
//          
//          case 2:
//          {

//          }
//          break;
//          
//          case 3:
//          break;
//          
//          case 4:
//          break;
//          
//          case 5:
//          break;

//          case 6:   // ok
//            RequestValueFromTFT(TXT_FAN_SPEED_TARGET);
//            RequestValueFromTFT(TXT_PRINT_SPEED_TARGET);
//            ChangePageOfTFT(PAGE_TOOL);
//          break;
//    }
//    
//      if(millis() < (flash_time +1500)) {
//        return;
//      }
//      flash_time=millis();
//  
//       SendValueToTFT((uint16_t)getActualFan_percent(FAN0), TXT_FAN_SPEED_NOW);
//       SendValueToTFT((uint16_t)getFeedrate_percent(), TXT_PRINT_SPEED_NOW);
//    }

//    void DgusTFT::page11_handle(void)
//    {
//        switch (key_value)
//        {
//          case 0:
//          break;

//          case 1:   // return 
//          {
//            ChangePageOfTFT(PAGE_MAIN);
//            if(lcd_info_back.language != lcd_info.language ||
//               lcd_info_back.audio    != lcd_info.audio) {

//               lcd_info_back.language = lcd_info.language;
//               lcd_info_back.audio    = lcd_info.audio;

//               injectCommands_P(PSTR("M500"));
//            }
//          }
//          break;

//          case 2:   // language
//          {
//            if(lcd_info.language == ExtUI::CHS) {
//                lcd_info.language = ExtUI::ENG;
//                if(lcd_info.audio == ExtUI::ON) {
//                    ChangePageOfTFT(11);  // PAGE_SYSTEM_ENG_AUDIO_ON-120
//                } else if(lcd_info.audio == ExtUI::OFF) {
//                    ChangePageOfTFT(50);  // PAGE_SYSTEM_ENG_AUDIO_OFF-120
//                }
//            } else if(lcd_info.language == ExtUI::ENG) {
//                lcd_info.language = ExtUI::CHS;
//                if(lcd_info.audio == ExtUI::ON) {
//                    ChangePageOfTFT(PAGE_SYSTEM_CHS_AUDIO_ON);
//                } else if(lcd_info.audio == ExtUI::OFF) {
//                    ChangePageOfTFT(PAGE_SYSTEM_CHS_AUDIO_OFF);
//                }
//            }
//          }
//          break;

//          case 3:
//          break;

//          case 4:   // audio
//          {
//            if(lcd_info.audio == ExtUI::ON) {
//                lcd_info.audio = ExtUI::OFF;
//                if(lcd_info.language == ExtUI::CHS) {
//                  ChangePageOfTFT(PAGE_SYSTEM_CHS_AUDIO_OFF);
//                } else if(lcd_info.language == ExtUI::ENG) {
//                  ChangePageOfTFT(50);    // PAGE_SYSTEM_ENG_AUDIO_OFF - 120
//                }
//            } else if(lcd_info.audio == ExtUI::OFF) {
//                lcd_info.audio = ExtUI::ON;
//                if(lcd_info.language == ExtUI::CHS) {
//                  ChangePageOfTFT(PAGE_SYSTEM_CHS_AUDIO_ON);
//                } else if(lcd_info.language == ExtUI::ENG) {
//                  ChangePageOfTFT(11);    // PAGE_SYSTEM_ENG_AUDIO_ON - 120
//                }
//            }

//            LcdAudioSet(lcd_info.audio);
//          }
//          break;

//          case 5:   // about
//            char str_ver[32];
////            sprintf(str_ver, "%04d-%02d-%02d %02d:%02d:%02d\n", BUILD_YEAR, BUILD_MONTH, BUILD_DAY, BUILD_HOUR, BUILD_MIN, BUILD_SEC);
//            sprintf(str_ver, MAIN_BOARD_FIRMWARE_VER);
//            SendTxtToTFT(str_ver, TXT_VERSION);
//            ChangePageOfTFT(PAGE_ABOUT);
//            break;

//          case 6:
//            ChangePageOfTFT(PAGE_RECORD);
//            break;
//        }
//    }
//    
//    void DgusTFT::page12_handle(void)
//    {
//        switch (key_value)
//            {
//                    case 0:
//             break;
//              case 1://return
//              {
//               ChangePageOfTFT(PAGE_SYSTEM_CHS_AUDIO_ON);
//              }
//              break;
//        
//            }
//    
//    }

//    void DgusTFT::page13_handle(void)
//    {
//        switch (key_value)
//        {
//          case 0:
//          break;

//          case 1://return
//          {
//              if(lcd_info.language == ExtUI::ENG) {
//                if(lcd_info.audio == ExtUI::ON) {
//                  ChangePageOfTFT(11);    // PAGE_SYSTEM_ENG_AUDIO_ON - 120
//                } else if(lcd_info.audio == ExtUI::OFF) {
//                  ChangePageOfTFT(50);   // PAGE_SYSTEM_ENG_AUDIO_OFF - 120
//                }
//              } else if(lcd_info.language == ExtUI::CHS) {
//                if(lcd_info.audio == ExtUI::ON) {
//                  ChangePageOfTFT(PAGE_SYSTEM_CHS_AUDIO_ON);
//                } else if(lcd_info.audio == ExtUI::OFF) {
//                  ChangePageOfTFT(PAGE_SYSTEM_CHS_AUDIO_OFF);
//                }
//              }
//          }
//          break;

//          case 2:
//          {
//            
//          }
//          break;
//        }
//    }

//    void DgusTFT::page14_handle(void)
//    {
//        switch (key_value)
//            {
//                    case 0:
//             break;
//              case 1://return
//              {

//              }
//              break;
//              case 2:
//              {
//                
//              }
//              break;

//              case 3:
//        
//              break;

//              case 4:
//        
//              break;
//            }
//    
//    
//    }
//    
//        
//        void DgusTFT::page15_handle(void) 
//        {
//            char str_buf[16];

//            switch (key_value)
//            {
//              case 0:
//              break;

//              case 1://return
//              {
//                ChangePageOfTFT(PAGE_MAIN);

//              }
//              break;

//              case 2:
//              {
//                ChangePageOfTFT(PAGE_PreLEVEL);
//              }
//              break;

//              case 3:
//                ChangePageOfTFT(PAGE_PREHEAT);
//                sprintf(str_buf,"%u/%u",(uint16_t)getActualTemp_celsius(E0), (uint16_t)getTargetTemp_celsius(E0));
//                SendTxtToTFT(str_buf, TXT_PREHEAT_HOTEND);
//                sprintf(str_buf,"%u/%u",(uint16_t)getActualTemp_celsius(BED), (uint16_t)getTargetTemp_celsius(BED));
//                SendTxtToTFT(str_buf, TXT_PREHEAT_BED);
//              break;

//              case 4:
//              {
//                str_buf[0] = 0;
//                sprintf(str_buf,"%u/%u", (uint16_t)getActualTemp_celsius(E0), (uint16_t)getTargetTemp_celsius(E0));
//                SendTxtToTFT(str_buf, TXT_FILAMENT_TEMP);
//                ChangePageOfTFT(PAGE_FILAMENT);
//              }
//              break;
//            }
//        }

//        void DgusTFT::page16_handle(void)//AUTO LEVELING
//        {
//            switch (key_value)
//                {
//                        case 0:
//             break;
//                  case 1://return
//                  {
//                   ChangePageOfTFT(PAGE_PREPARE);
//                  }
//                  break;

//                  case 2:
//                    if(!isPrinting())
//                    {
////                      setAxisPosition_mm(10.0, Z, 5);
//                      ChangePageOfTFT(PAGE_LEVEL_ENSURE);
//                    }
//                  break;

//                  case 3:
//                  char str_buf[10];
//                  str_buf[0]=0;
//                  strcat(str_buf, ftostr(getZOffset_mm()));
//                  SendTxtToTFT(str_buf, TXT_LEVEL_OFFSET);
//                  ChangePageOfTFT(PAGE_LEVEL_ADVANCE);
//                  break;        
//    
//                  case 4:
//                   ChangePageOfTFT(PAGE_AUTO_OFFSET);
//                  break;
//                  
//                }
//        }

//        void DgusTFT::page17_handle(void)
//        {
//            char str_buf[10];
//            float z_off;

//            switch (key_value)
//            {
//              case 0:

//              break;

//              case 1://return
//              {
//                ChangePageOfTFT(PAGE_PreLEVEL);
//              }
//              break;

//              case 2:
//              {
//                setSoftEndstopState(false);
//                z_off = getZOffset_mm() - 0.0500f;
//                setZOffset_mm(z_off);

//                strcpy_P(str_buf, ftostr(z_off));
//                SendTxtToTFT(str_buf, TXT_LEVEL_OFFSET);

//                if (isAxisPositionKnown(Z)) {
//                  const float currZpos = getAxisPosition_mm(Z);
//                  setAxisPosition_mm(currZpos-0.05, Z);
//                }

//                setSoftEndstopState(true);
//              }
//              break;

//              case 3:
//              {
//                setSoftEndstopState(false);
//                z_off = getZOffset_mm() + 0.0500f;
//                setZOffset_mm(z_off);

//                strcpy_P(str_buf, ftostr(z_off));
//                SendTxtToTFT(str_buf, TXT_LEVEL_OFFSET);

//                if (isAxisPositionKnown(Z)) {  // Move Z axis
//                  const float currZpos = getAxisPosition_mm(Z);
//                  setAxisPosition_mm(currZpos+0.05, Z);
//                }

//                setSoftEndstopState(true);
//              }     
//              break;

//              case 4:

//#if ACDEBUG(AC_MARLIN)
//                SERIAL_ECHOLNPAIR("z off: ", ftostr(getZOffset_mm()));
//#endif
//                GRID_LOOP(x, y) {
//                  const xy_uint8_t pos { x, y };
//                  const float currval = getMeshPoint(pos);
//                  setMeshPoint(pos, constrain(currval + getZOffset_mm(), AC_LOWEST_MESHPOINT_VAL, 5));
//                }

//                injectCommands_P(PSTR("M500"));
//                ChangePageOfTFT(PAGE_PREPARE);
//              break;
//            }
//        }

//        void DgusTFT::page18_handle(void) //preheat
//        {
//            static millis_t flash_time = 0;
//            char str_buf[16];

//            switch (key_value)
//            {
//            case 0:
//                break;

//              case 1: // return
//              {
//                ChangePageOfTFT(PAGE_PREPARE);

//              }
//              break;

//              case 2: // PLA
//              { 
//                setTargetTemp_celsius(190, E0);
//                setTargetTemp_celsius(60, BED);
//                ChangePageOfTFT(PAGE_PREHEAT);
//              }
//              break;

//              case 3: // ABS
//              { 
//                setTargetTemp_celsius(240, E0);
//                setTargetTemp_celsius(100, BED);
//                ChangePageOfTFT(PAGE_PREHEAT);
//              }

//            }

//            if(millis() < (flash_time +1500) ) {
//              return;
//            }
//            flash_time=millis();

//            sprintf(str_buf,"%u/%u",(uint16_t)getActualTemp_celsius(E0), (uint16_t)getTargetTemp_celsius(E0));
//            SendTxtToTFT(str_buf, TXT_PREHEAT_HOTEND);
//            sprintf(str_buf,"%u/%u",(uint16_t)getActualTemp_celsius(BED), (uint16_t)getTargetTemp_celsius(BED));
//            SendTxtToTFT(str_buf, TXT_PREHEAT_BED);
//        }

//        void DgusTFT::page19_handle(void)   // Filament
//        {
//            char str_buf[20];
//            static char filament_status = 0;
//            static millis_t flash_time = 0;
//            switch (key_value)
//            {
//              case 0:
//              break;

//              case 1:   // return
//              {
//                filament_status = 0;
//                injectCommands_P(PSTR("G90"));
//                ChangePageOfTFT(PAGE_PREPARE);
//              }
//              break;

//              case 2:   // Filament in
//              if(getActualTemp_celsius(E0)<220)
//              {
//                filament_status = 0;
//                ChangePageOfTFT(PAGE_FILAMENT_HEAT);
//              }
//              else
//              {
//               filament_status = 1;
//               injectCommands_P(PSTR("G91")); 
//              }
//              break;

//              case 3:   // filament out
//              if(getActualTemp_celsius(E0)<220)
//              {
//                filament_status = 0;
//                ChangePageOfTFT(PAGE_FILAMENT_HEAT);
//              }
//              else
//              {
//                filament_status = 2;
//                injectCommands_P(PSTR("G91")); 
//              }
//              break;

//              case 4:   // stop
//                filament_status = 0;
//              break;
//              
//            }

//            if(millis() < (flash_time +1000) ) {
//              return;
//            }
//            flash_time=millis();

//            sprintf(str_buf,"%u/%u",(uint16_t)getActualTemp_celsius(E0), (uint16_t)getTargetTemp_celsius(E0));
//            SendTxtToTFT(str_buf, TXT_FILAMENT_TEMP);

//            if(!isPrinting()) {
//              char value[20];
//              if(filament_status==1) {
//                if(canMove(E0) && !commandsInQueue()) {
//                  injectCommands_P(AC_cmnd_manual_load_filament);
//                }
//              } else if(filament_status==2) {
//                if(canMove(E0) && !commandsInQueue()) {
//                  injectCommands_P(AC_cmnd_manual_unload_filament);
//                }
//              }
//            }
//        }

//        void DgusTFT::page20_handle(void)   // confirm
//        {
//            static millis_t flash_time = 0;
//            switch (key_value)
//            {
//              case 0:
//              break;
//              case 1://return
//              {
//                ChangePageOfTFT(page_index_last);
//              }
//              break;

//            }
//            if(millis() < (flash_time +1000) )return;
//            flash_time=millis();
//        
//        }

//        void DgusTFT::page21_handle(void)
//        {
//            static millis_t flash_time = 0;
//            switch (key_value)
//            {
//              case 0:
//              break;

//              case 1://return
//              {
//                ChangePageOfTFT(page_index_last);
//              }
//              break;

//              case 2:
//              {
//              }
//              break;
//            }

//            if(millis() < (flash_time +1000) )return;
//             flash_time=millis();
//        }
//    
//        void DgusTFT::page22_handle(void)   // print finish
//        {
//            static millis_t flash_time = 0;
//            switch (key_value)
//            {
//              case 0:
//              break;

//              case 1:  // OK to finish
//              {
//#if ENABLED(CASE_LIGHT_ENABLE)
//                setCaseLightState(0);
//#endif
//                ChangePageOfTFT(PAGE_MAIN);
//              }
//              break;

//              case 2:
//              {
//              }
//              break;
//            }
//                if(millis() < (flash_time +1000) )return;
//                flash_time=millis();
//            }

//            void DgusTFT::page23_handle(void)
//            {
//                static millis_t flash_time = 0;
//                switch (key_value)
//                {
//                  case 0:
//                  break;

//                  case 1:
//                  {
//                   ChangePageOfTFT(page_index_last);
//                  }
//                  break;

//                  case 2:
//                  {
//                    ChangePageOfTFT(page_index_last);
//                  }
//                  break;
//                }

//             if(millis() < (flash_time +1000) )return;
//               flash_time=millis();
//            
//            }
//    
//            
//            void DgusTFT::page24_handle(void)
//            {static millis_t flash_time = 0;
//                switch (key_value)
//                    {
//            
//                      case 0:
//                      break;
//                      case 1://return
//                      {
//                       ChangePageOfTFT(page_index_last);
//                      }
//                      break;
//                      case 2:
//                      {
//                         ChangePageOfTFT(page_index_last);
//                      }
//                      break;
//            
//                    }
//                    if(millis() < (flash_time +1000) )return;
//                  flash_time=millis();
//            }

//            void DgusTFT::page25_handle(void)   // lack filament
//            {
//               static millis_t flash_time = 0;
//                switch (key_value)
//                {
//                  case 0:
//                  break;

//                  case 1: // return
//                  {
//#if ACDEBUG(AC_MARLIN)
//                    SERIAL_ECHOLNPAIR("printer_state: ", printer_state);
//                    SERIAL_ECHOLNPAIR("pause_state: ", pause_state);
//#endif
//                    if(AC_printer_printing == printer_state) {
//                      ChangePageOfTFT(PAGE_STATUS2);  // show pause
//                    } else if(AC_printer_paused == printer_state) {
////                      injectCommands_P(PSTR("M108"));
//                      ChangePageOfTFT(PAGE_STATUS1);  // show resume
//                    }
//                  }
//                  break;
//                }
//                if(millis() < (flash_time +1000) )return;
//                flash_time=millis();
//            }

//            void DgusTFT::page26_handle(void)
//            {
//            static millis_t flash_time = 0;
//                switch (key_value)
//                    {
//            
//                      case 0:
//                      break;
//                      case 1://return
//                      {
//                         ChangePageOfTFT(page_index_last);
//            
//                      }
//                      break;
//                      case 2:
//                      {
//                        
//                      }
//                      break;
//            
//                    }
//    
//    
//              if(millis() < (flash_time +1000) )return;
//              flash_time=millis();
//            
//            }
//    
//    
//        void DgusTFT::page27_handle(void)
//        {
//        static millis_t flash_time = 0;
//            switch (key_value)
//            {
//    
//              case 0:
//              break;

//              case 1:   // print stop confirmed
//              {
//                if (isPrintingFromMedia()) {
//                  printer_state = AC_printer_stopping;
//                  stopPrint();
//                  message_index = 6;
//                  ChangePageOfTFT(PAGE_MAIN);
//                } else {
//                  if (printer_state == AC_printer_resuming_from_power_outage)
//                    injectCommands_P(PSTR("M1000 C")); // Cancel recovery
//                  printer_state = AC_printer_idle;
//                }
//              }
//              break;

//              case 2:   // return
//              {
//                if(AC_printer_printing == printer_state) {
//                  ChangePageOfTFT(PAGE_STATUS2);  // show pause
//                } else if(AC_printer_paused == printer_state) {
//                  ChangePageOfTFT(PAGE_STATUS1);  // show print
//                }
//              }
//            }

//          if(millis() < (flash_time +1000) )return;
//          flash_time=millis();
//        }
//    
//        void DgusTFT::page28_handle(void)
//        {
//        static millis_t flash_time = 0;
//            switch (key_value)
//                {
//        
//                  case 0:
//                  break;
//                  case 1://return
//                  {
//                     ChangePageOfTFT(page_index_last);
//        
//                  }
//                  break;
//                  case 2:
//                  {
//                    
//                  }
//                  break;
//        
//                }
//    
//    
//          if(millis() < (flash_time +1000) )return;
//          flash_time=millis();
//        
//        }
//    
//        void DgusTFT::page29_handle(void)
//        {
//        static millis_t flash_time = 0;
//            switch (key_value)
//            {
//              case 0:
//              break;

//              case 1://return
//              {
//#if ENABLED(CASE_LIGHT_ENABLE)
//                setCaseLightState(0);
//#endif
//                ChangePageOfTFT(PAGE_MAIN);
//              }
//              break;

//              case 2:
//              {
//                
//              }
//              break;
//    
//            }

//          if(millis() < (flash_time +1000) )return;
//          flash_time=millis();
//        
//        }

//        void DgusTFT::page30_handle(void)   // Auto heat filament
//        {
//            static millis_t flash_time = 0;
//            switch (key_value)
//            {
//              case 0:
//              break;

//              case 1:   // return
//              {
//                setTargetTemp_celsius(230, E0);
//                ChangePageOfTFT(PAGE_FILAMENT);
//              }
//              break;
//            }

//          if(millis() < (flash_time +1000) )return;
//          flash_time=millis();
//        }

//        void DgusTFT::page31_handle(void)
//        {
//            static millis_t flash_time = 0;
//            switch (key_value)
//            {
//              case 0:
//              break;

//              case 1://return
//              {
//              }
//              break;

//              case 2:
//              {
//              }
//              break;
//            }

//          if(millis() < (flash_time +1000) )return;
//          flash_time=millis();
//        }

//        void DgusTFT::page32_handle(void)
//        {
//          static millis_t flash_time = 0;
//    
//             if(millis() < (flash_time +1000) )return;
//             flash_time=millis();
//        
//        }

//        void DgusTFT::page33_handle(void)
//        {
//          static millis_t flash_time = 0;
//          switch (key_value)
//          {
//            case 0:
//            break;

//            case 1:   // auto leveling start
//            {
//              injectCommands_P(PSTR("M851 Z0\nG28\nG29"));
//              printer_state = AC_printer_probing;
//              ChangePageOfTFT(PAGE_LEVELING);
//            }
//            break;

//            case 2:
//            {
//              ChangePageOfTFT(PAGE_PreLEVEL);
//            }
//            break;
//          }

//          if(millis() < (flash_time +1000) )return;
//          flash_time=millis();
//        }

//        void DgusTFT::page34_handle(void)
//        {
//          static millis_t flash_time = 0;

//          if(millis() < (flash_time +1000) )return;
//          flash_time=millis();
//    
//          if(pop_up_index==25)
//          {
//            pop_up_index=100;
//            ChangePageOfTFT(PAGE_PreLEVEL);
//          }
//        
//        }
//    
//    void DgusTFT::page115_handle(void)
//    {
//        static millis_t flash_time = 0;
//            switch (key_value)
//            {
//    
//              case 0:
//              break;
//              case 1:
//              {            
//               ChangePageOfTFT(PAGE_PreLEVEL);
//              }
//              break;
//              
//              case 2:
//              {
//                injectCommands_P(PSTR("M1024 S3"));      // -1
//              //  char value[20]
//                //sprintf_P(value,PSTR("G1 Z%iF%i")); enqueue_and_echo_command_now(value); }                                     
//                    
//              }
//              break;
//              
//              case 3:
//              {
//                injectCommands_P(PSTR("M1024 S4"));      // 1
//              }
//              
//              break;
//              
//              case 4:
//              {            
//                injectCommands_P(PSTR("M1024 S1"));      // -0.1
//              }
//              break;
//              case 5:
//              {
//                injectCommands_P(PSTR("M1024 S2"));      // 0.1
//              }
//              break;
//    
//              case 6:
//              {
//                injectCommands_P(PSTR("M1024 S0"));      // prepare, move x y to center
//              }
//              break;
//              case 7:
//              {            
//                injectCommands_P(PSTR("M1024 S5"));      // 0.1
//              }
//              break;
//            }
//    
//    
//        if(millis() < (flash_time +1000) )return;
//        flash_time=millis();
//    
//        
//    }

//    void DgusTFT::page117_handle(void)  // Page CHS Mute handler
//    {
//        switch (key_value)
//        {
//          case 0:
//          break;

//          case 1:
//          {
//            ChangePageOfTFT(PAGE_MAIN);
//            if(lcd_info_back.language != lcd_info.language ||
//               lcd_info_back.audio    != lcd_info.audio) {

//               lcd_info_back.language = lcd_info.language;
//               lcd_info_back.audio    = lcd_info.audio;

//               injectCommands_P(PSTR("M500"));
//            }
//          }
//          break;

//          case 2:   // language
//          {
//            if(lcd_info.language == ExtUI::CHS) {
//                lcd_info.language = ExtUI::ENG;
//                if(lcd_info.audio == ExtUI::ON) {
//                    ChangePageOfTFT(11);  // PAGE_SYSTEM_ENG_AUDIO_ON-120
//                } else if(lcd_info.audio == ExtUI::OFF) {
//                    ChangePageOfTFT(50);  // PAGE_SYSTEM_ENG_AUDIO_OFF-120
//                }
//            } else if(lcd_info.language == ExtUI::ENG) {
//                lcd_info.language = ExtUI::CHS;
//                if(lcd_info.audio == ExtUI::ON) {
//                    ChangePageOfTFT(PAGE_SYSTEM_CHS_AUDIO_ON);
//                } else if(lcd_info.audio == ExtUI::OFF) {
//                    ChangePageOfTFT(PAGE_SYSTEM_CHS_AUDIO_OFF);
//                }
//            }
//          }
//          break;

//          case 3:
//          break;

//          case 4:   // audio
//          {
//            if(lcd_info.audio == ExtUI::ON) {
//                lcd_info.audio = ExtUI::OFF;
//                ChangePageOfTFT(PAGE_SYSTEM_CHS_AUDIO_OFF);
//            } else if(lcd_info.audio == ExtUI::OFF) {
//                lcd_info.audio = ExtUI::ON;
//                ChangePageOfTFT(PAGE_SYSTEM_CHS_AUDIO_ON);
//            }

//            LcdAudioSet(lcd_info.audio);
//          }
//          break;

//          case 5:   // about
//            char str_ver[32];
////            sprintf(str_ver, "%04d-%02d-%02d %02d:%02d:%02d\n", BUILD_YEAR, BUILD_MONTH, BUILD_DAY, BUILD_HOUR, BUILD_MIN, BUILD_SEC);
//            sprintf(str_ver, MAIN_BOARD_FIRMWARE_VER);
//            SendTxtToTFT(str_ver, TXT_VERSION);
//            ChangePageOfTFT(PAGE_ABOUT);
//            break;

//          case 6:
//            ChangePageOfTFT(PAGE_RECORD);
//            break;
//        }
//    }

//    void DgusTFT::page170_handle(void)  // ENG Mute handler
//    {
//        switch (key_value)
//        {
//          case 0:
//          break;

//          case 1:
//          {
//            ChangePageOfTFT(PAGE_MAIN);
//            if(lcd_info_back.language != lcd_info.language ||
//               lcd_info_back.audio    != lcd_info.audio) {

//               lcd_info_back.language = lcd_info.language;
//               lcd_info_back.audio    = lcd_info.audio;

//               injectCommands_P(PSTR("M500"));
//            }
//          }
//          break;

//          case 2:   // language
//          {
//            if(lcd_info.language == ExtUI::CHS) {
//                lcd_info.language = ExtUI::ENG;
//                if(lcd_info.audio == ExtUI::ON) {
//                    ChangePageOfTFT(11);  // PAGE_SYSTEM_ENG_AUDIO_ON-120
//                } else if(lcd_info.audio == ExtUI::OFF) {
//                    ChangePageOfTFT(50);  // PAGE_SYSTEM_ENG_AUDIO_OFF-120
//                }
//            } else if(lcd_info.language == ExtUI::ENG) {
//                lcd_info.language = ExtUI::CHS;
//                if(lcd_info.audio == ExtUI::ON) {
//                    ChangePageOfTFT(PAGE_SYSTEM_CHS_AUDIO_ON);
//                } else if(lcd_info.audio == ExtUI::OFF) {
//                    ChangePageOfTFT(PAGE_SYSTEM_CHS_AUDIO_OFF);
//                }
//            }
//          }
//          break;

//          case 3:
//          break;

//          case 4:   // audio
//          {
//            if(lcd_info.audio == ExtUI::ON) {
//                lcd_info.audio = ExtUI::OFF;
//                ChangePageOfTFT(PAGE_SYSTEM_CHS_AUDIO_OFF);
//            } else if(lcd_info.audio == ExtUI::OFF) {
//                lcd_info.audio = ExtUI::ON;
//                ChangePageOfTFT(PAGE_SYSTEM_CHS_AUDIO_ON);
//            }

//            LcdAudioSet(lcd_info.audio);
//          }
//          break;

//          case 5:   // about
//            char str_ver[32];
////            sprintf(str_ver, "%04d-%02d-%02d %02d:%02d:%02d\n", BUILD_YEAR, BUILD_MONTH, BUILD_DAY, BUILD_HOUR, BUILD_MIN, BUILD_SEC);
//            sprintf(str_ver, MAIN_BOARD_FIRMWARE_VER);
//            SendTxtToTFT(str_ver, TXT_VERSION);
//            ChangePageOfTFT(PAGE_ABOUT);
//            break;

//          case 6:
//            ChangePageOfTFT(PAGE_RECORD);
//            break;
//        }
//    }

//    void DgusTFT::page171_handle(void)  // CHS power outage resume handler
//    {
//        char str_buf[20] = { '\0' };

//        switch (key_value)
//        {
//          case 0:
//          break;

//          case 1:   // resume
//          {
//            char filename[64] = { '\0' };

//          
//            ChangePageOfTFT(PAGE_OUTAGE_RECOVERY);
//           // card.getLongPath(filename, recovery.info.sd_filename);
//            
//          //  filename[17] = '\0';
//          //  SendTxtToTFT(filename, TXT_OUTAGE_RECOVERY_FILE);

//            SendTxtToTFT(recovery.info.sd_filename, TXT_OUTAGE_RECOVERY_FILE);
//            sprintf(str_buf, "%u", (uint16_t)getFeedrate_percent());
//            SendTxtToTFT(str_buf, TXT_PRINT_SPEED);

//            sprintf(str_buf, "%u", (uint16_t)getProgress_percent());
//            SendTxtToTFT(str_buf, TXT_PRINT_PROGRESS);

//            ChangePageOfTFT(PAGE_STATUS2);               // show pause
//            injectCommands_P(PSTR("M355 S1\nM1000"));    // case light on, home and start recovery
//          }
//          break;

//          case 2:   // cancel
//          {
//            printer_state = AC_printer_idle;
//            ChangePageOfTFT(PAGE_MAIN);
//            injectCommands_P(PSTR("M355 S0\nM1000 C"));  // cancel recovery
//          }
//          break;
//        }
//    }

//    void DgusTFT::page173_handle(void)  // ENG power outage resume handler
//    {
//        char str_buf[20] = { '\0' };

//        switch (key_value)
//        {
//          case 0:
//          break;

//          case 1:   // resume
//          {
//            char filename[64] = { '\0' };
//            ChangePageOfTFT(PAGE_OUTAGE_RECOVERY);
//            //card.getLongPath(filename, recovery.info.sd_filename);
//          
//           // filename[17] = '\0';
//           // SendTxtToTFT(filename, TXT_OUTAGE_RECOVERY_FILE);

//            SendTxtToTFT(recovery.info.sd_filename, TXT_OUTAGE_RECOVERY_FILE);

//            sprintf(str_buf, "%u", (uint16_t)getFeedrate_percent());
//            SendTxtToTFT(str_buf, TXT_PRINT_SPEED);

//            sprintf(str_buf, "%u", (uint16_t)getProgress_percent());
//            SendTxtToTFT(str_buf, TXT_PRINT_PROGRESS);

//            ChangePageOfTFT(PAGE_STATUS2);      // show pause
//            injectCommands_P(PSTR("M355 S1\nM1000"));    // case light on, home and start recovery
//          }
//          break;

//          case 2:   // cancel
//          {
//            printer_state = AC_printer_idle;
//            ChangePageOfTFT(PAGE_MAIN);
//            injectCommands_P(PSTR("M355 S0\nM1000 C"));  // cancel recovery
//          }
//          break;
//        }
//    }


//    void DgusTFT::page175_handle(void)     // CHS probe preheating handler
//    {

//    }

//    void DgusTFT::page176_handle(void)     // ENG probe preheating handler
//    {

//    }


//    void DgusTFT::pop_up_manager(void)
//    {
//     char str_buf[20];

//       switch (pop_up_index)
//       {
//        case 10:    //T0 error
//        if(page_index_now!=PAGE_ABNORMAL)
//        ChangePageOfTFT(PAGE_ABNORMAL);
//         pop_up_index = 100;
//        break;

//        case 15:    // filament lack
//        if(page_index_now!=PAGE_FILAMENT_LACK)
//        ChangePageOfTFT(PAGE_FILAMENT_LACK);
//         pop_up_index = 100;
//        break;
//        
//        case 16:    // stop wait
//        //ChangePageOfTFT(PAGE_WAIT_STOP);
//         pop_up_index = 100;
//        break;
//        
//        case 18:    //
//         ChangePageOfTFT(PAGE_STATUS1);
//         pop_up_index = 100;
//        break;

//        case 23:    //
//        if(page_index_now!=PAGE_FILAMENT_LACK) {
//          ChangePageOfTFT(PAGE_FILAMENT_LACK);
//        }
//        pop_up_index = 100;
//        break;

//        case 24://
//        {
//           uint32_t time = getProgress_seconds_elapsed() / 60;
//           sprintf(str_buf, "%s H ", utostr3(time/60));
//           sprintf(str_buf+strlen(str_buf), "%s M", utostr3(time%60));
//           SendTxtToTFT(str_buf, TXT_FINISH_TIME);
//           ChangePageOfTFT(PAGE_PRINT_FINISH);
//           pop_up_index = 100;
//        }
//        break;

//        case 25://LEVEL DONE
//         ChangePageOfTFT(PAGE_PreLEVEL);
//         pop_up_index = 100;
//        break;
//       }
//    }


  
} // namespace

#endif