#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""扫描 artifacts/*/index.json，生成 assets/data/artifacts.json。

产物是「页面文件名(小写) -> 归档入口」的映射，shared.js 在算法页加载时按当前页查表，
命中就在页底注入「本课材料」入口（指向 artifacts/<课号>/index.html）。

用法:
    python3 tools/build_artifacts.py            # 生成映射（无归档时写空对象）
"""
import glob
import json
import os

SITE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ART = os.path.join(SITE, 'artifacts')
OUT = os.path.join(SITE, 'assets', 'data', 'artifacts.json')


def main():
    mapping = {}
    for p in sorted(glob.glob(os.path.join(ART, '*', 'index.json'))):
        try:
            d = json.load(open(p, encoding='utf-8'))
        except Exception:
            continue
        course = d.get('course')
        if not course:
            continue
        href = '../artifacts/%s/index.html' % course
        for page in d.get('pages', []):
            key = os.path.basename(str(page)).lower()
            if not key.endswith('.html'):
                continue
            mapping[key] = {
                'course': course,
                'href': href,
                'title': d.get('title', ''),
            }
    os.makedirs(os.path.dirname(OUT), exist_ok=True)
    tmp = OUT + '.tmp'
    with open(tmp, 'w', encoding='utf-8') as f:
        json.dump(mapping, f, ensure_ascii=False, indent=1, sort_keys=True)
    os.replace(tmp, OUT)
    courses = sorted({v['course'] for v in mapping.values()})
    print('artifacts 映射：%d 页 → %d 课%s，已写 %s'
          % (len(mapping), len(courses),
             '（%s）' % '/'.join(map(str, courses)) if courses else '',
             os.path.relpath(OUT, SITE)))


if __name__ == '__main__':
    main()
