#!/usr/bin/env python

import os
import subprocess
from اختبارات.مساعدات import run, print_err, print_title


path = os.path
sys = os.sys


def print_tit(*lines):
    print_title(*lines, level=2, indent="", top_line=False)


def config_git_hooks():
    print_tit("إعداد خطافات جيت")
    cmd = "git config --local core.hooksPath ./.خطافات_جيت"
    exit_code = os.system(cmd)
    if exit_code:
        print_err("حدث مشكلة أثناء إعداد خطافات جيت، يرجى إعدادها يدويها وإعداد المشروع للتطوير مجددا")
        print("-" * 35)
        print(cmd)
        exit(1)


def install_pip_packages():
    print_tit("تنصيب حزم بيب")
    cmd = "pip install pyyaml"
    exit_code = os.system(cmd)
    if exit_code:
        print_err("حدث مشكلة أثناء تنصيب حزمة بيب، يرجى تنصيبها يدويها وإعداد المشروع للتطوير مجددا")
        print("-" * 35)
        print(cmd)
        exit(1)


def ensure_requiremets():
    try:
        import yaml
    except ImportError:
        install_pip_packages()


if __name__ == "__main__":
    ensure_requiremets()
    config_git_hooks()

