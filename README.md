## Fallout 2 Unofficial Patch, updated

[![Build Status](https://travis-ci.org/BGforgeNet/Fallout2_Unofficial_Patch.svg?branch=master)](https://travis-ci.org/BGforgeNet/Fallout2_Unofficial_Patch)

[__Download__](https://github.com/BGforgeNet/Fallout2_Unofficial_Patch/releases/latest)
 | [__Installation__](#installation)
 | [__Reporting issues__](#reporting-issues)
 | [__Changelog__](docs/CHANGELOG.md)

This is an update for [killap's Unofficial Patch](http://killap.net/) for Fallout 2. This is _not_ an independent mod, it should be installed on top of UP 1.02.31.

### Installation
1. Start with a clean humongous installation of the game.
1. Download and install [UP 1.02.31](http://killap.net/) normally.
1. Download the latest UPU [release](https://github.com/BGforgeNet/Fallout2_Unofficial_Patch/releases/latest) (upu_vX.zip).
1. Unzip the archive into the main game directory, overwriting files when asked. That's it.

**Notes:**
1. UPU does not _require_ starting a new game. However, if you continue with a save from an older version, you won't see some of the fixes in the areas that you've already visited.
1. If you made any changes to `ddraw.ini`, these changes will be lost when you overwrite that file. If you want to preserve them, make sure to make a backup copy and then transfer the settings manually to the new `ddraw.ini`.

### Reporting issues
If you've found an issue:

1. Keep in mind that UP's scope is only to fix bugs. Not to add "nice to to have" things.
2. Check [known issues](docs/KNOWN.md).
3. Before opening a new Github issue, check existing ones (both open and closed). Maybe it's already been reported.
4. Next, depending on the problem type:
    * **Typo**. Attach a screenshot, explain what's wrong in the wording and how it should be corrected. (Screenshot key is F12).
    * **Quest bug**. Explain the problem. Attach **two** savegames: one where the problem can be seen right away, another one _right before_ starting the quest. Indicate if any other mods are installed, and their versions.
    * **Crash**. See [reporting crashes](docs/CRASH.md).
    * **Something else**. Use your best judgement. Provide as much info as possible.

**Notes:**
- The best way to get the issue fixed is to submit a pull request.
- One issue per problem.
- Attach the files directly on Github, don't use external file sharing sites.
- Some bugs may be better illustrated with a gif or a video, you can do that.
