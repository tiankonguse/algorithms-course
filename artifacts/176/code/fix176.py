#!/usr/bin/env python3
# 修复双层 pre 嵌套：去外层标签 + 每行剥 4 空格（apply_placeholders 的缩进）
import sys, re, html

def fix(page):
    s = open(page, encoding='utf-8').read()
    # 双层开标签归一（同一行的情况：<pre class="code">    <pre class="code">）
    s = s.replace('<pre class="code">    <pre class="code">', '<pre class="code">')
    s = s.replace('<pre class="code">\n    <pre class="code">', '<pre class="code">')
    # 闭合归一
    s = s.replace('</pre></pre>', '</pre>')
    out = []
    in_pre = False
    for line in s.split('\n'):
        if in_pre and line.startswith('    '):
            line = line[4:]
        if '<pre class="code">' in line:
            in_pre = True
            if line.lstrip().startswith('<pre class="code">') and line.startswith('    '):
                line = line[4:]
        if '</pre>' in line:
            in_pre = False
        out.append(line)
    open(page, 'w', encoding='utf-8').write('\n'.join(out))
    print('fixed', page)

def extract_pre(page):
    s = open(page, encoding='utf-8').read()
    blocks = re.findall(r'<pre class="code">(.*?)</pre>', s, re.S)
    return [html.unescape(re.sub(r'<[^>]+>', '', b)) for b in blocks]

def check_subseq(block_lines, src_lines):
    """block 是否为 src 的行级子序列"""
    it = iter(src_lines)
    i = 0
    for bl in block_lines:
        matched = False
        for sl in it:
            if sl == bl:
                matched = True
                break
        if not matched:
            return i, bl
        i += 1
    return -1, ''

for p in sys.argv[1:]:
    fix(p)

# 核对：页面 pre 块 vs 源码行级子序列
if len(sys.argv) > 1 and sys.argv[1] == 'check':
    pass
