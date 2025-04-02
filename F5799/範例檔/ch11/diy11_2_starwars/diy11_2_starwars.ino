// 動手做11-2：星際大戰主題曲

#include <analogWave.h>
analogWave wave(DAC);

int melody[][2] = {
  {65, 2}, {72, 2}, {71, 12}, {69, 12}, {67, 12}, {77, 2}, {72, 4}, 
  {71, 12}, {69, 12}, {67, 12}, {77, 2}, {72, 4}, {71, 12}, 
  {69, 12}, {71, 12}, {67, 2}, {60, 8*1.5}, {60, 16}
};

int tempo = 120;  // 節拍
int beatNote = 4;  // 一拍的音符
int wholenote = (60000 * beatNote) / tempo;

int noteTotal = sizeof(melody) / sizeof(melody[0]);
int noteIndex = 0;

void setup() {
  wave.sine(10);
  wave.stop();
}

void loop() {
  int currentNote = melody[noteIndex][0];  // 取出一個音
  float noteFreq =  440 * pow(2, ((currentNote - 69) / 12.0));  // 求出音符的頻率
  int noteDuration = wholenote / melody[noteIndex][1];
  
  wave.freq(noteFreq);
  delay(noteDuration * 0.9);
  wave.stop();
  delay(noteDuration * 0.1);

  noteIndex++;
  if (noteIndex > noteTotal) {
    noteIndex = 0;
    wave.stop();
    delay(1000);
  }
}