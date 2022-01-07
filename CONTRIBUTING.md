# Contributing to Utopia

First off thank you for taking the time for reading and contributing to Utopia.

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
- Don't comment on issues just comment, don't make jokes or irrelevant comments, we have the notifications enabled and this is just extreamly annoying.

## Human language and communication

In our project Utopia, we treat language and communication as important as we treat code quality.

Be sure to follow the rules mentioned below to ensure proper communication.

- The official language of the project is english, and we prefer you to use metric system units, if you would like to use imperial, please include the conversion in parethesis.
- The language tone is casual, thus, making jokes in the addecuate channels is okay, we don't expect you to have perfect grammar and punctuation, but try to be as serious as possible and do your best.
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
  - Don't use whole cathegories for your changes, for example "[Userland]" or "[Package manager]" unless you did substantial changes to a lot of said codebase, for example, implementing a new API in libSystem and making the userland use it.
  - Don't use templates/structs/classes/etc as Category names, for example do "[Winter] Improve the way we handle HTTPS" instead of "[DownloadEvent] Improve the way we handle HTTPS".
  - Multiple categories can be merged using the `+` sign, for example "[Winter+BigBrother]"
 - Use plural language like "we".
 - Commit messages on the subject line should be imperative, for example, "[Winter] Improve the way we handle HTTPS" and not "[Winter] Improved the way we handle HTTPS".
 - Write your commits in proper english, be careful of punctuation.
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
