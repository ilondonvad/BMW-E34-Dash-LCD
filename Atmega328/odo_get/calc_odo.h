//const uint8_t _0[] PROGMEM = {113, 115, 33, 114, 112, 34, 35}; // Последняя цифра в суточном пробеге

byte num(byte by) {             // Функция парсинга маски битов данных в цифры
  if (by == 3 || by == 6)       return 1;
  else if (by == 181 || by == 211) return 2;
  else if (by == 151 || by == 87)  return 3;
  else if (by == 71 || by == 102)  return 4;
  else if (by == 214 || by == 117) return 5;
  else if (by == 246 || by == 245)  return 6;
  else if (by == 19 || by == 7) return 7;
  else if (by == 247)  return 8;
  else if (by == 215 || by == 119) return 9;
  else if (by == 243 || by == 183) return 0;
}


ISR(INT0_vect) {
  buf[0] |= ((PIND & (1 << PD4)) >> PD4);
  buf[3] = (buf[3] << 1) | (buf[2] >> 31);
  buf[2] = (buf[2] << 1) | (buf[1] >> 31);
  buf[1] = (buf[1] << 1) | (buf[0] >> 31);
  buf[0] <<= 1;
  asm("nop");
}



ISR(INT1_vect) {
  if (EIMSK & 1) {
    asm("cli");
    EIMSK &= ~(1 << INT0);
    byte _1 = num(((buf[0] >> 1 & 15) << 4) | (buf[2] >> 17 & 7));
    byte _2 = num((buf[0] >> 5 & 7) | ((buf[2] >> 20 & 15) << 4));
    byte _3 = num(((buf[0] >> 8 & 15) << 4) | (buf[2] >> 24 & 7));
    byte _4 = num((buf[0] >> 12 & 7) | ((buf[2] >> 27 & 15) << 4));
    byte _5 = num(((buf[0] >> 15 & 15) << 4) | (((buf[2] >> 31) | ((buf[3] & 3) << 1)) & 7));
    byte _6 = num((buf[0] >> 19 & 7) | ((buf[3] >> 2 & 15) << 4));

    byte _7 = num(((buf[0] >> 22 & 15) << 4) | (buf[3] >> 6 & 7));
    byte _8 = num((buf[0] >> 26 & 7) | ((buf[3] >> 9 & 15) << 4));
    byte _9 = num(((((buf[0] >> 29)) | ((buf[1] << 3)) & 15) << 4) | (buf[3] >> 13 & 7));

    odo = (_1 * 100000) + (_2 * 10000) + (_3 * 1000) + (_4 * 100) + (_5 * 10) + (_6);
    dayOdo = (_7 * 100) + (_8 * 10) + (_9);
    /*
        if (od < 999999 && dOd<999) {
          odo = filterData(od);
          dayOdo = filterData2(dOd);
        }
    */

    /*
        for (byte i = 0; i < 5; i++) {
          if (filter[i] == od && filterDayOdo[i] == dOd) {
            filterOdoCount[i]++;
            //break;
          }
          else if (filter[i] != od && filter[i] == 0) {
            filter[i] = od;
            filterDayOdo[i] = dOd;
            filterOdoCount[i]++;
            break;
          }
        }

        byte index = 0;
        byte num = 0;
        for (byte i = 0; i < 5; i++) {
          if (filter[i] > num) {
            index = i;
            num = filter[i];
          }
        }

        if (num > 200 && filterDayOdo[index] < 999 && filter[index] < 999999) {
          dayOdo = filterDayOdo[index];
          odo = filter[index];
          memset(filter, 0, sizeof(filter));
          index = 0;
          num = 0;
        }

        if (filterCount > 250) {
          filterCount = 0;
          memset(filter, 0, sizeof(filter));
        }

        filterCount++;*/
    buf[3] = 0;
    buf[2] = 0;
    buf[1] = 0;
    buf[0] = 0;
    asm("nop");
    asm("sei");
  }
  else {
    EIMSK |= (1 << INT0);
  }
}
