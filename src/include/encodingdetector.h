/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 */

#ifndef ENCODINGDETECTOR_H
#define ENCODINGDETECTOR_H

#include "settings.h"
#include "filemanager.h"
#include <wx/fontmap.h>
#include "nsUniversalDetector.h"

class wxString;

/** Try to detect the encoding of a file on disk. */
class DLLIMPORT EncodingDetector : public nsUniversalDetector
{
    public:
        EncodingDetector(const wxString& filename, bool useLog=true);
        EncodingDetector(LoaderBase* fileLdr, bool useLog=true);
        EncodingDetector(const wxByte* buffer, size_t size, bool useLog=true);
        EncodingDetector(const EncodingDetector& rhs, bool useLog=true);
        ~EncodingDetector();

        /** @return True if file was read, false if not. */
        bool IsOK() const;
        /** @return True if the file contains a BOM (Byte Order Mark), false if not. */
        bool UsesBOM() const;
        /** @return The BOM size in bytes. Returns zero if no BOM. */
        int GetBOMSizeInBytes() const;
        /** @return The detected encoding. Currently ISO8859-1 is returned if no BOM is present. */
        wxFontEncoding GetFontEncoding() const;
        wxString GetWxStr() const;
    protected:
        /** @return True if succeeded, false if not (e.g. file didn't exist). */
        bool DetectEncoding(const wxString& filename, bool convert_to_wxstring = true);
        bool DetectEncoding(const wxByte* buffer, size_t size, bool convert_to_wxstring = true);
        bool DetectEncodingEx(const wxByte* buffer, size_t len);
        void Report(const char* aCharset);

        bool           m_IsOK;
        bool           m_UseBOM;
        bool           m_UseLog;
        int            m_BOMSizeInBytes;
        wxFontEncoding m_Encoding;
    private:
        bool ConvertToWxString(const wxByte* buffer, size_t size);

        wxString m_MozillaResult;
        wxString m_ConvStr;
};

#endif // ENCODINGDETECTOR_H
