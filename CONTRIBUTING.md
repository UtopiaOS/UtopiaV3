# Contributing to Utopia

First off, thank you for taking the time for reading and contributing to Utopia.

The following is a set of guidelines and recommendations in order to contribute to the Utopia Operating System, which are hosted by Trip Technologies on GitHub. These are mostly guidelines, not rules. Use your best judgment, and feel free to propose changes to this document in a pull request.

#### Table Of Contents

[Code of conduct](#code-of-conduct)

[I don't want to read this whole thing, I just have a question!!!](#i-dont-want-to-read-this-whole-thing-i-just-have-a-question)

## Code of conduct

This project and everyone participating in it is governed by the [Utopia Code of Conduct](CODE_OF_CONDUCT.md). By participating, you are expected to uphold this code. Please report unacceptable behavior to [contact@utopiaos.org](mailto:contact@utopiaos.org).

## I don't want to read this whole thing I just have a question!!!

> **Note:** Please don't file an issue to ask a question. You'll get faster results by using the resources below.

We have an official FAQ and an official Discord server where you can ask the community for helpful advice if you have any questions.

- [FAQ](https://utopiaos.github.io/faq.html)
- [Discord Server](https://discord.gg/sD7fMrmxBV)

## Issue policy 

Please follow the guidelines mentioned below, in order to report issues in Utopia OS:

- One bug per issue. Putting many bugs in the same issue or unrelated stuff to the bug makes it hard to track and will be deleted.
- Maximum of 3 ideas if you haven't contributed to Utopia, when you contribute to Utopia, you gain the opportunity to contribute more ideas.
- No build issues (not the same as build bugs), any build issues you experience should be reported on the `#build-assistance` channel on our Discord
- Don't comment on issues just comment, don't make jokes or irrelevant comments, we have the notifications enabled and this is just extremely annoying.

## Human language and communication

In our project Utopia, we treat language and communication as important as we treat code quality.

Be sure to follow the rules mentioned below to ensure proper communication.

- The official language of the project is English, and we prefer you to use metric system units, if you would like to use imperial, please include the conversion in parenthesis.
- The language tone is casual, thus, making jokes in the adequate channels is okay, we don't expect you to have perfect grammar and punctuation, but try to be as serious as possible and do your best.
- When in development channels or GitHub issues, be technical and describe everything in great detail.

## Code submission policy

We understand that this project isn't perfect, and it will probably never be, but we would like some guidelines before you contribute, below we explain and expose some Do's and Don'ts when contributing to Utopia.

**Do:**
- Write in idiomatic C11 BSD-style code in the case of C.
- Write in idiomatic Rust in the case of Rust projects.
- Conform to the project's formatting style by reading our [CodeStyle.md](CodeStyle.md) or by running `clang-format` and `cargo clippy` when necessary.
- Split changes into separate, atomic commits.
- Make sure every commit you do, doesn't break Utopia or prevents it from compiling (Breaking is not the same as breaking changes).
- Make sure your commits are rebased on the `master` branch.
- The first line of the commit message (The subject line), must have the following format: "[Category] Brief description of the change".
  - Examples: `Winter`, `libigloo`, `libSystem`
  - Don't use whole categories for your changes, for example "[Userland]" or "[Package manager]" unless you did substantial changes to a lot of said codebase, for example, implementing a new API in libSystem and making the userland use it.
  - Don't use templates/structs/classes/etc as Category names, for example do "[Winter] Improve the way we handle HTTPS" instead of "[DownloadEvent] Improve the way we handle HTTPS".
  - Multiple categories can be merged using the `+` sign, for example "[Winter+BigBrother]"
 - Use plural language like "we".
 - Commit messages on the subject line should be imperative, for example, "[Winter] Improve the way we handle HTTPS" and not "[Winter] Improved the way we handle HTTPS".
 - Write your commits in proper English, be careful of punctuation.
 - Add your personal copyright lines to new files you create or to substantial changes.
 - Check the spelling of your code, and comments.
 - Use american english, for example, use `color`instead of `colour`
- Preferably, your code should explain _why_ is doing what it is doing, not _what_ is doing, good code explains itself.

**Dont:**
- Touch anything outside of the scope of the PR.
- Overengineer your contribution.
- Use weasel-words that don't actually mean anything like fix, refactor, please be more specific.
- Include commented out code. 
- Use a period at the end of the commit message.
- Write in programming languages that aren't supported by Utopia.
- Attempt large architectural changes without discussing it with the maintainers first.
- Comment useless information just to comment.

## Pull request Q&A

**I've submitted a PR, and it passes the CI, when can I expect it to be merged?**

While your PR passing the CI is a good sign, specially if your PR closes an issue or implements a feature that has been requested and approved by the Utopia maintainers, it doesn't mean it might get merged right away, your pull request might need some reviews and changes before it can be part of the Utopia codebase.

Also, unadvertised PRs might sometimes fail to get attention, it is best to notify the maintainers via the Discord server, or ping the maintainers via a GitHub mention, however, once we have assigned reviewers to your PR, is best for you to be patient and wait for us to review your code.

**If my PR isn't getting attention, how do I get the maintainers to review it**

If your PR isn't getting attention, is probably a great idea to link it on the `#code-review` channel on the Discord server.

**Who are the project maintainers**

The current project maintainers are @DiegoMagdaleno @Spencer05 and @Luth31, although Luth has been inactive for quite some time.

**Are there specific people I can reach out for different components of the OS**

Yes, in order to find out who did what, it is a good idea to look at the Copyright headers, however as some of our code is derived from FreeBSD and OpenBSD, some headers might contain two persons, the original developer and the maintainer that ported it over to Utopia, to try to differentiate who is who, just look at the dates, the original BSD developments dates back to the 90s.

**Which one is the best place to discuss ideas or my PR Discord or GitHub**

Discord, we use Discord as our main way of communication, and it is best if you use Discord, however, once the process of discussing something has been done on Discord, it is great if you transition over to GitHub issues.

## General recommendations

If you really want to start contributing to Utopia, it is better to start doing so in a non-so-critical system component, or issues marked with the `good first issue` label, look for issues that you see and say "I can definitely take up on this" and feel free to ask any questions on our Discord.

We are also very open to criticism, the main developer of Utopia, is a 18 year old Mexican Computer Engineering Student, that of course, doesn't really have the experience or the knowledge to be developing its own OS, this could be the case of many, if you are more experienced and you have feedback, we are more than open to recieve your feedback so we can improve this operating system.

Contribute for fun, Utopia is not really a project you might be able to main on your desktop, sure it runs on top of the Linux kernel, but because of some of the changes (Like NeoHFS, mach-o, code-signing), some programs might be broken, we would love if in the future we could get some ports of this amazing programs, but for now, it is better to use Utopia as a cool PoC.

## Find documentation

In case you need documentation about the internals of the Utopia Operating system, the following resources are always ready to help and provide the latest information:

- [The Utopia book](https://utopiaos.github.io/book)

These resources, contain how the Utopia operating system works, it might also help to learn about the Linux kernel, as it is the Kernel used the by Utopia Operating System, while this might change in the future, it will stay like that for a couple of years.

## Credits

These guidelines are heavily inspired by the [Serenity OS project](https://github.com/serenityOS) and the [Atom project](https://github.com/atom).
