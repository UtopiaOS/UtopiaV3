# F.A.Q

## Table of contents

## What is the goal of Utopia?

Utopia's goal is to provide a powerful and simple operating system, which is a generic purpose everyone seems to set for their project, Utopia is more than that.

Utopia's goal is to provide a tightly integrated operating system, where every component is connected with another, for example, Utopia benefits from it's own kernel custom-Linux based kernel,
and it reflects a lot of that functionality on the userland, for example:

- Mach-O as the default format, allows us to benefit from diverse advanced linking features Mach-O has, like symbol reexporting
- Our file hidder, allows us to hide legacy entries at a system level

These changes are reflected even in the code, for example Utopia's libc, is split into smaller libraries, this allows us to maintain specific parts of the codebase, while still representing our
system library as a whole.

## What aren't the goals of Utopia?

Utopia clearly has a particular vision, which end up being incompatible with some other philosophies:

1. Utopia's goal is not to be macOS compatible, while we have a lot of concepts brought up from macOS (Mach Ports, Mach-O binaries, LaunchServices, Frameworks and single installed apps) our purpose isn't for us to be _source_ or _binary_ compatible with macOS, for that, you the amazing project know as [AiryxOS](https://airyxos.org).

2. Utopia's goal is not to be POSIX compatible, Utopia might or might not break the POSIX standard, we consider Utopia a playground, for our dreams and ideas, while standards are amazing for big projects, we feel like this limits us when making a full blown operating system

## Why the Linux kernel?

This is a great question! if we are making an operating system from scratch (Not really) we should have our own kernel right?, well you are certantly right, but this was beyond the scope of our project, Utopia was started by a single developer while in university, and it was extreamly hard to develop our own kernel, so yes, Utopia uses the Linux kernel for convenience.
