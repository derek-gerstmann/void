/***************************************************************************************************
//
// License:    The MIT License
// 
// Copyright (c) 2011. Derek Gerstmann, The University of Western Australia.
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//  
***************************************************************************************************/

#ifndef __GSCR_TIMER_H__
#define __GSCR_TIMER_H__

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <getopt.h>
#include <time.h>
#include <sys/time.h>

/**************************************************************************************************/

#ifdef __APPLE__
    #include <mach/mach_time.h>
    typedef uint64_t                    time_delta_t;
    typedef mach_timebase_info_data_t   frequency_t;
#else
    typedef struct timeval              time_delta_t;
    typedef double                      frequency_t;
#endif

/**************************************************************************************************/

static double 
SubtractTimeInSecondsForHost(
    time_delta_t a, time_delta_t b, frequency_t frequency)
{
    double delta;
#ifdef __APPLE__
    time_delta_t difference = a - b;                                    
    delta = 1e-9 * (double) frequency.numer / (double)frequency.denom;  
    delta = delta * difference;                                     
#else
    (void) (frequency);                                                 
    delta = ((double)a.tv_sec + 1.0e-6 * (double)a.tv_usec);    
    delta -= ((double)b.tv_sec + 1.0e-6 * (double)b.tv_usec);   
#endif
    return delta;
}

static frequency_t 
GetTimerFrequencyForHost(void)
{
    frequency_t frequency;
#ifdef __APPLE__
    mach_timebase_info(&(frequency));
#else
    frequency = 1.0;
#endif
    return frequency;
}

static time_delta_t
GetCurrentTimeForHost(void)
{
    time_delta_t current;
#ifdef __APPLE__
    current = mach_absolute_time();
#else
    gettimeofday(&current, NULL);
#endif
    return current;
}

/**************************************************************************************************/

#endif /* __GSCR_TIMER_H__ */

