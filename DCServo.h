#include <Arduino.h>
#include <Adafruit_DotStar.h>
#undef max
#undef min
#include "ArduinoC++BugFixes.h"
#include "ThreadHandler.h"

#include <Eigen.h>
#include "CurrentControlLoop.h"
#include "EncoderHandler.h"
#include "KalmanFilter.h"

#include "config/config.h"

#ifndef DC_SERVO_H
#define DC_SERVO_H

class ReferenceInterpolator
{
 public:
    ReferenceInterpolator();

    void set(float position, float velocity, float feedForward);

    void get(float& position, float& velocity, float& feedForward);

 private:
    float pos[2], vel[2], feed[2];
    uint16_t time[2];
};

class DCServo
{
 public:
    static DCServo* getInstance();

    void enable(bool b = true);

    void onlyUseMotorEncoder(bool b = true);

    void setReference(float pos, int16_t vel, int16_t feedForwardU = 0);

    float getPosition();

    int16_t getVelocity();
    
    int16_t getControlSignal();

    int16_t getCurrent();

    uint16_t getLoopNumber();

    int16_t getMotorPosition();

    bool runIdentTest1(int16_t amplitude);

    bool runIdentTest2(int16_t amplitude);

 private:
    DCServo();

    void controlLoop();

    void identTestLoop();

    int16_t setOutput(float u);

    bool controlEnabled;
    bool onlyUseMotorEncoderControl;

    Eigen::Matrix<float, 5, 1> L;

    uint16_t loopNumber;
    float rawMotorPos;
    float rawOutputPos;
    float outputPosOffset;
    float initialOutputPosOffset;
    Eigen::Vector3f x;

#ifdef SIMULATE
    Eigen::Vector3f xSim;
#endif

    int16_t current;
    float controlSignal;
    float uLimitDiff;

    ReferenceInterpolator refInterpolator;

    float Ivel;

    std::unique_ptr<CurrentControlLoop> currentControl;
    std::unique_ptr<EncoderHandlerInterface> motorEncoderHandler;
    std::unique_ptr<EncoderHandler> outputEncoderHandler;
    std::unique_ptr<KalmanFilter> kalmanFilter;

    Adafruit_DotStar dotStarLed;
    uint8_t dotstarState;
    uint8_t dotstarStateRequest;

    const int16_t testOutputArray[100] = {-1, -1, 1, 1, 1, -1, -1, 1, 1, 1, -1, -1, 1, -1, 1, 1, -1, 1, -1, -1, 1, -1, 1, 1, -1, 1, -1, -1, 1, -1, 1, -1, -1, 1, -1, 1, -1, 1, 1, 1, -1, -1, 1, 1, -1, -1, -1, 1, 1, -1, -1, 1, 1, 1, -1, 1, -1, 1, -1, -1, -1, 1, -1, 1, -1, 1, 1, 1, -1, -1, 1, -1, 1, -1, 1, -1, 1, -1, -1, 1, -1, 1, -1, 1, 1, -1, -1, -1, 1, 1, -1, 1, -1, 1, 1, -1, 1, 1, -1, -1};
    const int16_t testOutputArray2[100] = {-1, -1, 1, 1, -1, 1, 1, -1, 1, -1, 1, 1, -1, -1, -1, 1, 1, -1, 1, -1, 1, -1, -1, 1, -1, -1, 1, -1, 1, 1, -1, -1, 1, 1, 1, -1, 1, -1, 1, -1, -1, -1, 1, -1, 1, -1, 1, 1, 1, -1, -1, 1, 1, -1, -1, -1, 1, 1, -1, -1, 1, 1, 1, -1, 1, -1, 1, -1, -1, 1, -1, 1, -1, -1, 1, -1, 1, 1, -1, 1, -1, -1, 1, -1, 1, -1, 1, 1, -1, -1, 1, 1, 1, -1, -1, 1, 1, 1, -1, 1};

    enum IdentTestState
    {
        NORMAL_CONTROL,
        IDENT_TEST_1_INIT,
        IDENT_TEST_1,
        IDENT_TEST_1_COMP,
        IDENT_TEST_2_INIT,
        IDENT_TEST_2,
        IDENT_TEST_2_COMP
    };

    IdentTestState identTestState;
    size_t identTestArrayIndex;
    int16_t identTestAmplitude;
    int16_t pwmOutputOnDisabled;

    std::vector<Thread*> threads;
};
#endif
