@echo off
chcp 65001 >nul
cls
echo ════════════════════════════════════════════════════════════════
echo   SISTEMA DE GERENCIAMENTO DE ALIMENTOS
echo   Trabalho de Programação Imperativa - RA2
echo ════════════════════════════════════════════════════════════════
echo.

if not exist dados.bin (
    echo [ERRO] Arquivo dados.bin não encontrado!
    echo.
    echo Por favor, certifique-se de que o arquivo dados.bin
    echo está na mesma pasta que P2.exe
    echo.
    pause
    exit /b 1
)

if not exist P2.exe (
    echo [ERRO] Arquivo P2.exe não encontrado!
    echo.
    echo Faça download do P2.exe do repositório GitHub.
    echo.
    pause
    exit /b 1
)

echo Iniciando o sistema...
echo.
P2.exe

if %errorlevel% neq 0 (
    echo.
    echo ════════════════════════════════════════════════════════════════
    echo [ERRO] O programa encerrou com erro.
    echo ════════════════════════════════════════════════════════════════
    pause
    exit /b 1
)

echo.
echo ════════════════════════════════════════════════════════════════
echo Programa encerrado com sucesso!
echo ════════════════════════════════════════════════════════════════
pause
