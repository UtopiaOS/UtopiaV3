# F.A.Q

## Table of contents

[What is the goal of Utopia?](what-is-the-goal-of-utopia)

[What aren't the goals of Utopia](what-arent-the-goals-of-utopia)

[Why the Linux kernel?](#why-the-linux-kernel)

[Will Utopia have $THING?](#will-utopia-have-thing)

[When you will you implement $THING?](#when-will-you-implement-thing)

[Is Utopia open to feedback?](#is-utopia-open-to-feedback)

[Where are the ISO images?](#where-are-the-iso-images)

[How do I run this thing?](#how-do-i-run-this-thing)

[I did a git pull and the build is broken!](#i-did-a-git-pull-and-the-build-is-broken)

[Why don't you use $THING instead of implementing it yourself?](#why-dont-you-use-thing-instead-of-implementing-it-yourself)

[Why yet another OS?](#why-yet-another-os)

## What is the goal of Utopia?

Utopia's goal is to redesign and re-imagine a lot of the concepts we use in Linux (Or UNIX-like computers) today so everything feels like a cohesive operating system.

The goal of Utopia is to be a playground that satisfies the necessities the user might have, a new breeding ground for ideas and modern concepts.

Is a project made for fun, made to showcase how an OS would look like in our vision.

## What aren't the goals of Utopia?

1. Utopia's goal isn't to be most popular operating system: Unlike many other projects Utopia's goal isn't to become the #1 OS or to even fuel the so called "year of Linux on the desktop"

2. Utopia's goal isn't to be compatible with current standards: Utopia is a breeding ground for experimentation, and we believe that following already established standard limits our creativity and limits us to what we can do, this does not mean that we aren't open to implement some standards in the form of compatibility layers in Utopia (For example POSIX).

3. Utopia's goal is not be macOS compatible: While it might look like we are striking to redo macOS (After all, we use Mach ports, Mach-O binaries, we have LaunchServices, Frameworks and drag and drop applications), we target the Linux kernel, the Utopia ABI and POSIX exclusively, we do not aim to provide neither _source_ or _binary_ compatibility with macOS, however, we would be very excited if you 
bring your macOS program to Utopia, if you do want a macOS compatible operating system, check out [AiryxOS](https://airyxos.org).

4. Utopia's goal is not be minimal: We truly believe in simplicity! But simplicity does not equal minimalism, even if used interchangeably in this day and age, Utopia aims to have a unique user experience, and we consider that minimalism just for the sake of being minimal limits our creativity.

## Why the Linux kernel?

This is a great question, and it seems like Google answered it already with Android (We know they are making their own kernel now): It works well enough, while we don't like or particularly enjoy the GNU userland, we do like the way the Linux kernel works, with all its drivers, stability and in general extensibility, makes a great candidate for an operating system, while there are other options (BSD, XNU) we choose Linux, because at the time of starting Utopia it was what the original creator (Diego Magdaleno) was more experienced on and comfortable with, and he didn't want to write his own kernel.

## Will Utopia have `$THING`

Maybe! Maybe not. There is not plan. (Most of the time)

## When will you implement `$THING`

Maybe tomorrow. Maybe someday. Maybe never. If you want to see something happen, you can do it yourself!

## Is Utopia open to feedback?

Yes! We love feedback, put before doing so, it is recommend to read the contributing guidelines, your voice is better heard when what you speak is clear.

## Where are the ISO images?

For now Utopia does not have an ISO image, are there plans for one?, not for now, Utopia (as of now) targets developers, computer enthusiasts and technical users.

## How do I run this thing?

Well, you just need a Linux distro and the [build instructions!](https://github.com/UtopiaOS/Utopia/blob/master/Documentation/Building.md)

## I did a `git pull` and the build is broken!

If it builds on our CI then it should build for you too. You might need to rebuild the toolchain, or clean the repo.

We are happy to help, ask in the `#build-problems`channel on Discord if you can't seem to figure it out!

## Why don't you use `$THING` instead of implementing it yourself?

Well, we do use third party software in Utopia!, for starters some of our libraries and executables derive from other libraries or operating systems, but we like to implement things ourselves, mainly for fun!

## Why yet another OS?

Why not? It is a fun experiment everyone should try someday! Why yet another car brand? Why yet another `$THING`the answer is freedom, and fun!, trust us when we say building an OS can be fun (and frustrating!) and that is the most important goal of the project.
