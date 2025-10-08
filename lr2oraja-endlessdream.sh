#!/usr/bin/env bash
set -e

# read-only directory where default game assets exists
export BEATORAJA_VARIANT="lr2oraja-endlessdream"
export BEATORAJA_INSTALL_DIR="/usr/share/${BEATORAJA_VARIANT}"

# include application-specific native libraries in library load path
export LD_LIBRARY_PATH="${BEATORAJA_INSTALL_DIR}/natives:${LD_LIBRARY_PATH:-}"

# working directory where configuration & player-specific files exists
export BEATORAJA_USER_DIR="${BEATORAJA_USER_DIR:-${XDG_CONFIG_HOME:-$HOME/.config}/${BEATORAJA_VARIANT}}"
mkdir -p "${BEATORAJA_USER_DIR}" && cd "${BEATORAJA_USER_DIR}"

# pull in additional java options from JDK_JAVA_OPTIONS
build_java_options() {
  local options=(
    -Dsun.java2d.opengl=true
    -Dawt.useSystemAAFontSettings=on
    -Dswing.aatext=true
    -Dswing.defaultlaf=com.sun.java.swing.plaf.gtk.GTKLookAndFeel
    -Dfile.encoding="UTF-8"
  )

  if [[ -n "${JDK_JAVA_OPTIONS}" ]]; then
    while IFS= read -r opt; do
      [[ -n "$opt" ]] && options+=("$opt")
    done < <(echo "${JDK_JAVA_OPTIONS}" | tr ' ' '\n')
  fi

  echo "${options[*]}"
}

# launch the game!
JDK_JAVA_OPTIONS=$(build_java_options) \
exec java \
  -Djdk.gtk.version=2 \
  -Xms"${JAVA_HEAP_MIN:-1g}" \
  -Xmx"${JAVA_HEAP_MAX:-4g}" \
  -cp /usr/share/java/${BEATORAJA_VARIANT}.jar:ir/* \
  bms.player.beatoraja.MainLoaderEntrypoint \
  "$@"
