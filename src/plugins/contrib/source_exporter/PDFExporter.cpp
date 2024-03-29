/*
 * This plugin was created thanks to the wxPdfDocument library
 */

#include "PDFExporter.h"
#include "wx/pdfdoc.h"
#include <configmanager.h>
#include <wx/fontutil.h>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <sstream>
#include <iomanip>

using std::size_t;
using std::find;
using std::ostringstream;
using std::setw;
using std::right;

namespace
{
  // Helper function to calculate the width of a number (ugly way)
  inline int calcWidth(int num)
  {
    if (num < 0)
    {
      return 0;
    }

    int width = 1;

    while ((num /= 10) != 0)
    {
      ++width;
    }

    return width;
  }

  // Helper function to convert i to a string
  inline std::string to_string(int i, int width = 0)
  {
    ostringstream ostr;

    if (width > 0)
    {
      ostr << setw(width) << right;
    }

    ostr << i;

    return ostr.str();
  }

  // Helper function to write text
  inline void PDFWriteText(wxPdfDocument &pdf, const wxString &text, bool fill)
  {
    if (!text.IsEmpty())
    {
      pdf.WriteCell(4., text, wxPDF_BORDER_NONE, fill);
    }
  }
}

bool PDFExporter::Style::operator == (int aValue)
{
  return value == aValue;
}

void PDFExporter::PDFSetFont(wxPdfDocument &pdf)
{
  wxString fontstring = Manager::Get()->GetConfigManager(_T("editor"))->Read(_T("/font"), wxEmptyString);
  wxString faceNameDefault(_T("Courier"));
  wxString faceName(faceNameDefault);
  double   fontSize = 8; // Default point size

  pdf.SetFont(faceNameDefault);

  if (!fontstring.IsEmpty())
  {
    wxFont tmpFont;
    wxNativeFontInfo nfi;
    nfi.FromString(fontstring);
    tmpFont.SetNativeFontInfo(nfi);

    fontSize = tmpFont.GetPointSize();
    faceName = tmpFont.GetFaceName();
  }

  // Try to set the new font, if it fails it'll use the default one
  if (!pdf.SetFont(faceName))
      pdf.SetFont(faceNameDefault);

  pdf.SetFontSize(fontSize);
}

void PDFExporter::PDFGetStyles(const EditorColourSet *c_color_set, HighlightLanguage lang)
{
  m_styles.clear(); // Be sure the styles are cleared
  defStyleIdx = -1; // No default style

  if (lang != HL_NONE)
  {
    const int count = const_cast<EditorColourSet *>(c_color_set)->GetOptionCount(lang);

    for (int i = 0; i < count; ++i)
    {
      OptionColour *optc = const_cast<EditorColourSet *>(c_color_set)->GetOptionByIndex(lang, i);

      if (!optc->isStyle)
      {
        continue;
      }

      Style tmpStyle =
      {
        optc->value,
        optc->back,
        optc->fore,
        optc->bold,
        optc->italics,
        optc->underlined
      };

      m_styles.push_back(tmpStyle);

      // Default Style
      if (optc->value == 0)
      {
        defStyleIdx = m_styles.size() - 1;
      }
    }
  }
}

void PDFExporter::PDFBody(wxPdfDocument &pdf, const wxMemoryBuffer &styled_text, int lineCount, int tabWidth)
{
  const char *buffer = reinterpret_cast<char *>(styled_text.GetData());
  const size_t buffer_size = styled_text.GetDataLen();
  bool fill = false;
  int lineno = 1;
  int width = calcWidth(lineCount);
  std::string text;

  pdf.AddPage();

  if (buffer_size == 0)
  {
    return;
  }

  if (lineCount != -1)
  {
    pdf.SetFont(wxEmptyString);
    pdf.SetTextColour(*wxBLACK);
    text += to_string(lineno, width);
    text += "  ";
    PDFWriteText(pdf, wxString(text.c_str(), wxConvUTF8), false);
    text.clear();
    ++lineno;
  }

  // Get the current style from the first character
  char current_style = buffer[1];

  // If the first style isn't the default style...
  if (current_style != 0)
  {
    vector<Style>::iterator i = find(m_styles.begin(), m_styles.end(), current_style);

    if (i != m_styles.end())
    {
      wxString style;

      if (i->bold)
      {
        style += _T("B");
      }

      if (i->italics)
      {
        style += _T("I");
      }

      if (i->underlined)
      {
        style += _T("U");
      }

      pdf.SetFont(wxEmptyString, style);
      pdf.SetTextColour(i->fore);

      if (i->back.Ok())
      {
        pdf.SetFillColour(i->back);
        fill = true;
      }
      else
      {
        fill = false;
      }
    }
  }

  int charLinePos = 0;

  for (size_t i = 0; i < buffer_size; i += 2, ++charLinePos)
  {
    if (buffer[i + 1] != current_style)
    {
      if (!isspace(buffer[i]))
      {
        PDFWriteText(pdf, wxString(text.c_str(), wxConvUTF8), fill);
        text.clear();

        current_style = buffer[i + 1];

        vector<Style>::iterator newStyle = find(m_styles.begin(), m_styles.end(), current_style);

        if (newStyle != m_styles.end())
        {
          wxString style;

          if (newStyle->bold)
          {
            style += _T("B");
          }

          if (newStyle->italics)
          {
            style += _T("I");
          }

          if (newStyle->underlined)
          {
            style += _T("U");
          }

          pdf.SetFont(wxEmptyString, style);
          pdf.SetTextColour(newStyle->fore);

          if (newStyle->back.Ok())
          {
            pdf.SetFillColour(newStyle->back);
            fill = true;
          }
          else
          {
            fill = false;
          }
        }
        else if (defStyleIdx != -1)
        {
          pdf.SetFont(wxEmptyString);
          pdf.SetTextColour(*wxBLACK);
          fill = false;
        }
      }
    }

    switch (buffer[i])
    {
      case '\r':
      	--charLinePos; // account for auto-increment
        break;

      case '\n':
        PDFWriteText(pdf, wxString(text.c_str(), wxConvUTF8), fill);
        text.clear();
        pdf.Ln();

        if (lineCount != -1)
        {
          pdf.SetFont(wxEmptyString);
          pdf.SetTextColour(*wxBLACK);
          text += to_string(lineno, width);
          text += "  ";
          PDFWriteText(pdf, wxString(text.c_str(), wxConvUTF8), false);
          text.clear();
          current_style = defStyleIdx;
          ++lineno;
        }

        charLinePos = -1; // account for auto-increment
        break;

      case '\t':
        {
            const int extraSpaces = tabWidth - charLinePos % tabWidth;
            text += std::string(extraSpaces, ' ');
            charLinePos += extraSpaces - 1; // account for auto-increment
        }
        break;

      default:
        text += buffer[i];
        break;
    }
  }

  PDFWriteText(pdf, wxString(text.c_str(), wxConvUTF8), fill);
}

void PDFExporter::Export(const wxString &filename, const wxString &title, const wxMemoryBuffer &styled_text, const EditorColourSet *color_set, int lineCount, int tabWidth)
{
  wxPdfDocument pdf;
  pdf.SetCompression(false);

  HighlightLanguage lang = const_cast<EditorColourSet *>(color_set)->GetLanguageForFilename(title);

  PDFSetFont(pdf);
  PDFGetStyles(color_set, lang);
  PDFBody(pdf, styled_text, lineCount, tabWidth);

  pdf.SaveAsFile(filename);
}
