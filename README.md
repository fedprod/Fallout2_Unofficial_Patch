## Fallout 2 Unofficial Patch, updated

[![Build Status](https://travis-ci.org/BGforgeNet/Fallout2_Unofficial_Patch.svg?branch=master)](https://travis-ci.org/BGforgeNet/Fallout2_Unofficial_Patch)
[![Translation status](https://tra.bgforge.net/widgets/fallout/-/up/svg-badge.svg)](https://tra.bgforge.net/projects/fallout/up/)

* [Download](https://github.com/BGforgeNet/Fallout2_Unofficial_Patch/releases/latest)
* [Installation](#installation), [update](#update)
* [Reporting issues](#reporting-issues) (read this)
* [Translations](#translations) (you can help)
* [Discord](https://discord.gg/4Yqfggm)
* [Changelog](docs/changelog.md)

This is an updated version of [killap's Unofficial Patch](http://killap.net/) for Fallout 2.

### Installation
1. Start with a clean humongous installation of the game.
1. Download the latest UPU [release](https://github.com/BGforgeNet/Fallout2_Unofficial_Patch/releases/latest).
1. Extract the archive into the main game directory, overwriting files when asked.
1. Launch `upu-install.bat` (or `upu-install.sh`, if you're on Linux/MacOS).

**Notes:**
1. UPU does not _require_ starting a new game. However, if you continue with a save from an older version, you won't see some of the fixes in the areas that you've already visited.
1. If you made any changes to `ddraw.ini`, these changes will be lost when you overwrite that file. If you want to preserve them, make sure to make a backup copy and then transfer the settings manually to the new `ddraw.ini`.

### Update

* You can install UPU on top of UP 1.02.31 to make use of the fixes.
* You can update from an older UPU version by deleting it from `mods` subdirectory and extracting the new version into it.

In both cases, you won't see some of the fixes on the already visited maps. Other than that, savegames should be compatible, but no guarantees.

### Translations
If you want to play a localized version, grab the corresponding language file from the latest release. Extract it into `mods` subdirectory.

Most translations are incomplete, they are provided as is. You can [help with completion](https://tra.bgforge.net/projects/fallout/up/). Some details are [here](docs/translations.md).

### Reporting issues
If you've found an issue:

1. Keep in mind that UP's scope is only to fix bugs. Not to add "nice to to have" things.
2. Check [known issues](docs/known.md).
3. Follow the [instruction](docs/reporting.md).
