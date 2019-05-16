;sequence for awake behaving electrode/tetrode scripts
;modified "reward" sequence
;
; Analog Input Channels
; ----------------------
; 1 = Left eye X pos'n
; 2 = Left eye Y pos'n
;      (...)
; 9 = Right eye X pos'n
; 10 = Right eye Y pos'n
;      (...)
;
; Analog Output Channels
; ----------------------
; 2 = Left eye X pos'n
; 3 = Left eye Y pos'n
; 4 = Right eye X pos'n
; 5 = Right eye Y pos'n
;
;Digital Inputs
;[7.......] pin 5  - VSG pin 1 - VSG Dig out 0 - trial running | use general-style rewards
;[.6......] pin 18 - VSG pin 2 - VSG Dig out 1 - unique/repeat | reset pulse
;[..5.....] pin 6  - VSG pin 3 - VSG Dig out 2 - Eye contingency of fixation spot, 0=left, 1=right
;[...4....] pin 19 - VSG pin 6 - VSG Dig out 5 - right shutter: 1=closed, 0=open
;[....3...] pin 7
;[.....2..] pin 20 - VSG pin 7 - VSG Dig out 6 - left shutter: 1=closed, 0=open
;[......1.] pin 8
;[.......0] pin 21 - VSG pin 8 - VSG Dig out 7 - remote reward
;
;Digital Outputs
;[7.......] pin 1  - VSG pin 14 - VSG Dig in 0 - trial end / stop trial
;[.6......] pin 14 - VSG pin 15 - VSG Dig in 1 - correct/incorrect
;[..5.....] pin 2  - VSG pin 16 - VSG Dig in 2 - trial start
;[...4....] pin 15
;[....3...] pin 3  - right shutter: 0=closed, 1=open
;[.....2..] pin 16 - left shutter: 0=closed, 1=open
;[......1.] pin 4  - event in 1 - on target buzzer
;[.......0] pin 17 - event in 0 - feeder buzzer and pump
;
;Last modified
;2/11/08 MKF changed paradigm. reset with nonfixation rather than advance with fixation
;2/8/08 MKF moved shutter L to a different connection to avoid noise
;2/7/08 MKF fixed shutter bugs. note that input and output have reverse coding for open/closed
;2/5/08 MKF replaced depricated counter instructions with updated instructions
;10/24/07 MKF yet more bug fixes with reward lines. moved some lines
;10/23/07 MKF more bug fixes
;10/22/07 MKF added auto shutter control, removed dichroic & button stuff, bug fixes
;10/16/07 MKF added docs, reward scheme fix
;9/24/07 MKF created from reward.pls to get sampling configuration up and running
;
;To do
;-get reward actually working (normally, and conditionally from gratings stim)
;-get shutters working
;
            SET      0.100 2 0
            DIGOUT [......00]
			;init vars. s; means set from script using SampleSeqVar
            VAR    V1              ;eye position
            VAR    V2              ;target position
            VAR    V3              ;s; horizontal offset
            VAR    V4              ;s; vertical offset
            VAR    V5              ;feed pulse counter (for behavior contingency)
            ;var    v6;used to relay eye positions to vsg
            VAR    V7              ;s; horizontal window
            VAR    V8              ;s; vertical window
            VAR    V10             ; Temporary for relaying eye positions to STIM
            VAR    V11,TOn=33      ;s;
            VAR    V12,TFeed=200   ;s;
            VAR    V13,TBlank=13   ;s;
            VAR    V14,criterio=3  ;3 = 1(horiz in range) + 2(vert in range) see v16,v17
            VAR    V16             ;left eye accumulator: 3+ on target
            VAR    V17             ;right eye accumulator: 3+ on target
            ;VAR    V18             ;s; next stimulus
            ;VAR    V19             ;which eyes?
            VAR    V20             ;s; manual feed
            VAR    V22             ;s; targetGain
            VAR    V23             ;s; hOffsetL
            VAR    V24             ;s; hGainL
            VAR    V25             ;s; vOffsetL
            VAR    V26             ;s; vGainL
            VAR    V27             ;s; hOffsetR
            VAR    V28             ;s; hGainR
            VAR    V29             ;s; vOffsetR
            VAR    V30             ;s; vGainR

            VAR    V31,counton=0             ;counter 1
            VAR    V32,countoff=0             ;counter 2
            ;VAR    V33            ;counter 1, depricated
            ;VAR    V34             ;counter 2, depricated

            VAR    V42             ;shutter left
            VAR    V43             ;shutter right

            DAC    0,0
            DAC    1,0
            DAC    2,0
            DAC    3,0
            MOVI   V5,0
;*************************************
REWSTART:   DIGOUT [000....0]
            MOV    countoff, TBlank ;reset off-target counter
            MOV    counton, TOn ;reset on-target counter
;*************************************
ONSTART:    DIBNE  [.0......],REWSTART ;if the reset pulse has been given, reset

			;relay eye positions to vsg
            MOVI   V10,0            ;test
            CHAN   V10,1            ;lx
            MULI   V10,32768
            DAC    2,V10
            MOVI   V10,0            ;test
            CHAN   V10,2            ;ly
            MULI   V10,32768
            NEG    V10,V10
            DAC    3,V10
            MOVI   V10,0            ;test
            CHAN   V10,9            ;rx
            MULI   V10,32768
            DAC    4,V10
            MOVI   V10,0            ;test
            CHAN   V10,10           ;ry
            MULI   V10,32768
            NEG    V10,V10
            DAC    5,V10
			;relay eye positions to vsg


            MOVI   V16,0
            MOVI   V17,0

			; Change Shutters

            DIBEQ  [.....1..],shutlc ;stim control left shutter
            DIGOUT [.....1..]      ;shutter left open
            JUMP   shutro

SHUTLC:     DIGOUT [.....0..]      ;shutter left closed
SHUTRO:     DIBEQ  [...1....],shutrc ;stim control right shutter
            DIGOUT [....1...]      ;shutter right open
            JUMP   rwfeed

SHUTRC:     DIGOUT [....0...]      ;shutter right closed


			; Reward Period Feed

RWFEED:     DIBEQ  [0......1],FEEDNOW ;beh. cong. trial running and remote feed now
			;DIBEQ  [1.0.....],FEEDNOW ;"trial running" means reward period, no prevent reward


		    ; Manual Feed

            BEQ    V20,0,SAMPHE
FEEDNOW:    DIGOUT [1......1]      ;supply pulse to juice pump
            DELAY  TFeed
            DIGOUT [0......0]
            ;DIGOUT [.......1]      ;supply pulse to juice pump
            ;DELAY  TFeed
            ;DIGOUT [000....0]
            MOVI   V20,0

;*************************************


			; Horizontal Channel Left Eye 

SAMPHE:     CHAN   V1,1            ;heyeL
            CHAN   V2,3            ;htarg

            MUL    V1,V24,0,10
            ADD    V1,V23

            ADD    V1,V3
            SUB    V1,V2

            BLT    V1,0,invhe
            JUMP   comphe
INVHE:      NEG    V1,V1
COMPHE:     BGT    V1,V7,sampve
            ADDI   V16,1


			; Vertical Channel Left Eye 

SAMPVE:     CHAN   V1,2            ;veyeL
            CHAN   V2,4            ;vtarg
            NEG    V1,V1

            MUL    V1,V26,0,10
            ADD    V1,V25

            ADD    V1,V4
            SUB    V1,V2
            BLT    V1,0,invve      ;originally BLT, 8/26/03 to account for inverted vert sig
            JUMP   compve
INVVE:      NEG    V1,V1
COMPVE:     BGT    V1,V8,sampher
            ADDI   V16,2


			; Horizontal Channel Right Eye 

SAMPHER:    CHAN   V1,9            ;heyeR
            CHAN   V2,3            ;htarg

            MUL    V1,V28,0,10
            ADD    V1,V27

            ADD    V1,V3
            SUB    V1,V2
            BLT    V1,0,invher
            JUMP   compher
INVHER:     NEG    V1,V1
COMPHER:    BGT    V1,V7,sampver
            ADDI   V17,1


			; Vertical Channel Right Eye 

SAMPVER:    CHAN   V1,10           ;veyeR
            CHAN   V2,4            ;vtarg
            NEG    V1,V1

            MUL    V1,V30,0,10
            ADD    V1,V29

            ADD    V1,V4
            SUB    V1,V2
            BLT    V1,0,invver     ;originally BLT, 8/26/03 to account for inverted vert sig
            JUMP   compver
INVVER:     NEG    V1,V1
COMPVER:    BGT    V1,V8,test
            ADDI   V17,2

;*************************************


			; Branch based on shutter input

TEST:       DIBEQ  [...0.1..],CMPREYE ;right eye open, left eye closed
            DIBEQ  [...1.0..],CMPLEYE ;right eye closed, left eye open
			DIBEQ  [..00.0..],CMPLEYE ;both eyes open, require left eye
			DIBEQ  [..10.0..],CMPREYE ;both eyes open, require right eye
            DIBEQ  [...1.1..],OFFTARG ;both eyes are closed. do not reward
			;DIBEQ  [...1.1..],CMPEITH;both shutters open, compare either eye
            JUMP   CMPEITH

CMPLEYE:    BGE    V16,criterio,ONTARG
            JUMP   OFFTARG

CMPREYE:    BGE    V17,criterio,ONTARG
            JUMP   OFFTARG

CMPEITH:    BGE    V16,criterio,ONTARG
            BGE    V17,criterio,ONTARG
            JUMP   OFFTARG

;*************************************

ONTARG:     DIGOUT [......1.]       ;turn on sonalert
            DIBEQ  [0.......],ONTRGNEW
ONTRGOLD:   DBNZ   counton,ONSTART ;tick off TOn counter until it hits 0
REWARD:     DIGOUT [.......1]      ;supply pulse to juice pump
REW2:       DELAY  TFeed           ;must delay even when not feeding to advance stim
            JUMP   rewstart
ONTRGNEW:   MOV    countoff, TBlank ;reset off-target counter
            JUMP   ONSTART

;*************************************

OFFTARG:    DIGOUT [......0.]       ;sonalert off
            DIBEQ  [0.......],OFFTGNEW
OFFTGOLD:   DBNZ   counton,OFF2     ;off trg but still must count for reward
            JUMP   REWARD
OFF2:       DBNZ   countoff,ONSTART ;off target for a short period? ok
            JUMP   REWSTART         ;off target too long... start over


OFFTGNEW:   DBNZ   countoff,ONSTART ;off target for a short period? ok
            DIGOUT [1.......]       ;send reset pulse to stop trial. off target too long
            DELAY  500
            DIGOUT [0.......]       ;turn off next stim pulse
            JUMP   REWSTART         ;stim should be reset now. start over

;*************************************

