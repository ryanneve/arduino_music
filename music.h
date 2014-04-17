class Music {
  public:
    Music(uint8_t music_pin);
    void setMusic(char * melody,uint16_t note_count);
    void playMelody();
    void playNote(uint16_t melody_idx,float frequency,uint16_t tempo);
    void buzzOn();
    void buzzOn(uint16_t pwm);
    void buzzOff();
    bool toggleMute() { _muted = !_muted; return _muted;}
  private:
    uint8_t _music_pin;
    char * _melody;
    uint16_t _note_count;
    bool _muted;
};
