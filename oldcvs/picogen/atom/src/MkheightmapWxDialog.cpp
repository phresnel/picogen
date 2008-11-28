/***************************************************************************
 *            MkheightmapWxDialog.cpp
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
 
 
#include "MkheightmapWxDialog.h"

#include <iostream>

#define wxUSE_ABOUTDLG 1
//#include <wx/aboutdlg.h>
//#include <wx/generic/aboutdlgg.h>

#include <foreign/SciLexer.h>

#include <iostream>
#include <sstream>
#include <fstream>


namespace {
    const wxString hsKeywords [] = {
        wxString(_("<"))
        , wxString(_("<="))
        , wxString(_(">"))
        , wxString(_(">="))
        , wxString(_("="))
        , wxString(_("<>"))
        , wxString(_("+")) // '^' is missing    
        , wxString(_("-")) // '^' is missing    
        , wxString(_("*")) // '^' is missing    
        , wxString(_("/")) // '^' is missing    
        , wxString(_("inv"))
        , wxString(_("sin"))
        , wxString(_("cos"))
        , wxString(_("floor"))
        , wxString(_("abs"))
        , wxString(_("trunc"))
        , wxString(_("frac"))
        , wxString(_("neg"))
        , wxString(_("sqrt"))
        , wxString(_("not"))
        , wxString(_("and"))
        , wxString(_("or"))
        , wxString(_("xor"))
        , wxString(_("min"))
        , wxString(_("max"))
        , wxString(_("delta"))
        , wxString(_("?"))
        , wxString(_("lerp"))
        , wxString(_("Noise"))
        , wxString(_("LayeredNoise"))
        , wxString(_("Layers "))
    };
    const int numHsKeywords = sizeof (hsKeywords) / sizeof (hsKeywords [0]);
}

MkheightmapWxDialog::MkheightmapWxDialog( wxWindow* parent )
:
MkheightmapWxDialogGui( parent )
{
    scintilla->SetFocus();
    wxFont f ( 9, 76, 90, 90, false, wxT("Monospace") );
    scintilla->StyleSetFont( wxSCI_STYLE_DEFAULT, f );
    
    scintilla->SetLexer (wxSCI_LEX_LISP);
  
    wxString hskwCat;
    for (int i=0; i<numHsKeywords; ++i) {
        hskwCat += hsKeywords [i] + wxString (_(" "));
    }
    scintilla->SetKeyWords (0, hskwCat);
    
    scintilla->StyleSetForeground (SCE_LISP_KEYWORD, wxColour (_T("BLUE")));
    scintilla->StyleSetForeground (SCE_LISP_OPERATOR, wxColour (255,128,64));
    scintilla->StyleSetForeground (SCE_LISP_NUMBER, wxColour (0,128,0));
    
    scintilla->StyleSetForeground (wxSCI_STYLE_BRACELIGHT, wxColour (_T("GREEN")));
    scintilla->StyleSetForeground (wxSCI_STYLE_BRACEBAD, wxColour (_T("RED")));
    
    scintilla->StyleSetForeground (wxSCI_STYLE_INDENTGUIDE, wxColour (64,64,64));
    
    scintilla->SetViewWhiteSpace (false);
    scintilla->SetTabIndents (false);
    scintilla->SetUseTabs (false);
    
    scintilla->SetText(wxString (_("    (-  1 \n\
        (^  (-  1\n\
                (abs ([2 LayeredNoise \n\
                    frequency(5) \n\
                    layercount(8) \n\
                    persistence((* 0.4 (-1 (abs x))))\n\
                 ] x y))) \n\
            3))")));
}

void MkheightmapWxDialog::UpdateTextWithTemplate (const wxString &tpl)
{
    scintilla->ReplaceSelection (tpl);
}

void MkheightmapWxDialog::OnMenu_Execute( wxCommandEvent& event )
{
    OnExecute (event);
}

void MkheightmapWxDialog::OnAutoformat( wxCommandEvent& event )
{
    #define doIndent() do { for (int i=0; i<indent*tablen; ++i) { formatted+=' '; } } while(0)
    const int tablen = 4;
    int indent=0;
    /*    
    wxString code = scintilla->GetText();
    wxString formatted;

    bool hasDoneLineBreak = false;
    for (size_t i=0; i<code.Length(); ++i) {        
        // Eat whitespace.
        int numSpace = 0;
        while ((code [i] == ' ' || code [i] == '\t' || code [i] == '\n' || code [i] == '\r') && (i < code.Length())) {
            ++i;
            ++numSpace;
        }
        if (0<numSpace) {
            if (!hasDoneLineBreak) {
                hasDoneLineBreak = true;
                formatted += '\n';
                doIndent ();
            }
        } else {
            hasDoneLineBreak = false;
        }

        // Format?
        if (i < code.Length()) {
            switch (code [i]) {
                case '(':
                    ++indent;
                    if (!hasDoneLineBreak)
                        formatted += '\n';
                    formatted += '(';
                    formatted += '\n';
                    doIndent ();
                    hasDoneLineBreak = true;
                    break;
                case ')':                    
                    indent -= indent>0 ? 1 : 0;
                    formatted += '\n';
                    doIndent ();
                    formatted += ')';
                    formatted += '\n';
                    doIndent ();
                    hasDoneLineBreak = true;
                    break;                
                default:
                    formatted += code [i];                
                    break;
            };
        }
    }
    scintilla->SetText(formatted);    
    */
    
    wxString code = scintilla->GetText();
    wxString formatted;
    
    int c = 0;
    while (c < code.Length()) {
        std::cout << "c=" << c << std::endl;
        bool wasKw = false;        
        for (int i=0; i<numHsKeywords && !wasKw; ++i) {            
            const int kwLen = hsKeywords [i].Length();
            std::cout << "[" << i << "](" << kwLen << ")" << code.Mid (c, kwLen).mb_str() << "==" << hsKeywords [i].mb_str() << "?" << std::endl;            
            if (hsKeywords [i] == code.Mid (c, kwLen)) {                
                formatted += hsKeywords [i] + _("\n");
                ++indent;
                doIndent ();                
                c += kwLen;
                wasKw = true;
            }            
        }
        if (!wasKw) {
            formatted += code [c];
            ++c;
        }
    }
    std::cout << std::endl;
    
    scintilla->SetText(formatted);
    #undef doIndent
}

void MkheightmapWxDialog::OnFast1MakeChoice ( wxCommandEvent& event )
{
    UpdateTextWithTemplate (fast1->GetStringSelection());
}

void MkheightmapWxDialog::OnFast2MakeChoice ( wxCommandEvent& event )
{
    UpdateTextWithTemplate (fast2->GetStringSelection());
}

void MkheightmapWxDialog::OnFast3MakeChoice ( wxCommandEvent& event )
{
    UpdateTextWithTemplate (fast3->GetStringSelection());
}

void MkheightmapWxDialog::OnFast4MakeChoice ( wxCommandEvent& event )
{
    UpdateTextWithTemplate (fast4->GetStringSelection());
}

void MkheightmapWxDialog::OnFast5MakeChoice ( wxCommandEvent& event )
{
    UpdateTextWithTemplate (fast5->GetStringSelection());
}

void MkheightmapWxDialog::OnFast6MakeChoice ( wxCommandEvent& event )
{
    if (wxString(_("\"Noise (cosine-filter)\"")) == fast6->GetStringSelection()) {
        UpdateTextWithTemplate (
            wxString (_("([2 Noise seed(42) frequency(16) filter(cosine)] x y)"))
        );
    } else if (wxString(_("\"Noise (bilinear-filter)\"")) == fast6->GetStringSelection()) {
        UpdateTextWithTemplate (
            wxString (_("([2 Noise seed(42) frequency(16) filter(bilinear)] x y)"))
        );
    } else if (wxString(_("\"Noise (nearest-filter)\"")) == fast6->GetStringSelection()) {
        UpdateTextWithTemplate (
            wxString (_("([2 Noise seed(42) frequency(16) filter(nearest)] x y)"))
        );
    } else if (wxString(_("\"Layers\"")) == fast6->GetStringSelection()) {
        UpdateTextWithTemplate (
            wxString (_("([2 Layers base( ([2 Noise] x y) )] x y)"))
        );
    } else if (wxString(_("\"Layers (more parameters)\"")) == fast6->GetStringSelection()) {
        UpdateTextWithTemplate (
            wxString (_("([2 Layers base( ([2 Noise] x y) )  persistence(0.5) layercount(8)] x y)"))
        );
    } else if (wxString(_("\"Layers (more parameters!)\"")) == fast6->GetStringSelection()) {
        UpdateTextWithTemplate (
            wxString (_("([2 Layers base( ([2 Noise] x y) )  persistence(0.5) layercount(8)  frequency(2)] x y)"))
        );
    } else if (wxString(_("\"Layered Noise\"")) == fast6->GetStringSelection()) {
        UpdateTextWithTemplate (
            wxString (_("([2 LayeredNoise seed(42) frequency(4) layercount(8) filter(cosine) persistence(0.6)] x y)"))
        );
    }     
}

void MkheightmapWxDialog::OnPresets1 ( wxCommandEvent& event )
{
    // Presets from http://picogen.org/wiki/index.php?title=Examples_and_Interesting_Programs_for_mkheightmap
    if (wxString(_("Edges")) == presets1->GetStringSelection()) {
        UpdateTextWithTemplate (
            wxString (_("\n\
    (^  (-  1 \n\
        (abs ([2 LayeredNoise \n\
                 frequency(15) \n\
                 layercount(8)\n\
                 persistence((* 0.4 (-1 (abs x))))\n\
              ] x y))) \n\
    3)\n\
"))
        );
    } else if (wxString(_("Rivers")) == presets1->GetStringSelection()) {
        UpdateTextWithTemplate (
            wxString (_("\n\
    (-  1 \n\
        (^  (-  1\n\
                (abs ([2 LayeredNoise \n\
                    frequency(15) \n\
                    layercount(8) \n\
                    persistence((* 0.4 (-1 (abs x))))\n\
                 ] x y))) \n\
            3))\n\
"))
        );
    } else if (wxString(_("Interesting")) == presets1->GetStringSelection()) {
        UpdateTextWithTemplate (
            wxString (_("\n\
    (lerp (sin (* 10 x)) \n\
      (sin (* 10 y)) \n\
      (^ (- 1\n\
            (abs ([2 LayeredNoise \n\
                     frequency(15) \n\
                     layercount(8) \n\
                     persistence((* 0.4 (-1 (abs x))))\n\
                  ] x y)))\n\
          3))\n\
"))
        );
    } else if (wxString(_("Funky Noise")) == presets1->GetStringSelection()) {
        UpdateTextWithTemplate (
            wxString (_("\n\
    (?  (>  0.1 \n\
        (+  (sin (* 30 x)) \n\
            (sin (* 30 y))))\n\
        (*  0.2 \n\
            (sin (* y 500)))\n\
        (^  (- 1\n\
                (abs (  [2 LayeredNoise \n\
                            frequency(15) \n\
                            layercount(8)\n\
                            persistence((* 0.4 (-1 (abs x))))\n\
                        ] x y))) \n\
            3))\n\
"))
        );
    } else if (wxString(_("A bit like Marbles")) == presets1->GetStringSelection()) {
        UpdateTextWithTemplate (
            wxString (_("\n\
(sin (* 3.141\n\
        (^ (- 1\n\
              (abs ([2 LayeredNoise \n\
                       frequency(15)\n\
                       layercount(8)\n\
                       persistence((* 0.4 (-1 (abs x))))\n\
                    ] x y)))\n\
           3)))\n\
"))
        );
    } else if (wxString(_("Sketchy")) == presets1->GetStringSelection()) {
        UpdateTextWithTemplate (
            wxString (_("\n\
(sin (* 20 (^ (- 1\n\
                 (abs ([2 LayeredNoise \n\
                          frequency(15)\n\
                          layercount(8)\n\
                          persistence((* 0.4 (-1 (abs x))))\n\
                       ] x y)))\n\
              3)))\n\
"))
        );
    } else if (wxString(_("I can no more")) == presets1->GetStringSelection()) {
        UpdateTextWithTemplate (
            wxString (_("\n\
(sin (* (/ (* 100 \n\
             ([2 LayeredNoise frequency(10)] x y)) \n\
          (^ y x))\n\
        (sqrt (+ (^ (- x 0.5) 2)\n\
                (^ (- y 0.5) 2)))))\n\
"))
        );
    } else if (wxString(_("I can no more 2")) == presets1->GetStringSelection()) {
        UpdateTextWithTemplate (
            wxString (_("\n\
(sin (* (/ (* (^ (- 1\n\
                    (abs ([2 LayeredNoise \n\
                             frequency(15) \n\
                             layercount(8) \n\
                             persistence((* 0.4 (-1 (abs x))))\n\
                          ] x y)))\n\
                 3)\n\
              (^ (- 1\n\
                    (abs ([2 LayeredNoise\n\
                             frequency(15)\n\
                             layercount(8)\n\
                             persistence((* 0.4 (-1 (abs x))))\n\
                          ] x y))) \n\
                 3))\n\
           (^ y x))\n\
        (sqrt (+ (^ (- x 0.5) 2) \n\
                 (^ (- y 0.5) 2)))))\n\
"))
        );
    } else if (wxString(_("Weird")) == presets1->GetStringSelection()) {
        UpdateTextWithTemplate (
            wxString (_("\n\
(sin (* (/ 100 (^ y x))\n\
        (sqrt (+ (^ (- x 0.5) 2)\n\
                 (^ (- y 0.5) 2)))))  \n\
"))
        );
    } else if (wxString(_("Sin/Cos-Checkers")) == presets1->GetStringSelection()) {
        UpdateTextWithTemplate (
            wxString (_("\n\
(+ (sin (* 100 x))\n\
   (cos (* 100 y)))\n\
"))
        );
    } else if (wxString(_("Another-Checkers")) == presets1->GetStringSelection()) {
        UpdateTextWithTemplate (
            wxString (_("\n\
(sqrt (* (sin (* 20 x))\n\
         (cos (* 20 y))))\n\
"))
        );
    } else if (wxString(_("Funky")) == presets1->GetStringSelection()) {
        UpdateTextWithTemplate (
            wxString (_("\n\
(sin (* 20\n\
        (sqrt (* (sin (* 20 x))\n\
                 (cos (* 20 y)))))) \n\
"))
        );
    } else if (wxString(_("This shalt be a heightmap?!")) == presets1->GetStringSelection()) {
        UpdateTextWithTemplate (
            wxString (_("\n\
(sin (* 20\n\
        (sqrt (+ (sin (* 20 x))\n\
                 (cos (* 20 y))))))\n\
"))
        );
    } else if (wxString(_("Interf-0")) == presets1->GetStringSelection()) {
        UpdateTextWithTemplate (
            wxString (_("\n\
(+ (* 2\n\
      (+ (cos (* y x))\n\
         (sin (* x 20)))) \n\
    (sin (* 200\n\
            (* x y))))\n\
"))
        );
    } else if (wxString(_("Interf-1")) == presets1->GetStringSelection()) {
        UpdateTextWithTemplate (
            wxString (_("\n\
(+ (* 2\n\
      (+ (cos (* y 40))\n\
         (sin (* x 20))))\n\
   (sin (* 200 \n\
           (* x y))))    \n\
"))
        );
    } else if (wxString(_("Interf-2")) == presets1->GetStringSelection()) {
        UpdateTextWithTemplate (
            wxString (_("\n\
(+ (* 2 \n\
      (* (cos (* y 90)) \n\
         (sin (* x 30))))\n\
   (sin (* 200 \n\
           (* x y))))\n\
"))
        );
    } 
}

void MkheightmapWxDialog::OnExecute( wxCommandEvent& event )
{
	wxString x = scintilla->GetText();
    // TODO: find better de-weaponing
    x.Replace (_("\""), _(" "));
    x.Replace (_("\\"), _(" "));
    x.Replace (_("`"), _(" "));
    
    wxString x_usrbin = wxString (_("picogen mkheightmap -Lhs \"")) 
        + x
        + wxString(_("\" -p ")) // preview
        + wxString::Format (wxString(_(" -a %i ")), antialiasing->GetValue ()) // antialiasing
        + (normalise->IsChecked() ? wxString(_(" -n ")) : wxString(_(""))) // normalise?
    ;
    wxMessageDialog (this, x_usrbin);
    wxString x_currentfolder = wxString (_("./") + x_usrbin);
    
    // try picogen in current folder
    {
        wxString statusText = x_currentfolder;
        statusText.Replace (_("\n"), _(" "));    
        SetStatusText(statusText);
    }
    
    int i;
    if (0 != (i=wxExecute (x_currentfolder, wxEXEC_SYNC))) {        
        // retry picogen in /usr/bin        
        {
            wxString statusText = x_usrbin;
            statusText.Replace (_("\n"), _(" "));    
            SetStatusText(statusText);
        }
        
        if (0 != (i=wxExecute (x_usrbin, wxEXEC_SYNC))) {
            wxMessageDialog msg (this, 
                wxString (_("An error occured while running the command \"")) + x_usrbin + wxString(_("\"."))
            );
        } else {            
        }
    } else {        
    }
    scintilla->SetFocus();
}


void MkheightmapWxDialog::OnQuickPreview( wxCommandEvent& event )
{
    //const wxString tmpfilename (_("/tmp/picogen-quick-preview.ssdf"));
    
    {
        using namespace std;
        ofstream tmpfile;    
        tmpfile.open ("/tmp/picogen-quick-preview.ssdf");
        tmpfile << "// auto generated picogen scene file for quick-preview\n";
        
        // brdf for the water
        tmpfile << "list {\n";
            
        tmpfile << "    brdf = specular_distorted-height(\n";
        tmpfile << "        reflectance:0.8; \n";
        tmpfile << "        code:  ( + 1 ( * 1.0 ( * (  [2 LayeredNoise frequency(0.05) persistence(0.55) levelEvaluationFunction(x) layercount(6) filter(cosine) seed(321) ] x (*y 5.0) ) ( * 1.0 (  [2 LayeredNoise frequency(0.005) persistence(0.55) levelEvaluationFunction(x) layercount(6) filter(cosine) seed(132) ] x y ) ) ) ) ) ) \n";
        
        tmpfile << "    // the water\n";
        tmpfile << "    hs-heightfield (\n";
        tmpfile << "        code: 1;\n";
        tmpfile << "        size:130000,0.5,130000;\n";
        tmpfile << "        material: hs( {const-rgb(1.0,1.0,1.0)},( * 0.0 (abs(   [2 LayeredNoise frequency(64) layercount(6) persistence(0.5) filter(cosine)] xy ) ) ) ) ;\n";
        tmpfile << "        center:0,-1600,0;\n";
        tmpfile << "        resolution:8\n";
        tmpfile << "    )\n";
        
        
        tmpfile << "    // the actual height-field\n";
        tmpfile << "    brdf = lambertian(reflectance:0.9)   \n";
        tmpfile << "    hs-heightfield (        \n";
        tmpfile << "        material: hs( { const-rgb(0.3,0.8,0.2), const-rgb(1.0,1.0,1.0) }, ( + 0  ( * 1.0 (abs(   [2 LayeredNoise  frequency(40) layercount(12) persistence(0.7)  filter(cosine)]xy) ) ) ) ) ;                \n";
        tmpfile << "        code:  ";
        //tmpfile << "            ( ^ (   [2 LayeredNoise frequency(4) persistence(0.55) levelEvaluationFunction(x) layercount(12) filter(cosine) seed(55) ] x y ) 1 )\n";
        //tmpfile << "            ;\n";
        tmpfile << scintilla->GetText().mb_str();
        tmpfile << ";\n";
        //tmpfile << "        resolution: 256; \n";
        tmpfile << "        resolution: " << (1<<(1+heightmapSize->GetSelection())) << "; \n";
        tmpfile << "        center:     0, -1400, 0; \n";
        tmpfile << "        size:       20000, 1500, 20000 \n";
        tmpfile << "    )\n";
            
        tmpfile << "}\n";
        
        switch (cameraType->GetSelection ()) {
            case 0: {
                float yaw, pitch, roll, x, y, z;                
                GetYprOrientation (yaw, pitch, roll);
                GetYprPosition (x, y, z);
                tmpfile << "camera-yaw-pitch-roll("
                    << "position: " << x << "," << y << "," << z << ";"
                    << "yaw:" << yaw << "; "
                    << "pitch:" << pitch << "; "
                    << "roll:" << roll << ")\n "
                ;
                break;
            }
        };        
        
        tmpfile << "sunsky (turbidity:2.1)\n";
        tmpfile << "sunsky (direction: 1.0,0.125,0.5)\n";
        tmpfile << "sunsky (color-filter-rgb: 0.05, 0.05, 0.05)\n";
        tmpfile << "sunsky (sun-color-rgb: 1800, 1600, 1300)\n";
        tmpfile << "sunsky (fog-exponent: 0.00005; fog-max-distance:10000000)\n";
        
        tmpfile.close();
    }
    {

        wxString x_usrbin = wxString (_("picogen ssdf -f /tmp/picogen-quick-preview.ssdf ")) 
            + wxString(_(" -s ws ")) // surface integrator
            + wxString(_(" -w 640 ")) // surface integrator
            + wxString(_(" -h 480 ")) // surface integrator
            //+ wxString::Format (wxString(_(" -a %i ")), antialiasing->GetValue ()) // antialiasing            
        ;
        wxMessageDialog (this, x_usrbin);
        wxString x_currentfolder = wxString (_("./") + x_usrbin);
        
        // try picogen in current folder
        {
            wxString statusText = x_currentfolder;
            statusText.Replace (_("\n"), _(" "));    
            SetStatusText(statusText);
        }
        
        int i;
        if (0 != (i=wxExecute (x_currentfolder, wxEXEC_SYNC))) {        
            // retry picogen in /usr/bin        
            {
                wxString statusText = x_usrbin;
                statusText.Replace (_("\n"), _(" "));    
                SetStatusText(statusText);
            }
            
            if (0 != (i=wxExecute (x_usrbin, wxEXEC_SYNC))) {
                wxMessageDialog msg (this, 
                    wxString (_("An error occured while running the command \"")) + x_usrbin + wxString(_("\"."))
                );
            } else {            
            }
        } else {        
        }
    }
}


void MkheightmapWxDialog::GetYprPosition (float &x, float &y, float &z) 
{
    using namespace std;

    {
        stringstream ss;
        ss << ypr_x->GetValue().mb_str() << flush;
        ss >> x;
    }
    
    {
        stringstream ss;
        ss << ypr_y->GetValue().mb_str() << flush;
        ss >> y;
    }
    
    {
        stringstream ss;
        ss << ypr_z->GetValue().mb_str() << flush;
        ss >> z;    
    }
}

void MkheightmapWxDialog::GetYprOrientation (float &yaw, float &pitch, float &roll) 
{
    float deg_yaw   = static_cast<float>(ypr_yaw->GetValue ()) + 0.01f * static_cast<float>(ypr_yaw_fine->GetValue ());
    float deg_pitch = -static_cast<float>(ypr_pitch->GetValue ())  + 0.01f * static_cast<float>(ypr_pitch_fine->GetValue ());
    float deg_roll  = -static_cast<float>(ypr_roll->GetValue ()) + 0.01f * static_cast<float>(ypr_roll_fine->GetValue ());
    
    const float to_degree = 0.0174532925f;
    yaw   = deg_yaw * to_degree;
    pitch = deg_pitch * to_degree;
    roll  = deg_roll * to_degree;
}


void MkheightmapWxDialog::OnClose( wxCommandEvent& event )
{
    wxTheApp->Exit();
}

void MkheightmapWxDialog::OnMenu_Copyright( wxCommandEvent& event )
{
    /*wxAboutDialogInfo info;
    info.SetName(_("picogen-minwx (experimental title)"));
    info.SetWebSite(_("http://picogen.org"));
    info.SetVersion(_("0.1.beta"));
    info.SetDescription(_("A minimalist's graphical interface for picogen"));
    info.SetCopyright(_T("(C) 2008 Sebastian Mach <phresnel@gmail.com>"));
    
    info.AddDeveloper(_T("Sebastian Mach <phresnel@gmail.com>"));
    info.AddArtist(_T("Sebastian Mach <phresnel@gmail.com>"));
    
    info.SetLicense(_T(
        "\"picogen\", \"picogen-minwx\"  Copyright (C) 2007,2008  Sebastian Mach (*1983)\n"
        "This program comes with ABSOLUTELY NO WARRANTY.\n"
        "This is free software, and you are welcome to redistribute it\n"
        "under certain conditions. \n\n"
    ));

    wxGenericAboutBox (info);*/
    MyAboutDlg about (this);
    about.ShowModal();
}
