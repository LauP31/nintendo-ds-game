# Nintendo DS Game

An infinite runner built for the Nintendo DS using [BlocksDS](https://blocksds.skylyrac.net/docs/).

## Features

- Player movement and jumping
- Procedural obstacle generation
- Scrolling background
- Collision detection
- Score tracking
- Sound effects and music

## Prerequisites

- [Docker](https://docs.docker.com/get-docker/)
- The BlocksDS Docker image:
```bash
  docker image pull skylyrac/blocksds:slim-latest
```
- An NDS emulator for testing — [melonDS](https://melonds.kuribo64.net/) or [DeSmuME](https://desmume.org/) — or a flashcart to run on real hardware

## Setup

1. Clone the repo:
```bash
   git clone https://github.com/LauP31/nintendo-ds-game.git
   cd nintendo-ds-game
```

2. Copy BlocksDS headers locally (needed for editor IntelliSense/code completion):
```bash
   ./copy_headers.sh
```

3. Build the ROM:
```bash
   ./compile.sh
```
   or directly via Docker:
```bash
   docker run --rm -v "$(pwd):/work:z" -w /work skylyrac/blocksds:slim-latest make
```

   The built `.nds` file will appear in the project root after a successful build.

## Running the game

Open the generated `.nds` file in melonDS, DeSmuME, or copy it to a flashcart to run on real DS hardware.
