@echo off
echo ========================================================
echo   COMPILANDO SNES9XTYL PARA PSP EN SERVIDOR LOCAL DOCKER
echo ========================================================
ssh khatddog@192.168.3.12 "/home/khatddog/scripts/build_psp.sh"
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Fallo la compilacion en el servidor Docker.
    pause
    exit /b %ERRORLEVEL%
)

echo Descargando EBOOT.PBP a tu PC...
scp khatddog@192.168.3.12:/home/khatddog/docker/devpsp/snes-psp-go/EBOOT.PBP "%~dp0EBOOT.PBP"

echo ========================================================
echo   COMPILACION EXITOSA! EBOOT.PBP LISTO EN TU CARPETA.
echo ========================================================
