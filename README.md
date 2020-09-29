Turn-grid strategy game. Eliminate all the red units before they eliminate you!

## Controls

WASD moves the camera and mouse drag to turn the camera around.

Arrow keys for moving the grid cursor. Use this to see the info of different units

Shift + Up or Down to iterate between the units.

Ctrl + arrow keys to move the selected unit if they still have movement left.

During your turn, move a blue unit besides a red unit, move the cursor on the red unit and press C to attack it.

The units can only attack once and cannot move further after they attack, even if they have stil movement left.

Press Enter to finish your turn and let the AI play their turn.

During your turn you can press M to stop time and redo your and the enemie's turns. Press the left and right arrows to choose what turn you want to go back to. Press M again to reset the game to that turn.

## Maps

The game comes with different pre-made maps. To load a different map you can change the `"mapsBaseLocation"` setting in `GDP2019_20/configFile/globalConfig.json`.

The current maps are:

- "./assets/textures/maps/small/"
- "./assets/textures/maps/medium/"
- "./assets/textures/maps/large/"

Each of these maps have three images: `enemyUnits.png`, `playerUnits.png`, and `Terrain.png`.

Each pixel of those images corresponds to a position in the map of the scene. They can be edited to change the scene of the game:

In the units images, the pixels where a unit will be loaded must be red (255,0,0)

In the terrain:

- Normal ground is green (0,255,0)
- Forest is red (255,0,0)
- Rocks are black (0,0,0)

## Credits

- Animations and knight model (Paladin W/Prop J Nordstrom) from mixamo.com
- Tree models and textures by  Mykhailo Ohorodnichuk from https://www.turbosquid.com/3d-models/3d-low-poly-trees-1431822
- Rock model and textures by  Hemppainen from https://www.turbosquid.com/FullPreview/Index.cfm/ID/789528
- Terrain cubes model and textures by devilworkshop on https://devilsworkshop.itch.io/essential-low-poly-isometric-3d-block-and-hex-pack
- Cubemap texture by Emil Persson from http://www.humus.name
