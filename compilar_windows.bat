@echo off
echo ========================================
echo Instalador Automatico - Sistema de Alimentos
echo ========================================
echo.

echo Verificando se o GCC esta instalado...
where gcc >nul 2>&1
if %errorlevel% neq 0 (
    echo.
    echo [ERRO] GCC nao encontrado!
    echo.
    echo Por favor, instale o MSYS2 primeiro:
    echo 1. Baixe de: https://www.msys2.org/
    echo 2. Instale o MSYS2
    echo 3. Abra o terminal MSYS2 e execute:
    echo    pacman -S mingw-w64-x86_64-gcc
    echo    pacman -S mingw-w64-x86_64-json-c
    echo 4. Adicione ao PATH: C:\msys64\mingw64\bin
    echo.
    pause
    exit /b 1
)

echo GCC encontrado!
echo.

echo Verificando biblioteca json-c...
echo #include ^<json-c/json.h^> > test.c
echo int main() { return 0; } >> test.c
gcc test.c -ljson-c -o test.exe >nul 2>&1
if %errorlevel% neq 0 (
    echo.
    echo [ERRO] Biblioteca json-c nao encontrada!
    echo.
    echo No terminal MSYS2, execute:
    echo    pacman -S mingw-w64-x86_64-json-c
    echo.
    del test.c >nul 2>&1
    pause
    exit /b 1
)
del test.c test.exe >nul 2>&1
echo json-c encontrada!
echo.

echo ========================================
echo Compilando P1.exe...
echo ========================================
gcc -Wall -Wextra -std=c99 -o P1.exe P1.c -ljson-c
if %errorlevel% neq 0 (
    echo [ERRO] Falha ao compilar P1.exe
    pause
    exit /b 1
)
echo [OK] P1.exe compilado com sucesso!
echo.

echo ========================================
echo Compilando P2.exe...
echo ========================================
gcc -Wall -Wextra -std=c99 -c utils.c
gcc -Wall -Wextra -std=c99 -c arvore.c
gcc -Wall -Wextra -std=c99 -c categoria.c
gcc -Wall -Wextra -std=c99 -c arquivo.c
gcc -Wall -Wextra -std=c99 -c menu.c
gcc -Wall -Wextra -std=c99 -c P2.c
gcc -Wall -Wextra -std=c99 -o P2.exe P2.o utils.o arvore.o categoria.o arquivo.o menu.o
if %errorlevel% neq 0 (
    echo [ERRO] Falha ao compilar P2.exe
    pause
    exit /b 1
)
echo [OK] P2.exe compilado com sucesso!
echo.

echo ========================================
echo COMPILACAO CONCLUIDA!
echo ========================================
echo.
echo Executaveis gerados:
echo - P1.exe (Conversor JSON para Binario)
echo - P2.exe (Sistema de Gerenciamento)
echo.
echo Para executar:
echo   P1.exe
echo   P2.exe
echo.
pause
