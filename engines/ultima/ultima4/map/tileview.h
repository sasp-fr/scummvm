/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef ULTIMA4_MAP_TILEVIEW_H
#define ULTIMA4_MAP_TILEVIEW_H

#include "ultima/ultima4/game/view.h"

namespace Ultima {
namespace Ultima4 {

class Tile;
class Tileset;
class MapTile;

/**
 * A view of a grid of tiles.  Used to draw Maps.
 * @todo
 * <ul>
 *      <li>use for gem view</li>
 *      <li>intialize from a Layout?</li>
 * </ul>
 */
class TileView : public View {
public:
	TileView(int x, int y, int columns, int rows);
	TileView(int x, int y, int columns, int rows, const Common::String &tileset);
	virtual ~TileView();

	void reinit();
	void drawTile(MapTile &mapTile, bool focus, int x, int y);
	void drawTile(Std::vector<MapTile> &tiles, bool focus, int x, int y);

	/**
	 * Draw a focus rectangle around the tile
	 */
	void drawFocus(int x, int y);
	void loadTile(MapTile &mapTile);
	void setTileset(Tileset *tileset);

protected:
	int _columns, _rows;
	int _tileWidth, _tileHeight;
	Tileset *_tileSet;
	Image *_animated;            /**< a scratchpad image for drawing animations */
};

} // End of namespace Ultima4
} // End of namespace Ultima

#endif
