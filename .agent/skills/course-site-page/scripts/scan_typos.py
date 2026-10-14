#!/usr/bin/env python3
"""杂字 / 占位符 / 代码块嵌套扫描（收口前必跑，2026-10-14 从 SKILL.md 内联块抽成脚本）。

用法（站点根目录执行）：
    python3 $SKILL/scan_typos.py                 # 扫 algorithms/*.html
    python3 $SKILL/scan_typos.py a.html b.html   # 只扫指定页（文件名或 algorithms/x.html 都收）

判定三条：
 1. 杂字：ASCII 区间外的「外文字母」（俄文/希腊文混入等）。老页常见的 ᵢ Σ α ⁿ 这类下标与符号会被一起扫出，
    属正常忽略——只需要看有没有明显不是本站该出现的字母。
 2. 代码块真嵌套：<pre class="code"> 直接套 <pre class="code">（占位符外面多套了一层）。
    单独的 </pre></pre> 多闭合在 40 多个老页上都有、浏览器会忽略，不是自己这课的问题就别动别人的页。
 3. 占位符残留：@@xxx@@ 没被 apply_placeholders.py 替换掉。
"""
import glob
import os
import re
import sys


def norm(files):
    out = []
    for f in files:
        out.append(f if f.startswith('algorithms/') else os.path.join('algorithms', f))
    return out


def main(argv):
    files = norm(argv[1:]) if len(argv) > 1 else sorted(glob.glob('algorithms/*.html'))
    hit = 0
    for f in files:
        if not os.path.exists(f):
            print('!! 找不到文件：', f)
            hit += 1
            continue
        s = open(f, encoding='utf-8').read()
        bad = sorted({c for c in s if not c.isascii() and c.isalpha() and ord(c) < 0x4E00})
        flag = []
        if bad:
            flag.append('杂字' + str(bad))
        if re.search(r'<pre class="code">\s*<pre class="code">', s):
            flag.append('代码块真嵌套')
        if re.search(r'@@[A-Za-z0-9_]*@@', s):
            flag.append('占位符')
        if flag:
            hit += 1
            print(f, flag)
    print('=== 扫描 %d 个文件，%d 个有问题 ===' % (len(files), hit))


if __name__ == '__main__':
    main(sys.argv)
