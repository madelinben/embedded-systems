#ifndef SYSTEM_SELF_TEST_H
#define SYSTEM_SELF_TEST_H

class SelfTest
{
    public:
        SelfTest();
        void TestRotaryEncoder();
        void TestLDR();
        void TestDHT11();
        bool DHT11Error;
        bool RotaryEncoderError;
        bool ScreenError;
        bool SDCardError;
        bool LRDError;
        bool RTCError;
};
#endif