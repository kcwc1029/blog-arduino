// 超級瑪利兄弟主題曲

#include <analogWave.h>
analogWave wave(DAC);

int tempo = 200;
int beatNote = 2;
int wholenote = (60000 * beatNote) / tempo;

int melody[][2] = {
  {76, 4}, {76, 4}, {0, 4}, {76, 4},
  {0, 4}, {72, 4}, {76, 4}, {0, 4},
  {79, 4}, {0, 4}, {0, 2}
};

int noteTotal = sizeof(melody) / sizeof(melody[0]);
int noteCounter = 0;

void setup() {
  wave.sine(10);
  wave.stop();
}

void loop() {
  int currentNote = melody[noteCounter][0];
  int noteDuration = wholenote / melody[noteCounter][1];
  float noteFreq =  440 * pow(2, ((currentNote - 69) / 12.0));

  if (currentNote != 0) {
    wave.freq(noteFreq);
    delay(noteDuration * 0.9);
    wave.stop();
    delay(noteDuration * 0.1);
  } else {  // 休止符
    wave.stop();
    delay(noteDuration);
  }

  noteCounter++;
  if (noteCounter > noteTotal) {
    noteCounter = 0;
    wave.stop();
    delay(1000);
  }
}