#include "ControlTime.h"


ControlTime::ControlTime()
{
	// init.
	m_iDurTime = 0;
	m_bRT = true;
	m_dStepTime = 0.0;
}

void ControlTime::Initialize()
{
#ifdef WIN32
	// timeGetTimeの精度を向上
	// 精度の最小値，最大値の範囲を取得
	// （注）これがないとNT系は誤差が10msほどあるらしい．
	TIMECAPS timecaps;
	timeGetDevCaps(&timecaps, sizeof(timecaps));

	// 最小値に設定
	timeBeginPeriod(timecaps.wPeriodMin);
#else

#endif
}


void ControlTime::SetStartTime()
{

	if( m_bRT == true ) {
#ifdef WIN32
		m_dStartTime = timeGetTime();
#else
		gettimeofday( &m_dStartTime, &m_tz );
#endif
	}
	else {
#ifdef WIN32
		m_dStartTime = timeGetTime();
		m_dNowTime = m_dStartTime;
#else
		gettimeofday( &m_dStartTime, &m_tz );
		m_dNowTime = m_dStartTime;
#endif
	}
}

void ControlTime::SetNowTime()
{
	if( m_bRT == true ) {
#ifdef WIN32
		m_dNowTime = timeGetTime();
#else
		gettimeofday( &m_dNowTime, &m_tz );
#endif
	}
	else {
#ifdef WIN32
		m_dNowTime += m_dStepTime;
#else
		m_dNowTime.tv_usec += (int)(m_dStepTime * 1000);
#endif
	}
}

void ControlTime::SetDurTime()
{
#ifdef WIN32
	m_iDurTime = (int)(m_dNowTime - m_dStartTime);
#else
	m_iDurTime = (int)((m_dNowTime.tv_sec - m_dStartTime.tv_sec) * 1000 +
		(m_dNowTime.tv_usec - m_dStartTime.tv_usec) / 1000 );
#endif
}

void ControlTime::SleepTime(int ms)
{
	if( m_bRT == true ) {
#ifdef WIN32
		Sleep(ms);
#else
		usleep(ms * 1000);
#endif
	}
}

void ControlTime::GetStartTime(int &ms, int &ss, int &mm, int &hr, int &day)
{
#ifdef WIN32
	ms =          (int)m_dStartTime % 1000;
	ss =        (((int)m_dStartTime - ms) / 1000) % 60;
	mm =      (((((int)m_dStartTime - ms) / 1000) - ss) / 60) % 60;
	hr =    (((((((int)m_dStartTime - ms) / 1000) - ss) / 60) - mm ) / 60) % 24;
	day = (((((((((int)m_dStartTime - ms) / 1000) - ss) / 60) - mm ) / 60) - hr ) / 24);
#else
	ms = m_dStartTime.tv_usec / 1000;
	ss = m_dStartTime.tv_sec % 60;
	mm =      ((m_dStartTime.tv_sec - ss) / 60) % 60;
	hr =    ((((m_dStartTime.tv_sec - ss) / 60) - mm ) / 60 ) % 24;
	day = ((((((m_dStartTime.tv_sec - ss) / 60) - mm ) / 60 ) - hr ) / 24 );
#endif
}

