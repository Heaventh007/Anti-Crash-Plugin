# Anti-Crash-Plugin
This anti crash plugin patches an out of bounds write exploit in BO1 and BO2, as well as many exploits, such as crashes and kicks. \
This is not by any means an up to date plugin.

# Supports
- MW2
- MW3
- BO1
- BO2

# Notes
Since I am leaving the scene, I thought I'd drop a few of my older projects.\
With that as well, if anyone wants to add to this, feel free to submit a pull request.

### Here are some additional patches in case someone wants them, or would like to update this plugin:
```cpp
// Relay crash patches.

*(short*)0x822D3350 = 0x4800; // Call Of Duty 4 (default_mp)

*(short*)0x821B8F04 = 0x4800; // World At War (default_mp)
*(short*)0x822EC02C = 0x4800; // World At War (default)

*(short*)0x82147534 = 0x4800; // Modern Warfare 2 (default_mp)

*(short*)0x82241F08 = 0x4800; // Black Ops 1 (default_mp)
*(short*)0x823D5978 = 0x4800; // Black Ops 1 (default)

*(short*)0x82176BF4 = 0x4800; // Modern Warfare 3 (default_mp)

*(short*)0x82273C80 = 0x4800; // Black Ops 2 (default_mp)

*(short*)0x822DFEB8 = 0x4800; // Ghosts (default_mp)

*(short*)0x825B3374 = 0x4800; // Advanced Warfare (default_mp)

*(short*)0x822EA414 = 0x4800; // Black Ops 3 (default)
*(short*)0x822E6344 = 0x4800; // Black Ops 3 (default_zm)
```
