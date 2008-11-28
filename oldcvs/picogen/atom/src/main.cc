/***************************************************************************
 *            main.cc
 *
 *  Copyright  2008  Sebastian Mach
 *  phresnel@gmail.com
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 3 of the License, or (at your
 *  option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
 
 
#include <wx/wx.h>
#include "MkheightmapWxDialog.h"

class wxWidgetsApp : public wxApp
{
public:
    wxWidgetsApp();
    virtual ~wxWidgetsApp();
    virtual bool OnInit();
};

DECLARE_APP(wxWidgetsApp)





IMPLEMENT_APP(wxWidgetsApp)

wxWidgetsApp::wxWidgetsApp()
{
}

wxWidgetsApp::~wxWidgetsApp()
{
}

bool wxWidgetsApp::OnInit()
{
    MkheightmapWxDialog* dialog = new MkheightmapWxDialog ((wxWindow*)0);
    dialog ->Show();
    SetTopWindow( dialog );
    return true;
}
