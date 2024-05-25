
/***
 *      ____|    \  |       |  /           |  _)               |             
 *      |       |\/ |       ' /     _` |   |   |   __ `__ \    __ \     _` | 
 *      __|     |   |       . \    (   |   |   |   |   |   |   |   |   (   | 
 *     _|      _|  _|      _|\_\  \__,_|  _|  _|  _|  _|  _|  _.__/   \__,_| 
 *                                                 by Z00keep 2024                          
 */

// Arduino polyphonic FM sound 
// * 31250 Hz sampling rate
// * 9-bit resolution
// * 4-fold polyphony (4 different tones can play simulatenously)
// * FM-synthesis with time-varying modulation amplitude
// * ADSR envelopes
// * 12 preset instruments
// rework of code by Rolf Oldeman Feb 2019 
// Licence CC BY-NC-SA 2.5 https://creativecommons.org/licenses/by-nc-sa/2.5/


//instrument definitions 
         
/*classic instrument from the original code 
#define ninstr 12           //   piano xlphn guitar cmbll bell funky vibr metal violin bass trumpt harm
unsigned int ldness[ninstr]  = {   64,   64,   64,   64,   64,   64,   64,   64,   64,   64,   64,   64}; // loudness   
unsigned int pitch0[ninstr]  = {   12,   12,   12,   12,   24,   24,    0,   12,   24,   12,   12,   24}; // pitch of key0         
unsigned int ADSR_a[ninstr]  = { 4096, 8192, 8192, 8192, 4096,  512,  512, 8192,  128,  128,  256,  256}; // attack parameter  
unsigned int ADSR_d[ninstr]  = {    8,   32,   16,   16,    8,   16,   16,    8,   16,   16,   64,   32}; // decay parameter   
unsigned int ADSR_s[ninstr]  = {    0,    0,    0,    0,    0,    0,    0,    0,  240,  240,  192,  192}; // sustain parameter 
unsigned int ADSR_r[ninstr]  = {   64,  128,   32,   32,   16,   32,   32,   32,   32,   32,   64,   64}; // release parameter 
unsigned int FM_inc[ninstr]  = {  256,  512,  768,  400,  200,   96,  528,  244,  256,  128,   64,  160}; // FM frequency wrt pitch
unsigned int FM_a1[ninstr]  =  {  128,  512,  512, 1024,  512,    0, 1024, 2048,  256,  256,  384,  256}; // FM amplitude start
unsigned int FM_a2[ninstr]  =  {   64,    0,  128,  128,  128,  512,  768,  512,  128,  128,  256,  128}; // FM amplitude end
unsigned int FM_dec[ninstr]  = {   64,  128,  128,  128,   32,  128,  128,  128,  128,  128,   64,   64}; // FM decay
*/

/*shorter attack*/
/*
 #define ninstr 12           //   piano xlphn guitar cmbll bell funky vibr metal violin bass trumpt harm
unsigned int ldness[ninstr]  = {   64,   64,   64,   64,   64,   64,   64,   64,   64,   64,   64,   64}; // loudness   
unsigned int pitch0[ninstr]  = {   12,   12,    0,   12,   24,   24,    0,   12,   24,   12,   12,   24}; // pitch of key0         
unsigned int ADSR_a[ninstr]  = { 4096, 8192, 8192, 8192, 4096,  512,  512, 8192,  4096,  8192,  4096,  8192}; // attack parameter  
unsigned int ADSR_d[ninstr]  = {    8,   32,   16,   16,    8,   16,   16,    8,   16,   16,   64,   32}; // decay parameter   
unsigned int ADSR_s[ninstr]  = {    0,    0,    0,    0,    0,    0,    0,    0,  240,  240,  192,  192}; // sustain parameter 
unsigned int ADSR_r[ninstr]  = {   128,  128,   128,   128,   256,   256,   128,   128,   64,   256,   64,   256}; // release parameter 
unsigned int FM_inc[ninstr]  = {  256,  512,  768,  400,  200,   96,  528,  244,  256,  128,   64,  160}; // FM frequency wrt pitch
unsigned int FM_a1[ninstr]  =  {  128,  512,  512, 1024,  512,    0, 1024, 2048,  256,  256,  384,  256}; // FM amplitude start
unsigned int FM_a2[ninstr]  =  {   64,    0,  128,  128,  128,  512,  768,  512,  128,  128,  256,  128}; // FM amplitude end
unsigned int FM_dec[ninstr]  = {   64,  128,  128,  128,   32,  128,  128,  128,  128,  128,   64,   64}; // FM decay
*/
/*weirder sound my favorite*/
#define ninstr 12           //   kora harm funky xlphn cmbll  bell vibr  metal guitar violin bass trumpt 
unsigned int ldness[ninstr]  = {   64,   64,   64,   64,   64,   64,   64,   64,   64,   64,    64,   64}; // loudness   
unsigned int pitch0[ninstr]  = {   24,   24,   12,   12,   12,   24,   12,   12,    0,   24,     0,   24}; // pitch of key0         
unsigned int ADSR_a[ninstr]  = { 8192, 8192, 1024, 8192, 8192, 4096, 4096, 8192, 8192, 4096,  8192, 4096}; // attack parameter  
unsigned int ADSR_d[ninstr]  = {   32,   32,   16,   32,   16,    8,   16,    8,   16,   16,    16,   64}; // decay parameter   
unsigned int ADSR_s[ninstr]  = {    0,  192,    0,    0,    0,    0,    0,    0,    0,  240,   240,  64}; // sustain parameter 
unsigned int ADSR_r[ninstr]  = {  128,  256,  128,  128,  128,   96,   96,  128,   96,   64,   256,  128}; // release parameter 
unsigned int FM_inc[ninstr]  = {   32,  160,  800,  256,  400,  200,  128,  128,  256,  256,   128,   16}; // FM frequency wrt pitch
unsigned int FM_a1[ninstr]  =  {  512,  256,   64,  512,  512,  512, 1024, 2048,  512,  256,   256,  384}; // FM amplitude start
unsigned int FM_a2[ninstr]  =  {    0,  128,  512,    0,  128,  128,  768,  512,  128,  128,   128,  128}; // FM amplitude end
unsigned int FM_dec[ninstr]  = {  128,   64,    8,  128,  128,   32,  128,   32,  128,  128,   128,   32}; // FM decay

//define the pitch2key mapping
#define keyC4   0
#define keyC4s  1
#define keyD4   2
#define keyD4s  3
#define keyE4   4
#define keyF4   5
#define keyF4s  6
#define keyG4   7
#define keyG4s  8
#define keyA4   9
#define keyA4s 10
#define keyB4  11
#define keyC5  12
#define keyC5s 13
#define keyD5  14
#define keyD5s 15
#define keyE5  16
#define keyF5  17
#define keyF5s  18
#define keyG5  19
#define keyG5s  20
#define keyA5  21
#define keyA5s  22
#define keyB5  23

#define nokey 255
#define instrkey 254

//define the pin to key mapping for 18-key keyboard 
/* si major*/
/*
#define pinD2 keyB5    //Arduino pin D2
#define pinD3 keyF5s    //Arduino pin D3
#define pinD4 keyC5s   //Arduino pin D4
#define pinD5 keyG4s    //Arduino pin D5
#define pinD6 keyE4    //Arduino pin D6
#define pinD7 keyF4s    //Arduino pin D7
#define pinB0 keyB4    //Arduino pin D8
#define pinB1 nokey    //Arduino pin D9  used for audio out
#define pinB2 keyE5   //Arduino pin D10
#define pinB3 keyG5s    //Arduino pin D11 
#define pinB4 instrkey //Arduino pin D12
*/

/* mi  majeur*/
#define pinD2 keyG5s    //Arduino pin D2
#define pinD3 keyE5    //Arduino pin D3
#define pinD4 keyB4   //Arduino pin D4
#define pinD5 keyF4s    //Arduino pin D5
#define pinD6 keyE4    //Arduino pin D6
#define pinD7 keyG4s    //Arduino pin D7
#define pinB0 keyC5s    //Arduino pin D8
#define pinB1 nokey    //Arduino pin D9  used for audio out
#define pinB2 keyF5s   //Arduino pin D10
#define pinB3 keyB5    //Arduino pin D11 
#define pinB4 instrkey //Arduino pin D12

//set up array with sine values in signed 8-bit numbers 
const float pi = 3.14159265;
char sine[256];
void setsine() {
  for (int i = 0; i < 256; ++i) {
    sine[i] = (sin(2 * 3.14159265 * (i + 0.5) / 256)) * 128;
  }
}

//setup frequencies/phase increments, starting at C3=0 to B6. (A4 is defined as 440Hz)
unsigned int tone_inc[48];
void settones() {
  for (byte i=0; i<48; i++){
    tone_inc[i]= 440.0 * pow(2.0, ( (i-21) / 12.0)) * 65536.0 / (16000000.0/512) + 0.5;
  }
}

byte butstatD=0;
byte butstatB=0;
byte butstatC=0;
byte prevbutstatD=0;
byte prevbutstatB=0;
byte prevbutstatC=0;

byte instr=0;

void setup() {

  //disable all inerrupts to avoid glitches
  noInterrupts();

  //setup the array with sine values
  setsine();

  //setup array with tone frequency phase increments
  settones();

  //Set a fast PWM signal on TIMER1A, 9-bit resolution, 31250Hz
  pinMode(9, OUTPUT);
  TCCR1A = 0B10000010; //9-bit fast PWM
  TCCR1B = 0B00001001;

  //setup pins for input with pull-up 
  /*if(pinD0 != nokey){DDRD &=~(1<<0);PORTD |=(1<<0);}; 
  if(pinD1 != nokey){DDRD &=~(1<<1);PORTD |=(1<<1);};*/ 
  if(pinD2 != nokey){DDRD &=~(1<<2);PORTD |=(1<<2);}; 
  if(pinD3 != nokey){DDRD &=~(1<<3);PORTD |=(1<<3);}; 
  if(pinD4 != nokey){DDRD &=~(1<<4);PORTD |=(1<<4);}; 
  if(pinD5 != nokey){DDRD &=~(1<<5);PORTD |=(1<<5);}; 
  if(pinD6 != nokey){DDRD &=~(1<<6);PORTD |=(1<<6);}; 
  if(pinD7 != nokey){DDRD &=~(1<<7);PORTD |=(1<<7);}; 
  if(pinB0 != nokey){DDRB &=~(1<<0);PORTB |=(1<<0);}; 
  if(pinB1 != nokey){DDRB &=~(1<<1);PORTB |=(1<<1);}; 
  if(pinB2 != nokey){DDRB &=~(1<<2);PORTB |=(1<<2);}; 
  if(pinB3 != nokey){DDRB &=~(1<<3);PORTB |=(1<<3);}; 
  if(pinB4 != nokey){DDRB &=~(1<<4);PORTB |=(1<<4);}; 
   
  //store button setting at startup
  butstatD = PIND;
  butstatB = PINB;
}


//initialize the main parameters of the pulse length setting
#define nch 4 //number of channels that can produce sound simultaneously
unsigned int phase[nch]  = {0,0,0,0};
int          inc[nch]    = {0,0,0,0};
byte         amp[nch]    = {0,0,0,0};
unsigned int FMphase[nch]= {0,0,0,0};
unsigned int FMinc[nch]  = {0,0,0,0};
unsigned int FMamp[nch]  = {0,0,0,0};

// main function (forced inline) to update the pulse length
inline void setPWM() __attribute__((always_inline));
inline void setPWM() {

  //wait for the timer to complete loop
  while ((TIFR1 & 0B00000001) == 0);

  //Clear(!) the overflow bit by writing a 1 to it
  TIFR1 |= 0B00000001;

  //increment the phases of the FM
  FMphase[0] += FMinc[0];
  FMphase[1] += FMinc[1];
  FMphase[2] += FMinc[2];
  FMphase[3] += FMinc[3];

  //increment the phases of the note
  phase[0] += inc[0];
  phase[1] += inc[1];
  phase[2] += inc[2];
  phase[3] += inc[3];

  //calculate the output value and set pulse width for timer2
  int val = sine[(phase[0]+sine[FMphase[0]>>8]*FMamp[0]) >> 8] * amp[0];
  val += sine[(phase[1]+sine[FMphase[1]>>8]*FMamp[1]) >> 8] * amp[1];
  val += sine[(phase[2]+sine[FMphase[2]>>8]*FMamp[2]) >> 8] * amp[2];
  val += sine[(phase[3]+sine[FMphase[3]>>8]*FMamp[3]) >> 8] * amp[3];

  //set the pulse length
  OCR1A = val/128 + 256;
}

//properties of each note played
byte         iADSR[nch]     = {0, 0, 0, 0}; 
unsigned int envADSR[nch]   = {0, 0, 0, 0}; 
unsigned int ADSRa[nch]     = {0, 0, 0, 0};
unsigned int ADSRd[nch]     = {0, 0, 0, 0};
unsigned int ADSRs[nch]     = {0, 0, 0, 0};
unsigned int ADSRr[nch]     = {0, 0, 0, 0};
byte         amp_base[nch]  = {0, 0, 0, 0};
unsigned int inc_base[nch]  = {0, 0, 0, 0};
unsigned int FMa0[nch]      = {0, 0, 0, 0};
int          FMda[nch]      = {0, 0, 0, 0};
unsigned int FMinc_base[nch]= {0, 0, 0, 0};
unsigned int FMdec[nch]     = {0, 0, 0, 0};
unsigned int FMexp[nch]     = {0, 0, 0, 0};
unsigned int FMval[nch]     = {0, 0, 0, 0};
byte         keych[nch]     = {0, 0, 0, 0}; 
unsigned int tch[nch]       = {0, 0, 0, 0}; 


// main loop. Duration of loop is determined by number of setPWM calls
// Each setPWMcall corresponds to 512 cylcles=32mus
// Tloop= 32mus * #setPWM. #setPWM=15 gives Tloop=0.48ms
void loop() {

  //read and interpret input buttons
  prevbutstatD = butstatD;
  prevbutstatB = butstatB;
  prevbutstatC = butstatC;
  butstatD = PIND;
  butstatB = PINB;
  butstatC = PINC;
  byte keypressed = nokey;
  byte keyreleased = nokey;
  if(butstatD!=prevbutstatD){
    if ( pinD2!=nokey and (butstatD & (1<<2)) == 0 and (prevbutstatD & (1<<2)) >  0 ) keypressed  = pinD2;
    if ( pinD2!=nokey and (butstatD & (1<<2)) >  0 and (prevbutstatD & (1<<2)) == 0 ) keyreleased = pinD2;
    if ( pinD3!=nokey and (butstatD & (1<<3)) == 0 and (prevbutstatD & (1<<3)) >  0 ) keypressed  = pinD3;
    if ( pinD3!=nokey and (butstatD & (1<<3)) >  0 and (prevbutstatD & (1<<3)) == 0 ) keyreleased = pinD3;
    if ( pinD4!=nokey and (butstatD & (1<<4)) == 0 and (prevbutstatD & (1<<4)) >  0 ) keypressed  = pinD4;
    if ( pinD4!=nokey and (butstatD & (1<<4)) >  0 and (prevbutstatD & (1<<4)) == 0 ) keyreleased = pinD4;
    if ( pinD5!=nokey and (butstatD & (1<<5)) == 0 and (prevbutstatD & (1<<5)) >  0 ) keypressed  = pinD5;
    if ( pinD5!=nokey and (butstatD & (1<<5)) >  0 and (prevbutstatD & (1<<5)) == 0 ) keyreleased = pinD5;
    if ( pinD6!=nokey and (butstatD & (1<<6)) == 0 and (prevbutstatD & (1<<6)) >  0 ) keypressed  = pinD6;
    if ( pinD6!=nokey and (butstatD & (1<<6)) >  0 and (prevbutstatD & (1<<6)) == 0 ) keyreleased = pinD6;
    if ( pinD7!=nokey and (butstatD & (1<<7)) == 0 and (prevbutstatD & (1<<7)) >  0 ) keypressed  = pinD7;
    if ( pinD7!=nokey and (butstatD & (1<<7)) >  0 and (prevbutstatD & (1<<7)) == 0 ) keyreleased = pinD7;
  }
  if(butstatB!=prevbutstatB){
    if ( pinB0!=nokey and (butstatB & (1<<0)) == 0 and (prevbutstatB & (1<<0)) >  0 ) keypressed  = pinB0;
    if ( pinB0!=nokey and (butstatB & (1<<0)) >  0 and (prevbutstatB & (1<<0)) == 0 ) keyreleased = pinB0;
    if ( pinB1!=nokey and (butstatB & (1<<1)) == 0 and (prevbutstatB & (1<<1)) >  0 ) keypressed  = pinB1;
    if ( pinB1!=nokey and (butstatB & (1<<1)) >  0 and (prevbutstatB & (1<<1)) == 0 ) keyreleased = pinB1;
    if ( pinB2!=nokey and (butstatB & (1<<2)) == 0 and (prevbutstatB & (1<<2)) >  0 ) keypressed  = pinB2;
    if ( pinB2!=nokey and (butstatB & (1<<2)) >  0 and (prevbutstatB & (1<<2)) == 0 ) keyreleased = pinB2;
    if ( pinB3!=nokey and (butstatB & (1<<3)) == 0 and (prevbutstatB & (1<<3)) >  0 ) keypressed  = pinB3;
    if ( pinB3!=nokey and (butstatB & (1<<3)) >  0 and (prevbutstatB & (1<<3)) == 0 ) keyreleased = pinB3;
    if ( pinB4!=nokey and (butstatB & (1<<4)) == 0 and (prevbutstatB & (1<<4)) >  0 ) keypressed  = pinB4;
    if ( pinB4!=nokey and (butstatB & (1<<4)) >  0 and (prevbutstatB & (1<<4)) == 0 ) keyreleased = pinB4;
    
  }
  
  setPWM(); //#1

  //change instrument if instrument select button is pressed
  if ( keypressed==instrkey) {
    instr++;
    if (instr>=ninstr) instr=0;
    keypressed=keyE4;
  }
  if (keyreleased==instrkey) keyreleased=keyE4;
  
  setPWM(); //#2

  //find the best channel to start a new note
  byte nextch = 255;
  //first check if the key is still being played
  if (iADSR[0] > 0 and keypressed == keych[0])nextch = 0;
  if (iADSR[1] > 0 and keypressed == keych[1])nextch = 1;
  if (iADSR[2] > 0 and keypressed == keych[2])nextch = 2;
  if (iADSR[3] > 0 and keypressed == keych[3])nextch = 3;
  //then check for an empty channel
  if (nextch == 255) {
    if (iADSR[0] == 0)nextch = 0;
    if (iADSR[1] == 0)nextch = 1;
    if (iADSR[2] == 0)nextch = 2;
    if (iADSR[3] == 0)nextch = 3;
  }
  //otherwise use the channel with the longest playing note
  if (nextch == 255) {
    nextch = 0;
    if (tch[0] > tch[nextch])nextch = 0;
    if (tch[1] > tch[nextch])nextch = 1;
    if (tch[2] > tch[nextch])nextch = 2;
    if (tch[3] > tch[nextch])nextch = 3;
  }

  setPWM(); //#3

  //initiate new note if needed
  if (keypressed != nokey) {
    phase[nextch]=0;
    amp_base[nextch] = ldness[instr];
    inc_base[nextch] = tone_inc[pitch0[instr]+keypressed];
    ADSRa[nextch]=ADSR_a[instr];
    ADSRd[nextch]=ADSR_d[instr];
    ADSRs[nextch]=ADSR_s[instr]<<8;
    ADSRr[nextch]=ADSR_r[instr];
    iADSR[nextch] = 1;
    FMphase[nextch]=0;
    FMinc_base[nextch] = ((long)inc_base[nextch]*FM_inc[instr])/256;
    FMa0[nextch] = FM_a2[instr];
    FMda[nextch] = FM_a1[instr]-FM_a2[instr];
    FMexp[nextch]=0xFFFF;
    FMdec[nextch]=FM_dec[instr];
    keych[nextch] = keypressed;
    tch[nextch] = 0;
  }

  setPWM(); //#4

  //stop a note if the button is released
  if (keyreleased != nokey) {
    if (keych[0] == keyreleased)iADSR[0] = 4;
    if (keych[1] == keyreleased)iADSR[1] = 4;
    if (keych[2] == keyreleased)iADSR[2] = 4;
    if (keych[3] == keyreleased)iADSR[3] = 4;
  }
  
  setPWM(); //#5

  //update FM decay exponential 
  FMexp[0]-=(long)FMexp[0]*FMdec[0]>>16;
  FMexp[1]-=(long)FMexp[1]*FMdec[1]>>16;
  FMexp[2]-=(long)FMexp[2]*FMdec[2]>>16;
  FMexp[3]-=(long)FMexp[3]*FMdec[3]>>16;
  
  setPWM(); //#6
  
  //adjust the ADSR envelopes
  for (byte ich = 0; ich < nch; ich++) {
    if (iADSR[ich] == 4) {
      if (envADSR[ich] <= ADSRr[ich]) {
        envADSR[ich] = 0;
        iADSR[ich] = 0;
      }
      else envADSR[ich] -= ADSRr[ich];
    }
    if (iADSR[ich] == 2) {
      if (envADSR[ich] <= (ADSRs[ich] + ADSRd[ich])) {
        envADSR[ich] = ADSRs[ich];
        iADSR[ich] = 3;
      }
      else envADSR[ich] -= ADSRd[ich];
    }
    if (iADSR[ich] == 1) {
      if ((0xFFFF - envADSR[ich]) <= ADSRa[ich]) {
        envADSR[ich] = 0xFFFF;
        iADSR[ich] = 2;
      }
      else envADSR[ich] += ADSRa[ich];
    }
    tch[ich]++;
    setPWM(); //#7-10
  }

  //update the tone for channel 0
  amp[0] = (amp_base[0] * (envADSR[0] >> 8)) >> 8;
  inc[0] = inc_base[0];
  FMamp[0] = FMa0[0] + ((long)FMda[0] * FMexp[0]>>16);
  FMinc[0] = FMinc_base[0];
  setPWM(); //#11

  //update the tone for channel 1
  amp[1] = (amp_base[1] * (envADSR[1] >> 8)) >> 8;
  inc[1] = inc_base[1];
  FMamp[1] = FMa0[1] + ((long)FMda[1] * FMexp[1]>>16);
  FMinc[1] = FMinc_base[1];
  setPWM(); //#12

  //update the tone for channel 2
  amp[2] = (amp_base[2] * (envADSR[2] >> 8)) >> 8;
  inc[2] = inc_base[2];
  FMamp[2] = FMa0[2] + ((long)FMda[2] * FMexp[2]>>16);
  FMinc[2] = FMinc_base[2];
  setPWM(); //#13

  //update the tone for channel 3
  amp[3] = (amp_base[3] * (envADSR[3] >> 8)) >> 8;
  inc[3] = inc_base[3];
  FMamp[3] = FMa0[3] + ((long)FMda[3] * FMexp[3]>>16);
  FMinc[3] = FMinc_base[3];
  setPWM(); //#14

  //update counters
  tch[0]++;
  tch[1]++;
  tch[2]++;
  tch[3]++;

  setPWM(); //#15

}
