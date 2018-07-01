if not exist "%EfiOutputStage%" (
  echo The output stage does not exist %EfiOutputStage%
  exit 1
)

setlocal enableextensions enabledelayedexpansion

call "%EfiProjectBuildTools%\SetVersion.bat"

if "%EfiProjectIso%" == "" set EfiProjectIso=%EfiProjectOutput%\%EfiProjectName%-%EfiProjectVersion%.iso

set EfiIsoElTorito=
set EfiProjectDisableElTorito=
if defined EfiProjectDisableElTorito (
  echo Skipping EFI boot image because ElTorito is disabled ...
  goto:EfiProjectIsoCreate
)

set EfiBootArchitectures=IA32 IA64 X64 ARM AA64
set EfiBootSectorCount=20
set EfiBootFilesSize=0
for %%g in (%EfiBootArchitectures%) do (
  if exist "%EfiOutputStage%\EFI\BOOT\BOOT%%g.efi" (
    call:AddFileSize "%EfiOutputStage%\EFI\BOOT\BOOT%%g.efi"
    set /a EfiBootFileSectors=^(^(!EfiBootFileSize!+511^)/512^)
    echo Found boot loader "%EfiOutputStage%\EFI\BOOT\BOOT%%g.efi" ^(!EfiBootFileSectors! sectors, !EfiBootFileSize! bytes^) ...
  )
)
set /a EfiBootSectorCount+=(%EfiBootFilesSize%+511)/512

echo Creating EFI boot image %EfiOutputStage%\BOOTEFI.IMG ^(%EfiBootSectorCount% sectors, %EfiBootFilesSize% bytes^) ...
"%EfiProjectBuildTools%\mformat.exe" -C -h 1 -s 1 -T %EfiBootSectorCount% -v "%EfiProjectName%" -i "%EfiOutputStage%\BOOTEFI.IMG" ::
if not exist "%EfiOutputStage%\BOOTEFI.IMG" (
  echo The EFI boot image does not exist %EfiOutputStage%\BOOTEFI.IMG
  endlocal
  exit 2
)
"%EfiProjectBuildTools%\mmd.exe" -i "%EfiOutputStage%\BOOTEFI.IMG" ::EFI ::EFI/BOOT
if errorlevel 1 (
  echo Could not format the EFI boot image %EfiOutputStage%\BOOTEFI.IMG
  endlocal
  exit 3
)
for %%g in (%EfiBootArchitectures%) do (
  if exist "%EfiOutputStage%\EFI\BOOT\BOOT%%g.efi" (
    echo Adding boot loader "%EfiOutputStage%\EFI\BOOT\BOOT%%g.efi" to boot image ...
    "%EfiProjectBuildTools%\mcopy.exe" -i "%EfiOutputStage%\BOOTEFI.IMG" "%EfiOutputStage%\EFI\BOOT\BOOT%%g.efi" ::EFI/BOOT/BOOT%%g.efi
    if not !errorlevel! equ 0 (
      echo Could not format the EFI boot image %EfiOutputStage%\BOOTEFI.IMG
      endlocal
      exit 4
    )
  )
)
set EfiIsoElTorito=-eltorito-catalog "BOOT.CATALOG" -hide "BOOT.CATALOG" -eltorito-platform efi -eltorito-boot "BOOTEFI.IMG" -no-emul-boot -hide "BOOTEFI.IMG"

:EfiProjectIsoCreate

echo Creating ISO %EfiProjectIso% from %EfiOutputStage% ...
"%EfiProjectBuildTools%\mkisofs.exe" -v -force-uppercase %EfiIsoElTorito% -V "%EfiProjectName% %EfiProjectFullVersion%" -o "%EfiProjectIso%" "%EfiOutputStage%"
endlocal
goto:eof

:AddFileSize

  set EfiBootFileSize=%~z1
  set /a EfiBootFilesSize+=%~z1
  goto:eof
