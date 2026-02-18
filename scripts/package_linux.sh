#!/usr/bin/env bash
set -euo pipefail

if [ "$#" -lt 1 ]; then
  echo "Usage: $0 <version> [build_dir] [out_dir]"
  exit 1
fi

VERSION="$1"
BUILD_DIR="${2:-build}"
OUT_DIR="${3:-dist}"
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

BIN_CANDIDATES=(
  "$ROOT_DIR/$BUILD_DIR/meow"
  "$ROOT_DIR/$BUILD_DIR/Release/meow"
  "$ROOT_DIR/$BUILD_DIR/Debug/meow"
)

BIN_PATH=""
for candidate in "${BIN_CANDIDATES[@]}"; do
  if [ -f "$candidate" ]; then
    BIN_PATH="$candidate"
    break
  fi
done

if [ -z "$BIN_PATH" ]; then
  echo "Could not find meow binary in '$BUILD_DIR'. Build first with CMake."
  exit 1
fi

PACKAGE_NAME="meowlang-${VERSION}-linux-x64"
PACKAGE_DIR="$ROOT_DIR/$OUT_DIR/$PACKAGE_NAME"
ARCHIVE_PATH="$ROOT_DIR/$OUT_DIR/$PACKAGE_NAME.tar.gz"

rm -rf "$PACKAGE_DIR" "$ARCHIVE_PATH"
mkdir -p "$PACKAGE_DIR"

cp "$BIN_PATH" "$PACKAGE_DIR/meow"
chmod +x "$PACKAGE_DIR/meow"

if [ -f "$ROOT_DIR/README.md" ]; then
  cp "$ROOT_DIR/README.md" "$PACKAGE_DIR/"
fi
if [ -d "$ROOT_DIR/examples" ]; then
  cp -R "$ROOT_DIR/examples" "$PACKAGE_DIR/"
fi

(
  cd "$ROOT_DIR/$OUT_DIR"
  tar -czf "$PACKAGE_NAME.tar.gz" "$PACKAGE_NAME"
)

echo "Created $ARCHIVE_PATH"
