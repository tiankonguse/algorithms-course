#!/usr/bin/env python3
import re, html, os, sys

def page_blocks(path):
    s = open(path, encoding='utf-8').read()
    blocks = re.findall(r'<pre class="code">(.*?)</pre>', s, re.S)
    out = []
    for b in blocks:
        b = re.sub(r'<[^>]+>', '', b)
        b = html.unescape(b)
        lines = [re.sub(r'[ \t]+', ' ', ln).strip() for ln in b.split('\n')]
        lines = [ln for ln in lines if ln]
        out.append(lines)
    return out

def course_lines(path):
    s = open(path, encoding='utf-8').read()
    lines = [re.sub(r'[ \t]+', ' ', ln).strip() for ln in s.split('\n')]
    lines = [ln for ln in lines if ln and not ln.startswith('//')]
    return lines

def diff(page_lines, course):
    missing = []
    j = 0
    for ln in course:
        found = False
        while j < len(page_lines):
            if page_lines[j] == ln:
                found = True
                j += 1
                break
            j += 1
        if not found:
            missing.append(ln)
    return missing

CP = '/tmp/cpp200'
maps = {
  'algorithms/cactus-dp.html': [('Code01_CactusDeleteEdge2.cpp','题1 删边'), ('Code02_CactusAddEdge2.cpp','题2 加边'),
                     ('Code03_CactusLongestRoute2.cpp','题3 最长路线'), ('Code04_CactusIndependentSet2.cpp','题4 独立集'), ('Code05_CactusDiameter2.cpp','题5 直径')],
  'algorithms/cactus-graph.html': [('Code06_CactusShortestPaths2.cpp','题6 最短路'), ('Code07_MaximumOfShortestPath2.cpp','题7 最短路最大值'), ('Code08_CactusPathFlip2.cpp','题8 路径翻转')],
}
for page, items in maps.items():
    blocks = page_blocks(page)
    print('=== %s: %d 个代码块' % (page, len(blocks)))
    for i, b in enumerate(blocks):
        print('  块%d: %d 行' % (i + 1, len(b)))
    for name, label in items:
        f = os.path.join(CP, name)
        if not os.path.exists(f):
            continue
        course = course_lines(f)
        best = None
        for i, b in enumerate(blocks):
            if len(b) < len(course) * 0.6 or len(b) > len(course) * 1.4:
                continue
            miss = diff(b, course)
            if best is None or len(miss) < len(best[1]):
                best = (i, miss)
        if best:
            i, miss = best
            print('  %s: 课程 %d 行 vs 页面块%d %d 行, missing=%d' % (label, len(course), i + 1, len(blocks[i]), len(miss)))
            for m in miss[:8]:
                print('      MISS: ' + m[:90])
        else:
            print('  %s: 无匹配块!' % label)
