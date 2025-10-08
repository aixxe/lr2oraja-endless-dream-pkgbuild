# Maintainer: aixxe <me@aixxe.net>
_pkgname=lr2oraja-endlessdream
pkgname="${_pkgname}-git"
pkgver=pre0.3.1.g1c6923d3
pkgrel=1
pkgdesc="A featureful fork of beatoraja"
arch=('x86_64')
url="https://github.com/seraxis/lr2oraja-endlessdream"
license=('GPL-3.0-only')
provides=("${_pkgname}")
conflicts=("${_pkgname}")
depends=('bash' 'java-runtime>=17')
makedepends=('java-environment=17' 'portaudio' 'git' 'cmake' 'gcc')
optdepends=('portaudio: enables low-latency audio output options'
            'gamescope: workaround for crashes on nvidia wayland environments')
_portaudio_java_commit="2ec5cc47d6f8abe85ddb09c34e69342bfe72c60b"
source=("git+https://github.com/seraxis/${_pkgname}.git"
        "https://github.com/philburk/portaudio-java/archive/${_portaudio_java_commit}.tar.gz"
        "${_pkgname}.sh"
        "${_pkgname}.desktop"
        "${_pkgname}-icon.png"
        "0001-add-fallback-default-path-for-game-assets.patch"
        "0002-integrate-javafx-into-build.patch")
sha256sums=('SKIP'
            '7a70d90d449fc9d91026c54e1d08303242749475ad20b30b5bfa45fc93f18043'
            '10dfe13fe7794e9e05ec329f2d76f15312f77aca30ecdc536a573c2c570fb855'
            'bfa611672d6926830edc33d7845ce1430b415cf42325f565e609dc6997dcb3f9'
            'fdbd37ff43aa6af20f9eb643bf271a77ef579014970a7a3dcecf78e65123d83d'
            '9fec8e8140929490d59a714dd07d648ea5508ba4960ac0ea512f559a067fde4a'
            '7873839e89696c0ebe4865f954805ccec8d4d173dc20abdaeda4239b09c13c5e')

pkgver() {
  cd "${srcdir}/${_pkgname}"
  local version=$(grep '^endlessdream = ' gradle/libs.versions.toml | sed 's/.*= "\(.*\)"/\1/')
  local commit=$(git rev-parse --short HEAD)
  echo "${version}.g${commit}"
}

prepare() {
  cd "${srcdir}/${_pkgname}"
  git submodule update --init --recursive

  patch -Np1 -i "${srcdir}/0001-add-fallback-default-path-for-game-assets.patch"
  patch -Np1 -i "${srcdir}/0002-integrate-javafx-into-build.patch"
}

build() {
  # check for a usable build environment
  if [[ -z "${JAVA_HOME}" ]]; then
    echo "error: set JAVA_HOME to a Java 17 installation before building"
    echo "       e.g. 'JAVA_HOME=/usr/lib/jvm/java-17-openjdk makepkg'"
    return 1
  fi

  # build native dependencies
  cd "${srcdir}/portaudio-java-${_portaudio_java_commit}"
  cmake -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DJAVA_HOME="${JAVA_HOME}" .
  cmake --build .

  # build game using gradle wrapper
  cd "${srcdir}/${_pkgname}"
  PATH="${JAVA_HOME}/bin:${PATH}" ./gradlew core:shadowJar -Dplatform=linux
}

package() {
  cd "${srcdir}/${_pkgname}"

  # launcher script
  install -Dm755 "${srcdir}/${_pkgname}.sh" \
    "${pkgdir}/usr/bin/${_pkgname}"
  
  # main game executable
  install -Dm644 dist/lr2oraja-*.jar \
    "${pkgdir}/usr/share/java/${_pkgname}.jar"
  
  # default game assets
  install -dm755 "${pkgdir}/usr/share/${_pkgname}"
  cp -r assets/* "${pkgdir}/usr/share/${_pkgname}/"
  rm -f "${pkgdir}/usr/share/${_pkgname}"/beatoraja-config*

  # portaudio-java native library
  install -Dm644 "${srcdir}/portaudio-java-${_portaudio_java_commit}/libjportaudio_0_1_0.so" \
    "${pkgdir}/usr/share/${_pkgname}/natives/libjportaudio.so"

  # desktop application icon
  install -Dm644 "${srcdir}/${_pkgname}-icon.png" \
    "${pkgdir}/usr/share/pixmaps/${_pkgname}-icon.png"
  
  # desktop application entry
  install -Dm644 "${srcdir}/${_pkgname}.desktop" \
    "${pkgdir}/usr/share/applications/${_pkgname}.desktop"
  sed -i "s/__PKGVER__/${pkgver}/" \
    "${pkgdir}/usr/share/applications/${_pkgname}.desktop"
}
