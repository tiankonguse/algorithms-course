#!/usr/bin/env python3
"""把页面里的 @@NAME@@ 占位符替换成 /tmp/hl203/out/NAME.html 的内容。"""
import os, re, sys

OUT = os.environ.get('HL_OUT', '/tmp/hl203/out')


def make(name):
    path = os.path.join(OUT, name + '.html')
    if not os.path.exists(path):
        raise SystemExit('missing highlight: ' + path)
    return open(path, encoding='utf-8').read()


def apply(page, indent='    '):
    src = open(page, encoding='utf-8').read()
    used = []

    def sub(m):
        name = m.group(1)
        used.append(name)
        body = make(name)
        return '\n'.join(indent + ln if ln else ln for ln in body.split('\n'))

    out = re.sub(r'@@([A-Za-z0-9_]+)@@', sub, src)
    open(page, 'w', encoding='utf-8').write(out)
    print(page, '->', ', '.join(used))


if __name__ == '__main__':
    for p in sys.argv[1:]:
        apply(p)
