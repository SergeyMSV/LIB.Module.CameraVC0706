# Camera VC0706

## Bugs

###### [2025-11-26] Image resolution

It's impossible to switch image resolution via config file.

"resolution": "640x480",
"comment 1 - resolution": "640x480",
"comment 2 - resolution": "320x240",
"comment 3 - resolution": "160x120",

The camera based on Vimicro VC0607 works only with 640x480 in spite of I2C_EEPROM contains the other value.
(long long ago it worked...)
Maybe the resolution should be written in another memory, I don't remember and I don't have enought time to find it out.
I haven't tried to change the memory because I'm afraid that it can spoil the camera.
