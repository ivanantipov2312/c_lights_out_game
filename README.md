# Map loading
Maps are stored in assets/boards directory. Map names should be "lvl%NUMBER%.brd" and should be added to assets/boards directory. (i.e. "assets/boards/lvl4.brd")
# Map format
Your file should consist of 5 rows and 5 columns of 0s and 1s separated by spaces, rows must be separated by '\n' character.
- 1 means light square
- 0 means dark square
# Launch
`cd /path/to/game_directory/`

`./bin/lights_out`
