## The Utopia operating system kernel

Utopia doesn't actually have it's own kernel (as of now) and will probably never have it's own kernel (I hope someone proves me wrong).

Utopia is built upon the Linux kernel by Linus Tolvards, however, this doesn't mean we ship the "stock" or "vanilla" Linux kernel, Utopia is an operating system designed for integration, so a lot of our userland, uses Utopia-specific kernel additions to give the user experience it currently provides.

This subdirectory contiains the code for Utopia kernel additions, to name the most important:

- GoboHide (Known as UtopiaHideClassic): This patch was originally developed by the folks at GoboLinux, it is a way to hide files based on inodes and a linked list inside the kernel.

- UtopiaHide: UtopiaHide is Utopia's kernel level implementation of the GoboLinux patch, however, there are some crusial differences, mainly that the UtopiaHide patch needs the developer to integrate it into their Userland program (While using GoboLinux patch, every program just works), UtopiaHide works by using extended attributes, similarly to the Gobo patch, if an inode contains the hidden xattr attribute, it will not be displayed.

- DNASC: Definitely Not A SIP Copy, is Utopia's kernel level, System integrity protection implementation, it works by xattrs, which are set at boot, and can only be set by the PID 1.

- Mach-O: Based on work by the Darling team, Utopia is able to load (and it is its primary format) Mach-o binaries, this kernel patch allows us to map and load Mach-o binaries.