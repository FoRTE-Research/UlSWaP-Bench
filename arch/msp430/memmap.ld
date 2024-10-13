/*  Basic MSP430FR5994 picolibc linker script
 */

/* For large memory model, quick hack
 * to correctly redirect reset vector
 */

/* Use FRAM for RO memory
 * and SRAM for RW memory
 */
/*
*__flash =      0x00010000;
*__flash_size = 0x00019FF0;
*__ram =        0x00001C00;
*__ram_size   = 0x00001000;
*__stack_size = 0xC00;
*/

/* Use part of high FRAM for RO memory
 * and part of high FRAM for RW memory
 */

__flash       = 0x00010000;
__flash_size  = 0x00019FF0;
__ram         = 0x00029FF0;
__ram_size    = 0x00008000;
__stack_size  = 0xC00;

INCLUDE /home/harrison/ti/msp430-gcc/include/msp430fr5994_symbols.ld

/*  Need to define a bunch of stuff missing from
 *  picolibc boilerplate linker script
 */

MEMORY{
  VECT0            : ORIGIN = 0xFF90, LENGTH = 0x0002
  VECT1            : ORIGIN = 0xFF92, LENGTH = 0x0002
  VECT2            : ORIGIN = 0xFF94, LENGTH = 0x0002
  VECT3            : ORIGIN = 0xFF96, LENGTH = 0x0002
  VECT4            : ORIGIN = 0xFF98, LENGTH = 0x0002
  VECT5            : ORIGIN = 0xFF9A, LENGTH = 0x0002
  VECT6            : ORIGIN = 0xFF9C, LENGTH = 0x0002
  VECT7            : ORIGIN = 0xFF9E, LENGTH = 0x0002
  VECT8            : ORIGIN = 0xFFA0, LENGTH = 0x0002
  VECT9            : ORIGIN = 0xFFA2, LENGTH = 0x0002
  VECT10           : ORIGIN = 0xFFA4, LENGTH = 0x0002
  VECT11           : ORIGIN = 0xFFA6, LENGTH = 0x0002
  VECT12           : ORIGIN = 0xFFA8, LENGTH = 0x0002
  VECT13           : ORIGIN = 0xFFAA, LENGTH = 0x0002
  VECT14           : ORIGIN = 0xFFAC, LENGTH = 0x0002
  VECT15           : ORIGIN = 0xFFAE, LENGTH = 0x0002
  VECT16           : ORIGIN = 0xFFB0, LENGTH = 0x0002
  VECT17           : ORIGIN = 0xFFB2, LENGTH = 0x0002
  VECT18           : ORIGIN = 0xFFB4, LENGTH = 0x0002
  VECT19           : ORIGIN = 0xFFB6, LENGTH = 0x0002
  VECT20           : ORIGIN = 0xFFB8, LENGTH = 0x0002
  VECT21           : ORIGIN = 0xFFBA, LENGTH = 0x0002
  VECT22           : ORIGIN = 0xFFBC, LENGTH = 0x0002
  VECT23           : ORIGIN = 0xFFBE, LENGTH = 0x0002
  VECT24           : ORIGIN = 0xFFC0, LENGTH = 0x0002
  VECT25           : ORIGIN = 0xFFC2, LENGTH = 0x0002
  VECT26           : ORIGIN = 0xFFC4, LENGTH = 0x0002
  VECT27           : ORIGIN = 0xFFC6, LENGTH = 0x0002
  VECT28           : ORIGIN = 0xFFC8, LENGTH = 0x0002
  VECT29           : ORIGIN = 0xFFCA, LENGTH = 0x0002
  VECT30           : ORIGIN = 0xFFCC, LENGTH = 0x0002
  VECT31           : ORIGIN = 0xFFCE, LENGTH = 0x0002
  VECT32           : ORIGIN = 0xFFD0, LENGTH = 0x0002
  VECT33           : ORIGIN = 0xFFD2, LENGTH = 0x0002
  VECT34           : ORIGIN = 0xFFD4, LENGTH = 0x0002
  VECT35           : ORIGIN = 0xFFD6, LENGTH = 0x0002
  VECT36           : ORIGIN = 0xFFD8, LENGTH = 0x0002
  VECT37           : ORIGIN = 0xFFDA, LENGTH = 0x0002
  VECT38           : ORIGIN = 0xFFDC, LENGTH = 0x0002
  VECT39           : ORIGIN = 0xFFDE, LENGTH = 0x0002
  VECT40           : ORIGIN = 0xFFE0, LENGTH = 0x0002
  VECT41           : ORIGIN = 0xFFE2, LENGTH = 0x0002
  VECT42           : ORIGIN = 0xFFE4, LENGTH = 0x0002
  VECT43           : ORIGIN = 0xFFE6, LENGTH = 0x0002
  VECT44           : ORIGIN = 0xFFE8, LENGTH = 0x0002
  VECT45           : ORIGIN = 0xFFEA, LENGTH = 0x0002
  VECT46           : ORIGIN = 0xFFEC, LENGTH = 0x0002
  VECT47           : ORIGIN = 0xFFEE, LENGTH = 0x0002
  VECT48           : ORIGIN = 0xFFF0, LENGTH = 0x0002
  VECT49           : ORIGIN = 0xFFF2, LENGTH = 0x0002
  VECT50           : ORIGIN = 0xFFF4, LENGTH = 0x0002
  VECT51           : ORIGIN = 0xFFF6, LENGTH = 0x0002
  VECT52           : ORIGIN = 0xFFF8, LENGTH = 0x0002
  VECT53           : ORIGIN = 0xFFFA, LENGTH = 0x0002
  VECT54           : ORIGIN = 0xFFFC, LENGTH = 0x0002
  RESETVEC         : ORIGIN = 0xFFFE, LENGTH = 0x0002
  RSTJMP           : ORIGIN = 0x4000, LENGTH = 0x0010
}

SECTIONS{
  __interrupt_vector_0   : { KEEP (*(__interrupt_vector_0 )) } > VECT0
  __interrupt_vector_1   : { KEEP (*(__interrupt_vector_1 )) } > VECT1
  __interrupt_vector_2   : { KEEP (*(__interrupt_vector_2 )) } > VECT2
  __interrupt_vector_3   : { KEEP (*(__interrupt_vector_3 )) } > VECT3
  __interrupt_vector_4   : { KEEP (*(__interrupt_vector_4 )) } > VECT4
  __interrupt_vector_5   : { KEEP (*(__interrupt_vector_5 )) } > VECT5
  __interrupt_vector_6   : { KEEP (*(__interrupt_vector_6 )) } > VECT6
  __interrupt_vector_7   : { KEEP (*(__interrupt_vector_7 )) } > VECT7
  __interrupt_vector_8   : { KEEP (*(__interrupt_vector_8 )) } > VECT8
  __interrupt_vector_9   : { KEEP (*(__interrupt_vector_9 )) } > VECT9
  __interrupt_vector_10  : { KEEP (*(__interrupt_vector_10)) } > VECT10
  __interrupt_vector_11  : { KEEP (*(__interrupt_vector_11)) } > VECT11
  __interrupt_vector_12  : { KEEP (*(__interrupt_vector_12)) } > VECT12
  __interrupt_vector_13  : { KEEP (*(__interrupt_vector_13)) } > VECT13
  __interrupt_vector_14  : { KEEP (*(__interrupt_vector_14)) } > VECT14
  __interrupt_vector_15  : { KEEP (*(__interrupt_vector_15)) } > VECT15
  __interrupt_vector_16  : { KEEP (*(__interrupt_vector_16)) } > VECT16
  __interrupt_vector_17  : { KEEP (*(__interrupt_vector_17)) } > VECT17
  __interrupt_vector_18  : { KEEP (*(__interrupt_vector_18)) KEEP (*(__interrupt_vector_lea)) } > VECT18
  __interrupt_vector_19  : { KEEP (*(__interrupt_vector_19)) KEEP (*(__interrupt_vector_port8)) } > VECT19
  __interrupt_vector_20  : { KEEP (*(__interrupt_vector_20)) KEEP (*(__interrupt_vector_port7)) } > VECT20
  __interrupt_vector_21  : { KEEP (*(__interrupt_vector_21)) KEEP (*(__interrupt_vector_eusci_b3)) } > VECT21
  __interrupt_vector_22  : { KEEP (*(__interrupt_vector_22)) KEEP (*(__interrupt_vector_eusci_b2)) } > VECT22
  __interrupt_vector_23  : { KEEP (*(__interrupt_vector_23)) KEEP (*(__interrupt_vector_eusci_b1)) } > VECT23
  __interrupt_vector_24  : { KEEP (*(__interrupt_vector_24)) KEEP (*(__interrupt_vector_eusci_a3)) } > VECT24
  __interrupt_vector_25  : { KEEP (*(__interrupt_vector_25)) KEEP (*(__interrupt_vector_eusci_a2)) } > VECT25
  __interrupt_vector_26  : { KEEP (*(__interrupt_vector_26)) KEEP (*(__interrupt_vector_port6)) } > VECT26
  __interrupt_vector_27  : { KEEP (*(__interrupt_vector_27)) KEEP (*(__interrupt_vector_port5)) } > VECT27
  __interrupt_vector_28  : { KEEP (*(__interrupt_vector_28)) KEEP (*(__interrupt_vector_timer4_a1)) } > VECT28
  __interrupt_vector_29  : { KEEP (*(__interrupt_vector_29)) KEEP (*(__interrupt_vector_timer4_a0)) } > VECT29
  __interrupt_vector_30  : { KEEP (*(__interrupt_vector_30)) KEEP (*(__interrupt_vector_aes256)) } > VECT30
  __interrupt_vector_31  : { KEEP (*(__interrupt_vector_31)) KEEP (*(__interrupt_vector_rtc_c)) } > VECT31
  __interrupt_vector_32  : { KEEP (*(__interrupt_vector_32)) KEEP (*(__interrupt_vector_port4)) } > VECT32
  __interrupt_vector_33  : { KEEP (*(__interrupt_vector_33)) KEEP (*(__interrupt_vector_port3)) } > VECT33
  __interrupt_vector_34  : { KEEP (*(__interrupt_vector_34)) KEEP (*(__interrupt_vector_timer3_a1)) } > VECT34
  __interrupt_vector_35  : { KEEP (*(__interrupt_vector_35)) KEEP (*(__interrupt_vector_timer3_a0)) } > VECT35
  __interrupt_vector_36  : { KEEP (*(__interrupt_vector_36)) KEEP (*(__interrupt_vector_port2)) } > VECT36
  __interrupt_vector_37  : { KEEP (*(__interrupt_vector_37)) KEEP (*(__interrupt_vector_timer2_a1)) } > VECT37
  __interrupt_vector_38  : { KEEP (*(__interrupt_vector_38)) KEEP (*(__interrupt_vector_timer2_a0)) } > VECT38
  __interrupt_vector_39  : { KEEP (*(__interrupt_vector_39)) KEEP (*(__interrupt_vector_port1)) } > VECT39
  __interrupt_vector_40  : { KEEP (*(__interrupt_vector_40)) KEEP (*(__interrupt_vector_timer1_a1)) } > VECT40
  __interrupt_vector_41  : { KEEP (*(__interrupt_vector_41)) KEEP (*(__interrupt_vector_timer1_a0)) } > VECT41
  __interrupt_vector_42  : { KEEP (*(__interrupt_vector_42)) KEEP (*(__interrupt_vector_dma)) } > VECT42
  __interrupt_vector_43  : { KEEP (*(__interrupt_vector_43)) KEEP (*(__interrupt_vector_eusci_a1)) } > VECT43
  __interrupt_vector_44  : { KEEP (*(__interrupt_vector_44)) KEEP (*(__interrupt_vector_timer0_a1)) } > VECT44
  __interrupt_vector_45  : { KEEP (*(__interrupt_vector_45)) KEEP (*(__interrupt_vector_timer0_a0)) } > VECT45
  __interrupt_vector_46  : { KEEP (*(__interrupt_vector_46)) KEEP (*(__interrupt_vector_adc12_b)) } > VECT46
  __interrupt_vector_47  : { KEEP (*(__interrupt_vector_47)) KEEP (*(__interrupt_vector_eusci_b0)) } > VECT47
  __interrupt_vector_48  : { KEEP (*(__interrupt_vector_48)) KEEP (*(__interrupt_vector_eusci_a0)) } > VECT48
  __interrupt_vector_49  : { KEEP (*(__interrupt_vector_49)) KEEP (*(__interrupt_vector_wdt)) } > VECT49
  __interrupt_vector_50  : { KEEP (*(__interrupt_vector_50)) KEEP (*(__interrupt_vector_timer0_b1)) } > VECT50
  __interrupt_vector_51  : { KEEP (*(__interrupt_vector_51)) KEEP (*(__interrupt_vector_timer0_b0)) } > VECT51
  __interrupt_vector_52  : { KEEP (*(__interrupt_vector_52)) KEEP (*(__interrupt_vector_comp_e)) } > VECT52
  __interrupt_vector_53  : { KEEP (*(__interrupt_vector_53)) KEEP (*(__interrupt_vector_unmi)) } > VECT53
  __interrupt_vector_54  : { KEEP (*(__interrupt_vector_54)) KEEP (*(__interrupt_vector_sysnmi)) } > VECT54
  __reset_vector :
  {
    KEEP (*(__interrupt_vector_55))
    KEEP (*(__interrupt_vector_reset))
    KEEP (*(.resetvec))
  } > RESETVEC
  __rstjmp :
  {
    KEEP (*(.rstjmp))
  } > RSTJMP
}

INCLUDE picolibc.ld
