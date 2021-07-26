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
    print("تم بنجاح")


def prepare_tests():
    print_tit("تجهيز للاختبارات")

    try:
        import yaml
        import colors
    except ImportError:
        cmd = "pip install pyyaml ansicolors"
        exit_code = os.system(cmd)
        if exit_code:
            print_err("حدث مشكلة أثناء تنصيب حزمة بيب، يرجى تنصيبها يدويها وإعداد المشروع للتطوير مجددا")
            print("-" * 35)
            print(cmd)
            exit(1)

    print("تم بنجاح")


if __name__ == "__main__":
    what_to_prepare = sys.argv[1]

    if what_to_prepare == "اختبارات":
        prepare_tests()
        exit(0)

    prepare_tests()
    config_git_hooks()

