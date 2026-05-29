# RM West Voice Asset License Audit

This is a local metadata audit for the assets currently bundled into the plugin
binary through `RMWestVoiceBinaryData`. It is not legal advice and it does not
replace keeping original license files, purchase records, or written permission.

Audit date: 2026-05-29

## Summary

The codebase is buildable, but the current bundled visual/font assets are not
ready for public distribution. At least one font declares personal-use-only
metadata, and several assets have missing or incomplete redistribution evidence.

Public release should either replace these assets with clearly redistributable
alternatives or add verified license records next to the assets.

## Bundled Assets

| Asset | Local metadata found | Release status |
| --- | --- | --- |
| `Source/font/caviar.ttf` | Family: Caviar Dreams. Metadata says personal/commercial use is allowed, but also says the font data may not be redistributed. | Blocked until redistribution permission is confirmed or the font is replaced. |
| `Source/font/gameboy.ttf` | Family: Early GameBoy. Metadata references Creative Commons Attribution Share Alike 3.0. | Needs attribution/share-alike review before distribution. |
| `Source/font/graffiti.ttf` | Family: Graffiti City. Metadata references Woodcutter Manero / woodcutter.es, but no clear redistribution grant was found locally. | Blocked until license/provenance is verified. |
| `Source/font/heaters.ttf` | Family: Heaters. Metadata references MLKWSN Studio / Malik Wisnu, but no clear redistribution grant was found locally. | Blocked until license/provenance is verified. |
| `Source/font/timegoing.ttf` | Family: Timegoing. Local string scan did not yield a clear license statement. | Blocked until license/provenance is verified. |
| `Source/font/wholecar.ttf` | Family: Wholecar PERSONAL USE ONLY. Metadata says to obtain a commercial license. | Blocked for public distribution unless a commercial license is added. |
| `Source/img/background.jpg` | 800x600 JPEG with limited image metadata and no local license record. | Blocked until source/license/provenance is verified or the image is replaced. |

## Recommended Release Actions

1. Replace all uncertain fonts and the background image with owned assets or
   assets that include clear redistribution and embedding terms.
2. Keep license files or purchase records in a tracked `licenses/` or
   `third_party_notices/` directory before any public binary release.
3. Update `README.md` and release notes after the asset set is resolved.
4. Rebuild Standalone and VST3 artifacts after asset replacement.
