#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""递归扫描 nav/ 目录树，生成 assets/data/algorithms.json。

导航的真值是 nav/ 目录树，每层只管自己这一层：

    nav/index.json                          [{dir, name, desc}]  一级分类，数组顺序即展示顺序
    nav/<cat>/index.json                    [{dir, name, desc}]  该一级下的二级分类
    nav/<cat>/<group>/index.json            [{file, title, description, level}]  该二级下的页面
    file 只写文件名，实际路径由本脚本拼成 algorithms/<file>

用法:
    python3 tools/build_nav.py                     生成（覆盖 assets/data/algorithms.json）
    python3 tools/build_nav.py --check             只校验不写文件
    python3 tools/build_nav.py --strict            警告升级为错误
    python3 tools/build_nav.py --stats             只打印统计
    python3 tools/build_nav.py --verify-against F  与旧 json 做六层比对
"""
import argparse
import json
import os
import re
import sys
from collections import Counter

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
NAV = os.path.join(ROOT, 'nav')
ALGO = os.path.join(ROOT, 'algorithms')
OUT = os.path.join(ROOT, 'assets', 'data', 'algorithms.json')

SLUG_RE = re.compile(r'^[a-z0-9]+(?:-[a-z0-9]+)*$')
FILE_RE = re.compile(r'^[a-z0-9][a-z0-9._-]*\.html$')
CAT_FIELDS = {'dir', 'name', 'desc'}
GRP_FIELDS = {'dir', 'name', 'desc'}
PAGE_FIELDS = {'file', 'title', 'description', 'level'}

ERRORS = []
WARNS = []


def err(msg):
    ERRORS.append(msg)
    print('  [ERROR] ' + msg)


def warn(msg, strict):
    if strict:
        err(msg)
    else:
        WARNS.append(msg)
        print('  [WARN ] ' + msg)


def load_index(path, what):
    """E1：文件不存在 / 不是合法 JSON / 顶层不是数组"""
    if not os.path.isfile(path):
        err('%s：%s 不存在' % (what, rel(path)))
        return None
    try:
        with open(path, encoding='utf-8') as f:
            data = json.load(f)
    except Exception as e:
        err('%s：%s 不是合法 JSON（%s）' % (what, rel(path), e))
        return None
    if not isinstance(data, list):
        err('%s：%s 顶层必须是数组' % (what, rel(path)))
        return None
    return data


def rel(path):
    return os.path.relpath(path, ROOT)


def check_node(item, fields, what, where, allow_empty_desc=False):
    """E2/E8：缺字段、多字段、值为空串一律报错。多字段专抓 desc/desciption 这类手误。"""
    if not isinstance(item, dict):
        err('%s：%s 条目不是对象 -> %r' % (what, where, item))
        return False
    miss = fields - set(item)
    extra = set(item) - fields
    if miss:
        err('%s：%s 缺字段 %s -> %r' % (what, where, sorted(miss), item))
    if extra:
        err('%s：%s 多字段 %s（字段名写错？）-> %r' % (what, where, sorted(extra), item))
    ok = not (miss or extra)
    for k in sorted(fields & set(item)):
        if k == 'level':            # level 是 1~5 的整数，由 E12 单独校验
            continue
        v = item[k]
        if not isinstance(v, str) or (not v.strip() and not (k == 'desc' and allow_empty_desc)):
            err('%s：%s 字段 %s 必须是非空字符串 -> %r' % (what, where, k, item))
            ok = False
    return ok


def scan(strict):
    """递归扫描 nav/，返回 (categories, groups, algorithms)。顺序只来自各层数组的书写顺序。"""
    root_items = load_index(os.path.join(NAV, 'index.json'), '一级列表')
    if root_items is None:
        return None

    cat_names, cat_dirs = [], []
    seen_cat_name, seen_cat_dir = {}, {}
    for i, it in enumerate(root_items):
        where = 'nav/index.json[%d]' % i
        # 一级 desc 允许为空（生成物里 categories 只输出名字）
        if not check_node(it, CAT_FIELDS, '一级列表', where, allow_empty_desc=True):
            continue
        d, n = it.get('dir'), it.get('name')
        if d and not SLUG_RE.match(d):
            err('一级列表：%s 的 dir %r 不符合 slug 规则（小写字母数字 + 连字符）' % (where, d))
            continue
        if d in seen_cat_dir:                                       # E4
            err('一级列表：dir %r 重复（%s 与 %s）' % (d, seen_cat_dir[d], where))
            continue
        if n in seen_cat_name:                                      # E5
            err('一级列表：name %r 重复（%s 与 %s）' % (n, seen_cat_name[n], where))
            continue
        seen_cat_dir[d] = where
        seen_cat_name[n] = where
        cat_names.append(n)
        cat_dirs.append((d, n))

    groups, algos = [], []
    seen_grp_name, seen_grp_dir = {}, {}
    seen_files = {}
    referenced = set()
    listed_group_dirs = set()

    for cdir, cname in cat_dirs:
        cpath = os.path.join(NAV, cdir)
        if not os.path.isdir(cpath):                                # E7
            err('一级 %s：目录 nav/%s/ 不存在' % (cname, cdir))
            continue
        sub_items = load_index(os.path.join(cpath, 'index.json'), '二级列表 nav/%s' % cdir)
        if sub_items is None:
            continue
        for j, g in enumerate(sub_items):
            where = 'nav/%s/index.json[%d]' % (cdir, j)
            if not check_node(g, GRP_FIELDS, '二级列表', where):
                continue
            gd, gname = g.get('dir'), g.get('name')
            if gd and not SLUG_RE.match(gd):
                err('二级列表：%s 的 dir %r 不符合 slug 规则' % (where, gd))
                continue
            if gd in seen_grp_dir:                                  # E6 全局唯一
                err('二级列表：dir %r 全局重复（%s 与 %s）' % (gd, seen_grp_dir[gd], where))
                continue
            if gname in seen_grp_name:                              # E5 全局唯一
                err('二级列表：name %r 全局重复（%s 与 %s）' % (gname, seen_grp_name[gname], where))
                continue
            seen_grp_dir[gd] = where
            seen_grp_name[gname] = where
            listed_group_dirs.add((cdir, gd))

            gpath = os.path.join(cpath, gd)
            if not os.path.isdir(gpath):                            # E7
                err('二级 %s：目录 nav/%s/%s/ 不存在' % (gname, cdir, gd))
                continue
            pages = load_index(os.path.join(gpath, 'index.json'), '页面列表 nav/%s/%s' % (cdir, gd))
            if pages is None:
                continue

            groups.append({'name': gname, 'cat': cname, 'desc': g['desc']})
            if not pages:
                warn('二级 %s（nav/%s/%s）页面数为 0，不会出现在站点上' % (gname, cdir, gd), strict)
                continue

            for k, p in enumerate(pages):
                pwhere = 'nav/%s/%s/index.json[%d]' % (cdir, gd, k)
                if not check_node(p, PAGE_FIELDS, '页面列表', pwhere):
                    continue
                f = p.get('file')
                if f and not FILE_RE.match(f):                      # E9
                    err('页面列表：%s 的 file %r 不合法（小写字母数字开头、.html 结尾、不含路径）'
                        % (pwhere, f))
                    continue
                if f in seen_files:                                 # E11
                    err('页面列表：file %r 被重复收录（%s 与 %s）' % (f, seen_files[f], pwhere))
                    continue
                lv = p.get('level')
                if isinstance(lv, bool) or not isinstance(lv, int) or not (1 <= lv <= 5):
                    err('页面列表：%s 的 level %r 必须是 1~5 的整数' % (pwhere, lv))  # E12
                    continue
                seen_files[f] = pwhere
                referenced.add(f)
                algos.append({'file': 'algorithms/' + f,
                              'title': p['title'],
                              'description': p['description'],
                              'group': gname,
                              'level': lv})

    orphan_scan(referenced, {c for c, _ in cat_dirs}, listed_group_dirs, strict)
    return cat_names, groups, algos


def orphan_scan(referenced, listed_cat_dirs, listed_group_dirs, strict):
    """孤儿四类：磁盘有页没收录 / 收录了磁盘没有（含大小写）/ nav 下有野目录 / 空组（已在 scan 里报）"""
    disk = set(f for f in os.listdir(ALGO) if f.endswith('.html'))

    for f in sorted(disk - referenced):                              # W1
        warn('孤儿页 algorithms/%s：磁盘存在但没有任何 index.json 收录' % f, strict)

    lower = {x.lower(): x for x in disk}
    for f in sorted(referenced - disk):                              # E10/E10b
        err('悬空条目 %s：nav 收录但 algorithms/ 下不存在' % f)
        if f.lower() in lower:
            err('  大小写不一致：磁盘实际是 %s（mac 能跑，Linux 部署会 404）' % lower[f.lower()])

    for cdir in sorted(d for d in os.listdir(NAV) if os.path.isdir(os.path.join(NAV, d))):
        if cdir not in listed_cat_dirs:                              # W2
            warn('野目录 nav/%s/：nav/index.json 未登记，该分支不会出现在站点上' % cdir, strict)
        cpath = os.path.join(NAV, cdir)
        for gd in sorted(d for d in os.listdir(cpath) if os.path.isdir(os.path.join(cpath, d))):
            if (cdir, gd) not in listed_group_dirs:                  # W2
                warn('野目录 nav/%s/%s/：父层未登记，该组页面不会出现在站点上' % (cdir, gd), strict)


def stale_check(new_obj):
    """零状态陈旧检测：重算一份与磁盘现有 json 比对，抓「有人直接手改了生成物」。"""
    if not os.path.isfile(OUT):
        return
    try:
        cur = json.load(open(OUT, encoding='utf-8'))
    except Exception:
        return
    old = [(g['name'], g['cat'], g['desc']) for g in cur.get('groups', [])]
    new = [(g['name'], g['cat'], g['desc']) for g in new_obj['groups']]
    old_pages = per_group(cur.get('algorithms', []))
    new_pages = per_group(new_obj['algorithms'])
    if cur.get('categories') != new_obj['categories'] or old != new or old_pages != new_pages:
        print('  [WARN ] %s 与 nav/ 不一致 —— 有人直接手改了生成物，本次将被覆盖'
              % rel(OUT))


def per_group(algos):
    m = {}
    for a in algos:
        m.setdefault(a['group'], []).append(
            (a['file'], a['title'], a['description'], a['level']))
    return m


def flat_of(data):
    """照抄 shared.js:286-303 的铺平逻辑：categories 序 → groups 序 → 组内序"""
    by = {}
    for a in data['algorithms']:
        by.setdefault(a['group'], []).append(a)
    out = []
    for c in data['categories']:
        for g in data['groups']:
            if g['cat'] == c and by.get(g['name']):
                out.extend(by[g['name']])
    return out


def emit(obj):
    text = json.dumps(obj, ensure_ascii=False, indent=2)
    tmp = OUT + '.tmp'
    with open(tmp, 'w', encoding='utf-8') as f:
        f.write(text)
    os.replace(tmp, OUT)


def stats(obj):
    flat = flat_of(obj)
    nonempty = len({g['cat'] for g in obj['groups']
                    if any(a['group'] == g['name'] for a in obj['algorithms'])})
    print('一级分类 %d（非空 %d）｜二级分类 %d｜页面 %d'
          % (len(obj['categories']), nonempty, len(obj['groups']), len(obj['algorithms'])))
    print('难度分布 %s' % dict(sorted(Counter(a['level'] for a in obj['algorithms']).items())))


def verify(old_path, obj):
    """六层比对。注意：不按下标比 algorithms 数组（生成器按组输出必然重排）。"""
    old = json.load(open(old_path, encoding='utf-8'))
    ok = True

    def chk(cond, msg):
        nonlocal ok
        print('  [%s] %s' % ('OK  ' if cond else 'FAIL', msg))
        ok = ok and cond

    chk(set(obj) == {'categories', 'groups', 'algorithms'}, 'L1 顶层三键一致')
    chk(all('id' not in a for a in obj['algorithms']), 'L1 新产物不含 id')
    chk(obj['categories'] == old['categories'], 'L2 categories 同序（%d）' % len(old['categories']))

    f3 = lambda d: [(g['name'], g['cat'], g['desc']) for g in d['groups']]
    chk(f3(obj) == f3(old), 'L3 groups 三元组同序（%d）' % len(old['groups']))

    ng, og = per_group(obj['algorithms']), per_group(old['algorithms'])
    bad = [g for g in og if og[g] != ng.get(g)]
    chk(not bad and set(ng) == set(og), 'L4 每组页面序列一致（不一致组 %s）' % bad[:5])

    of = [a['file'] for a in flat_of(old)]
    nf = [a['file'] for a in flat_of(obj)]
    chk(of == nf, 'L5 flat 铺平顺序一致 = 上下篇顺序一字不差（%d 条）' % len(of))

    weird = [a['id'] for a in old['algorithms']
             if a['id'] not in (a['file'].split('/')[-1][:-5],
                                a['file'].split('/')[-1][:-5].replace('-', '_'))]
    n_diff = sum(1 for a in old['algorithms'] if a['id'] != a['file'].split('/')[-1][:-5])
    chk(not weird, 'L6 丢弃的 id 全部可由 file 复原（%d 条 id≠basename，异常 %d）'
        % (n_diff, len(weird)))

    # 补一条：406 个页面的上下篇链逐一模拟，旧（按 id）与新（按 file）必须完全一致
    def pager_map(data, key):
        flat = flat_of(data)
        pos = {a[key]: i for i, a in enumerate(flat)}
        res = {}
        for a in flat:
            i = pos[a[key]]
            res[a['file']] = (flat[i - 1]['title'] if i > 0 else None,
                              flat[i + 1]['title'] if i < len(flat) - 1 else None)
        return res
    old_pager = pager_map(old, 'id')
    new_pager = pager_map(obj, 'file')
    diff = [f for f in old_pager if old_pager[f] != new_pager.get(f)]
    chk(not diff, 'L7 406 页上下篇链逐页一致（差异 %s）' % diff[:5])
    return ok


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--check', action='store_true', help='只校验不写文件')
    ap.add_argument('--strict', action='store_true', help='警告升级为错误')
    ap.add_argument('--stats', action='store_true', help='只打印统计')
    ap.add_argument('--verify-against', metavar='JSON', help='与旧 json 做六层比对')
    args = ap.parse_args()

    result = scan(args.strict)
    if result is None:
        print('\n%d 个错误，生成中止' % len(ERRORS))
        sys.exit(2)
    cat_names, groups, algos = result
    obj = {'categories': cat_names, 'groups': groups, 'algorithms': algos}

    ok = True
    if args.verify_against:
        ok = verify(args.verify_against, obj)

    if args.stats:
        stats(obj)

    if ERRORS:
        print('\n%d 个错误，%s' % (len(ERRORS), '未写入' if not args.check else '校验失败'))
        sys.exit(2)
    if WARNS:
        print('\n%d 条警告' % len(WARNS))

    if args.check or args.verify_against:
        print('\n校验通过：一级 %d / 二级 %d / 页面 %d'
              % (len(cat_names), len(groups), len(algos)))
        sys.exit(0 if ok else 1)

    stale_check(obj)
    emit(obj)
    stats(obj)
    print('已生成 %s' % rel(OUT))


if __name__ == '__main__':
    main()
