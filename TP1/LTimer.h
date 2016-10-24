/*
 * LTimer.h
 *
 *  Created on: Oct 24, 2016
 *      Author: franco
 */

#ifndef LTIMER_H_
#define LTIMER_H_

#include <stdio.h>
#include <string>
#include <sstream>
#include <stdint.h>
#include <cstdint>
#include <SDL2/SDL.h>
//The application time based timer
class LTimer
{
    public:
		//Initializes variables
		LTimer();
			//The various clock actions
		void start();
		void stop();
		void pause();
		void unpause();
			//Gets the timer's time
		uint32_t getTicks();
			//Checks the status of the timer
		bool isStarted();
		bool isPaused();
	    private:
		//The clock time when the timer started
		uint32_t mStartTicks;
			//The ticks stored when the timer was paused
		uint32_t mPausedTicks;
			//The timer status
		bool mPaused;
		bool mStarted;
};
#endif /* LTIMER_H_ */
