

# create h files for a mod/xm track or sfx
## mod file 
File name: example.mod
``` shell script
python3 -m ttblit raw --input_file example.mod --input_type binary --output_file example_mod.h --output_format c_header --symbol_name example_mod --force
```
## xm file
Conversion: Same call as for the mod file

## sfx
The sfx must be in the following format:
- format type: PCM data
- mono
- sample rate: 22050

Conversion: Same call as for the mod file
 