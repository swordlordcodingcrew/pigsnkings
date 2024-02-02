# Pigs n Kings

![Pigs n Kings](https://raw.githubusercontent.com/swordlordcodingcrew/pigsnkings/main/gfx/loader-splash.png)

![Build Pigs n Kings for the 32blit](https://github.com/swordlordcodingcrew/pigsnkings/workflows/Build%20Pigs%20n%20Kings%20for%20the%2032blit/badge.svg)

**Pigs n Kings** (c) 2020-22 by [SwordLord - the coding crew](https://www.swordlord.com/)

A jump and run game for the 32blit handheld console. 

Fight as the King against the evil Pigs and hunt them through your Castle. See our [Wiki](https://github.com/swordlordcodingcrew/pigsnkings/wiki/Story) for all the story about that.

This game is still work in progress. No finished game yet.


## DANG!
The game uses the SwordLord [DANG!](https://github.com/swordlordcodingcrew/DANG/) (Dainty Animations 'N Gears) Game Framework for the [32blit](https://32blit.com/) handheld open game console.

See the DANG project website for more details.

# Download / Builds

This project does auto compile a binary for the 32blit platform. See the [releases section](https://github.com/swordlordcodingcrew/pigsnkings/releases) for a recent build.

Please bear in mind that we still are in the development phase. The game is not ready for prime time yet.

# Development Howto

This section contains all the relevant information to build the pigsnkings game yourself.

This game is making use of two other projects. The 32blit firmware / base and the DANG game framework. Both these projects are integrated as git submodules in the /lib folder to this repository. 

## How to set up
Initial download of all source code: 

```
git clone https://github.com/swordlordcodingcrew/pigsnkings.git
cd pigsnkings
git submodule update --init --recursive
```

## How to regularly update the repository
Update the project after that (making sure, to update the submodules as well): 

```
git pull
git submodule foreach git pull origin master
```

## How to build the game
### 32blit-hw
```
mkdir build.stm32
cd build.stm32
cmake .. -DCMAKE_TOOLCHAIN_FILE=../libs/32blit-sdk/32blit.toolchain
make
```

Upload the resulting pigsnkings.blit file to your 32blit.

Make sure to clean the build.stm32 folder before making if you change the build toolchain.

### Emscripten
First git clone and prepare emsdk.
(Further reading: [Emscripten docs](https://emscripten.org/docs/getting_started/downloads.html))
```
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
git pull
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh
```
Then cd to pigsnkings and build as follows.
(Further reading: [32blit readme](https://github.com/32blit/32blit-sdk/blob/master/docs/Emscripten.md))
```
mkdir build.em
cd build.em
emcmake cmake .. -D32BLIT_DIR="/absolute/path/to/32blit-sdk"
make
python3 -m http.server
```
Connect to the given address of the server and enjoy.

## Licence
SwordLord Pigs n Kings sources and tools are released under the AGPLv3 Licence.

Graphics, Tracks and Sound Effects are copyrighted to their respective owners. SwordLord makes sure to get the respective licences from the owners for this, official version of the game. If you fork this game, make sure you either change these resources or get your own licence. 
