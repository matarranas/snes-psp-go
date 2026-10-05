#!/bin/bash
set -e

PROJECT_DIR="/home/khatddog/docker/devpsp/snes-psp-go"

cd "$PROJECT_DIR"
git fetch origin
git reset --hard origin/main

echo "=== INICIANDO COMPILACION LOCAL CON DOCKER (PSPDEV) ==="
docker run --rm -v "$PROJECT_DIR:/src" -w /src pspdev/pspdev:latest bash -c "
  psp-pacman -Sy --noconfirm psp-libpng psp-libjpeg-turbo psp-zlib >/dev/null 2>&1 || true
  make me -j\$(nproc)
"

echo "=== COMPILACION COMPLETADA EXITOSAMENTE ==="
ls -lh "$PROJECT_DIR/EBOOT.PBP"
