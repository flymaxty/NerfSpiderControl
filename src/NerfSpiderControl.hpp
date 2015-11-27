/**
 * Copyright 2015, MakerCollider.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/

#pragma once

#include <pthread.h>

#include "mraa.hpp"

#define BARBETTE_CLOCK              mraa::INTEL_EDISON_MINIBOARD_J18_7
#define BARBETTE_COUNTER_CLOCK      mraa::INTEL_EDISON_MINIBOARD_J17_1
#define FOOT_CLOCK                  mraa::INTEL_EDISON_MINIBOARD_J18_1
#define FOOT_COUNTER_CLOCK          mraa::INTEL_EDISON_MINIBOARD_J18_8
#define SHOOT_CLOCK                 mraa::INTEL_EDISON_MINIBOARD_J17_9
#define SHOOT_COUNTER_CLOCK         mraa::INTEL_EDISON_MINIBOARD_J17_7
#define ENCODER_PIN                 mraa::INTEL_EDISON_MINIBOARD_J19_5
#define LASER_PIN                   mraa::INTEL_EDISON_MINIBOARD_J19_6


class NerfSpiderControl
{
public:
    mraa::Result m_result;

    mraa::Gpio* m_laser;

    mraa::Gpio* m_encoder;
    uint16_t m_counter;

    mraa::Pwm* m_barbetteClock;
    mraa::Pwm* m_barbetteCounterClock;

    mraa::Pwm* m_footClock;
    mraa::Pwm* m_footCounterClock;

    mraa::Gpio* m_shootClock;
    mraa::Gpio* m_shootCounterClock;

    bool running;
public:
    NerfSpiderControl();
    ~NerfSpiderControl();

    //Barbette Function
    void barbetteRotate(float in_speed, float in_theta);
    void barbetteRotate(float in_speed);
    void barbetteStop();

    //Foot Function
    void footWalk(float in_speed);
    void footStop();

    //Shoot Function
    void shootControl();
    void shootControl(uint8_t in_bulletNumber);
    void shooterControl();
    void shootAllStop();

    //Laser Function
    void laserControl(bool in_enable);

    pthread_t m_encoderThread;
    static void* s_encoderThreadFunc(void* in_data);
};
