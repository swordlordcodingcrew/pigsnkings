# pigsnkings

![pigs n kings](https://raw.githubusercontent.com/swordlordcodingcrew/pigsnkings/main/gfx/loader-splash.png)

A game for the 32blit handheld console. This is still work in progress. No finished game yet.

**pigs n kings** (c) 2020-21 by [SwordLord - the coding crew](https://www.swordlord.com/)

## Foundation
The game uses the SwordLord [DANG!](https://github.com/swordlordcodingcrew/DANG/) (Dainty Animations 'N Gears) Game Framework for the [32blit](https://32blit.com/) handheld open game console.

# Howto
## Update
Initial download: 

```
git submodule update --init --recursive
```

Update after that: 

```
git pull --recurse-submodules
```

## Build

```
mkdir build.stm32
cd build.stm32
cmake .. -D32BLIT_PATH="../libs/32blit-beta/" -DCMAKE_TOOLCHAIN_FILE=../libs/32blit-beta/32blit.toolchain
make
```

Upload the resulting pigsnkings.blit file to your 32blit.

Make sure to clean the build.stm32 folder before making if you change the build toolchain.

## Licence
SwordLord pigsnkings and all its tools are released under the AGPLv3 Licence. Drop us a note if you need another licence.

