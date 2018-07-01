set EfiProjectMajorVersion=
set EfiProjectMinorVersion=
set EfiProjectUpdateVersion=
set EfiProjectVersionTag=
set EfiProjectVersionHash=
for /f "delims=" %%a in ('type "%EfiProjectSource%\version"') do (
  set EfiProjectVersion=%%a
  set EfiProjectVersionTag=

  for /f "tokens=1,* delims=.-" %%b in ("!EfiProjectVersion!") do (
    for /f "delims=0123456789" %%i in ("%%b") do (
      set EfiProjectVersionTag=!EfiProjectVersionTag!%%i
    )
    if not defined EfiProjectVersionTag set EfiProjectMajorVersion=%%b
    set EfiProjectVersion=%%c
  )
  for /f "tokens=1,* delims=.-" %%b in ("!EfiProjectVersion!") do (
    for /f "delims=0123456789" %%i in ("%%b") do (
      set EfiProjectVersionTag=!EfiProjectVersionTag!%%i
    )
    if not defined EfiProjectVersionTag set EfiProjectMinorVersion=%%b
    set EfiProjectVersion=%%c
  )
  for /f "tokens=1,* delims=.-" %%b in ("!EfiProjectVersion!") do (
    for /f "delims=0123456789" %%i in ("%%b") do (
      set EfiProjectVersionTag=!EfiProjectVersionTag!%%i
    )
    if not defined EfiProjectVersionTag set EfiProjectUpdateVersion=%%b
    set EfiProjectVersion=%%c
  )
  if defined EfiProjectVersion set EfiProjectVersionTag=!EfiProjectVersionTag!!EfiProjectVersion!
)


for /f %%a in ('git rev-parse -n 1 HEAD') do (
  set EfiProjectVersionHash=%%a
)
for /f %%a in ('git rev-list "--count" HEAD') do (
  if not defined EfiProjectUpdateVersion (
    set EfiProjectUpdateVersion=%%a
    set /a EfiProjectUpdateVersion-=1
  )
  set EfiProjectVersionHash=%EfiProjectVersionHash% [%%a]
)
if "%EfiProjectDebug%" == "1" (
  for /f "tokens=* delims=" %%a in ('git rev-list -n 1 "--pretty=%%s" HEAD') do (
    set EfiProjectVersionHash=%EfiProjectVersionHash%: %%a
  )
)

if not defined EfiProjectVersionTag (
  for /f %%a in ('git rev-parse --abbrev-ref HEAD') do (
    set EfiProjectVersionTag=%%a
  )
)

if not defined EfiProjectMajorVersion set EfiProjectMajorVersion=1
if not defined EfiProjectMinorVersion set EfiProjectMinorVersion=0
if not defined EfiProjectUpdateVersion set EfiProjectUpdateVersion=0

set EfiProjectVersion=%EfiProjectMajorVersion%
set EfiProjectVersion=%EfiProjectVersion%.%EfiProjectMinorVersion%
if not "%EfiProjectUpdateVersion%" == "" if not "%EfiProjectUpdateVersion%" == "0" set EfiProjectVersion=%EfiProjectVersion%.%EfiProjectUpdateVersion%
set EfiProjectFullVersion=%EfiProjectVersion%
if not "%EfiProjectVersionTag%" == "" set EfiProjectVersion=%EfiProjectVersion%-%EfiProjectVersionTag%
