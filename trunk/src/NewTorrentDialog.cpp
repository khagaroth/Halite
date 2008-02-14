
//         Copyright Eóin O'Callaghan 2006 - 2008.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "stdAfx.hpp"
#include "NewTorrentDialog.hpp"

LRESULT NewTorrentDialog::OnInitDialog(...)
{
	CenterWindow();
/*	SetWindowText(windowText_.c_str());
	prog_.Attach(GetDlgItem(IDC_PROG_PROGRESS));
	prog_.SetRange(0, 100);
	
	thread_ptr.reset(new thread(bind(&ProgressDialog::ProgressThread, this)));
*/		
	return TRUE;
}

void NewTorrentDialog::onCancel(UINT, int, HWND hWnd)
{
//	stop_ = true;
}

NewTorrentDialog::CWindowMapStruct* NewTorrentDialog::GetWindowMap()
{
	BEGIN_WINDOW_MAP_INLINE(NewTorrentDialog, 6, 6, 3, 3)
		WMB_HEAD(WMB_COL(_gap), WMB_COL(_exp), WMB_COL(120), WMB_COL(_gap)), 
		WMB_END() 
	END_WINDOW_MAP_INLINE()	}

void NewTorrentDialog::OnClose()
{
	if(::IsWindow(m_hWnd)) 
	{
		::DestroyWindow(m_hWnd);
	}
}