
HardwareSerial SDS011(1);
unsigned int Pm25 = 0;
unsigned int Pm10 = 0;

void ProcessSerialData(){
  uint8_t mData = 0;
  uint8_t i = 0;
  uint8_t mPkt[10] = {0};
  uint8_t mCheck = 0;
  while (SDS011.available() > 0)  {
     // packet format: AA C0 PM25_Low PM25_High PM10_Low PM10_High 0 0 CRC AB
    mData = SDS011.read();
    delay(2);//wait until packet is received
//    Serial.println(mData);
//    Serial.println("*");
    if (mData == 0xAA) //head1 ok
    {
      mPkt[0] =  mData;
      mData = SDS011.read();
      if (mData == 0xc0) //head2 ok
      {
        mPkt[1] =  mData;
        mCheck = 0;
        for (i = 0; i < 6; i++) //data recv and crc calc
        {
          mPkt[i + 2] = SDS011.read();
          delay(2);
          mCheck += mPkt[i + 2];
        }
        mPkt[8] = SDS011.read();
        delay(1);
        mPkt[9] = SDS011.read();
        if (mCheck == mPkt[8]) //crc ok
        {
          SDS011.flush();
          SDS011.write(mPkt, 10);

          Pm25 = (uint16_t)mPkt[2] | (uint16_t)(mPkt[3] << 8);
          Pm10 = (uint16_t)mPkt[4] | (uint16_t)(mPkt[5] << 8);
          if (Pm25 > 9999)
            Pm25 = 9999;
          if (Pm10 > 9999)
            Pm10 = 9999;
//          Serial.println(Pm25);
//          Serial.println(Pm10);
          //get one good packet
          return;
        }
      }
    }
  }
}





static const byte SLEEPCMD[19] = {
0xAA, // head
0xB4, // command id
0x06, // data byte 1
0x01, // data byte 2 (set mode)
0x00, // data byte 3 (sleep)
0x00, // data byte 4
0x00, // data byte 5
0x00, // data byte 6
0x00, // data byte 7
0x00, // data byte 8
0x00, // data byte 9
0x00, // data byte 10
0x00, // data byte 11
0x00, // data byte 12
0x00, // data byte 13
0xFF, // data byte 14 (device id byte 1)
0xFF, // data byte 15 (device id byte 2)
0x05, // checksum
0xAB // tail
};
