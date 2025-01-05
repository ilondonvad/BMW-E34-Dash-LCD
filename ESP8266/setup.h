void setup() {
  inputString.reserve(200);

  pinMode(DISP1, 1);
  pinMode(DISP2, 1);
  cs(0);
  Wire.begin();

  Serial.begin(115200);
  Serial.println("Serial OK");
  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed");
    return;
  }
  //MDNS.begin("tft");
  /*WiFi.mode(WIFI_STA);
  WiFi.begin("TRENDnet651", "avtomaster1");
  //WiFi.mode(WIFI_AP);
  //WiFi.softAP(ssid, password);

  Serial.println("FTP start");*/
  //ftpSrv.begin("esp8266", "123456");
  //server.begin();

  cs(3);
  tft.init(); // Инициализация TFT дисплея
  tft.setRotation(1);
  tft.invertDisplay(true);
  tft.fillScreen(TFT_BLACK);
  tft.loadFont("NotoSansBold15", LittleFS);
  cs(3);

  drawEngLoad();
  renderLoadProgres(100);

  i2cSend(250);
  //cs(0);
}
