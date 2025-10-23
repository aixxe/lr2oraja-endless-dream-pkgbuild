# LR2oraja \~Endless Dream\~

Yet another Arch Linux package for [LR2oraja Endless Dream](https://github.com/seraxis/lr2oraja-endlessdream)

- Provides stable release and `-git` variants
- Installs to standard `/usr/share` directories
- Builds game and native JPortAudio library from source
- Launching from desktop entry no longer opens a terminal
- Includes basic launcher script with configurable options
- Can be built and run with a wider range of Java environments
- Uses `~/.config/lr2oraja-endlessdream/` directory for configuration
- Does not include any custom themes or external internet ranking services

### Building

```bash
# lr2oraja-endlessdream-git
makepkg --syncdeps

# lr2oraja-endlessdream
makepkg --syncdeps -p PKGBUILD.release
```

Alternatively, grab a [pre-built version](https://github.com/aixxe/lr2oraja-endlessdream-pkgbuild/actions/workflows/build.yml) from CI artifacts and install with `pacman -U`

### Usage

On first boot, the default configuration will be created in `~/.config/lr2oraja-endlessdream/`

To install a custom IR, copy the .jar file into the `ir` directory, e.g.

- [bokutachiIR-X.X.X.jar](https://github.com/zkrising/tachi-beatoraja-ir/releases) → `~/.config/lr2oraja-endlessdream/ir/bokutachiIR-X.X.X.jar`

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