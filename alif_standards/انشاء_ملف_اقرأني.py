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

TEMPLATE = """## المعايير

|استفتاءاته| مواضيعه | تاريخه |وصفه |المعيار |
|:----:|:----:|:----:|:----:|:----:|
{rows}

</div>"""

ROW = "|{}|{}|{}|{}|{}|"


def get_readme() -> str:
    """ ارجاع محتوى ملف اقرأني بدون المعايير

    المخرجات:
        str: محتوى الملف بدون المعايير
    """
    with open("./README.md", "r") as f:
        content = f.read()
    return content[: content.find("## المعايير")]


def get_standard_data(standard: str) -> dict:
    """ ارجاع معلومات المعيار من الملف نفسه

    المعطيات:
        standard (str): رقم المعيار

    المخرجات:
        dict: معلومات المعيار
    """
    with open(f"./المعايير/{standard}.md", "r") as f:
        standard = f.read()
    standard_data = standard[: standard.find("---", 1)].strip("---").strip()
    return {
        line.split(":")[0].strip(): line.split(":")[1].strip().strip("'").strip('"')
        for line in standard_data.split("\n")
    }


def get_row(standard: str) -> str:
    """ انشاء صف للجدول الصف هو معيار

    المعطيات:
        standard (str): رقم المعيار

    المخرجات:
        str: صف
    """
    standard_data = get_standard_data(standard)
    return ROW.format(
        f"[`{standard}`](./الاستفتاءات/{standard})",
        ", ".join(tag.strip().strip("'").strip('"') for tag in standard_data["tags"].strip('][').split(', ')),
        standard_data["date"],
        standard_data["desc"],
        f"[`{standard}`](./المعايير/{standard}.md)",
    )


def main() -> None:
    """ انشاء المعايير """
    standards = [d.strip(".md") for d in os.listdir("المعايير")]
    rows = ""
    for standard in standards:
        rows += f"{get_row(standard)}\n"
    template = TEMPLATE.format(rows=rows)
    readme_content = get_readme()
    with open("./README.md", "w") as f:
        f.write(readme_content + template)

if __name__ == "__main__":
    main()