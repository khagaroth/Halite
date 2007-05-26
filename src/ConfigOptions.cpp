
#include "stdAfx.hpp"
#include <boost/xpressive/xpressive.hpp>
namespace xp = boost::xpressive;
namespace fs = boost::filesystem;

#include "ConfigOptions.hpp"
#include "ProgressDialog.hpp"

void BitTorrentOptions::onFilterImport(UINT, int, HWND hWnd)
{
	CSSFileDialog dlgOpen(TRUE, NULL, NULL, OFN_HIDEREADONLY, L"eMule ipfilter.dat. (*.dat)|*.dat|", m_hWnd);
	
	if (dlgOpen.DoModal() == IDOK) 
	{
		ProgressDialog progDlg(L"Importing IP filters...", bind(
			&hal::BitTorrent::ip_filter_import_dat, &hal::bittorrent(), path(hal::to_utf8(dlgOpen.m_ofn.lpstrFile)), _1, true));
		progDlg.DoModal();
	}
}	

BOOL GeneralOptions::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
	lang_list_.Attach(GetDlgItem(IDC_LANGLIST));
	lang_list_.AddString(L"English");
	lang_list_.SetCurSel(0);
	
	if (fs::is_directory(hal::app().exe_path().branch_path()))
	{
		fs::wdirectory_iterator end_iter;
		
		for (fs::wdirectory_iterator dir_itr(hal::app().exe_path().branch_path());
				dir_itr != end_iter; ++dir_itr )
		{
			if (fs::is_regular(dir_itr->status()))
			{
				xp::wsregex rex = xp::wsregex::compile(L"[\\s\\w]+\\.[dD][lL]{2}");
				xp::wsmatch what;

				if(xp::regex_match(dir_itr->path().leaf(), what, rex))
				{
					HMODULE hMod = ::LoadLibraryEx(dir_itr->path().string().c_str(), 0, LOAD_LIBRARY_AS_DATAFILE);

					const int buffer_size = 512;
					boost::array<wchar_t, buffer_size> buffer;
					::LoadString(hMod, HALITE_LANGUAGE, buffer.elems, buffer_size);
					
					wstring lang_name(buffer.elems);
					lang_map_[lang_name] = dir_itr->path().leaf();
					int index = lang_list_.AddString(lang_name.c_str());
					
					if (dir_itr->path().leaf() == halite().dll_) lang_list_.SetCurSel(index);
					
					::FreeLibrary(hMod);
				}
			}
		}
	}
	return DoDataExchange(false);
}
