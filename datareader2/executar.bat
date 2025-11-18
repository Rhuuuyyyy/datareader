@echo off
echo ========================================
echo Sistema de Gerenciamento de Alimentos
echo ========================================
echo.

if not exist P1.exe (
    echo [ERRO] P1.exe nao encontrado!
    echo Execute primeiro: compilar_windows.bat
    echo.
    pause
    exit /b 1
)

if not exist P2.exe (
    echo [ERRO] P2.exe nao encontrado!
    echo Execute primeiro: compilar_windows.bat
    echo.
    pause
    exit /b 1
)

echo Executando P1.exe (Gerando dados.bin)...
echo.
P1.exe
if %errorlevel% neq 0 (
    echo.
    echo [ERRO] Falha ao executar P1.exe
    pause
    exit /b 1
)

echo.
echo ========================================
echo Pressione qualquer tecla para abrir P2
echo ========================================
pause

cls
P2.exe

echo.
echo Programa encerrado.
pause
