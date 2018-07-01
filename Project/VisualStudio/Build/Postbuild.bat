if not exist "%EfiDefaultLoader%" (
  echo Could not find default loader %EfiDefaultLoader%
  exit 1
)

echo Creating output stage %EfiOutputStage% ...
echo Creating default loader output stage %EfiOutputStage%\EFI\BOOT ...
mkdir "%EfiOutputStage%\EFI\BOOT" >NUL 2>NUL
if not exist "%EfiOutputStage%\EFI\BOOT" (
  echo Could not create directory %EfiOutputStage%\EFI\BOOT
  exit 2
)
echo Creating architecture output stage %EfiProjectStage% ...
mkdir "%EfiProjectStage%" >NUL 2>NUL
if not exist "%EfiProjectStage%" (
  echo Could not create directory %EfiProjectStage%
  exit 3
)
echo Copying default loader to default loader output stage ...
echo   %EfiDefaultLoader% =^> %EfiOutputStage%\EFI\BOOT\BOOT%EfiProjectShortArch%.efi
copy /y /b "%EfiDefaultLoader%" "%EfiOutputStage%\EFI\BOOT\BOOT%EfiProjectShortArch%.efi" >NUL 2>NUL
if errorlevel 1 (
  echo Could not copy %EfiDefaultLoader% to %EfiOutputStage%\EFI\BOOT\BOOT%EfiProjectShortArch%.efi
  exit 4
)
echo   %EfiDefaultLoader% =^> %EfiProjectStage%\%EfiDefaultLoaderName%
copy /y /b "%EfiDefaultLoader%" "%EfiProjectStage%\%EfiDefaulttLoaderName%" >NUL 2>NUL
if errorlevel 1 (
  echo Could not copy %EfiDefaultLoader% to %EfiProjectStage%\%EfiDefaultLoaderName%
  exit 5
)
if exist "%EfiBuildStage%" (
  echo Copying applications to application output stage ...
  pushd "%EfiBuildStage%"
  for %%b in ("*.efi") do (
    if /i not "%EfiBuildStage%\%%b" == "%EfiDefaultLoader%" (
      if not exist "%EfiProjectStage%\Apps\" (
        echo Creating application output stage %EfiProjectStage%\Apps ...
        mkdir "%EfiProjectStage%\Apps" >NUL 2>NUL
        if not exist "%EfiProjectStage%\Apps" (
          echo Could not create directory %EfiProjectStage%\Apps
          exit 3
        )
      )
      echo   %EfiBuildStage%\%%b =^> %EfiProjectStage%\Apps\%%b
      copy /y /b "%EfiBuildStage%\%%b" "%EfiProjectStage%\Apps\%%b" >NUL 2>NUL
      if errorlevel 1 (
        echo Could not copy %EfiBuildStage%\%%b to %EfiProjectStage%\Apps\%%b
        popd
        exit 6
      )
    )
  )
  popd
)
if exist "%EfiBuildStage%\Tools" (
  echo Copying tools to tool output stage ...
  pushd "%EfiBuildStage%\Tools"
  for %%b in ("*.efi") do (
    if /i not "%EfiBuildStage%\Tools\%%b" == "%EfiDefaultLoader%" (
      if not exist "%EfiProjectStage%\Tools\" (
        echo Creating tool output stage %EfiProjectStage%\Tools ...
        mkdir "%EfiProjectStage%\Tools" >NUL 2>NUL
        if not exist "%EfiProjectStage%\Tools" (
          echo Could not create directory %EfiProjectStage%\Tools
          exit 3
        )
      )
      echo   %EfiBuildStage%\Tools\%%b =^> %EfiProjectStage%\Tools\%%b
      copy /y /b "%EfiBuildStage%\Tools\%%b" "%EfiProjectStage%\Tools\%%b" >NUL 2>NUL
      if errorlevel 1 (
        echo Could not copy %EfiBuildStage%\Tools\%%b to %EfiProjectStage%\Tools\%%b
        popd
        exit 6
      )
    )
  )
  popd
)
if exist "%EfiBuildStage%\Drivers" (
  echo Copying drivers to driver output stage ...
  pushd "%EfiBuildStage%\Drivers"
  for %%b in ("*.efi") do (
    if /i not "%EfiBuildStage%\Drivers\%%b" == "%EfiDefaultLoader%" (
      if not exist "%EfiProjectStage%\Drivers\" (
        echo Creating driver output stage %EfiProjectStage%\Drivers ...
        mkdir "%EfiProjectStage%\Drivers" >NUL 2>NUL
        if not exist "%EfiProjectStage%\Drivers" (
          echo Could not create directory %EfiProjectStage%\Drivers
          exit 3
        )
      )
      echo   %EfiBuildStage%\Drivers\%%b =^> %EfiProjectStage%\Drivers\%%b
      copy /y /b "%EfiBuildStage%\Drivers\%%b" "%EfiProjectStage%\Drivers\%%b" >NUL 2>NUL
      if errorlevel 1 (
        echo Could not copy %EfiBuildStage%\Drivers\%%b to %EfiProjectStage%\Drivers\%%b
        popd
        exit 6
      )
    )
  )
  popd
)
