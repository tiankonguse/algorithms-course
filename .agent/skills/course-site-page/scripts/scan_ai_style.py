#!/usr/bin/env python3
"""AI 味扫描（收口前必跑，2026-10-14 从 SKILL.md 内联块抽成脚本）。

用法（站点根目录执行）：
    python3 $SKILL/scan_ai_style.py                 # 扫 algorithms/*.html
    python3 $SKILL/scan_ai_style.py a.html b.html   # 只扫指定页

扫三类：
 1. 中文套话词表（下面 BAD）。词表只覆盖中文套话，英文类痕迹照 references/humanize.md 自己读一遍。
 2. 「不是 X，是 Y」否定并列——最隐蔽的 AI 句式。判据：在两个具体选项里挑一个的留
    （「写 1u << i 而不是 1 << i」），拿它当修辞强调的一律删。
 3. 顺带扫破折号 ——、通篇 <strong>、emoji。

技术对比类的「不是…而是…」是误报，留着；**站点老页面大量存在这些问题，只对自己这轮产出的页面负责**。
"""
import glob
import os
import re
import sys

BAD = ['总而言之', '毫无疑问', '不可否认', '综上所述', '双刃剑', '首先', '其次', '再者',
       '值得注意的是', '我们可以看到', '不难发现', '由此可见', '总体而言', '基于以上',
       '不仅', '赋能', '底层逻辑', '降维打击', '矩阵', '值得一提', '换句话说']

EMOJI = re.compile('[\U0001F300-\U0001FAFF]')


def norm(files):
    return [f if f.startswith('algorithms/') else os.path.join('algorithms', f) for f in files]


def main(argv):
    files = norm(argv[1:]) if len(argv) > 1 else sorted(glob.glob('algorithms/*.html'))
    hit = 0
    for f in files:
        if not os.path.exists(f):
            print('!! 找不到文件：', f)
            hit += 1
            continue
        s = open(f, encoding='utf-8').read()
        m = s[s.find('<main'):s.find('</main>')] or s
        t = re.sub(r'<[^>]+>', '', m)
        hits = [w for w in BAD if w in t]
        neg = [x.strip() for x in re.findall(r'[^。；！？\n]*?(?:不是|而是|并非)[^。；！？\n]*。', t)]
        dash = t.count('——')
        bold = s.count('<strong')
        emoji = len(EMOJI.findall(s))
        if hits or neg or dash or emoji:
            hit += 1
            print(f, '| 套话:', hits or '-', '| 否定并列:', len(neg),
                  '| 破折号:', dash, '| strong:', bold, '| emoji:', emoji)
            for x in neg:
                print('    ·', x[:80])
    print('=== 扫描 %d 个文件，%d 个有痕迹 ===' % (len(files), hit))


if __name__ == '__main__':
    main(sys.argv)
