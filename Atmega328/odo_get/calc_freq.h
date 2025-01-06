ISR(PCINT0_vect) {
  unsigned long currentTime = micros();
  for (int i = 0; i < 3; i++) {
    bool currentState = PINB & (1 << i); // Считываем текущее состояние пина
    if (currentState && !lastState[i]) { // Проверяем фронт LOW -> HIGH
      period[i] = currentTime - lastTime[i];
      lastTime[i] = currentTime;
    }
    if (currentTime - lastTime[i] > 1000000UL) {
      period[i] = 0;
    }
    lastState[i] = currentState; // Обновляем предыдущее состояние
  }
}
