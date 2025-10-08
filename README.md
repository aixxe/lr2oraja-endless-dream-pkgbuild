# LR2oraja \~Endless Dream\~

Yet another Arch Linux package for [LR2oraja Endless Dream](https://github.com/seraxis/lr2oraja-endlessdream)

### Differences to the [AUR package](https://aur.archlinux.org/packages/lr2oraja-endlessdream)

- Currently only targets latest Git commit
- Installs to standard `/usr/share` directories
- Builds game and native JPortAudio library from source
- Launching from desktop entry no longer opens a terminal
- Includes basic launcher script with configurable options
- Supports a wider range of Java runtimes by bundling JavaFX
- Uses `~/.config/lr2oraja-endlessdream/` directory for configuration
- Does not include any custom themes or external internet ranking services

### Building

Ensure `JAVA_HOME` is set to a compatible JDK, then run `makepkg`

```bash
pacman --sync --refresh --sysupgrade --noconfirm jdk17-openjdk
JAVA_HOME=/usr/lib/jvm/java-17-openjdk makepkg --syncdeps --noconfirm
```

Alternatively, grab a pre-built version from CI artifacts:
- [lr2oraja-endlessdream-git](https://nightly.link/aixxe/lr2oraja-endless-dream-pkgbuild/workflows/lr2oraja-endlessdream-git/master)

### Usage

On first boot, the default configuration will be created in `~/.config/lr2oraja-endlessdream/`

To install a custom IR, create the `ir` directory and copy the .jar file into it, e.g.

```bash
curl -LO https://github.com/zkrising/tachi-beatoraja-ir/releases/download/v3.1.1/bokutachiIR-3.1.1.jar
mkdir -vp ~/.config/lr2oraja-endlessdream/ir
mv -v bokutachiIR-3.1.1.jar ~/.config/lr2oraja-endlessdream/ir/
```

If you want to use a different directory, set the `BEATORAJA_USER_DIR` variable

### Options

By default, the launcher will open the configurator window. This can be skipped with the `-s` flag

```bash
lr2oraja-endlessdream -s
```

Additional JVM options set through `JDK_JAVA_OPTIONS` will be passed transparently

```bash
JDK_JAVA_OPTIONS="-Dxyz=123" lr2oraja-endlessdream -s
```

Heap size can be configured through the `JAVA_HEAP_MIN` and `JAVA_HEAP_MAX` variables

```bash
JAVA_HEAP_MIN="1g" JAVA_HEAP_MAX="8g" lr2oraja-endlessdream -s
```