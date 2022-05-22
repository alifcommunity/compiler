#   The Alif Programming Language
#   Version 4.x Series
#   (C)2022 TheAwiteb <https://github.com/TheAwiteb>
#   www.aliflang.org

#   This file is part of Alif compiler.

#   Alif compiler is free software; you can redistribute it and/or modify it
#   under the terms of the GNU General Public License as published by the Free
#   Software Foundation; either version 3, or (at your option) any later
#   version.

#   Alif compiler is distributed in the hope that it will be useful, but
#   WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
#   for more details.

#   You should have received a copy of the GNU General Public License
#   along with Alif compiler; see the file COPYING3. If not see
#   <http://www.gnu.org/licenses/>.

import os
import argparse
from datetime import datetime
from pathlib import Path
from typing import List, Tuple

TEMPLATE = """## المعايير

|استفتاءاته| مواضيعه | تاريخه |وصفه |المعيار |
|:----:|:----:|:----:|:----:|:----:|
{rows}

</div>"""

ROW = "|{}|{}|{}|{}|{}|"


def get_readme() -> Tuple[str, str]:
    """ ارجاع محتوى ملف اقرأني بدون المعايير

    المخرجات:
        Tuple[str, str]: المحتوى العلوي من الملف، المحتوى السفلي من الملف
    """
    with open("./README.md", "r") as f:
        content = f.read()
    return (
        content[: content.find("## المعايير")].strip(),
        content[content.find("</div>") + 6 :].strip(),
    )


def get_standard_data(standard: str) -> dict:
    """ ارجاع معلومات المعيار من الملف نفسه

    المعطيات:
        standard (str): رقم المعيار

    المخرجات:
        dict: معلومات المعيار
    """
    with open(f"./المعايير/{standard}.md", "r") as f:
        standard = f.read()
    standard_data = standard[4 : standard.find("---", 4)].strip()
    return {
        line.split(":")[0].strip(): line.split(":")[1].strip().strip("'").strip('"')
        for line in standard_data.split("\n")
    }


def get_row(standard: str) -> str:
    """ انشاء صف للجدول، الصف هو معيار

    المعطيات:
        standard (str): رقم المعيار

    المخرجات:
        str: صف
    """
    standard_data = get_standard_data(standard)
    return ROW.format(
        f"[`{standard}`](./الاستفتاءات/{standard})",
        ", ".join(
            tag.strip().strip("'").strip('"')
            for tag in standard_data["tags"].strip("][").split(", ")
        ),
        standard_data["date"],
        standard_data["desc"],
        f"[`{standard}`](./المعايير/{standard}.md)",
    )


def get_standards() -> List[str]:
    """ ارجاع المعايير الموجودة

    المخرجات:
        List[str]: المعايير
    """
    return [d.strip(".md") for d in os.listdir("المعايير")]


def standards_readme() -> None:
    """ كتابة المعايير في ملف اقرأني """
    standards = get_standards()
    rows = ""
    for standard in standards:
        rows += f"{get_row(standard)}\n"
    template = TEMPLATE.format(rows=rows)
    readme_content = get_readme()
    with open("./README.md", "w") as f:
        f.write(f"{readme_content[0]}\n\n{template}\n\n{readme_content[1]}")


def make_standard_template(standard_path: str, standard_number: str) -> None:
    """ انشاء ملف به نموذج جاهز لكتابة المعيار

    المعطيات:
        standard_file (str): مسار المعيار
        standard_number (str): رقم المعيار
    """
    standard_template = """---
title: "المعيار رقم {standard}"
date: '{date}'
desc: 'الوصف'
tags: ["الموضوع الثالث", "الموضوع الثاني", "الموضوع الاول"]
---

# معيار رقم {standard} الخاص بلغة البرمجة العربية ألف

<center>
<img alt="شعار لغة ألف" src="https://avatars.githubusercontent.com/alifcommunity" />
</center>

---

## اكتب العنوان هنا (وفقك الله)
""".format(
        standard=standard_number, date=datetime.now().strftime("%Y/%m/%d")
    )
    with open(standard_path, "w") as f:
        f.write(standard_template)


def create_new_standar() -> None:
    """ انشاء معيار جديد """
    standard = datetime.now().strftime("%m%d%y")
    standards = get_standards()
    while standard in standards:
        if "-" in standard:
            number = standard.split("-")[1]
            standard = f"{standard.split('-')[0]}-{int(number) + 1}"
        else:
            standard += "-1"
    pools_folder = os.getcwd() + f"/الاستفتاءات/{standard}"
    standard_file = os.getcwd() + f"/المعايير/{standard}.md"
    os.makedirs(pools_folder)
    make_standard_template(standard_file, standard)
    print(f"رقم المعيار: {standard}")
    print(f"المعيار: {standard_file}")
    print(f"استفتاءاته: {pools_folder}")


def add_pool_to(standard_number: str, pool_path: str) -> None:
    """ اضافة استفتاء الى المجلد الخاص بالمعيار

    المعطيات:
        standard_number (str): رقم المعيار
        pool_path (str): المسار الخاص بالاستفتاء
    """
    if standard_number in get_standards():
        if os.path.exists(pool_path):
            pool_path: Path = Path(pool_path)
            os.rename(
                pool_path,
                os.getcwd() + f"/الاستفتاءات/{standard_number}/" + pool_path.name,
            )
        else:
            print(f"الاستفتاء غير موجود: {pool_path}")
    else:
        print(f"المعيار غير موجود: {standard_number}")


def main() -> None:
    parent_parser = argparse.ArgumentParser(
        description="اداة طرفية مساعدة لاضافة المعايير والاستفتاءات وانشاء ملف اقرأني."
    )
    action_parsers = parent_parser.add_subparsers(dest="action", title="الاجراءات")
    action_parsers.add_parser(
        "readme",
        description="اجراء يمكنك من اعادة تعبئة المعايير في ملف اقرأني.",
        help="اعادة تعبئة المعايير في ملف اقرأني.",
    )
    parser_create = action_parsers.add_parser(
        "create",
        description="اجراء يمكنك من انشاء معيار.",
        help="انشاء معيار بتاريخ اليوم، اضافة رقم تصاعدي اذا كان مكرر.",
    )
    parser_pools = action_parsers.add_parser(
        "pools",
        description="اجراء يمكنك من اضافة صورة استفتاء الى المجلد الخاص بالمعيار.",
        help="اضافة صورة استفتاء الى المجلد الخاص بالمعيار.",
    )
    parser_pools.add_argument(
        "-s", "--standard", dest="standard", help="رقم المعيار (مطلوب)"
    )
    parser_pools.add_argument(
        "-p", "--photo", dest="photo", help="مسار الاستفتاء (مطلوب)"
    )

    args = {key: val for key, val in parent_parser.parse_args()._get_kwargs()}
    action = args.pop("action")
    if not args and not action:
        parent_parser.print_help()
    elif not all(args.values()) and action == "pools":
        parser_pools.print_help()
    else:
        try:
            if action == "readme":
                if os.path.exists("./README.md"):
                    standards_readme()
                else:
                    raise FileNotFoundError("يجب ان يكون هناك ملف باسم `README.md`")
            elif action == "create":
                if os.path.exists("./الاستفتاءات") and os.path.exists("./المعايير"):
                    create_new_standar()
                else:
                    raise FileNotFoundError(
                        "يجب ان يكون هناك مجلد باسم `الاستفتاءات` و `المعايير`"
                    )
            elif action == "pools":
                if os.path.exists("./الاستفتاءات"):
                    add_pool_to(args.get("standard"), args.get("photo"))
                else:
                    raise FileNotFoundError("يجب ان يكون هناك مجلد باسم `الاستفتاءات`")
        except FileNotFoundError as err:
            print(err)


if __name__ == "__main__":
    main()
