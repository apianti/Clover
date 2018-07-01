
# Readme

## <a name="table-of-contents"></a> Table of Contents

  * [**Build in Windows**](#build-windows)

    * [Prerequisites](#windows-prerequisites)
    * [Install Visual Studio](#windows-install-vs)
    * [Install NASM](#windows-install-nasm)
    * [Setup build environment](#windows-setup-env)
    * [Clone repository](#windows-clone)
    * [Build](#windows-build)
    * [References](#windows-references)

  * [**Build in macOS**](#build-macos)

    * [Prerequisites](#macos-prerequisites)
    * [Install Xcode](#macos-install-xcode)
    * [Setup build environment](#macos-setup-env)
    * [Clone repository](#macos-clone)
    * [Build](#macos-build)
    * [References](#macos-references)

  * [**Build in Linux**](#build-linux)

    * [Prerequisites](#linux-prerequisites)
    * [Install GNU Compiler Collection (GCC)](#linux-install-gcc)
    * [Setup build environment](#linux-setup-env)
    * [Clone repository](#linux-clone)
    * [Build](#linux-build)
    * [References](#linux-references)

  * [**References**](#references)

## <a name="build-windows"></a> Build in Windows  

  * **Table of Contents**

    * [Prerequisites](#windows-prerequisites)
    * [Install Visual Studio](#windows-install-vs)
    * [Install NASM](#windows-install-nasm)
    * [Setup build environment](#windows-setup-env)
    * [Clone source code repository](#windows-clone)
    * [Build](#windows-build)
    * [References](#windows-references)

  * <a name="windows-prerequisites"></a> **Prerequisites**  

    * [git](https://git-scm.com/downloads)
    * [NASM](https://www.nasm.us/pub/nasm/snapshots/latest/)
    * [Visual Studio](https://www.visualstudio.com/downloads/)
    * [doxygen](https://sourceforge.net/projects/doxygen/files/) (Optionally, for building documentation)
    * [git graphical client](https://git-scm.com/download/gui/windows) (Optionally, to have a graphical interface for git)

  * <a name="windows-install-vs"></a> **Install Visual Studio 2010/2012/2013/2015/2017**
    * Supports all variants, including community and free versions
    * All versions support building IA32 (32-bit) and X64 (64-bit) architectures
    * Only Visual Studio 2013 or later can build ARM (32-bit) architecture
    * Only Visual Studio 2015 or later can build ARM64 (64-bit) architecture
    * There is no support for IA64 (Itanium 64-bit) architecture

  * <a name="windows-install-nasm"></a> **Install NASM**
    * After installing NASM, the windows PATH variable needs to be updated to point to the installation location of NASM
      * Run the control panel item `sysdm.cpl` from the run dialog (Windows + R) or a command prompt
      * Go to the `Advanced` tab, then click `Environmental Variables...` at the bottom
      * Edit the variable PATH either for your user or for the system by selecting the variable and clicking `Edit...`
      * Click `New` and enter the installation location for NASM, usually `C:\Program Files\NASM`
      * Click `OK` until no more dialogs are left open
      * You may need to logout or restart for the changes to take affect

  * <a name="windows-setup-env"></a> **Setup build environment**
    * Select a path to use for building or development, for example `C:\Development`
    * Create the directory using Windows explorer  
  ***or***
    * Create the directory using command prompt:  
      `mkdir C:\Development`  
      `cd C:\Development`  

  * <a name="windows-clone"></a> **Clone source code repository**

    * Clone the source code repository using [graphical client](https://git-scm.com/download/gui/windows) from [https://git.code.sf.net/p/cloverefiboot/Clover](https://git.code.sf.net/p/cloverefiboot/Clover)  
  ***or***
    * Clone the source code repository using command prompt:  
      `git clone https://git.code.sf.net/p/cloverefiboot/Clover Clover`

  * <a name="windows-build"></a> **Build**

    * Open `Project.sln` inside the cloned repository directory (usually `C:\Development\Clover\Project.sln`) with Visual Studio
    * Choose the architecture to build from configuration manager in Visual Studio
    * Press the `F7` key or select `Build > Build Solution` from the menu
    * Multiple architectures can be built at the same time by selecting `Build > Batch Build...` from the menu
    * The build outputs are by default in the `Output` directory of the cloned repository directory (usually `C:\Development\Clover\Output`)

  * <a name="windows-references"></a> **References**

    * doxygen ([https://www.doxygen.org](https://www.doxygen.org))
    * doxygen download ([https://sourceforge.net/projects/doxygen/files/](https://sourceforge.net/projects/doxygen/files/))
    * doxygen documentation ([https://www.doxygen.org/manual/index.html](https://www.doxygen.org/manual/index.html))
    * git ([https://git-scm.com/](https://git-scm.com/))
    * git download ([https://git-scm.com/downloads](https://git-scm.com/downloads))
    * git documentation ([https://git-scm.com/docs](https://git-scm.com/docs))
    * git graphical clients ([https://git-scm.com/download/gui/windows](https://git-scm.com/download/gui/windows))
    * NASM ([https://www.nasm.us/pub/nasm/snapshots/latest/](https://www.nasm.us/pub/nasm/snapshots/latest/))
    * Visual Studio ([https://www.visualstudio.com/downloads/](https://www.visualstudio.com/downloads/))
    * Windows Command Prompt documentation ([https://technet.microsoft.com/en-us/library/bb490890.aspx](https://technet.microsoft.com/en-us/library/bb490890.aspx))

  [Return to table of contents](#table-of-contents)

## <a name="build-macos"></a> Build in macOS

  * **Table of Contents**

    * [Prerequisites](#macos-prerequisites)
    * [Install Xcode](#macos-install-xcode)
    * [Setup build environment](#macos-setup-env)
    * [Clone source code repository](#macos-clone)
    * [Build](#macos-build)
    * [References](#macos-references)

  * <a name="macos-prerequisites"></a> **Prerequisites**
    * [NASM](https://www.nasm.us/pub/nasm/snapshots/latest/)
    * [Xcode.app](itms://appstore.com/apps/xcode)
    * [doxygen](https://sourceforge.net/projects/doxygen/files/) (Optionally, for building documentation)
    * [git graphical client](https://git-scm.com/download/gui/mac) (Optionally, to have a graphical interface for git)

  * <a name="macos-install-xcode"></a> **Install Xcode**
    * Open the macOS App Store and install [Xcode.app](https://appstore.com/mac/xcode)
    * Open Terminal.app and install Xcode.app command line developer tools:  
      `xcode-select --install`

  * <a name="macos-setup-env"></a> **Setup build environment**
    * Select a path to use for building or development, for example `~/Development`
    * Create the directory using macOS Finder  
  ***or***
    * Create the directory using Terminal.app (BASH shell):  
      `mkdir ~/Development`  
      `cd ~/Development`

  * <a name="macos-clone"></a> **Clone source code repository**

    * Clone the source code repository using [graphical client](https://git-scm.com/download/gui/mac) from [https://git.code.sf.net/p/cloverefiboot/Clover](https://git.code.sf.net/p/cloverefiboot/Clover)  
  ***or***
    * Clone the source code repository using Terminal.app (BASH shell):  
      `git clone https://git.code.sf.net/p/cloverefiboot/Clover Clover`

  * <a name="macos-build"></a> **Build**

    * TODO
    * The build outputs are by default in the `Output` directory of the cloned repository directory (usually `~\Development\Clover\Output`)

  * <a name="macos-references"></a> **References**

    * BASH shell documentation ([https://www.gnu.org/software/bash/manual/bash.html](https://www.gnu.org/software/bash/manual/bash.html))
    * doxygen ([https://www.doxygen.org](https://www.doxygen.org))
    * doxygen download ([https://sourceforge.net/projects/doxygen/files/](https://sourceforge.net/projects/doxygen/files/))
    * doxygen documentation ([https://www.doxygen.org/manual/index.html](https://www.doxygen.org/manual/index.html))
    * git ([https://git-scm.com/](https://git-scm.com/))
    * git download ([https://git-scm.com/downloads](https://git-scm.com/downloads))
    * git documentation ([https://git-scm.com/docs](https://git-scm.com/docs))
    * git graphical clients ([https://git-scm.com/download/gui/mac](https://git-scm.com/download/gui/mac))
    * NASM ([https://www.nasm.us/pub/nasm/snapshots/latest/](https://www.nasm.us/pub/nasm/snapshots/latest/))
    * Xcode.app ([https://appstore.com/mac/xcode](https://appstore.com/mac/xcode))

  [Return to table of contents](#table-of-contents)

## <a name="build-linux"></a> Build in Linux

### Ubuntu

  * **Table of Contents**

    * [Prerequisites](#linux-prerequisites)
    * [Install GNU Compiler Collection](#linux-install-gcc)
    * [Setup build environment](#linux-setup-env)
    * [Clone source code repository](#linux-clone)
    * [Build](#linux-build)
    * [References](#linux-references)

  * <a name="linux-prerequisites"></a> **Prerequisites**

    * TODO

  * <a name="linux-install-gcc"></a> **Install GNU Compiler Collection**

    * TODO

  * <a name="linux-setup-env"></a> **Setup build environment**
    * Select a path to use for building or development, for example `~/Development`
    * Create the directory using graphical file manager  
  ***or***
    * Create the directory using terminal shell:  
      `mkdir ~/Development`  
      `cd ~/Development`

  * <a name="linux-clone"></a> **Clone source code repository**

    * TODO

  * <a name="linux-build"></a> **Build**

    * TODO
    * The build outputs are by default in the `Output` directory of the cloned repository directory (usually `~\Development\Clover\Output`)

  * <a name="linux-references"></a> **References**
    * BASH shell documentation ([https://www.gnu.org/software/bash/manual/bash.html](https://www.gnu.org/software/bash/manual/bash.html))
    * doxygen ([https://www.doxygen.org](https://www.doxygen.org))
    * doxygen download ([https://sourceforge.net/projects/doxygen/files/](https://sourceforge.net/projects/doxygen/files/))
    * doxygen documentation ([https://www.doxygen.org/manual/index.html](https://www.doxygen.org/manual/index.html))
    * git ([https://git-scm.com/](https://git-scm.com/))
    * git download ([https://git-scm.com/downloads](https://git-scm.com/downloads))
    * git documentation ([https://git-scm.com/docs](https://git-scm.com/docs))
    * git graphical clients ([https://git-scm.com/download/gui/linux](https://git-scm.com/download/gui/linux))
    * NASM ([https://www.nasm.us/pub/nasm/snapshots/latest/](https://www.nasm.us/pub/nasm/snapshots/latest/))

  [Return to table of contents](#table-of-contents)

### More Linux distributions to come

  [Return to table of contents](#table-of-contents)

## <a name="references"></a> References

  * BASH shell documentation ([https://www.gnu.org/software/bash/manual/bash.html](https://www.gnu.org/software/bash/manual/bash.html))
  * doxygen ([https://www.doxygen.org](https://www.doxygen.org))
  * doxygen download ([https://sourceforge.net/projects/doxygen/files/](https://sourceforge.net/projects/doxygen/files/))
  * doxygen documentation ([https://www.doxygen.org/manual/index.html](https://www.doxygen.org/manual/index.html))
  * git ([https://git-scm.com/](https://git-scm.com/))
  * git download ([https://git-scm.com/downloads](https://git-scm.com/downloads))
  * git documentation ([https://git-scm.com/docs](https://git-scm.com/docs))
  * git graphical clients ([https://git-scm.com/download/guis](https://git-scm.com/download/guis))
  * NASM ([https://www.nasm.us/pub/nasm/snapshots/latest/](https://www.nasm.us/pub/nasm/snapshots/latest/))
  * Visual Studio ([https://www.visualstudio.com/downloads/](https://www.visualstudio.com/downloads/))
  * Windows Command Prompt documentation ([https://technet.microsoft.com/en-us/library/bb490890.aspx](https://technet.microsoft.com/en-us/library/bb490890.aspx))
  * Xcode.app ([https://appstore.com/mac/xcode](https://appstore.com/mac/xcode))

  [Return to table of contents](#table-of-contents)
