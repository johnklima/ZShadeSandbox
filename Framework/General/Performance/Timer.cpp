#include "Timer.h"
#include <math.h>
using ZShadeSandboxGlobal::Timer;
//================================================================================================================
//================================================================================================================
Timer::Timer()
{
}
//================================================================================================================
Timer::Timer(const Timer& other)
{
}
//================================================================================================================
Timer::~Timer()
{
}
//================================================================================================================
bool Timer::Initialize()
{
	// Check to see if this system supports high performance timers.
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);
	if(m_frequency == 0)
	{
		return false;
	}

	// Find out how many times the frequency counter ticks every millisecond.
	m_ticksPerMs = (float)(m_frequency / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);




	m_ConstantTime = 0;


	// Set the thread affinity of the calling thread so that it stays on the
    // same logical processor. QPC can give bogus results if you don't do this.
    HANDLE threadHandle = GetCurrentThread();
    //SetThreadAffinityMask(threadHandle, 1) != 0;

    // Query for the performance counter frequency
    LARGE_INTEGER largeInt;
    QueryPerformanceFrequency(&largeInt);
    frequency = largeInt.QuadPart;
    frequencyD = static_cast<double>(frequency);

    // Init the elapsed time
    QueryPerformanceCounter(&largeInt);
    startTime = largeInt.QuadPart;
    elapsed = largeInt.QuadPart - startTime;
    elapsedF = static_cast<float>(elapsed);
    elapsedSeconds = elapsed / frequency;
    elapsedSecondsD = elapsed / frequencyD;
    elapsedSecondsF = static_cast<float>(elapsedSecondsD);
    elapsedMilliseconds = static_cast<INT64>(elapsedSecondsD * 1000);
    elapsedMillisecondsD = elapsedSecondsD * 1000;
    elapsedMillisecondsF = static_cast<float>(elapsedMillisecondsD);
    elapsedMicroseconds = static_cast<INT64>(elapsedMillisecondsD * 1000);
    elapsedMicrosecondsD = elapsedMillisecondsD * 1000;
    elapsedMicrosecondsF = static_cast<float>(elapsedMillisecondsD);

    delta = 0;
    deltaF = 0;
    deltaMilliseconds = 0;
    deltaMillisecondsF = 0;
    deltaMicroseconds = 0;
    deltaMicrosecondsF = 0;


	return true;
}
//================================================================================================================
void Timer::Frame()
{
	INT64 currentTime;
	float timeDifference;


	// Query the current time.
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	// Calculate the difference in time since the last time we queried for the current time.
	timeDifference = (float)(currentTime - m_startTime);

	// Calculate the frame time by the time difference over the timer speed resolution.
	m_frameTime = timeDifference / m_ticksPerMs;

	// Restart the timer.
	m_startTime = currentTime;


	m_ConstantTime = fmod((m_ConstantTime + 0.01f), INT_MAX);


	LARGE_INTEGER largeInt;
    QueryPerformanceCounter(&largeInt);
    currentTime = largeInt.QuadPart - startTime;
    delta = currentTime - elapsed;
    deltaF = static_cast<float>(deltaF);
    deltaSeconds = delta / frequency;
    deltaSecondsD = delta / frequencyD;
    deltaSecondsF = static_cast<float>(deltaSecondsD);
    deltaMillisecondsD = deltaSecondsD * 1000;
    deltaMilliseconds = static_cast<INT64>(deltaMillisecondsD);
    deltaMillisecondsF = static_cast<float>(deltaMillisecondsD);
    deltaMicrosecondsD = deltaMillisecondsD * 1000;
    deltaMicroseconds = static_cast<INT64>(deltaMicrosecondsD);
    deltaMicrosecondsF = static_cast<float>(deltaMicrosecondsD);

    elapsed = currentTime;
    elapsedF = static_cast<float>(elapsed);
    elapsedSeconds = elapsed / frequency;
    elapsedSecondsD = elapsed / frequencyD;
    elapsedSecondsF = static_cast<float>(elapsedSecondsD);
    elapsedMilliseconds = static_cast<INT64>(elapsedSecondsD * 1000);
    elapsedMillisecondsD = elapsedSecondsD * 1000;
    elapsedMillisecondsF = static_cast<float>(elapsedMillisecondsD);
    elapsedMicroseconds = static_cast<INT64>(elapsedMillisecondsD * 1000);
    elapsedMicrosecondsD = elapsedMillisecondsD * 1000;
    elapsedMicrosecondsF = static_cast<float>(elapsedMillisecondsD);

	return;
}
//================================================================================================================
float Timer::GetTime()
{
	return m_frameTime;
}
//================================================================================================================
INT64 Timer::ElapsedSeconds() const
{
    return elapsedSeconds;
}
//================================================================================================================
float Timer::ElapsedSecondsF() const
{
    return elapsedSecondsF;
}
//================================================================================================================
double Timer::ElapsedSecondsD() const
{
    return elapsedSecondsD;
}
//================================================================================================================
INT64 Timer::DeltaSeconds() const
{
    return deltaSeconds;
}
//================================================================================================================
float Timer::DeltaSecondsF() const
{
    return deltaSecondsF;
}
//================================================================================================================
double Timer::DeltaSecondsD() const
{
    return deltaSecondsD;
}
//================================================================================================================
INT64 Timer::ElapsedMilliseconds() const
{
    return elapsedMilliseconds;
}
//================================================================================================================
float Timer::ElapsedMillisecondsF() const
{
    return elapsedMillisecondsF;
}
//================================================================================================================
double Timer::ElapsedMillisecondsD() const
{
    return elapsedMillisecondsD;
}
//================================================================================================================
INT64 Timer::DeltaMilliseconds() const
{
    return deltaMilliseconds;
}
//================================================================================================================
float Timer::DeltaMillisecondsF() const
{
    return deltaMillisecondsF;
}
//================================================================================================================
double Timer::DeltaMillisecondsD() const
{
    return deltaMillisecondsD;
}
//================================================================================================================
INT64 Timer::ElapsedMicroseconds() const
{
    return elapsedMicroseconds;
}
//================================================================================================================
float Timer::ElapsedMicrosecondsF() const
{
    return elapsedMicrosecondsF;
}
//================================================================================================================
double Timer::ElapsedMicrosecondsD() const
{
    return elapsedMicrosecondsD;
}
//================================================================================================================
INT64 Timer::DeltaMicroseconds() const
{
    return deltaMicroseconds;
}
//================================================================================================================
float Timer::DeltaMicrosecondsF() const
{
    return deltaMicrosecondsF;
}
//================================================================================================================
double Timer::DeltaMicrosecondsD() const
{
    return deltaMicrosecondsD;
}
//================================================================================================================
float Timer::ConstantTime() const
{
	return m_ConstantTime;
}
//================================================================================================================