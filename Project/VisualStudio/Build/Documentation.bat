
setlocal enabledelayedexpansion

call "%EfiProjectBuildTools%\SetVersion.bat"

set EfiProjectDocumentationPath=
if "%EfiProjectDebug%" == "1" (
  set EfiProjectDocumentationPath=%EfiProjectDocumentationPath% "$(EfiProjectSource)/Application"
  set EfiProjectDocumentationPath=%EfiProjectDocumentationPath% "$(EfiProjectSource)/Driver"
  set EfiProjectDocumentationPath=%EfiProjectDocumentationPath% "$(EfiProjectSource)/Library"
)

for /f "usebackq delims=" %%i in (` where doxygen 2^>NUL `) do set ToolDoxygen=%%~i
if not defined ToolDoxygen (
  echo doxygen: Not found
  goto:eof
)

echo doxygen: %ToolDoxygen%
set ToolDoxyfile="%EfiProjectSource%\Project\Doxygen\Doxyfile"
echo Creating documentation stage %EfiDocumentationStage% ...
mkdir "%EfiDocumentationStage%" >NUL 2>NUL

echo %EfiProjectVersion%
"%ToolDoxygen%" "%ToolDoxyfile%"

endlocal