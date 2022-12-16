# How to cheat and change the gamesaves

You may need to adjust the save file accordingly.

## read level
``` 
xxd -seek 0x14 -l 1 ~/.local/share/SwordLord/Pigs\ n\ Kings/save4
```

## read room
``` 
xxd -seek 0x16 -l 1 ~/.local/share/SwordLord/Pigs\ n\ Kings/save4
```

## write level
``` 
printf "00000014: 02" | xxd -r - ~/.local/share/SwordLord/Pigs\ n\ Kings/save4
```

## write room
``` 
printf "00000016: 01" | xxd -r - ~/.local/share/SwordLord/Pigs\ n\ Kings/save4
```