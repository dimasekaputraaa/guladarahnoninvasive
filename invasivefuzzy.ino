#include "Wire.h"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

const int ledPin = 9;
const int bt = 6;  // PIN TOMBOL
int btx, adcsensor, datafix, kalibrasi, i;
float fix;

float kondisiNormal(float nilai) {
  float nilaiNormal = 0;
  if (nilai <= 100) {
    nilaiNormal = 1;
  }
  else {
    if (nilai <= 101) {
      nilaiNormal = (101 - nilai);
    }
    else {
      nilaiNormal = 0;
    }
  }
  return nilaiNormal;
}

float kondisiPraDiabet(float nilai) {
  float nilaiPraDiabet = 0;
  if (nilai >= 100 && nilai <= 126) {
    if (nilai > 100 && nilai < 126) {
      nilaiPraDiabet = 1;
    }
    else {
      if (nilai >= 100 && nilai <= 101) {
        nilaiPraDiabet = (nilai - 100);
      }
      else {
        if (nilai >= 125 && nilai <= 126) {
          nilaiPraDiabet = (125 - nilai);
        }
        else {
          nilaiPraDiabet = 0;
        }
      }
    }
  }
  return nilaiPraDiabet;
}

float kondisiDiabet(float nilai) {
  float nilaiDiabet = 0;
  if (nilai >= 126) {
    nilaiDiabet = 1;
  }
  else {
    if (nilai >= 125 && nilai <= 126) {
      nilaiDiabet = (nilai - 125);
    }
    else {
      nilaiDiabet = 0;
    }
  }
  return nilaiDiabet;
}


void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(bt, INPUT_PULLUP);
}

void loop() {
  digitalWrite(13, HIGH);
  Serial.print("mg/dl= ");
  Serial.println(kalibrasi);
  btx = digitalRead(bt);
  float normal = kondisiNormal(kalibrasi);
  float pradiabet = kondisiPraDiabet(kalibrasi);
  float diabetes = kondisiDiabet (kalibrasi);
  String hasil;
  if (normal == 1) {
    hasil = "Normal";
  }
  else if (normal <= 0.5 && normal > 0) {
    hasil = "Normal";
  }
  else if (pradiabet == 1 || normal > 0.5) {
    hasil = "Pradiabet";
  }
  else if (diabetes == 1 || pradiabet > 0.5) {
    hasil = "Diabetes";
  }
  if (btx == 0) {
    i = 0;
    datafix = 0;
    fix = 0;
    proses();
  }
}

void proses() {
  adcsensor = analogRead(A0);
  Serial.print("ADC= ");
  Serial.println(adcsensor);
  datafix = adcsensor + datafix;
  delay(200);
  if (i >= 20) {
    fix = datafix / 20.0;  // untuk data ke kalibrasi
    kalibrasi = ((fix + 17.62) / 0.2917 / 10);  // data yang sudah di kalibrasi
    return;
  }
  i++;
  proses();
}
