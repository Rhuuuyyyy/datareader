@echo off
chcp 65001 >nul
cls

echo ════════════════════════════════════════════════════════════════
echo   SISTEMA DE GERENCIAMENTO DE ALIMENTOS
echo   Versão com Interface Gráfica (GUI)
echo ════════════════════════════════════════════════════════════════
echo.

if not exist dados.bin (
    echo [ERRO] Arquivo dados.bin não encontrado!
    echo.
    echo O arquivo dados.bin deve estar na mesma pasta.
    echo.
    pause
    exit /b 1
)

if not exist P2_GUI.exe (
    echo [ERRO] Arquivo P2_GUI.exe não encontrado!
    echo.
    echo Faça download do repositório GitHub.
    echo.
    pause
    exit /b 1
)

echo Iniciando interface gráfica...
echo.
echo ┌─────────────────────────────────────────────────────────────┐
echo │  Uma janela gráfica será aberta!                           │
echo │                                                             │
echo │  Recursos da interface:                                    │
echo │  ✓ Botões visuais para cada função                         │
echo │  ✓ Lista de resultados organizada                          │
echo │  ✓ Navegação por cliques do mouse                          │
echo │  ✓ Mensagens visuais de confirmação                        │
echo └─────────────────────────────────────────────────────────────┘
echo.

start "" P2_GUI.exe

echo.
echo Interface gráfica iniciada!
echo Você pode fechar esta janela.
echo.
timeout /t 3 >nul
