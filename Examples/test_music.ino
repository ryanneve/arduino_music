/* Play Melody
 * -----------
 *
 *  2014 Ryan Neve
 *  Based upon
 * (cleft) 2005 D. Cuartielles for K3
 * Improvements:
 * Uses actoal note frequencies
 * Made into library
 * Increased scale range.
 * Program to play melodies stored in an array. 
 * Every melody_idx is a frequency duration pair.
 * Future Improvements:
 * Make every melody_idx three characters. <melody_idx><duration><&|#| > where & is flat, and # is sharp.
 * Could also change notation to <duration><A-G><scale=0-8><&|#| >
 */

char mario[] = "2d2A1f2c2d2A2d2c2f2d2A2c2d2A1f2c2d2A2A2g2p8p8p8p";
char scale[] = "2a2b2c2d2e2f2g2A2B2C2D2E2F2G4a";
char bingo[] = "1a1d1d1d1a1b1b1a1a1e1e1f1f2g2e2g2g1A1A2A2f2f1g1g2g2e2e1f1f1f1e1c1a1b1c2d1g9p";
// melody_idx length: 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0
//                                10                  20                  30                  40
uint8_t mario_count = 24;
uint8_t scale_count = 15;
uint8_t bingo_count = 45;

int speakerOut = 13;
static char MUSIC_NOTES[] =      {'a',   'b',   'c',   'd',   'e',   'f',   'g',   'A',   'B',   'C',   'D',   'E',   'F',   'G'};  
static float MUSIC_FREQUENCY[] = {220.00,246.94,261.63,293.66,329.63,349.23,392.00,440.00,493.88,523.25,587.33,659.25,698.46,783.99};
static uint16_t MUSIC_TONES[] = {2273, 2025, 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956, 851, 758, 716, 638};
static uint8_t SCALE_LEN = 14;
static uint16_t MUSIC_PWM = 500; // Originally 500 (256 is very quiet)
static uint16_t MUSIC_PAUSE_LENGTH = 500 ; // Originally 500. 
static uint16_t MUSIC_TEMPO = 90; // Originally 30. Higher is slower

class Music {
  public:
    Music(uint8_t music_pin);
    void setMusic(char * melody,uint16_t note_count);
    void buzzOn();
    void buzzOn(uint16_t pwm);
    void buzzOff();
    void playMelody();
  private:
    void _playNote(uint16_t melody_idx,float frequency,uint16_t tempo);
    uint8_t _music_pin;
    char * _melody;
    uint16_t _note_count;
};

Music::Music(uint8_t music_pin) {
  _music_pin = music_pin;
  analogWrite(_music_pin, 0);
}

void Music::setMusic(char * melody,uint16_t note_count) {
  _melody = melody; 
  _note_count = note_count;
}

void Music::buzzOn() {
  analogWrite(_music_pin,MUSIC_PWM);
}

void Music::buzzOn(uint16_t pwm) {
  analogWrite(_music_pin,pwm);
}

void Music::buzzOff() {
  analogWrite(_music_pin,0);
}

void Music::_playNote(uint16_t melody_idx, float frequency,uint16_t music_tempo) {
  uint32_t time_high = (uint32_t) 1000000 / (frequency * 2); 
  uint32_t note_length = (_melody[melody_idx*2] - '0') * music_tempo;
  for (uint16_t count3 = 0; count3 <= note_length; count3++) { // Controls duration
    if (_melody[melody_idx*2 + 1] == 'p') {
      // make a pause of a certain size
      analogWrite(_music_pin, 0);
      delayMicroseconds(MUSIC_PAUSE_LENGTH);
    }
    else {
      analogWrite(_music_pin,MUSIC_PWM);
      delayMicroseconds(time_high);
      analogWrite(_music_pin, 0);
      delayMicroseconds(time_high);
    }
  }
}
void Music::playMelody() {
  uint16_t melody_idx;
  uint16_t n_idx = 0;
  uint32_t time_high;
  uint16_t music_tempo;
  analogWrite(_music_pin, 0);
  for (melody_idx = 0; melody_idx < _note_count; melody_idx++) {
    Serial.println(melody_idx);
    for (n_idx=0;n_idx<SCALE_LEN;n_idx++) { // See what melody_idx this is
      if (MUSIC_NOTES[n_idx] == _melody[melody_idx*2 + 1]) {
        music_tempo = MUSIC_TEMPO + n_idx * 12; // Try to account for higher notes not lasting long enough.
        _playNote(melody_idx,MUSIC_FREQUENCY[n_idx],music_tempo);
      }
    }
  }
}


void setup(){
  Serial.begin(9600);
  Music speaker = Music(speakerOut);
  uint16_t  i;
  for (i = 0; i < 10 ; i++) {
    Serial.println(i);
    delay(100);
    speaker.buzzOn(5);
    delay(200);
    speaker.buzzOff();
    delay(100);
    speaker.buzzOn();
  }
    speaker.buzzOff();
  //speaker.setMusic(scale,scale_count);
  speaker.setMusic(bingo,bingo_count);
  //speaker.playMelody();
}
void loop(){
}