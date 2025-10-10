_pkgbase=lr2oraja-endlessdream
pkgrel=1
pkgdesc="A featureful fork of beatoraja"
arch=('x86_64')
url="https://github.com/seraxis/lr2oraja-endlessdream"
license=('GPL-3.0-only')
depends=('bash' 'java-runtime>=17')
makedepends=('java-environment=17' 'portaudio' 'git' 'cmake' 'gcc')
optdepends=('portaudio: enables low-latency audio output options'
            'gamescope: workaround for crashes on nvidia wayland environments')
_javafx_version="17.0.10"
_portaudio_java_commit="2ec5cc47d6f8abe85ddb09c34e69342bfe72c60b"
_common_sources=("https://github.com/philburk/portaudio-java/archive/${_portaudio_java_commit}.tar.gz"
                 "${_pkgbase}.sh"
                 "${_pkgbase}.desktop"
                 "${_pkgbase}-icon.png")
_common_sha256sums=('7a70d90d449fc9d91026c54e1d08303242749475ad20b30b5bfa45fc93f18043'
                    'SKIP'
                    'bfa611672d6926830edc33d7845ce1430b415cf42325f565e609dc6997dcb3f9'
                    'fdbd37ff43aa6af20f9eb643bf271a77ef579014970a7a3dcecf78e65123d83d')

_prepare() {
  cd "${srcdir}/${_pkgbase}"
  git submodule update --init --recursive

  # add JavaFX to [versions] in gradle/libs.versions.toml
  sed -i "/^endlessdream = /a javafx = \"${_javafx_version}\"" gradle/libs.versions.toml
  
  # add JavaFX to [libraries] in gradle/libs.versions.toml
  sed -i '/^gdx-core = /i \
javafx-base = { group = "org.openjfx", name = "javafx-base", version.ref = "javafx" }\
javafx-controls = { group = "org.openjfx", name = "javafx-controls", version.ref = "javafx" }\
javafx-fxml = { group = "org.openjfx", name = "javafx-fxml", version.ref = "javafx" }\
javafx-graphics = { group = "org.openjfx", name = "javafx-graphics", version.ref = "javafx" }\
' gradle/libs.versions.toml
  
  # add JavaFX to [bundles] in gradle/libs.versions.toml
  printf '\njavafx = ["javafx-base", "javafx-controls", "javafx-fxml", "javafx-graphics"]' >> gradle/libs.versions.toml
  
  # add JavaFX dependencies to core/build.gradle.kts
  sed -i '/^dependencies {/a \
    implementation(libs.bundles.javafx)\
    \
    implementation("org.openjfx:javafx-base:${libs.versions.javafx.get()}:linux")\
    implementation("org.openjfx:javafx-controls:${libs.versions.javafx.get()}:linux")\
    implementation("org.openjfx:javafx-fxml:${libs.versions.javafx.get()}:linux")\
    implementation("org.openjfx:javafx-graphics:${libs.versions.javafx.get()}:linux")\
' core/build.gradle.kts
  
  # create wrapper entrypoint for MainLoader
  mkdir -p core/src/bms/player/beatoraja
  cat > core/src/bms/player/beatoraja/MainLoaderEntrypoint.java << 'EOF'
package bms.player.beatoraja;

public class MainLoaderEntrypoint {

    public static void main(String[] args) {
        MainLoader.main(args);
    }
}
EOF
}

_build() {
  # check for a usable build environment
  if [[ -z "${JAVA_HOME}" ]]; then
    echo "error: set JAVA_HOME to a Java 17 installation before building"
    echo "       e.g. 'JAVA_HOME=/usr/lib/jvm/java-17-openjdk makepkg'"
    return 1
  fi

  # build native dependencies
  cd "${srcdir}/portaudio-java-${_portaudio_java_commit}"
  cmake -DCMAKE_POLICY_VERSION_MINIMUM=3.5 .
  cmake --build .

  # build game using gradle wrapper
  cd "${srcdir}/${_pkgbase}"
  PATH="${JAVA_HOME}/bin:${PATH}" ./gradlew -Dplatform=linux core:shadowJar
}

_package() {
  cd "${srcdir}/${_pkgbase}"

  # launcher script
  install -Dm755 "${srcdir}/${_pkgbase}.sh" \
    "${pkgdir}/usr/bin/${_pkgbase}"
  
  # main game executable
  install -Dm644 dist/lr2oraja-*.jar \
    "${pkgdir}/usr/share/java/${_pkgbase}.jar"
  
  # default game assets
  install -dm755 "${pkgdir}/usr/share/${_pkgbase}"
  cp -r assets/* "${pkgdir}/usr/share/${_pkgbase}/"
  rm -f "${pkgdir}/usr/share/${_pkgbase}"/beatoraja-config*

  # portaudio-java native library
  install -Dm644 "${srcdir}/portaudio-java-${_portaudio_java_commit}/libjportaudio_0_1_0.so" \
    "${pkgdir}/usr/share/${_pkgbase}/natives/libjportaudio.so"

  # desktop application icon
  install -Dm644 "${srcdir}/${_pkgbase}-icon.png" \
    "${pkgdir}/usr/share/pixmaps/${_pkgbase}-icon.png"
  
  # desktop application entry
  install -Dm644 "${srcdir}/${_pkgbase}.desktop" \
    "${pkgdir}/usr/share/applications/${_pkgbase}.desktop"
  sed -i "s/__PKGVER__/${pkgver}/" \
    "${pkgdir}/usr/share/applications/${_pkgbase}.desktop"
}
