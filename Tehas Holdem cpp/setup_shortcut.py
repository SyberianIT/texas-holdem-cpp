#!/usr/bin/env python3
"""
Универсальный скрипт:
- компилирует игру
- создаёт ярлык/значок запуска в папке проекта

Работает на Linux и Windows.

Запуск из корня проекта:
    python3 setup_shortcut.py
или на Windows:
    py setup_shortcut.py
"""

import os
import platform
import subprocess
import sys
from textwrap import dedent


PROJECT_DIR = os.path.dirname(os.path.abspath(__file__))
EXEC_BASENAME = "holdem"


def run(cmd, cwd=None):
    print(">", " ".join(cmd))
    subprocess.check_call(cmd, cwd=cwd or PROJECT_DIR)


def build_binary():
    system = platform.system()
    exe_name = EXEC_BASENAME + (".exe" if system == "Windows" else "")
    exe_path = os.path.join(PROJECT_DIR, exe_name)

    # Если уже есть бинарник – можно пропустить пересборку
    sources = ["main.cpp", "igra.cpp", "igrok.cpp", "koloda.cpp", "ocenka_ruki.cpp"]
    missing = [s for s in sources if not os.path.exists(os.path.join(PROJECT_DIR, s))]
    if missing:
        print("❌ Не найдены исходники:", ", ".join(missing))
        sys.exit(1)

    print("Компиляция проекта...")
    cmd = ["g++", "-std=c++17"] + sources + ["-o", exe_name]
    try:
        run(cmd, cwd=PROJECT_DIR)
    except FileNotFoundError:
        print("❌ Не найден компилятор g++. Установи g++ и запусти скрипт снова.")
        sys.exit(1)

    if not os.path.exists(exe_path):
        print("❌ Не удалось собрать исполняемый файл:", exe_path)
        sys.exit(1)

    print("✅ Сборка успешна:", exe_path)
    return exe_path


def setup_linux(exe_path: str):
    print("Настройка ярлыка для Linux...")

    run_sh_path = os.path.join(PROJECT_DIR, "run_holdem.sh")
    desktop_path = os.path.join(PROJECT_DIR, "TexasHoldem.desktop")

    run_sh_content = dedent(f"""\
        #!/bin/bash
        cd "{PROJECT_DIR}"
        gnome-terminal -- "./{os.path.basename(exe_path)}"
    """)

    with open(run_sh_path, "w", encoding="utf-8") as f:
        f.write(run_sh_content)
    os.chmod(run_sh_path, 0o755)

    desktop_content = dedent(f"""\
        [Desktop Entry]
        Type=Application
        Name=Texas Hold'em
        Comment=Консольный техасский холдем
        Exec="{run_sh_path}"
        Terminal=false
        Icon=utilities-terminal
        Categories=Game;
    """)

    with open(desktop_path, "w", encoding="utf-8") as f:
        f.write(desktop_content)
    os.chmod(desktop_path, 0o755)

    print("✅ Созданы файлы:")
    print("   -", run_sh_path)
    print("   -", desktop_path)
    print("Двойной клик по TexasHoldem.desktop в этой папке запустит игру.")


def setup_windows(exe_path: str):
    print("Настройка ярлыка для Windows...")

    run_bat_path = os.path.join(PROJECT_DIR, "run_holdem.bat")
    lnk_path = os.path.join(PROJECT_DIR, "TexasHoldem.lnk")

    bat_content = dedent(f"""\
        @echo off
        cd /d "%~dp0"
        start cmd /k "{os.path.basename(exe_path)}"
    """)

    with open(run_bat_path, "w", encoding="cp1251", errors="ignore") as f:
        f.write(bat_content)

    # Создание .lnk через PowerShell + COM (WScript.Shell)
    ps_script = dedent(f"""\
        $shell = New-Object -ComObject WScript.Shell
        $shortcut = $shell.CreateShortcut("{lnk_path}")
        $shortcut.TargetPath = "{run_bat_path}"
        $shortcut.WorkingDirectory = "{PROJECT_DIR}"
        $shortcut.IconLocation = "{exe_path},0"
        $shortcut.Save()
    """)

    try:
        subprocess.run(
            ["powershell", "-NoProfile", "-Command", ps_script],
            cwd=PROJECT_DIR,
            check=False,
        )
        print("✅ Созданы файлы:")
        print("   -", run_bat_path)
        print("   -", lnk_path)
        print("Двойной клик по TexasHoldem.lnk в этой папке запустит игру.")
    except FileNotFoundError:
        print("⚠ PowerShell не найден. Ярлык .lnk не создан, но есть run_holdem.bat.")
        print("Можешь создать ярлык на run_holdem.bat вручную.")


def main():
    system = platform.system()
    exe_path = build_binary()

    if system == "Linux":
        setup_linux(exe_path)
    elif system == "Windows":
        setup_windows(exe_path)
    else:
        print(f"⚠ Неподдерживаемая ОС: {system}")
        print("Игра собрана, но автоматическое создание ярлыка не реализовано.")


if __name__ == "__main__":
    main()

