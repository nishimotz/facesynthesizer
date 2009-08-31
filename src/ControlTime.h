#ifndef __CONTROLTIME_H__
#define __CONTROLTIME_H__

// ŽžŠÔŠl“¾‚Ì‚½‚ß‚ÌInclude
#ifdef WIN32
#include <windows.h>
#include <process.h>
#include <mmsystem.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif

#ifdef WIN32
#pragma warning (disable: 4786)
#endif

#include <iostream>
#include <vector>
#include <string>

class ControlTime {

public:
	ControlTime();
	~ControlTime() {}

private:

#ifdef WIN32
	double m_dStartTime, m_dNowTime;
#else
	timeval m_dStartTime, m_dNowTime;
#endif

#ifndef WIN32
struct timezone m_tz; 
#endif

	int m_iDurTime;
	bool m_bRT;
	double m_dStepTime;

public:
	void Initialize();

	void SetStartTime();
	void SetStartTime( int i );
	void SetNowTime();
	void SetDurTime();
	void SetDurTime( int i ) { m_iDurTime = i; }

	int GetDurTime() { return m_iDurTime; }
	int GetDurTimeWithProcess() {
		SetNowTime();
		SetDurTime();
		return m_iDurTime;
	}
	void SetRT(bool flag) { m_bRT = flag; }
	bool GetRT() { return m_bRT; }
	void SetStepTime(double stepTime) { m_dStepTime = stepTime; }
	double GetStepTime() { return m_dStepTime; }

	void SleepTime(int ms);
	void GetStartTime(int &ms, int &ss, int &mm, int &hr, int &day);
};


#endif //__CONTROLTIME_H__

