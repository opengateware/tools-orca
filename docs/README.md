[![logo](logo.png)](#)

---

[![release](https://img.shields.io/github/release/opengateware/tools-orca.svg)](https://github.com/opengateware/tools-orca/releases)
[![license](https://img.shields.io/github/license/opengateware/tools-orca.svg?label=License&color=yellow)](#legal-notices)
[![discord](https://img.shields.io/discord/676418475635507210.svg?logo=discord&logoColor=white&label=Discord&color=5865F2)](https://chat.raetro.org)
[![forum](https://img.shields.io/github/discussions/opengateware/tools-orca.svg?label=Discussions&color=orange)](https://github.com/opengateware/tools-orca/discussions)
[![stars](https://img.shields.io/github/stars/opengateware/tools-orca.svg?label=Project%20Stars)](https://github.com/opengateware/tools-orca/stargazers)
[![issues](https://img.shields.io/github/issues/opengateware/tools-orca.svg?label=Issues&color=red)](https://github.com/opengateware/tools-orca/issues)

## What is ORCA

A tool to merge [MAME] ROMs using recipes (`.xml` or `.mra`) into a single `.rom` file to be used with an Arcade gateware IP Core.

## Installation

To set up ORCA, you only need to:

- Download the [latest release]
    - Debian/Ubuntu and WSL users can download the `.deb` package and install using `sudo dpkg -i ORCA-<version>-Linux.deb`
- Extract the executable to a directory in your PATH System Variable. e.g: `/usr/bin` on macOS/Linux or `%WINDIR%` on Windows

## Usage

### Single File

```bash
# Linux/macOS
orca -z /PATH/TO/MAME/ZIPS file.[xml/mra]

# Windows
orca.exe -z DRIVE:\PATH\TO\MAME\ZIPS file.[xml/mra]
```

### Batch processing using scripts

Assuming you have the following structure
```text
.
├── output
├── roms
├── tools
│   └── make_roms.[sh/ps1]
└── xml
```

On Linux/WSL and macOS:

> **Note:** Change the PATHs according to your needs

```bash
#!/usr/bin/env bash

PARENT=$(dirname $PWD)
XML=$PARENT/xml
ROMS=$PARENT/roms
ASSETS=$PARENT/output

find ${XML} -name '*.xml' | while read line; do
    echo "Processing '$line'"
    orca -z ${ROMS} -O ${ASSETS} "$line"
done
```

Windows Powershell:

```powershell
$currentPath = $(Get-Item $($MyInvocation.MyCommand.Path)).DirectoryName
$fileNames = Get-ChildItem -Path "$currentPath\..\xml" -Recurse -Include *.xml

try { 
    foreach ($f in $fileNames){
        $xmlfile = $f.FullName
        Write-Host "Processing $xmlfile ..."
        orca.exe -z ..\roms -O ..\output $xmlfile
    }
} catch {
    Write-Host "Error: $($_.Exception.Message)"
    exit 1
} finally {
    Write-Host "Done."
    timeout /t 5
}

exit 0
```

Output: 
```text
PS A:\rom-recipes\tools> .\make_roms.ps1
Processing A:\recipes\xml\Recipe.xml ...
Done.
```


## Contributing

This project welcomes contributions and suggestions.

If you've found a bug or have a suggestion, please don't hesitate to detail these in the [issue tracker].
At this time if you have questions please leave them in the [Q&A] section or use the `#tools-r-us` channel on [Rætro Discord].

## Legal Notices

This work is licensed under multiple licenses.

- All original source code is licensed under [Mozilla Public License 2.0] unless implicit indicated.
- All documentation is licensed under [Creative Commons Attribution Share Alike 4.0 International] Public License.
- Some configuration and data files are licensed under [Creative Commons Zero v1.0 Universal].

Open Gateware and any contributors reserve all others rights, whether under their respective copyrights, patents, or trademarks, whether by implication, estoppel or otherwise.

Individual files may contain the following SPDX license tags as a shorthand for the above copyright and warranty notices:

```text
SPDX-License-Identifier: MPL-2.0
SPDX-License-Identifier: CC-BY-SA-4.0
SPDX-License-Identifier: CC0-1.0
```

This eases machine processing of licensing information based on the SPDX License Identifiers that are available at <https://spdx.org/licenses/>.

All brands or product names are the property of their respective holders.

## Powered by Open-Source Software

This project borrowed and use code from several other projects. A great thanks to their efforts!

| Software/Library | License                             | Copyright/Developer           |
|:-----------------|:------------------------------------|:------------------------------|
| [mra-tool-c]     | [The Unlicense]                     | 2020 (c) Sebastien Delestaing |
| [JUnzip]         | [The Unlicense]                     | 2014 (c) Joonas Pihlajamaa    |
| [MD5]            | [The Unlicense]                     | 2001 (c) Alexander Peslyak    |
| [sxmlc]          | [BSD 2-Clause "Simplified" License] | 2010 (c) Matthieu Labas       |


<!-- Third-Party Dependencies and Licenses -->
[mra-tool-c]:https://github.com/mist-devel/mra-tools-c/
[sxmlc]:https://github.com/matthieu-labas/sxmlc
[MD5]:https://openwall.info/wiki/people/solar/software/public-domain-source-code/md5
[JUnzip]:https://github.com/jokkebk/JUnzip

[BSD 2-Clause "Simplified" License]: https://spdx.org/licenses/BSD-2-Clause.html
[The Unlicense]:https://spdx.org/licenses/Unlicense.html
<!-- END:Third-Party Dependencies and Licenses -->

<!-- ORCA Licenses and Links -->
[Creative Commons Attribution Share Alike 4.0 International]:https://spdx.org/licenses/CC-BY-SA-4.0.html
[Creative Commons Zero v1.0 Universal]:https://spdx.org/licenses/CC0-1.0.html
[Mozilla Public License 2.0]:https://spdx.org/licenses/MPL-2.0.html

[MAME]:https://www.mamedev.org/
[latest release]:https://github.com/raetro-labs/orca/releases/latest

[Rætro Discord]:https://chat.raetro.org
[OpenGateware.org]:https://opengateware.org
[GitHub discussions]:https://github.com/opengateware/tools-orca/discussions
[GitHub issues]:https://github.com/opengateware/tools-orca/issues
[issue tracker]:https://github.com/opengateware/tools-orca/issues
[Q&A]:https://github.com/opengateware/tools-orca/discussions/categories/q-a
<!-- END:ORCA Licenses and Links -->
