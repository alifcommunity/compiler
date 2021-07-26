import os
import subprocess
from os import path, sys


def run(cmd, return_prcss=False, inputs=None):
    process = subprocess.Popen(cmd, shell=True,
                               stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if return_prcss:
        return process
    if inputs:
        stdouts = []
        stderrs = []
        for i in inputs:
            out, err = process.communicate(i.encode())
            stdouts.append(out.decode())
            stderrs.append(err.decode())
        return stdouts, stderrs, process.returncode
    out, err = process.communicate()
    return out.decode(), err.decode(), process.returncode


def print_title(*lines, level=1, indent=None,
                top_line=True, bottom_line=True,
                file=sys.stdout, align="center",
                min_width=35, max_width=0):
    """
    print something like this, with customizations like `indent` and `level`:
    ---------------------------
                        هذا سطر
    ---------------------------
    """
    try:
        term_w = os.get_terminal_size().columns
    except:
        term_w = 100000000
    # min width is 35 and max width is term_w, otherwise the width will be the max length of `lines`
    width = min(term_w, max(min_width, *map(lambda l: len(l), lines)))
    # total printed line width has to be less than the terminal width
    if indent:
        width = width if len(indent) + width < term_w else term_w - len(indent)
    else:
        width = width if width < term_w else term_w
    levels = ["=", "-", "."]
    if type(level) == int:
        level = levels[level-1]
    if indent == None:
        if term_w < 200:
            a = term_w - width
            # `>> 1` means divide by 2 and truncate, numbers here are experimental, nothing make sense
            indent = ((a >> 1) - 5) * ' ' if a > 10 else ''
        else:
            indent = " " * 15
    if not len(indent) < term_w - 10:
        raise Exception(
            "Indentation must be smaller than term width by more than 10 chars")

    def print_these_lines():
        nonlocal max_width
        max_width = max_width or term_w - len(indent)
        for l in lines:
            # if the length of a line is greater than term_w,
            # split it into smaller lines
            if len(l) > max_width:
                ll = []
                while l:
                    ll.append(l[0:max_width])
                    l = l[max_width:]
                l = ll
            l = [l] if type(l) != list else l
            for ll in l:
                indent_to_center_text = \
                    (width - len(ll) >> 1) * " " if align == "center" else \
                    (width - len(ll)) * " " if align == "end" else ""
                print(indent + indent_to_center_text + ll, file=file)

    if top_line:
        print(indent + level * width, file=file)
    print_these_lines()
    if bottom_line:
        print(indent + level * width, file=file)


def print_indented(text, indent="", file=sys.stdout):
    """
    put intial indent to the text and print the indented text
    """
    lines = str(text).split("\n")
    try:
        term_w = os.get_terminal_size().columns
    except:
        term_w = 100000000
    max_width = term_w - len(indent)
    for l in lines:
        # if the length of a line is greater than term_w,
        # split it into smaller lines
        if len(l) > max_width:
            ll = []
            while l:
                ll.append(l[0:max_width])
                l = l[max_width:]
            l = ll
        l = [l] if type(l) != list else l
        for ll in l:
            print(indent + ll, file=file)


def print_err(*msgs):
    """
    print to the STDERR
    """
    print(*msgs, file=sys.stderr)


def is_equal(obj1, obj2):
    if type(obj1) == list and type(obj2) == list:
        for i in range(len(obj1)):
            if not is_equal(obj1[i], obj2[i]):
                return False
        return True
    elif type(obj1) == dict and type(obj2) == dict:
        for p in obj1:
            if not is_equal(obj1.get(p), obj2.get(p)):
                return False
        return True

    return obj1 == obj2
