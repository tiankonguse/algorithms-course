#!/usr/bin/env python3
"""把 class203 抽出的 C++ 源码转成站内 <pre class="code"> 的 HTML 片段。"""
import os, re, sys

SRC = os.environ.get('CPP_SRC', '/tmp/cpp203')
OUT = os.environ.get('HL_OUT', '/tmp/hl203/out')
os.makedirs(OUT, exist_ok=True)

KW = [
    'using', 'namespace',
    'void', 'bool', 'char', 'short', 'int', 'long', 'float', 'double',
    'unsigned', 'signed', 'const', 'struct', 'class', 'typename', 'template',
    'return', 'if', 'else', 'for', 'while', 'do', 'switch', 'case', 'default',
    'break', 'continue', 'true', 'false', 'nullptr', 'new', 'delete',
    'static', 'inline', 'public', 'private', 'protected', 'operator', 'sizeof',
    'll', 'string',
]
KW_RE = re.compile(r'\b(' + '|'.join(KW) + r')\b')
FN_TYPES = r'(?:void|bool|char|short|int|long|float|double|unsigned|signed|ll)'
FN_RE = re.compile(r'\b(' + FN_TYPES + r')\s+([A-Za-z_]\w*)(\s*\()')
# pair<...> key(...) 这种返回类型带尖括号的
FN2_RE = re.compile(r'(&gt;\s+)([A-Za-z_]\w*)(\s*\()')

ESC = {'&': '&amp;', '<': '&lt;', '>': '&gt;'}


def split_comment(line):
    """把一行拆成 (代码, 注释)，注释不含 // 本身之外的引号陷阱（本题代码无此情况）。"""
    in_str = False
    i = 0
    while i < len(line) - 1:
        c = line[i]
        if c == '"' and (i == 0 or line[i - 1] != '\\'):
            in_str = not in_str
        elif c == '/' and line[i + 1] == '/' and not in_str:
            return line[:i], line[i:]
        i += 1
    return line, ''


def esc(s):
    return ''.join(ESC.get(c, c) for c in s)


def hl(code):
    code, comment = split_comment(code)
    code = esc(code)
    # #include 单独认，避免里面的 include 再次被关键词规则扫到
    code = code.replace('#include', '\x07')
    # 先认函数定义名，用哨兵保护，避免后面关键词扫描二次破坏
    code = FN_RE.sub(lambda m: m.group(1) + ' ' + '\x01' + m.group(2) + '\x02' + m.group(3), code)
    code = FN2_RE.sub(lambda m: m.group(1) + '\x01' + m.group(2) + '\x02' + m.group(3), code)
    code = KW_RE.sub(r'<span class="kw">\1</span>', code)
    code = code.replace('\x07', '<span class="kw">#include</span>')
    code = code.replace('\x01', '<span class="fn">').replace('\x02', '</span>')
    if comment:
        code += '<span class="cm">' + esc(comment) + '</span>'
    return code


def convert(path, name):
    with open(path, encoding='utf-8') as f:
        raw = f.read().rstrip('\n')
    lines = [hl(l) for l in raw.split('\n')]
    body = '<pre class="code">' + '\n'.join(lines) + '</pre>'
    with open(os.path.join(OUT, name + '.html'), 'w', encoding='utf-8') as f:
        f.write(body)
    return len(raw.split('\n'))


if __name__ == '__main__':
    for fn in sorted(os.listdir(SRC)):
        if fn.endswith('.cpp'):
            n = convert(os.path.join(SRC, fn), fn[:-4])
            print(fn, n, 'lines')
