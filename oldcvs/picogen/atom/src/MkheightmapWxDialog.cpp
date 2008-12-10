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
    
    
    const wxString shKeywords [] = {
        wxString(_("const-rgb"))
        , wxString(_("hs"))
    };
    const int numShKeywords = sizeof (shKeywords) / sizeof (shKeywords [0]);
}

MkheightmapWxDialog::MkheightmapWxDialog( wxWindow* parent )
:
MkheightmapWxDialogGui( parent )
{
    terrainHeightmap->SetFocus();
    
    {
        wxFont f ( 9, 76, 90, 90, false, wxT("Monospace") );
        terrainHeightmap->StyleSetFont( wxSCI_STYLE_DEFAULT, f );    
        terrainHeightmap->SetLexer (wxSCI_LEX_LISP);
      
        wxString hskwCat;
        for (int i=0; i<numHsKeywords; ++i) {
            hskwCat += hsKeywords [i] + wxString (_(" "));
        }
        terrainHeightmap->SetKeyWords (0, hskwCat);
        
        terrainHeightmap->StyleSetForeground (SCE_LISP_KEYWORD, wxColour (_T("BLUE")));
        terrainHeightmap->StyleSetForeground (SCE_LISP_OPERATOR, wxColour (255,128,64));
        terrainHeightmap->StyleSetForeground (SCE_LISP_NUMBER, wxColour (0,128,0));
        
        terrainHeightmap->StyleSetForeground (wxSCI_STYLE_BRACELIGHT, wxColour (_T("GREEN")));
        terrainHeightmap->StyleSetForeground (wxSCI_STYLE_BRACEBAD, wxColour (_T("RED")));
        
        terrainHeightmap->StyleSetForeground (wxSCI_STYLE_INDENTGUIDE, wxColour (64,64,64));
        
        terrainHeightmap->SetViewWhiteSpace (false);
        terrainHeightmap->SetTabIndents (false);
        terrainHeightmap->SetUseTabs (false);
        
        terrainHeightmap->SetText(wxString (_("    (-  1 \n\
            (^  (-  1\n\
                    (abs ([2 LayeredNoise \n\
                        frequency(5) \n\
                        layercount(8) \n\
                        persistence((* 0.4 (-1 (abs x))))\n\
                     ] x y))) \n\
                3))")));            
    }
    
    {
        wxFont f ( 9, 76, 90, 90, false, wxT("Monospace") );
        terrainShader->StyleSetFont( wxSCI_STYLE_DEFAULT, f );    
        terrainShader->SetLexer (wxSCI_LEX_LISP);
      
        wxString hskwCat;
        for (int i=0; i<numHsKeywords; ++i) {
            hskwCat += hsKeywords [i] + wxString (_(" "));
        }
        for (int i=0; i<numShKeywords; ++i) {
            hskwCat += shKeywords [i] + wxString (_(" "));
        }
        terrainShader->SetKeyWords (0, hskwCat);
        
        terrainShader->StyleSetForeground (SCE_LISP_KEYWORD, wxColour (_T("BLUE")));
        terrainShader->StyleSetForeground (SCE_LISP_OPERATOR, wxColour (255,128,64));
        terrainShader->StyleSetForeground (SCE_LISP_NUMBER, wxColour (0,128,0));
        
        terrainShader->StyleSetForeground (wxSCI_STYLE_BRACELIGHT, wxColour (_T("GREEN")));
        terrainShader->StyleSetForeground (wxSCI_STYLE_BRACEBAD, wxColour (_T("RED")));
        
        terrainShader->StyleSetForeground (wxSCI_STYLE_INDENTGUIDE, wxColour (64,64,64));
        
        terrainShader->SetViewWhiteSpace (false);
        terrainShader->SetTabIndents (false);
        terrainShader->SetUseTabs (false);
        
        terrainShader->SetText(wxString (_(
            "hs(\n"
            "   {\n"
            "       const-rgb(0.0, 1.0, 0.0),\n"
            "       const-rgb(0.0, 1.0, 0.0)\n" // TODO: there's a bug when there's a space between 'const-rgb' and '('
            "   },\n"
            "   z\n"
            ")\n"
        )));            
    }
}

void MkheightmapWxDialog::OnSave (wxCommandEvent& event)
{
    std::ofstream ofs("raboof");
    boost::archive::xml_oarchive oa(ofs);
    oa << boost::serialization::make_nvp ("picogen-wx-scene-def", *this);
}

void MkheightmapWxDialog::UpdateTextWithTemplate (const wxString &tpl)
{
    switch (masterNotebook->GetSelection ()) {
        case 0: terrainHeightmap->ReplaceSelection (tpl); break;
        case 1: terrainShader->ReplaceSelection (tpl); break;
    };
}

void MkheightmapWxDialog::OnMenu_Execute( wxCommandEvent& event )
{
    OnShowHeightmap (event);
}

void MkheightmapWxDialog::OnAutoformat( wxCommandEvent& event )
{
    #define doIndent() do { for (int i=0; i<indent*tablen; ++i) { formatted+=' '; } } while(0)
    const int tablen = 4;
    int indent=0;
    /*    
    wxString code = terrainHeightmap->GetText();
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
    terrainHeightmap->SetText(formatted);    
    */
    
    wxString code = terrainHeightmap->GetText();
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
    
    terrainHeightmap->SetText(formatted);
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

void MkheightmapWxDialog::OnShowHeightmap( wxCommandEvent& event )
{
	wxString x = terrainHeightmap->GetText();
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
    //wxMessageDialog (this, x_usrbin);
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
    terrainHeightmap->SetFocus();
}


void MkheightmapWxDialog::OnShowShadedHeightmap( wxCommandEvent& event )
{
	wxString heightmap = terrainHeightmap->GetText();
    heightmap.Replace (_("\""), _(" "));
    heightmap.Replace (_("\\"), _(" "));
    heightmap.Replace (_("`"), _(" "));
    
    wxString shader = terrainShader->GetText();
    shader.Replace (_("\""), _(" "));
    shader.Replace (_("\\"), _(" "));
    shader.Replace (_("`"), _(" "));
    
    wxString x_usrbin = wxString (_("picogen mkheightmap -Lhs \"")) 
        + heightmap
        + wxString(_("\" -p ")) // preview
        + wxString (_(" -s \"")) + shader + wxString(_("\" ")) // shader
        + wxString::Format (wxString(_(" -a %i ")), antialiasing->GetValue ()) // antialiasing
        + (normalise->IsChecked() ? wxString(_(" -n ")) : wxString(_(""))) // normalise?
    ;
    //wxMessageDialog (this, x_usrbin);
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
    terrainShader->SetFocus();
}


void MkheightmapWxDialog::ObtainSunSkyParams (
    float atmoRGB [3], 
    float &fogDensity, float &fogMaxRange,
    float &turbidity,
    float &sunDiskSize,
    bool &falloffEnable, float falloffParameters [3], 
    float sunRGB [3], float sunDir [3]
) const {
    // ATMOSPHERE
    
    // Atmosphere Color Filter.
    const float atmoIntensity = static_cast <float> (atmosphereIntensity->GetValue ()) * 0.001f;
    atmoRGB [0] = static_cast <float> (atmosphereR->GetValue ()) * atmoIntensity * 0.01f;
    atmoRGB [1] = static_cast <float> (atmosphereG->GetValue ()) * atmoIntensity * 0.01f;
    atmoRGB [2] = static_cast <float> (atmosphereB->GetValue ()) * atmoIntensity * 0.01f;
    
    // Fog.
    fogDensity  = static_cast <float> (this->fogDensity->GetValue ()) * 0.0000001; // / 10k
    fogMaxRange = static_cast <float> (this->fogMaxRange->GetValue ());
    
    // Turbidity.
    turbidity = static_cast <float> (turbidityA->GetValue ()) + 0.01 * static_cast <float> (turbidityB->GetValue ());
    
    // SUN    
    // Disk Size.
    sunDiskSize = static_cast <float> (this->diskSize->GetValue ()) * 0.01;
    
    // Falloff.
    falloffParameters [0]   = static_cast <float> (this->falloffParameterA->GetValue ()) * 0.1;
    falloffParameters [1]   = static_cast <float> (this->falloffParameterB->GetValue ()) * 0.00001;
    falloffParameters [2]   = static_cast <float> (this->falloffParameterC->GetValue ()) * 0.1;
    falloffEnable = this->falloffEnable->IsChecked ();
    //falloffExponent = static_cast <float> (this->falloffExponent->GetValue ());
    
    // Sun Color.
    const float sunIntensity = static_cast <float> (this->sunIntensity->GetValue ());
    sunRGB [0] = static_cast <float> (sunR->GetValue ()) * sunIntensity * 0.01f;
    sunRGB [1] = static_cast <float> (sunG->GetValue ()) * sunIntensity * 0.01f;
    sunRGB [2] = static_cast <float> (sunB->GetValue ()) * sunIntensity * 0.01f;
    
    // Sun Direction (mkskymap will normalise it for us).
    const float to_radian = 0.0174532925f;
    const float pi = 3.14159265;
    const float sunPhi   = to_radian * static_cast <float> (this->sunPhi->GetValue()) + 0.5*pi;
    const float sunTheta = to_radian * static_cast <float> (this->sunTheta->GetValue());
    sunDir [0] = sin (sunTheta) * cos (sunPhi);
    sunDir [1] = cos (sunTheta);
    sunDir [2] = sin (sunTheta) * sin (sunPhi);
}


void MkheightmapWxDialog::OnShowHemisphere( wxCommandEvent& event ) 
{
    float atmoRGB [3]; 
    float fogDensity; float fogMaxRange;
    float turbidity;
    float sunDiskSize;
    bool falloffEnable; float falloffParameters [3]; 
    float sunRGB [3]; float sunDir [3];
    ObtainSunSkyParams (
        atmoRGB,
        fogDensity, fogMaxRange,
        turbidity,
        sunDiskSize,
        falloffEnable, falloffParameters, 
        sunRGB, sunDir
    );
    
    
    
    // COMMANDLINE
    
    
    
    // Command Line (using std::string, as wxString would compile e.g. a "2,1" e.g. on a german box, where instead a "2.1" is needed.
    using namespace std;
    stringstream commandline;
    commandline << "picogen mkskymap -p ";
    commandline << " -F " << atmoRGB [0] << ' ' << atmoRGB [1] << ' ' << atmoRGB [2] << ' ';
    if (fogEnable->IsChecked()) {
        commandline << " -O " << fogDensity << ' ' << fogMaxRange << ' ';
    }
    commandline << " -C " << sunRGB [0] << ' ' << sunRGB [1] << ' ' << sunRGB [2] << ' ';    
    commandline << " -d " << sunDir [0] << ' ' << sunDir [1] << ' ' << sunDir [2] << ' ';
    commandline << " -t " << turbidity << ' ';
    if (falloffEnable) {
        commandline << " -o " << falloffParameters [0] << ' ' << falloffParameters [1] << ' ' << falloffParameters [2] << ' ';
    }
    commandline << " -A " << sunDiskSize << ' ';
    
    const wxString x_usrbin (commandline.str().c_str(), wxConvUTF8);
    const wxString x_currentfolder = wxString (_("./") + wxString (x_usrbin));
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

std::string MkheightmapWxDialog::generateSceneTempFile (bool withPreviewSettings) const 
{
    #ifdef __LINUX__ 
    const std::string tmpfilename = "/tmp/picogen-tempfile.ssdf";
    #else
    #error Meh.    
    #endif

    using namespace std;
    ofstream tmpfile;    
    tmpfile.open (tmpfilename.c_str());
    tmpfile << "// auto generated picogen scene file for quick-preview\n";
    
    // brdf for the water
    tmpfile << "list {\n";
        
    tmpfile << "    brdf = specular_distorted-height(\n";
    tmpfile << "        reflectance:0.8; \n";
    tmpfile << "        code:  ( + 1 ( * 1.0 ( * (  [2 LayeredNoise frequency(0.05) persistence(0.55) levelEvaluationFunction(x) layercount(6) filter(cosine) seed(321) ] x (*y 5.0) ) ( * 1.0 (  [2 LayeredNoise frequency(0.005) persistence(0.55) levelEvaluationFunction(x) layercount(6) filter(cosine) seed(132) ] x y ) ) ) ) ) ) \n";
    
    /*tmpfile << "    // the water\n";
    tmpfile << "    hs-heightfield (\n";
    tmpfile << "        code: 1;\n";
    tmpfile << "        size:130000,0.5,130000;\n";
    tmpfile << "        material: hs( {const-rgb(1.0,1.0,1.0)},( * 0.0 (abs(   [2 LayeredNoise frequency(64) layercount(6) persistence(0.5) filter(cosine)] xy ) ) ) ) ;\n";
    tmpfile << "        center:0,-1600,0;\n";
    tmpfile << "        resolution:8\n";
    tmpfile << "    )\n";*/    
    
    tmpfile << "    // the actual height-field\n";
    tmpfile << "    brdf = lambertian(reflectance:0.9)   \n";
    tmpfile << "    hs-heightfield (        \n";
    //tmpfile << "        material: hs( { const-rgb(0.3,0.8,0.2), const-rgb(1.0,1.0,1.0) }, ( + 0  ( * 1.0 (abs(   [2 LayeredNoise  frequency(40) layercount(12) persistence(0.7)  filter(cosine)]xy) ) ) ) ) ;                \n";
    tmpfile << "        material: const-rgb (1,1,1);\n";
    tmpfile << "        code:  ";
    //tmpfile << "            ( ^ (   [2 LayeredNoise frequency(4) persistence(0.55) levelEvaluationFunction(x) layercount(12) filter(cosine) seed(55) ] x y ) 1 )\n";
    //tmpfile << "            ;\n";
    tmpfile << terrainHeightmap->GetText().mb_str();
    tmpfile << ";\n";
    //tmpfile << "        resolution: 256; \n";
    tmpfile << "        resolution: " << (1<<(1+heightmapSize->GetSelection())) << "; \n";
    tmpfile << "        center:     0, -1400, 0; \n";
    
    float width, height;
    {
            std::stringstream ss;
            ss << terrainDimWidth->GetValue().mb_str() << std::flush;
            ss >> width;
    }
    {
        std::stringstream ss;
        ss << terrainDimHeight->GetValue().mb_str() << std::flush;
        ss >> height;
    }
    tmpfile << "        size: " << width << ',' << height << ',' << width << "\n";
    tmpfile << "    )\n";
        
    tmpfile << "}\n";
    
    // Camera.
    switch (cameraType->GetSelection ()) {
        case 0: {
            float yaw, pitch, roll, x, y, z;                
            GetYprOrientation (yaw, pitch, roll);
            GetYprPosition (x, y, z);
            tmpfile << "camera-yaw-pitch-roll("
                << "position: " << x << ',' << y << ',' << z << ";"
                << "yaw:" << yaw << "; "
                << "pitch:" << pitch << "; "
                << "roll:" << roll << ")\n "
            ;
            break;
        }
    };  


    // Sunsky.
    {
        float atmoRGB [3]; 
        float fogDensity; float fogMaxRange;
        float turbidity;
        float sunDiskSize;
        bool falloffEnable; float falloffParameters [3]; 
        float sunRGB [3]; float sunDir [3];
        ObtainSunSkyParams (
            atmoRGB,
            fogDensity, fogMaxRange,
            turbidity,
            sunDiskSize,
            falloffEnable, falloffParameters, 
            sunRGB, sunDir
        );
        tmpfile << "sunsky (turbidity:" << turbidity << ")\n";
        tmpfile << "sunsky (direction: " << sunDir [0] << ',' << sunDir [1] << ',' << sunDir [2] << ")\n";
        tmpfile << "sunsky (color-filter-rgb: " << atmoRGB [0] << ", " << atmoRGB [1] << ", " << atmoRGB [2] << ")\n";
        tmpfile << "sunsky (sun-color-rgb: " << sunRGB [0] << ", " << sunRGB [1] << ", " << sunRGB [2] << ")\n";
        if (fogEnable) {
            //tmpfile << "sunsky (fog-exponent: 0.00005; fog-max-distance:10000000)\n";
            tmpfile << "sunsky (fog-exponent: " << fogDensity << "; fog-max-distance:" << fogMaxRange << ")\n";
        } else {
            tmpfile << "sunsky (fog-exponent: " << 0.0 << "; fog-max-distance:" << 0.0 << ")\n";
        }
        
        if (falloffEnable) {
            tmpfile << "sunsky (sun-falloff-parameters: " << falloffParameters [0] << ',' << falloffParameters [1] << ',' << falloffParameters [2] << ")\n";
        }
        
        tmpfile << "sunsky (sun-solid-angle-factor: " << sunDiskSize << ")\n";
    }
    
    tmpfile.close();
    
    return tmpfilename;
}

void MkheightmapWxDialog::OnQuickPreview( wxCommandEvent& event )
{
    //const wxString tmpfilename (_("/tmp/picogen-quick-preview.ssdf"));
    
    const std::string tmpfilename = generateSceneTempFile (true);
    {

        wxString x_usrbin = wxString (_("picogen ssdf -f ")) + wxString (tmpfilename.c_str(), wxConvUTF8) + wxString(_(" ")) 
            + wxString(_(" -s ws "))
            + wxString(_(" -w 640 "))
            + wxString(_(" -h 480 "))
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


void MkheightmapWxDialog::OnRender( wxCommandEvent& event )
{
    if (!ShowSaveFileDlg()) {
        return;
    }

    const std::string tmpfilename = generateSceneTempFile (false);
    {
        int width=-1, height=-1, aa=2;
        {
            std::stringstream ss;
            ss << renderWidth->GetValue().mb_str() << std::flush;
            ss >> width;
        }
        {
            std::stringstream ss;
            ss << renderHeight->GetValue().mb_str() << std::flush;
            ss >> height;
        }
        {
            std::stringstream ss;
            ss << renderAA->GetValue() << std::flush;
            ss >> aa;
        }

        wxString x_usrbin = wxString (_("picogen ssdf -f \"")) + wxString (tmpfilename.c_str(), wxConvUTF8) + wxString(_("\" ")) 
            + wxString::Format (_(" -o \"")) + wxString (renditionFilename->GetValue().c_str(), wxConvUTF8) + wxString (_("\""))
            + ((renderSurfaceIntegrator->GetSelection () == 0) ? wxString(_(" -s ws ")) : wxString(_(" -s pt ")))
            + wxString::Format (_(" -w %d -h %d -a %d "), width, height, aa)            
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


void MkheightmapWxDialog::GetYprPosition (float &x, float &y, float &z) const
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

void MkheightmapWxDialog::GetYprOrientation (float &yaw, float &pitch, float &roll) const
{
    float deg_yaw   = static_cast<float>(ypr_yaw->GetValue ()) + 0.01f * static_cast<float>(ypr_yaw_fine->GetValue ());
    float deg_pitch = -static_cast<float>(ypr_pitch->GetValue ())  + 0.01f * static_cast<float>(ypr_pitch_fine->GetValue ());
    float deg_roll  = -static_cast<float>(ypr_roll->GetValue ()) + 0.01f * static_cast<float>(ypr_roll_fine->GetValue ());
    
    const float to_radian = 0.0174532925f;
    yaw   = deg_yaw * to_radian;
    pitch = deg_pitch * to_radian;
    roll  = deg_roll * to_radian;
}


void MkheightmapWxDialog::OnClose( wxCommandEvent& event )
{    
    wxTheApp->Exit();
}


bool MkheightmapWxDialog::ShowSaveFileDlg () {
    // Strip filename.
    wxString filename = renditionFilename->GetValue().Strip (wxString::both);
    renditionFilename->SetValue(filename);
    
    // Suppress file dialog if a filename does not exist yet.
    if (!filename.IsEmpty ()) {
        if (!wxFile::Exists (renditionFilename->GetValue())) {
            return true;
        } else {
            wxMessageDialog msg (this, 
                wxString (_("The output file \"")) +  renditionFilename->GetValue() + wxString(_("\" already exists. Overwrite?")),
                wxString (_("")),
                wxYES_NO | wxNO_DEFAULT
            );
            if (msg.ShowModal() == wxID_YES) {
                return true;
            }
        }
    }

    wxFileDialog openFileDialog (
        this, _("Open file"), _(""), _(""), _("All Files|*.*|BMP files (*.bmp)|*.bmp"),
    #if wxCHECK_VERSION(2, 8, 0)
        wxFD_OVERWRITE_PROMPT | wxFD_SAVE | wxFD_CHANGE_DIR,
    #else
		wxOVERWRITE_PROMPT | wxSAVE | wxCHANGE_DIR,
    #endif
        wxDefaultPosition
    );
    
    openFileDialog.SetFilename (filename);
    if (openFileDialog.ShowModal() == wxID_OK) {
        renditionFilename->SetValue (openFileDialog.GetPath());
        return true;
    }
    return false;
}

void MkheightmapWxDialog::OnOpenSaveFile( wxCommandEvent& event )
{
    ShowSaveFileDlg();
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


void MkheightmapWxDialog::save (boost::archive::xml_oarchive & ar, const unsigned int /* file_version */) const {
    using namespace boost::serialization;
    
    // Terrain.
    {
        std::string terrain_code = std::string (terrainHeightmap->GetText().mb_str());
        float terrain_width, terrain_height;
        {
                std::stringstream ss;
                ss << terrainDimWidth->GetValue().mb_str() << std::flush;
                ss >> terrain_width;
        }
        {
            std::stringstream ss;
            ss << terrainDimHeight->GetValue().mb_str() << std::flush;
            ss >> terrain_height;
        }
        
        ar  & make_nvp("terrain", terrain_code);
        ar  & make_nvp("terrain-width", terrain_width);
        ar  & make_nvp("terrain-height", terrain_height);
        //ar  & make_nvp("terrain-resolution", heightmapSize->GetSelection()); // TODO: shalt be saved as integer
    }
    
    // Sun-/Sky
    {
        float atmoRGB [3]; 
        float fogDensity; float fogMaxRange;
        float turbidity;
        float sunDiskSize;
        bool falloffEnable; float falloffParameters [3]; 
        float sunRGB [3]; float sunDir [3];
        ObtainSunSkyParams (
            atmoRGB,
            fogDensity, fogMaxRange,
            turbidity,
            sunDiskSize,
            falloffEnable, falloffParameters, 
            sunRGB, sunDir
        );
        
        ar  & make_nvp("sunsky-atmosphere-color-filter-r", atmoRGB [0]);
        ar  & make_nvp("sunsky-atmosphere-color-filter-g", atmoRGB [1]);
        ar  & make_nvp("sunsky-atmosphere-color-filter-b", atmoRGB [2]);
        ar  & make_nvp("sunsky-fog-density", fogDensity);
        ar  & make_nvp("sunsky-fog-max-range", fogMaxRange);
        ar  & make_nvp("sunsky-turbidity", turbidity);
        ar  & make_nvp("sunsky-sundisk-size", sunDiskSize);
        ar  & make_nvp("sunsky-falloff-enable", falloffEnable);
        ar  & make_nvp("sunsky-falloff-parameters-0", falloffParameters [0]);
        ar  & make_nvp("sunsky-falloff-parameters-1", falloffParameters [1]);
        ar  & make_nvp("sunsky-falloff-parameters-2", falloffParameters [2]);
        ar  & make_nvp("sunsky-sun-color-r", sunRGB [0]);
        ar  & make_nvp("sunsky-sun-color-g", sunRGB [1]);
        ar  & make_nvp("sunsky-sun-color-b", sunRGB [2]);
        ar  & make_nvp("sunsky-sun-direction-x", sunDir [0]);
        ar  & make_nvp("sunsky-sun-direction-y", sunDir [1]);
        ar  & make_nvp("sunsky-sun-direction-z", sunDir [2]);
    }
    
    // Camera.
    {
        float yaw, pitch, roll, x, y, z;
        GetYprOrientation (yaw, pitch, roll);
        GetYprPosition (x, y, z);
        
        ar  & make_nvp("camera-ypr-x", x);
        ar  & make_nvp("camera-ypr-y", y);
        ar  & make_nvp("camera-ypr-z", z);
        ar  & make_nvp("camera-ypr-yaw", yaw);
        ar  & make_nvp("camera-ypr-pitch", pitch);
        ar  & make_nvp("camera-ypr-roll", roll);
    }
}
    
void MkheightmapWxDialog::load (boost::archive::xml_iarchive & ar, const unsigned int file_version){
    using namespace boost::serialization;
}
