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

#include <iostream>
#include <unistd.h>
#include <sys/select.h>
#include <pthread.h>
#include <math.h>

#include <mraa.hpp>

#include "NerfSpiderControl.hpp"

NerfSpiderControl::NerfSpiderControl()
{
    //Laser init
    m_laser = new mraa::Gpio(LASER_PIN);
    if(m_laser == NULL)
        mraa::printError(mraa::ERROR_UNSPECIFIED);
    m_result = m_laser->dir(mraa::DIR_OUT);
    if(m_result != mraa::SUCCESS)
        mraa::printError(m_result); 

    //Encoder init
    m_encoder = new mraa::Gpio(ENCODER_PIN);
    if(m_encoder == NULL)
        mraa::printError(mraa::ERROR_UNSPECIFIED);
    m_result = m_encoder->dir(mraa::DIR_IN);
    if(m_result != mraa::SUCCESS)
        mraa::printError(m_result);
    m_counter = 0;

    //Barbette init
    m_barbetteClock = new mraa::Pwm(BARBETTE_CLOCK);
    if(m_barbetteClock == NULL)
        mraa::printError(mraa::ERROR_UNSPECIFIED);
    m_result = m_barbetteClock->enable(true);
    if(m_result != mraa::SUCCESS)
        mraa::printError(m_result);
    m_barbetteCounterClock = new mraa::Pwm(BARBETTE_COUNTER_CLOCK);
    if(m_barbetteCounterClock == NULL)
        mraa::printError(mraa::ERROR_UNSPECIFIED);
    m_result = m_barbetteCounterClock->enable(true);
    if(m_result != mraa::SUCCESS)
        mraa::printError(m_result);

    //Foot init
    m_footClock = new mraa::Pwm(FOOT_CLOCK);
    if(m_footClock == NULL)
        mraa::printError(mraa::ERROR_UNSPECIFIED);
    m_result = m_footClock->enable(true);
    if(m_result != mraa::SUCCESS)
        mraa::printError(m_result);
    m_footCounterClock = new mraa::Pwm(FOOT_COUNTER_CLOCK);
    if(m_footCounterClock == NULL)
        mraa::printError(mraa::ERROR_UNSPECIFIED);
    m_result = m_footCounterClock->enable(true);
    if(m_result != mraa::SUCCESS)
        mraa::printError(m_result);

    //Shoot init
    m_shootClock = new mraa::Gpio(SHOOT_CLOCK);
    if(m_shootClock == NULL)
        mraa::printError(mraa::ERROR_UNSPECIFIED);
    m_result = m_shootClock->dir(mraa::DIR_OUT);
    if(m_result != mraa::SUCCESS)
        mraa::printError(m_result);
    m_shootCounterClock = new mraa::Gpio(SHOOT_COUNTER_CLOCK);
    if(m_shootCounterClock == NULL)
        mraa::printError(mraa::ERROR_UNSPECIFIED);
    m_result = m_shootCounterClock->dir(mraa::DIR_OUT);
    if(m_result != mraa::SUCCESS)
        mraa::printError(m_result);

    running = true;
    pthread_create(&m_encoderThread, NULL, s_encoderThreadFunc, (void*)this);
}

NerfSpiderControl::~NerfSpiderControl()
{
    barbetteStop();
    footStop();
    shootAllStop();
    laserControl(false);
}

void* NerfSpiderControl::s_encoderThreadFunc(void* in_data)
{
    NerfSpiderControl* control = (NerfSpiderControl*) in_data;
    int last = 0, now = 0;
    while(control->running)
    {
        now = control->m_encoder->read();
        if(now != last)
        {
            last = now;
            control->m_counter++;
        }
        usleep(10);
    }
}

void NerfSpiderControl::barbetteRotate(float in_speed, float in_theta)
{
    m_counter = 0;
    barbetteRotate(in_speed);
    while(m_counter < in_theta*2.15);
    barbetteStop();
    m_counter = 0;
}

void NerfSpiderControl::barbetteRotate(float in_speed)
{
    int direction = in_speed/fabs(in_speed);
    in_speed = fabs(in_speed);

    if(in_speed > 1)
        in_speed = 1;

    if(direction > 0)
    {
        m_barbetteClock->write(0);
        m_barbetteCounterClock->write(in_speed);
    }
    else
    {
        m_barbetteClock->write(in_speed);
        m_barbetteCounterClock->write(0);
    }
}

void NerfSpiderControl::barbetteStop()
{
    m_barbetteClock->write(0);
    m_barbetteCounterClock->write(0);
}

void NerfSpiderControl::footWalk(float in_speed)
{
    int direction = in_speed/fabs(in_speed);
    in_speed = fabs(in_speed);

    if(in_speed > 1)
        in_speed = 1;

    if(direction > 0)
    {
        m_footClock->write(0);
        m_footCounterClock->write(in_speed);
    }
    else
    {
        m_footClock->write(in_speed);
        m_footCounterClock->write(0);
    }
}

void NerfSpiderControl::footStop()
{
    m_footClock->write(0);
    m_footCounterClock->write(0);
}

void NerfSpiderControl::shootControl()
{
    m_shootClock->write(1);
    m_shootCounterClock->write(0);
}

void NerfSpiderControl::shootControl(uint8_t in_bulletNumber)
{
    shootControl();
    sleep(in_bulletNumber * 1);
    shootAllStop();
}

void NerfSpiderControl::shooterControl()
{
    m_shootClock->write(0);
    m_shootCounterClock->write(1);
}

void NerfSpiderControl::shootAllStop()
{
    m_shootClock->write(0);
    m_shootCounterClock->write(0);
}

void NerfSpiderControl::laserControl(bool in_enable)
{
    m_laser->write(in_enable);
}
