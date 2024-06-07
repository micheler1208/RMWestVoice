/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   caviar_ttf;
    const int            caviar_ttfSize = 59284;

    extern const char*   gameboy_ttf;
    const int            gameboy_ttfSize = 10616;

    extern const char*   graffiti_ttf;
    const int            graffiti_ttfSize = 482872;

    extern const char*   heaters_ttf;
    const int            heaters_ttfSize = 1023628;

    extern const char*   timegoing_ttf;
    const int            timegoing_ttfSize = 62108;

    extern const char*   wholecar_ttf;
    const int            wholecar_ttfSize = 1156528;

    extern const char*   background_jpg;
    const int            background_jpgSize = 164570;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 7;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
