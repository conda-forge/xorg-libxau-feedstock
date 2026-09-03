@echo on

meson setup builddir %MESON_ARGS% --wrap-mode=nofallback --default-library=shared
if errorlevel 1 exit /b 1

meson compile -C builddir -j %CPU_COUNT%
if errorlevel 1 exit /b 1

meson test -C builddir --num-processes %CPU_COUNT% --print-errorlogs
if errorlevel 1 exit /b 1

meson install -C builddir
if errorlevel 1 exit /b 1
