#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""把一课转换过程的珍贵中间产物归档到站点 artifacts/<课号>/：
   - /tmp/ppt<课号>.txt        -> artifacts/<课号>/ppt.txt（PPT 抽取文本，有才拷）
   - /tmp/cpp<课号>/ 下非 .bin 文件 -> artifacts/<课号>/code/（C++ 转换结果、对拍器 paipai*.py/.out/.log、辅助脚本）
   - 生成 artifacts/<课号>/index.json（课号/标题/页面清单/文件清单）
   - 生成 artifacts/<课号>/index.html（材料查看页：PPT 全文、C++ 源码、对拍程序与结果内联展示）
   - 重生成 assets/data/artifacts.json（页面 -> 归档入口映射，shared.js 用）

用法:
    python3 tools/archive_course.py 160 tot-basic.html tot-balanced.html tot-apps.html
    python3 tools/archive_course.py --backfill        # 批量回填（页名清单从 git 提交反查）
"""
import argparse
import html
import json
import os
import re
import shutil
import subprocess
import sys

SITE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
JOURNEY = os.path.expanduser('~/project/github/AIProject/algorithm-journey')
ART = os.path.join(SITE, 'artifacts')


def course_title(n):
    """从课件目录的 PPT/PDF 文件名取标题（算法讲解NNN【难度】标题.{pptx,pdf} -> 去掉扩展名）。"""
    d = os.path.join(JOURNEY, 'ppt')
    if not os.path.isdir(d):
        return '第 %d 课' % n
    prefix = '算法讲解%03d【' % n if n < 100 else '算法讲解%d【' % n
    for f in sorted(os.listdir(d)):
        if f.startswith(prefix) and f.endswith(('.pptx', '.pdf')):
            return f[:-5]
    return '第 %d 课' % n


KEEP_RE = re.compile(r'^paipai.*\.(out|log)$')


def keep(name):
    """只归档珍贵产物：源码、脚本、对拍结果；随机测试数据（in/want/txt 等）不入库。"""
    if name.endswith(('.cpp', '.cc', '.c', '.h', '.hpp', '.py')):
        return True
    return bool(KEEP_RE.match(name))


def is_binary(s):
    """按魔数判断编译产物（不只看扩展名）：ELF / Mach-O 直接跳过。"""
    try:
        with open(s, 'rb') as f:
            head = f.read(4)
    except OSError:
        return False
    if head[:4] == b'\x7fELF':
        return True
    return head in (b'\xcf\xfa\xed\xfe', b'\xca\xfe\xba\xbe', b'\xfe\xed\xfa\xcf', b'\xce\xfa\xed\xfe')


def iter_tmp_materials(n):
    """产出 (kind, src, rel_dst) 列表：kind in ('ppt','code')。"""
    ppt = '/tmp/ppt%d.txt' % n
    if os.path.isfile(ppt):
        yield ('ppt', ppt, 'ppt.txt')
    cpp = '/tmp/cpp%d' % n
    if os.path.isdir(cpp):
        for f in sorted(os.listdir(cpp)):
            s = os.path.join(cpp, f)
            if os.path.isfile(s) and keep(f) and not is_binary(s):
                yield ('code', s, os.path.join('code', f))


def build_viewer(n, title, pages, has_ppt, files):
    """生成材料查看页 HTML：内联展示 PPT 全文、C++ 源码、对拍程序与结果。"""
    esc = html.escape
    page_links = ' · '.join(
        '<a href="../algorithms/%s">%s</a>' % (esc(p), esc(p)) for p in pages)
    dl = []
    if has_ppt:
        dl.append('<a href="ppt.txt" download>ppt.txt</a>')
    for f in files:
        dl.append('<a href="%s" download>%s</a>' % (esc(f), esc(os.path.basename(f))))

    body = []
    body.append('<h1>%s</h1>' % esc(title))
    body.append('<p class="meta">课程 %d · 本课页面：%s</p>' % (n, page_links or '—'))
    body.append('<p class="meta">下载原始文件：%s</p>' % ('　'.join(dl) if dl else '—'))
    body.append('<p class="meta"><a href="../index.html">← 返回算法课程站首页</a></p>')

    if has_ppt:
        body.append('<h2>PPT 文本（讲稿抽取）</h2>')
        with open(os.path.join(ART, str(n), 'ppt.txt'), encoding='utf-8', errors='replace') as f:
            body.append('<pre>%s</pre>' % esc(f.read()))

    code_files = [f for f in files if not os.path.basename(f).startswith('paipai')]
    paipai_files = [f for f in files if os.path.basename(f).startswith('paipai')]

    if code_files:
        body.append('<h2>C++ 源码 / 辅助代码</h2>')
        base = os.path.join(ART, str(n))
        for f in code_files:
            body.append('<h3>%s</h3>' % esc(f))
            try:
                with open(os.path.join(base, f), encoding='utf-8', errors='replace') as fp:
                    body.append('<pre>%s</pre>' % esc(fp.read()))
            except OSError as e:
                body.append('<pre>读取失败：%s</pre>' % esc(str(e)))

    if paipai_files:
        body.append('<h2>对拍 / 压测程序与结果</h2>')
        base = os.path.join(ART, str(n))
        for f in paipai_files:
            body.append('<h3>%s</h3>' % esc(f))
            try:
                with open(os.path.join(base, f), encoding='utf-8', errors='replace') as fp:
                    body.append('<pre>%s</pre>' % esc(fp.read()))
            except OSError as e:
                body.append('<pre>读取失败：%s</pre>' % esc(str(e)))

    page = '''<!DOCTYPE html>
<html lang="zh-CN">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>%s · 课程材料</title>
<style>
  body{font-family:-apple-system,BlinkMacSystemFont,"Segoe UI","PingFang SC","Hiragino Sans GB","Microsoft YaHei",sans-serif;
       max-width:1000px;margin:0 auto;padding:28px 20px 64px;color:#1f2328;line-height:1.6}
  h1{font-size:22px;margin:0 0 6px}
  h2{font-size:17px;border-bottom:1px solid #e1e4e8;padding-bottom:6px;margin:36px 0 12px}
  h3{font-size:14px;margin:22px 0 6px;font-family:ui-monospace,SFMono-Regular,Menlo,monospace;color:#57606a}
  .meta{color:#57606a;font-size:13px;margin:4px 0}
  .meta a{color:#0b57d0;text-decoration:none}
  pre{background:#f6f8fa;border:1px solid #e1e4e8;border-radius:8px;padding:14px 16px;overflow-x:auto;
      font-size:13px;line-height:1.55;font-family:ui-monospace,SFMono-Regular,Menlo,Consolas,monospace;white-space:pre-wrap;word-break:break-word}
</style>
</head>
<body>
%s
</body>
</html>
''' % (esc('%d' % n), '\n'.join(body))
    return page


def archive_course(n, pages, title=None):
    d = os.path.join(ART, str(n))
    os.makedirs(os.path.join(d, 'code'), exist_ok=True)
    if title is None:
        title = course_title(n)

    has_ppt = False
    files = []
    fresh = set()
    for kind, src, rel in iter_tmp_materials(n):
        dst = os.path.join(d, rel)
        os.makedirs(os.path.dirname(dst), exist_ok=True)
        shutil.copy(src, dst)
        fresh.add(rel)
        if kind == 'ppt':
            has_ppt = True
        else:
            files.append(rel)

    # 清理上次归档留下的旧文件（比如白名单收紧后不再收录的测试数据）
    stale = []
    for f in os.listdir(os.path.join(d, 'code')):
        if ('code/' + f) not in fresh:
            stale.append(os.path.join(d, 'code', f))
    for s in stale:
        os.remove(s)

    meta = {
        'course': n,
        'title': title,
        'pages': [os.path.basename(p) for p in pages],
        'files': {'ppt': 'ppt.txt' if has_ppt else None, 'code': files},
    }
    with open(os.path.join(d, 'index.json'), 'w', encoding='utf-8') as f:
        json.dump(meta, f, ensure_ascii=False, indent=1)

    with open(os.path.join(d, 'index.html'), 'w', encoding='utf-8') as f:
        f.write(build_viewer(n, title, meta['pages'], has_ppt, files))

    subprocess.run([sys.executable, os.path.join(SITE, 'tools', 'build_artifacts.py')], check=True)
    print('已归档第 %d 课：%d 页，ppt=%s，code %d 个文件 -> %s'
          % (n, len(meta['pages']), '有' if has_ppt else '无', len(files),
             os.path.relpath(d, SITE)))


def backfill():
    """批量回填：找 /tmp 还有产物的课，页名清单从 git 提交反查。"""
    log = subprocess.run(['git', '-C', SITE, 'log', '--oneline'],
                         capture_output=True, text=True).stdout
    pages_by_course = {}
    for line in log.splitlines():
        # 兼容四种消息：「转换第 N 课…」「转换N课…（无第）」「N课二次转换…」「N 标题…（无课字）」
        m = re.match(r'^([0-9a-f]+)\s+(?:转换第?\s*)?(\d+)\s*(?:课|\S)', line)
        if not m:
            continue
        h, n = m.group(1), int(m.group(2))
        if not (1 <= n <= 207):
            continue
        out = subprocess.run(
            ['git', '-C', SITE, 'show', h, '--name-only', '--format='],
            capture_output=True, text=True).stdout
        pages = sorted({os.path.basename(x) for x in out.splitlines()
                        if x.startswith('algorithms/') and x.endswith('.html')})
        if pages:
            pages_by_course[n] = pages

    done, skipped = 0, []
    for n in sorted(pages_by_course):
        # 跳过锁还在的课（有人在转）
        if os.path.isfile('/tmp/algo-course-lock-%d/owner' % n):
            skipped.append(n)
            continue
        # 只有 /tmp 还有产物的课才回填
        if not (os.path.isfile('/tmp/ppt%d.txt' % n) or os.path.isdir('/tmp/cpp%d' % n)):
            continue
        archive_course(n, pages_by_course[n])
        done += 1
    print('回填完成：%d 课；跳过（锁占用）：%s' % (done, skipped or '无'))


def main():
    ap = argparse.ArgumentParser(description='归档一课材料到 artifacts/<课号>/')
    ap.add_argument('course', nargs='?', type=int, help='课号')
    ap.add_argument('pages', nargs='*', help='本课产出的页面文件名（xxx.html）')
    ap.add_argument('--backfill', action='store_true', help='批量回填 /tmp 存量')
    ap.add_argument('--title', help='课程标题（缺省从课件 PPT 文件名取）')
    args = ap.parse_args()

    if args.backfill:
        backfill()
        return
    if not args.course:
        ap.error('需要课号，或加 --backfill')
    if not args.pages:
        ap.error('缺少页面清单：python3 tools/archive_course.py <课号> <页1.html> [页2.html ...]')
    archive_course(args.course, args.pages, args.title)


if __name__ == '__main__':
    main()
