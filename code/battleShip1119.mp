Project:      .\battleShip1119
Build Number: 52
Date:         Mon Nov 26 21:47:55 2018
Compiler:     ICCAVR
Version:      8.04.03

NOTE: AVR Code addresses are word addresses
All other addresses (including ones in FLASH) are byte addresses

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                           idata   008C   02BE =    702. bytes (rel,con,rom,lit)

       Addr  Global Symbol
      -----  --------------------------------
       008C  __idata_start
       034A  __idata_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                            text   034A   8B48 =  35656. bytes (rel,con,rom,code)

       Addr  Global Symbol
      -----  --------------------------------
       01A5  __start
       01A5  __text_start
       01C7  _exit
       01C8  _GLCD_Rectangle
       01FC  _GLCD_Circle
       0294  _GLCD_Line
       0346  _delay
       035D  _GLCD_InitializeInterface
       0365  _GLCD_CheckStatus
       0375  _GLCD_WriteCommand
       0385  _GLCD_WriteData
       0395  _GLCD_ReadData
       03A9  _GLCD_SetAddressPointer
       03B8  _GLCD_ClearText
       03CA  _GLCD_ClearCG
       03DD  _GLCD_ClearGraphic
       03EF  _GLCD_WriteChar
       03F6  _GLCD_WriteString
       0405  _GLCD_TextGoTo
       0414  _GLCD_DefineCharacter
       0431  _GLCD_SetPixel
       0468  _GLCD_WriteDisplayData
       0470  _GLCD_GraphicGoTo
       0487  _GLCD_Initialize
       04B0  _delay_ms
       04C4  _GLCD_Clear
       04C7  _rGLCD_1block
       04FB  _rship1
       0522  _rship2
       0549  _rship3
       0570  _switch_turn
       0722  _rship4
       0749  _rGLCD_cursor_off
       0783  rsetting.h:do_rand
       0823  _rand
       0826  _srand
       0832  _rGLCD_mine
       0945  _rGLCD_item
       09A9  _rGLCD_fail
       0A33  _rGLCD_rock
       0BDF  _GLCD_screen
       0C39  _begin_cursoron
       0D7F  _begin_cursoroff
       0ECD  _GLCD_TextGoTo1
       0EE8  _lGLCD_success
       0F13  _lGLCD_cursor
       1014  _lGLCD_mine
       113E  _lGLCD_SemiCircle
       11AA  _lGLCD_item
       1212  _lGLCD_rock
       1310  _lGLCD_fail
       139F  _GLCD_cursor_off
       13D4  _rGLCD_success
       1400  _po_down_on
       144B  _timer2_ovf
       14E6  _USART0_Transmit
       14EA  _AtoD
       1533  _vibrating
       153C  _mode_select
       163D  _difficulty_select
       173A  _joystick_setting
       17DA  _joystick_playing
       1BBA  _timeover_select
       1DC7  _switch_select
       205E  _coordinate
       21C2  _is_found_rock
       22B5  _led_score
       22E5  _register_timer
       22EE  _off_bounded
       254A  _getchar$device_specific$
       254F  _set_tx_uart
       2576  _set_rx_uart
       259D  _main
       45B3  push_arg4
       45B5  push_arg2
       45B8  mod16s
       45BC  div16s
       45D6  mod16u
       45D8  div16u
       45D9  xdiv16u
       45F2  div32u
       45F4  mod32u
       45F8  div32s
       45FA  mod32s
       4626  long_div_prolog
       4639  long_div_epilog
       4649  tst_R16_R19
       464E  tst_R24_R27
       4653  neg_R24_R27
       465C  mod8u
       465E  div8u
       465F  xdiv8u
       4674  empy16s
       4684  empy32s
       4684  empy32u
       46B8  pop_xgset003C
       46BD  pop_xgsetF0FC
       46C8  push_xgsetF0FC
       46CA  push_xgset30FC
       46CC  push_xgset00FC
       46CE  push_xgset003C
       46D3  push_xgset300C
       46D8  pop_xgset300C
       46DD  push_xgsetF000
       46E2  pop_xgsetF000
       46E7  push_xgsetF00C
       46EE  pop_xgsetF00C
       46F5  push_xgset303C
       46FC  pop_xgset303C
       4703  push_xgsetF03C
       470C  pop_xgsetF03C
       4715  neg32
       471E  long_prolog
       472C  long_epilog
       4737  tstzero1
       473D  tstzero2
       4743  lsl8
       4749  __text_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                            data   0100   02BE =    702. bytes (rel,con,ram)

       Addr  Global Symbol
      -----  --------------------------------
       0100  _color
       0100  __data_start
       0101  _i1
       010B  _j1
       0115  _i2
       011F  _j2
       0129  _i3
       0133  _j3
       0137  _i4
       0141  _j4
       014B  _tmp
       014D  rsetting.h:next
       0151  _i
       0153  _j
       0155  _counter_set
       01D5  _counter_detect
       0255  _my_detect
       02D5  _flag
       02D6  _sleep
       02D7  _destroy
       02DF  _ship_cnt
       02E0  _item_cnt
       02E1  _mine_cnt
       02E2  _timercounter
       02E4  _timer_sec
       02E5  _time_flag
       02E6  _mode
       02E7  _difficult
       02E8  _select
       02E9  _state
       02EA  _cnt
       02EB  _p_i
       02ED  _p_j
       02EF  _error
       02F0  _AtoD_cnt
       02F2  _com_rest_ship
       03BE  __data_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                             bss   03BE   0119 =    281. bytes (rel,con,ram)

       Addr  Global Symbol
      -----  --------------------------------
       03BE  __bss_start
       03BE  _cnt_y
       03C0  _cnt_x
       03C2  _rate
       03C4  _rest_ship
       03C5  _rand_l
       03C7  _rand_k
       03C9  _rand_j
       03CB  _rand_i
       03CD  _val_y
       03CF  _val_x
       03D1  _n
       03D2  _crocky
       03D6  _crockx
       03DA  _cminey
       03DE  _cminex
       03E2  _citemy
       03E6  _citemx
       03EA  _cship4y
       03F2  _cship4x
       03FA  _cship3y
       03FE  _cship3x
       0402  _cship2y
       040C  _cship2x
       0416  _cship1y
       041E  _cship1x
       0426  _rock2y
       0428  _rock2x
       042A  _rock1y
       042C  _rock1x
       042E  _mine2y
       0430  _mine2x
       0432  _mine1y
       0434  _mine1x
       0436  _item2y
       0438  _item2x
       043A  _item1y
       043C  _item1x
       043E  _ship4y
       0440  _ship4x
       0442  _ship3y
       0444  _ship3x
       0446  _ship2y
       0448  _ship2x
       044A  _ship1y
       044C  _ship1x
       044E  _jmin
       0450  _jmax
       0452  _imin
       0454  _imax
       0456  _outdata
       0457  _game_set
       04D7  __bss_end

Area                               Start  End    Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                          vector   0000   002C =     44. bytes (abs,ovr,rom,code)

Files Linked      [ module(s) ]

C:\iccv8avr\lib\crtatmega.o	[ crtatmega.s ]
.\.objs\main.o	[ main.c ]
<library>	[ asave.s, div16s.s, div16u.s, div32.s, div8u.s, emul16s.s, emul32.s, gpop2.s, gpop5.s, gpush.s, gpushx.s ]
[ gpushy.s, gpushz.s, gpusha.s, gpushb.s, lneg.s, longutil.s, lsl8.s ]

User Global Definitions

ram_end = 0x10ff
hwstk_size = 0x14

Area Definitions

-bfunc_lit:0x8c.0x10000
-bdata:0x100.0x10ff

