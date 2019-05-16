; Trial Saccades -- Center fixation target followed by peripheral saccade target.
; 4.Nov.2011 -- Original version by JVB.
;
; Timing
; -------
; 0.1 ms per step, skips 13-15 on average
; Est 115 steps = 11 ms per cycle through seq
;
; Analog Input Channels
; ----------------------
;  1 = Ignored (Left eye X pos'n)
;  2 = Ignored (Left eye Y pos'n)
;  3 = Fixation target X pos'n
;  4 = Fixation target Y pos'n
;      (...)
;  7 = Periph target X pos'n (re-purposed Bckgrn channel)
;  8 = Periph target Y pos'n (re-purposed Bckgrn channel)
;  9 = Ignored (Right eye X pos'n)
; 10 = Ignored (Right eye Y pos'n)
;      (...)
; 13 = Fixation target color
; 14 = Periph target color
;      (...)
; 17 = Ignored (Lever voltage)
;
; Analog Output Channels
; ----------------------
; 2 = Left eye X pos'n
; 3 = Left eye Y pos'n
; 4 = Right eye X pos'n
; 5 = Right eye Y pos'n
;
; Digital Inputs
; ---------------
; [.6......] pin 18 - VSG pin 2/vsgDIG1 : 1 = 'Reset' (misbehav. or normal 'next trial')
; [...4....] pin 19 - VSG pin 6/vsgDIG5 : Right shutter, 0 = Open; 1 = Closed
; [....3...] pin 7  - 
; [.....2..] pin 20 - VSG pin 7/vsgDIG6 : Left shutter, 0 = Open; 1 = Closed
; [......1.] pin 8
; [.......0] pin 21 - VSG pin 8/vsgDIG7 : Remote reward, 0 = Don't; 1 = Do
;
; [7.......] pin 5  - VSG pin 1/vsgDIG0 : 
; [..5.....] pin 6  - VSG pin 3/vsgDIG2 : 
;     [0.0.....] = Center, either eye  
;     [0.1.....] = Center
;     [1.0.....] = Calibration
;     [1.1.....] = Peripheral
;
; Digital Outputs
; ----------------
; [7.......] pin 1  - VSG pin 14/vsgDIG0 : Behavior contingency, 0 = Fine; 1 = Failed, abort trial
; [.6......] pin 14 - VSG pin 15/vsgDIG1 : Saccade target: 0 = Not acquired yet; 1 = Acquired
; [..5.....] pin 2  - VSG pin 16/vsgDIG2 :
; [...4....] pin 15
; [....3...] pin 3  - Right shutter
; [.....2..] pin 16 - Left shutter
; [......1.] pin 4  - Event in 1 : On target buzzer, 0 = Off; 1 = On (sound on)
; [.......0] pin 17 - Event in 0 - Feeder buzzer and pump, 0 = Off; 1 = On (feed)
;
            SET      0.100 2 0     ; Scaling of 2 to go up to 10 V
            DIGOUT [......00]      ; Force buzzers and feed pump off

            ; Init vars. ";s;" means set from script using SampleSeqVar
            VAR    V1              ; Eye position
            VAR    V2              ; Target X pos'n (see V6, V41)
            VAR    V3              ;s; Horizontal offset
            VAR    V4              ;s; Vertical offset
            VAR    V5              ; Feed pulse counter (for behavior contingency)
            VAR    V6              ; Target Y pos'n (see V2, V41)
            VAR    V7              ;s; Horizontal window
            VAR    V8              ;s; Vertical window
            VAR    V9              ; Bool: saccade target acquired?
            VAR    V10             ; Temporary for relaying eye positions to STIM
            VAR    V11,TOn=33      ;s;
            VAR    V12,TFeed=200   ;s;
            VAR    V13,TBlank=13   ;s;
            VAR    V14,criterio=3  ; 3 = 1 (horiz in range) + 2 (vert in range)
            VAR    V15,tstCntr     ; Test counter
            VAR    V16             ; Left eye accumulator: 3+ on target
            VAR    V17             ; Right eye accumulator: 3+ on target
            VAR    V18             ;s; Next stimulus
            VAR    V19             ; Which eyes?
            VAR    V20             ;s; Manual feed

            VAR    V22             ;s; targetGain
            VAR    V23             ;s; hOffsetL
            VAR    V24             ;s; hGainL
            VAR    V25             ;s; vOffsetL
            VAR    V26             ;s; vGainL
            VAR    V27             ;s; hOffsetR
            VAR    V28             ;s; hGainR
            VAR    V29             ;s; vOffsetR
            VAR    V30             ;s; vGainR
            VAR    V31,counton=0   ; Counter 1
            VAR    V32,countoff=0  ; Counter 2

            VAR    V41             ; Target color (see V2, V6)

            VAR    V42             ; Shutter left
            VAR    V43             ; Shutter right
            VAR    V44,VOffset
            VAR    V45,VScale

            VAR    V47             ;s; Manual buzzer

            VAR    V60             ; Cross (ctr) color
            VAR    V61             ; Dot (periph) color

            DAC    0,0
            DAC    1,0
            DAC    2,0
            DAC    3,0
                                    ; main() {
            MOVI   VOffset,8192         ; VOffset = 8192
            MOVI   VScale,2             ; VScale = 2
            MOVI   V5,0                 ; V5 = 0  // Feed pulse counter
            MOVI   V9,0                 ; saccadeTargetAcquired = false
			MOVI   tstCntr,0
;*************************************
RESETBGN:   DIGOUT [00.....0]           ; Abort flag off
                                        ; Acquired flag off
                                        ; Feeder off
            MOV    counton,TOn          ; counter1 = TOn     // TOn = ';s;'
            MOV    countoff,TBlank      ; counter2 = TBlank  // TBlank = ';s;'
			MOVRND tstCntr,2            ; Generate 2 random bits, thus
			                            ; (tstCntr == 0) 1/4th of time
;*************************************
ONSTART:    DIGOUT [0.......]           ; Fine/Abort flag = "fine"
            DIBNE  [.0......],RESETBGN  ; if (told to reset) {
                                        ;     reset back to RESETBGN
                                        ; }

			;relay eye position of (0,0) to vsg
            MOVI   V10,0            
            DAC    2,V10            ;lx
            DAC    3,V10            ;ly
            DAC    4,V10            ;rx
            DAC    5,V10            ;ry
			;relay eye positions to vsg

            MOVI   V16,0                ; V16 = 0  // left eye accumulator
            MOVI   V17,0                ; V17 = 0  // right eye accumulator

            DIBEQ  [.....1..],shutlc    ; if (left shutter open) {
            DIGOUT [.....1..]           ;     output: shutter left open
            JUMP   SHUTRO               ; }
SHUTLC:     DIGOUT [.....0..]           ; else { 
                                        ;     output: shutter left closed 
                                        ; }
                                        
SHUTRO:     DIBEQ  [...1....],shutrc    ; if (right shutter open) {
            DIGOUT [....1...]           ;     output: shutter right open
            JUMP   RWFEED               ; }
SHUTRC:     DIGOUT [....0...]           ; else { 
                                        ;     output: shutter right closed 
                                        ; }

RWFEED:     DIBEQ  [.......1],FEEDNOW   ; if (feed || 
            BEQ    V20,0,MBUZZ          ;     V20 != 0) // V20 is ';s; manual feed'
                                        ; {
FEEDNOW:    DIGOUT [.......1]           ;     turn pump on
            DELAY  TFeed                ;     pump for TFeed steps
            DIGOUT [000....0]           ;     turn pump off
            MOVI   V20,0                ;     V20 = 0
                                        ; }

MBUZZ:      BEQ    V47,0,CTRTARG        ; if (V47) {    // V47 is 'manual buzzer'
            DIGOUT [......1.]           ;     turn on buzzer
            DELAY  1000                 ;     fixed delay of 100ms
            DIGOUT [......0.]           ;     turn off buzzer
            MOVI   V47,0                ;     V47 = 0
                                        ; }
;*************************************
CTRTARG:    DIBEQ [1.1.....],PERFTARG   ; if ( !(trial_sacc && sacc_targ_phase) ) {
            CHAN V2,3                   ;     V2 = sample(fixation target x posn)
            CHAN V6,4                   ;     V6 = sample(fixation target y posn)
            CHAN V41,13                 ;     V41 = sample(fixation target color)
            JUMP INTRIAL                ; } else {
PERFTARG:   CHAN V2,7                   ;     V2 = sample(periph target x posn)
            CHAN V6,8                   ;     V6 = sample(periph target y posn)
            CHAN V41,14                 ;     V41 = sample(periph target color)
                                        ; }
INTRIAL:    BGT V41,4,TEST              ; 
            BLT V41,-4,TEST             ; if (in trial) { proceed }
            JUMP ONSTART                ; else { loop back w/o new rand num }

			;; ----------------------------------
			;; Eliminated eye tracking logic here
			;; ----------------------------------


;*************************************

TEST:       DIBEQ  [...0.1..],CMPLEYE   ; if (only l. shutter open) { CMPLEYE = true }
            DIBEQ  [...1.0..],CMPREYE   ; else if (only r. shutter open) { CMPREYE = true }
            DIBEQ  [...1.1..],OFFTARG   ; else if (neither shutter open) { goto OFFTARG }
            DIBEQ  [0.0.....],CMPEITH   ; else if (ctr either eye) { CMPEITH = true }
            DIBEQ  [1.0.....],CMPEITH   ; else if (calibration) { CMPEITH = true }
            DIBEQ  [1.1.....],CMPEITH   ; else if (trial saccades && saccade phase) {
                                        ;     CMPEITH = true
                                        ; } else { 
 									    ;     // Center, color-specific
            BLT    V41,-4,CMPLEYE       ;     if (V41 == 'blue') { CMPLEYE = true }
            BGT    V41,4,CMPREYE        ;     else if (V41 == 'red') { CMPREYE = true }
            JUMP   CMPEITH              ;     else { CMPEITH = true } }

CMPLEYE:    NOP
CMPREYE:    NOP
CMPEITH:    BNE    V15,0,ONTARG         ; if ( on targ probability ) {
            JUMP   OFFTARG              ; 

ONTARG:     DIBNE  [1.0.....],ONTARGTS  ;     if (calibration_stim) {
            DIGOUT [......1.]           ;         turn on sonalert
            DBNZ   counton,ONSTART      ;         if (0 != (--counton)) {
                                        ;             loop to ONSTART
            ADDI   V5,1                 ;         else if ((V5 += 1) > V18) {
            BLT    V5,V18,RESETBGN      ;
            MOVI   V5,0                 ;             V5 = 0;
            DIGOUT [.......1]           ;             feed
            DELAY  TFeed                ;             wait for TFeed ticks
            JUMP   RESETBGN             ;         }
                                        ;         loop to RESETBGN
                                        ;     }
                                        ;
ONTARGTS:   DIBEQ  [0.......],ONTRGNZE  ;     if (saccade phase) {
            MOVI   V9,1                 ;         saccadeTargetAcquired = true
            DIGOUT [.1......]           ;         tell STIM subj acquired targ
            JUMP ENDONTRG               ;     } else {
ONTRGNZE:   DIGOUT [......1.]           ;         turn on sonalert
                                        ;     }
ENDONTRG:   JUMP   ONSTART              ;     loop to ONSTART;
                                        ; } 
                                        ; else {
OFFTARG:    DIGOUT [......0.]           ;     sonalert off
            DBNZ   countoff,ONSTART     ;     if (0 != (--leeway_counter)) {
                                        ;         loop to ONSTART
            DIBEQ  [0.......],TELLOFF   ;     } else if (saccade_phase &&
            BLT V9,1,ONSTART            ;           target_had_not_been_found) {
                                        ;         loop to ONSTART
                                        ;     } else {
TELLOFF :   DIGOUT [1.......]           ;         tell STIM subj off target
            DELAY  1000                 ;         ensure STIM gets msg (ticks @ .1ms/tick)
            DIGOUT [0.......]           ;         turn off ABORT flag
            JUMP   RESETBGN             ;         loop RESETBGN
                                        ;     }
                                        ; }
                                        ;} // main
