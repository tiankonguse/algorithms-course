# Humanizer（内联副本，无需安装）

去掉文本里的 AI 写作痕迹，让它读起来像人写的。

## Installation

**不用安装。** 这里是原 `humanizer` 技能（v2.1.1）内容的内联副本，`course-site-page` 自带的写作规则就在旁边两个文件里：

- `references/humanize.md` —— 完整 24 条模式与改法（收口扫描时查这份）
- `SKILL.md`「写作风格」块 —— 本站落地的精简版

本技能不依赖项目外的任何技能，别再去装 `humanizer` 或调 `去AI味`。

## Usage

写作或收口时按这两个文件自查，不单独触发别的技能：

```
按 references/humanize.md 过一遍这段正文
```

Or invoke directly when editing documents.

## Overview

Based on [Wikipedia's "Signs of AI writing"](https://en.wikipedia.org/wiki/Wikipedia:Signs_of_AI_writing) guide, maintained by WikiProject AI Cleanup. This comprehensive guide comes from observations of thousands of instances of AI-generated text.

### Key Insight

> "LLMs use statistical algorithms to guess what should come next. The result tends toward the most statistically likely result that applies to the widest variety of cases."

## 24 Patterns Detected

### Content Patterns
1. **Significance inflation** - "marking a pivotal moment..." → specific facts
2. **Notability name-dropping** - listing sources without context
3. **Superficial -ing analyses** - "symbolizing... reflecting..."
4. **Promotional language** - "nestled within the breathtaking..."
5. **Vague attributions** - "Experts believe..."
6. **Formulaic challenges** - "Despite challenges... continues to thrive"

### Language Patterns
7. **AI vocabulary** - "Additionally... testament... landscape..."
8. **Copula avoidance** - "serves as" instead of "is"
9. **Negative parallelisms** - "It's not just X, it's Y"
10. **Rule of three** - forcing ideas into groups of three
11. **Synonym cycling** - excessive synonym substitution
12. **False ranges** - "from X to Y" on non-meaningful scales

### Style Patterns
13. **Em dash overuse**
14. **Boldface overuse**
15. **Inline-header lists**
16. **Title Case Headings**
17. **Emoji decoration**
18. **Curly quotation marks**

### Communication Patterns
19. **Chatbot artifacts** - "I hope this helps!"
20. **Cutoff disclaimers** - "While details are limited..."
21. **Sycophantic tone** - "Great question!"

### Filler and Hedging
22. **Filler phrases** - "In order to", "Due to the fact that"
23. **Excessive hedging** - "could potentially possibly"
24. **Generic conclusions** - "The future looks bright"

## Full Example

**Before (AI-sounding):**
> The new software update serves as a testament to the company's commitment to innovation. Moreover, it provides a seamless, intuitive, and powerful user experience—ensuring that users can accomplish their goals efficiently.

**After (Humanized):**
> The software update adds batch processing, keyboard shortcuts, and offline mode. Early feedback from beta testers has been positive, with most reporting faster task completion.

## References

- [Wikipedia: Signs of AI writing](https://en.wikipedia.org/wiki/Wikipedia:Signs_of_AI_writing)
- [WikiProject AI Cleanup](https://en.wikipedia.org/wiki/Wikipedia:WikiProject_AI_Cleanup)

## License

MIT
