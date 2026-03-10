import asyncio
import os
import subprocess
import sys
from pathlib import Path

from pyppeteer import launch


def find_target_directory(index: int) -> Path:
    prefix = f"{index:02d}"
    candidates = [
        p for p in Path.cwd().iterdir() if p.is_dir() and p.name.startswith(prefix)
    ]
    if len(candidates) != 1:
        print("Directory issue:", candidates)
        sys.exit(1)
    return candidates[0]


def find_single_markdown_file(directory: Path) -> Path:
    md_files = list(directory.glob("*.md"))
    if len(md_files) != 1:
        print("Markdown file issue:", md_files)
        sys.exit(1)
    return md_files[0]


async def html_to_pdf_pageless(html_file: Path, pdf_file: Path):
    browser = await launch(
        headless=True,
        executablePath="C:\\Program Files\\Google\\Chrome\\Application\\chrome.exe",  # change if your Chrome path differs
        args=["--no-sandbox"],
    )
    page = await browser.newPage()
    await page.goto(f"file://{html_file.resolve()}", {"waitUntil": "networkidle0"})

    # Get full height of content
    dimensions = await page.evaluate("""() => {
        return {
            width: document.documentElement.scrollWidth,
            height: Math.ceil(document.documentElement.scrollHeight * 1.015),  // add some padding to avoid cutting off content,
        }
    }""")
    print("Content dimensions:", dimensions)

    await page.pdf(
        {
            "path": str(pdf_file),
            "width": f"{dimensions['width']}px",
            "height": f"{dimensions['height']}px",
            "printBackground": True,
            "pageRanges": "1",
        }
    )
    await browser.close()


def main():
    if len(sys.argv) != 2:
        print("Usage: python build_pdf_pageless.py <index>")
        sys.exit(1)
    try:
        index = int(sys.argv[1])
    except ValueError:
        print("Argument must be an integer.")
        sys.exit(1)

    # 1️⃣ Find target directory
    target_dir = find_target_directory(index)
    os.chdir(target_dir)

    # 2️⃣ Find markdown file
    md_file = find_single_markdown_file(Path("."))

    base_name = md_file.stem
    temp_html = Path(f"{base_name}_temp.html")
    output_pdf = Path(f"{base_name}.pdf")

    # 3️⃣ Convert Markdown -> HTML
    subprocess.run(
        [
            "pandoc",
            str(md_file),
            "-o",
            str(temp_html),
            "--standalone",
            "--from=markdown+raw_html",
            "--katex",
        ],
        check=True,
    )

    # 4️⃣ Convert HTML -> pageless PDF using headless Chrome
    asyncio.get_event_loop().run_until_complete(
        html_to_pdf_pageless(temp_html, output_pdf)
    )
    print("Pageless PDF successfully generated:", output_pdf)

    # 5️⃣ Cleanup
    temp_html.unlink()


if __name__ == "__main__":
    main()
