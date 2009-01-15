
//         Copyright Eoin O'Callaghan 2006 - 2008.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once 

#include "stdAfx.hpp"
#include "atldlgx.h"

namespace WTLx
{

typedef ATL::CWinTraits<WS_VISIBLE|WS_POPUP|WS_CAPTION|WS_SYSMENU|DS_CENTER, WS_EX_DLGMODALFRAME> RadioPaneTraits;

class CRadioPaneDialog : 
	public WTL::CIndirectDialogImpl<
	CRadioPaneDialog
	> // CIndirectDialogImpl
{
	typedef CRadioPaneDialog thisClass;
public:

	CRadioPaneDialog() {}

	BEGIN_MSG_MAP_EX(thisClass)
	END_MSG_MAP()
};

}