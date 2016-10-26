
// stdafx.cpp : source file that includes just the standard includes
// z25.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#include "PlotWnd.hpp"

IMPLEMENT_DYNAMIC(PlotWnd, CWnd)

BEGIN_MESSAGE_MAP(PlotWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_CREATE()
	//ON_WM_NCHITTEST()
END_MESSAGE_MAP()