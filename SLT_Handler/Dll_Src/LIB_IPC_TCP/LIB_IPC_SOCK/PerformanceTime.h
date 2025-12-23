#pragma once

class CPerformanceTime
{
	LARGE_INTEGER st,et,frq,ovrHead;

public:
	/* Constructor */
	CPerformanceTime()
	{
		QueryPerformanceFrequency(&frq);
		StartTmChk();
		EndTmChk();
		ovrHead.QuadPart = et.QuadPart-st.QuadPart;
	}
	/* GetPerformanceTime */
	LONGLONG GetPerformanceTime()
	{
		LARGE_INTEGER t;
		QueryPerformanceCounter(&t);
		return t.QuadPart;
	}
	/* Start Time Check */
	/*virtual*/ LONGLONG StartTmChk()
	{
		QueryPerformanceCounter(&st);
		et = st;
		return st.QuadPart;
	}
	/* End Time Check */
	/*virtual*/ LONGLONG EndTmChk()
	{
		QueryPerformanceCounter(&et);
		return et.QuadPart;
	}

	/* End-Start Time Check */
	double GetTmDistanceMS()
	{
		return (et.QuadPart - st.QuadPart - ovrHead.QuadPart) / (frq.QuadPart/1000.0);
	}
};
