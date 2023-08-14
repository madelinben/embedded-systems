#include "systemSelfTest.h"
#include "DHT11Module.h"
#include "rotaryEncoderModule.h"

SelfTest::SelfTest()
{
    this->DHT11Error = false;
    this->RotaryEncoderError = false;
    this->ScreenError = false;
    this->SDCardError = false;
    this->LRDError = false;
    this->RTCError = false;
}

void SelfTest::TestRotaryEncoder()
{
    // this->RotaryEncoderError = checkRotaryEncoderConnection();
}

void SelfTest::TestLDR()
{

}

void SelfTest::TestDHT11()
{
    // this->DHT11Error = checkDHT11Connection();
}